#include "SourceDataGenerator.h"
#include "OutputHelper.h"
#include "fontManager.h"
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/regex.h>
#include <wx/debug.h>

CSourceDataGenerator::CSourceDataGenerator()
{
}


CSourceDataGenerator::~CSourceDataGenerator()
{
}

wxString CSourceDataGenerator::GetWildCardString()
{
	return "C source files(*.c & *.h)|*.c;*.h";
}

bool CSourceDataGenerator::IsFilenameSupported(const wxFileName &fileName)
{
	return fileName.GetExt() == "h" || fileName.GetExt() == "c";
}

void CSourceDataGenerator::SaveCharmap(const CharMap &charmap, bool compress, enum OutputFormat format, const wxString &fileName)
{
	wxFileName fileNameInfo(fileName, wxPATH_NATIVE);
	wxString hFilePath, cFilePath;
	if (fileNameInfo.GetExt() == "h") {
		hFilePath = fileName;
		fileNameInfo.SetExt("c");
		if (fileNameInfo.Exists() && wxMessageBox(_("Also the file \"") + fileNameInfo.GetFullName() + 
			_("\" will be generated. Do you want to overwrite it?"), _("Confirm"), wxYES_NO) == wxNO) {
			return;
		}
		cFilePath = fileNameInfo.GetFullPath();
	}
	else if (fileNameInfo.GetExt() == "c") {
		cFilePath = fileName;
		fileNameInfo.SetExt("h");
		if (fileNameInfo.Exists() && wxMessageBox(_("The file \"") + fileNameInfo.GetFullName() +
			_("\" will be generated. Do you want to overwrite it?"), _("Confirm"), wxYES_NO) == wxNO) {
			return;
		}
		hFilePath = fileNameInfo.GetFullPath();
	}
	wxFFileOutputStream hFile(hFilePath);
	wxFFileOutputStream cFile(cFilePath);
	wxTextOutputStream hOut(hFile);
	wxTextOutputStream cOut(cFile);

	EMGL_font_t *font = LoadEMGLFont(charmap, compress, format);

	hOut << GetHeaderFile(fileNameInfo.GetName());
	cOut << GetSourceFile(font, fileNameInfo.GetName());

	DeleteEMGLFont(font);
}


wxString CSourceDataGenerator::MakeCorrectName(const wxString &x)
{
	wxString result = x;
	wxRegEx valid("[^A-Za-z0-9_]");
	wxRegEx start("^([^A-Za-z_])");
	wxASSERT(valid.IsValid());
	wxASSERT(start.IsValid());
	valid.Replace(&result, "_");
	start.ReplaceFirst(&result, "_\\1");
	return result;
}

CSourceDataGenerator::EMGL_font_t * CSourceDataGenerator::LoadEMGLFont(const CharMap &charmap, bool compress, enum OutputFormat format)
{
	EMGL_font_t *result = new EMGL_font_t;
	int ascender = 0;
	int ascenderDiv = 0;
	result->bpp = GetBpp(format);
	result->numCodepages = charmap.GetCountCodePages();
	result->codePagePtrs = new EMGL_codePage_t[result->numCodepages];

	std::list<CodePage *>::const_iterator it = charmap.CBegin();
	for (int i = 0; i < result->numCodepages; ++i, ++it) {
		wxASSERT(it != charmap.CEnd());
		wxASSERT(*it != NULL);
		EMGL_codePage_t *outCodepage = &result->codePagePtrs[i];
		outCodepage->startCode = (*it)->GetRangeStart();
		outCodepage->endCode = (*it)->GetRangeEnd();
		const wxString &name = (*it)->GetName();
		for (unsigned int j = 0; j < 16; ++j) {
			outCodepage->name[j] = j < name.Length() ? name.ToAscii()[j] : '\0';
		}
		outCodepage->name[16] = '\0';
		int numGlyphs = outCodepage->endCode - outCodepage->startCode + 1;
		outCodepage->glyphPtrs = new EMGL_glyph_t *[numGlyphs];
		for (int j = 0; j < numGlyphs; ++j) {
			const CharMapEntry *entry = (*it)->GetCharMapEntry((*it)->GetRangeStart() + j);
			if (entry == NULL) {
				outCodepage->glyphPtrs[j] = NULL;
				continue;
			}
			LoadFont(entry->GetFamily(), entry->GetStyle(), entry->GetSize(), entry->GetEncodingID());
			wxUint32 glyph;
			if (!m_loadedFont.IsOk() || (glyph = m_loadedFont.GetGlyphIndex(entry->GetCode())) == 0)
			{
				wxLogWarning("Could not load glyph with code %u", (*it)->GetRangeStart() + j);
				outCodepage->glyphPtrs[j] = NULL;
				continue;
			}
			int bitmapWidth, bitmapHeight;
			int bitmapSize = m_loadedFont.GetGlyphBitmap(glyph, NULL, &bitmapWidth, &bitmapHeight);
			if (bitmapSize < 0) {
				wxLogWarning("Error while getting bitmap for glyph with code %u", (*it)->GetRangeStart() + j);
				outCodepage->glyphPtrs[j] = NULL;
				continue;
			}
			wxUint8 *bitmap = new wxUint8[bitmapSize];
			int result = m_loadedFont.GetGlyphBitmap(glyph, bitmap, &bitmapWidth, &bitmapHeight);
			wxASSERT(result == bitmapSize);

			bitmapSize = ConvertToFormat(bitmap, bitmapSize, format);

			if (compress) {
				CompressBitmap(&bitmap, &bitmapSize, bitmapWidth * bitmapHeight, format);
			}

			ascender += m_loadedFont.GetAscender();
			ascenderDiv++;
			wxPoint advance = m_loadedFont.GetGlyphAdvance(glyph);
			wxPoint bearing = m_loadedFont.GetGlyphBearing(glyph);

			outCodepage->glyphPtrs[j] = new EMGL_glyph_t;
			outCodepage->glyphPtrs[j]->advanceX = advance.x;
			outCodepage->glyphPtrs[j]->advanceY = advance.y;
			outCodepage->glyphPtrs[j]->bearingX = bearing.x;
			outCodepage->glyphPtrs[j]->bearingY = bearing.y;
			outCodepage->glyphPtrs[j]->bitmapWidth = bitmapWidth;
			outCodepage->glyphPtrs[j]->bitmapHeight = bitmapHeight;
			outCodepage->glyphPtrs[j]->bitmapSize = bitmapSize;
			outCodepage->glyphPtrs[j]->bitmapData = bitmap;
		}
	}

	ascender = ascenderDiv > 0 ? ascender / ascenderDiv : 0;
	wxASSERT(ascender >= INT16_MIN && ascender <= INT16_MAX);
	result->ascender = ascender;
	return result;
}

void CSourceDataGenerator::DeleteEMGLFont(EMGL_font_t *font)
{
	for (int i = 0; i < font->numCodepages; ++i) {
		int numGlyphs = font->codePagePtrs[i].endCode - font->codePagePtrs[i].startCode + 1;
		for (int j = 0; j < numGlyphs; ++j) {
			if(font->codePagePtrs[i].glyphPtrs[j] == NULL) continue;
			delete[] font->codePagePtrs[i].glyphPtrs[j]->bitmapData;
			delete font->codePagePtrs[i].glyphPtrs[j];
		}
		delete[] font->codePagePtrs[i].glyphPtrs;
	}
	delete[] font->codePagePtrs;
	delete font;
}

wxString CSourceDataGenerator::GetHeaderFile(wxString name)
{
	name = MakeCorrectName(name);
	wxString guard_define = "_FONT_" + name + "_H_";
	guard_define.MakeUpper();
	wxString guard_begin = "#ifndef " + guard_define + "\n"
		"#define " + guard_define + "\n";
	wxString guard_end = "#endif\n";
	wxString cplusplus_begin = "#ifdef __cplusplus\n"
		"extern \"C\" {\n"
		"#endif\n";
	wxString cplusplus_end = "#ifdef __cplusplus\n"
		"}\n"
		"#endif\n";

	return guard_begin + "\n" +
		cplusplus_begin + "\n" +
		GetHeaderIncludes() + "\n" +
		"// Choose between local declaration or remote declaration \n"
		"#include <emgl/font.h>\n"
		"/*\n" +
		GetStructureDeclaration() +
		"*/\n\n" +
		GetFontDeclaration(name) + "\n" + 
		cplusplus_end + "\n" +
		guard_end;
}

wxString CSourceDataGenerator::GetHeaderIncludes()
{
	return "#include <stdint.h>\n";
}

wxString CSourceDataGenerator::GetStructureDeclaration()
{
	return
		"typedef struct {\n"
		"\tint16_t advanceX;\n"
		"\tint16_t advanceY;\n"
		"\tint16_t bearingX;\n"
		"\tint16_t bearingY;\n"
		"\tuint16_t bitmapWidth;\n"
		"\tuint16_t bitmapHeight;\n"
		"\tuint32_t bitmapSize;\n"
		"\tconst uint8_t * const bitmapData;\n"
		"} EMGL_glyph_t;\n"
		"\n"
		"typedef struct {\n"
		"\tuint32_t startCode;\n"
		"\tuint32_t endCode;\n"
		"\tchar name[17];\n"
		"\tconst EMGL_glyph_t * const * const glyphPtrs;\n"
		"} EMGL_codePage_t;\n"
		"\n"
		"typedef struct {\n"
		"\tint16_t ascender;\n"
		"\tuint8_t bpp;\n"
		"\tuint8_t numCodepages;\n"
		"\tconst EMGL_codePage_t * const codePagePtrs;\n"
		"} EMGL_font_t;\n"
		;
}

void CSourceDataGenerator::LoadFont(wxString family, wxString style, float size, wxUint32 encoding)
{
	if (!m_loadedFont.IsOk() || m_loadedFont.GetFamily() != family || m_loadedFont.GetStyle() != style)
	{
		// Load different file
		m_loadedFont = FontManager::GetFont(family, style, 10.0f);
	}
	if (m_loadedFont.GetSize() != size)
	{
		m_loadedFont.SetSize(size);
	}
	if (m_loadedFont.GetSelectedEncodingId() != encoding)
	{
		m_loadedFont.SelectEncoding(encoding);
	}
}

wxString CSourceDataGenerator::GetFontDeclaration(const wxString &name)
{
	return "extern const EMGL_font_t font_" + name + ";\n";
}

wxString CSourceDataGenerator::GetSourceIncludes(const wxString &name)
{
	return "#include \"" + name + ".h\"\n";
}

wxString CSourceDataGenerator::GetGlyphDataDefinitions(const EMGL_font_t *font)
{
	wxString result;
	for (int codepage = 0; codepage < font->numCodepages; ++codepage) {
		int numGlyphs = font->codePagePtrs[codepage].endCode - font->codePagePtrs[codepage].startCode + 1;
		for (int glyph = 0; glyph < numGlyphs; ++glyph) {
			EMGL_glyph_t *ptr = font->codePagePtrs[codepage].glyphPtrs[glyph];
			if (ptr == NULL) continue;
			int dataSize = ptr->bitmapSize& ~(1 << 31);
			if (dataSize == 0) continue;
			result += wxString::Format("static const uint8_t glyphData_%d_%d[%d] = {%u", codepage, glyph, dataSize,
				ptr->bitmapData[0]);
			for (int i = 1; i < dataSize; ++i) {
				result += wxString::Format(",%u", ptr->bitmapData[i]);
			}
			result += "};\n";
		}
	}
	return result;
}

wxString CSourceDataGenerator::GetGlyphDefinitions(const EMGL_font_t *font)
{
	wxString result;
	for (int codepage = 0; codepage < font->numCodepages; ++codepage) {
		int numGlyphs = font->codePagePtrs[codepage].endCode - font->codePagePtrs[codepage].startCode + 1;
		for (int glyph = 0; glyph < numGlyphs; ++glyph) {
			EMGL_glyph_t *ptr = font->codePagePtrs[codepage].glyphPtrs[glyph];
			if (ptr == NULL) continue;
			int dataSize = ptr->bitmapSize& ~(1 << 31);
			result += wxString::Format("static const EMGL_glyph_t glyph_%d_%d = {%d, %d, %d, %d, %u, %u, %u, ", 
				codepage, glyph, ptr->advanceX, ptr->advanceY, ptr->bearingX, ptr->bearingY, ptr->bitmapWidth, ptr->bitmapHeight, 
				ptr->bitmapSize);
			if (dataSize == 0) {
				result += "(void *)0 };\n";
			}
			else {
				result += wxString::Format("glyphData_%d_%d};\n", codepage, glyph);
			}
		}
	}
	return result;
}

wxString CSourceDataGenerator::GetGlyphTableDefinition(const EMGL_font_t *font)
{
	wxString result;
	for (int codepage = 0; codepage < font->numCodepages; ++codepage) {
		int numGlyphs = font->codePagePtrs[codepage].endCode - font->codePagePtrs[codepage].startCode + 1;
		if (numGlyphs <= 0) continue;
		result += wxString::Format("static const EMGL_glyph_t *glyphTable_%d[%d] = { ", codepage, numGlyphs);
		if (font->codePagePtrs[codepage].glyphPtrs[0] == NULL) {
			result += "(void *)0";
		}
		else {
			result += wxString::Format("&glyph_%d_0", codepage);
		}
		for (int glyph = 1; glyph < numGlyphs; ++glyph) {
			if (font->codePagePtrs[codepage].glyphPtrs[glyph] == NULL) {
				result += ", (void *)0";
			}
			else {
				result += wxString::Format(", &glyph_%d_%d", codepage, glyph);
			}
		}
		result += " };\n";
	}
	return result;
}

wxString CSourceDataGenerator::GetCodepageDefinitions(const EMGL_font_t *font)
{
	wxString result = wxString::Format("static const EMGL_codePage_t codePageTable[%d] = {", font->numCodepages);
	bool first = true;
	for (int codepage = 0; codepage < font->numCodepages; ++codepage) {
		EMGL_codePage_t *page = &font->codePagePtrs[codepage];
		int numGlyphs = page->endCode - page->startCode + 1;
		if (numGlyphs <= 0) continue;
		if (first) {
			first = false;
			result += "\n\t";
		}
		else {
			result += ",\n\t";
		}
		result += wxString::Format("{%u, %u, \"%s\", glyphTable_%d}", 
			page->startCode, page->endCode, page->name, codepage);
	}
	result += "\n};\n";
	return result;
}

wxString CSourceDataGenerator::GetFontDefinition(const EMGL_font_t *font, const wxString &name)
{
	wxString result;
	result += wxString::Format("const EMGL_font_t font_%s = {%d, %u, %u, codePageTable};\n",
		name.ToAscii(), font->ascender, font->bpp, font->numCodepages);
	return result;
}

wxString CSourceDataGenerator::GetSourceFile(const EMGL_font_t *font, wxString name)
{
	name = MakeCorrectName(name);
	return GetSourceIncludes(name) + "\n" +
		GetGlyphDataDefinitions(font) + "\n" +
		GetGlyphDefinitions(font) + "\n" +
		GetGlyphTableDefinition(font) + "\n" +
		GetCodepageDefinitions(font) + "\n" +
		GetFontDefinition(font, name);
}
