#include "BinaryDataGenerator.h"
#include <wx/wfstream.h>
#include <wx/wx.h>
#include <wx/datstrm.h>
#include <wx/gdicmn.h>
#include "charmap.h"
#include "font.h"
#include "fontManager.h"
#include "OutputHelper.h"

BinaryDataGenerator::BinaryDataGenerator()
{
}


BinaryDataGenerator::~BinaryDataGenerator()
{
}

wxString BinaryDataGenerator::GetWildCardString()
{
	return "Binary format(*.bin)|*.bin";
}

bool BinaryDataGenerator::IsFilenameSupported(const wxFileName &fileName)
{
	return fileName.GetExt() == "bin";
}

void BinaryDataGenerator::SaveCharmap(const CharMap &charmap, bool compress,
	enum OutputFormat format, const wxString &filePath)
{
	// Try opening the file for writing
	wxFFileOutputStream file(filePath);
	if (!file.IsOk()) {
		wxLogError(_("Could not open the file for writing"));
		return;
	}
	wxDataOutputStream out(file);
	out.BigEndianOrdered(false);
	out.UseBasicPrecisions();

	int numCodePages = charmap.GetCountCodePages();
	int headerSize = sizeof(wxUint32) + sizeof(wxInt16) + 2 * sizeof(wxUint8) + numCodePages * sizeof(wxUint32);
	int codePageBaseSize = sizeof(wxUint32) * 2 + 16;

	wxASSERT(numCodePages <= UINT8_MAX);
	if(compress) wxLogDebug("Compression not supported yet");
	// TODO: add compression
	// Write header
	wxUint32 magic = 'EMGF';
	wxUint8 flags = format;
	if (compress) flags |= FLAG_COMPRESSED;
	int ascender = 0;
	int ascenderDiv = 0;
	out.Write32(magic);
	file.SeekO(sizeof(wxInt16), wxFromCurrent);
	out.Write8(flags);
	out.Write8((wxUint8)numCodePages);

	wxUint32 *codepagePtrs = new wxUint32[numCodePages];
	wxUint32 **glyphPtrs = new wxUint32*[numCodePages];

	// Write codepages
	file.SeekO(headerSize);
	std::list<CodePage *>::const_iterator it = charmap.CBegin();
	for (int i = 0; i < numCodePages; i++) {
		wxASSERT(it != charmap.CEnd());
		codepagePtrs[i] = file.TellO();
		out.Write32((*it)->GetRangeStart());
		out.Write32((*it)->GetRangeEnd());
		const wxString &name = (*it)->GetName();
		for (unsigned int j = 0; j < 16; ++j) {
			out.Write8(j < name.Length() ? name.ToAscii()[j] : '\0');
		}
		glyphPtrs[i] = new wxUint32[(*it)->GetSize()];
		file.SeekO(sizeof(wxUint32) * (*it)->GetSize(), wxFromCurrent);
	}
	// Write glyphs
	it = charmap.CBegin();
	for (int i = 0; i < numCodePages; i++) {
		wxASSERT(it != charmap.CEnd());
		for (unsigned int j = 0; j < (*it)->GetSize(); j++) {
			const CharMapEntry *entry = (*it)->GetCharMapEntry((*it)->GetRangeStart() + j);
			if (entry == NULL) {
				glyphPtrs[i][j] = 0;
				continue;
			}
			LoadFont(entry->GetFamily(), entry->GetStyle(), entry->GetSize(), entry->GetEncodingID());
			wxUint32 glyph;
			if (!m_loadedFont.IsOk() || (glyph = m_loadedFont.GetGlyphIndex(entry->GetCode())) == 0)
			{
				wxLogWarning("Could not load glyph with code %u", (*it)->GetRangeStart() + j);
				glyphPtrs[i][j] = 0;
				continue;
			}
			wxPoint advance = m_loadedFont.GetGlyphAdvance(glyph);
			wxPoint bitmapTL = m_loadedFont.GetGlyphBitmapTL(glyph);
			int bitmapWidth, bitmapHeight;
			int bitmapSize = m_loadedFont.GetGlyphBitmap(glyph, NULL, &bitmapWidth, &bitmapHeight);
			if (bitmapSize < 0) {
				wxLogWarning("Error while getting bitmap for glyph with code %u", (*it)->GetRangeStart() + j);
				glyphPtrs[i][j] = 0;
				continue;
			}
			wxUint8 *bitmap = new wxUint8[bitmapSize];
			int result = m_loadedFont.GetGlyphBitmap(glyph, bitmap, &bitmapWidth, &bitmapHeight);
			wxASSERT(result == bitmapSize);

			bitmapSize = ConvertToFormat(bitmap, bitmapSize, format);

			if (compress) {
				CompressBitmap(&bitmap, &bitmapSize,  bitmapWidth * bitmapHeight, format);
			}

			ascender += m_loadedFont.GetAscender();
			ascenderDiv++;

			glyphPtrs[i][j] = file.TellO();
			out.Write16((wxInt16)advance.x);
			out.Write16((wxInt16)advance.y);
			out.Write16((wxInt16)bitmapTL.x);
			out.Write16((wxInt16)bitmapTL.y);
			out.Write16((wxUint16)bitmapWidth);
			out.Write16((wxUint16)bitmapHeight);
			out.Write32(bitmapSize);
			file.Write(bitmap, bitmapSize & ~(1<<31));
			delete[] bitmap;
		}
	}
	// Write glyph pointers
	it = charmap.CBegin();
	for (int i = 0; i < numCodePages; i++) {
		wxASSERT(it != charmap.CEnd());
		file.SeekO(codepagePtrs[i] + codePageBaseSize);
		for (unsigned int j = 0; j < (*it)->GetSize(); j++) {
			out.Write32(glyphPtrs[i][j]);
		}
		delete[] glyphPtrs[i];
		file.Sync();
	}
	delete[] glyphPtrs;
	
	// Write codepage pointers
	file.SeekO(headerSize - numCodePages * sizeof(wxUint32));
	for (int i = 0; i < numCodePages; i++) {
		out.Write32(codepagePtrs[i]);
	}
	delete[] codepagePtrs;
	delete[] glyphPtrs;

	// Write font wide parameters
	file.SeekO(sizeof(wxUint32));
	ascender = ascenderDiv > 0 ? ascender / ascenderDiv : 0;
	wxASSERT(ascender >= INT16_MIN && ascender <= INT16_MAX);
	out.Write16(ascender);

	file.Close();
}

void BinaryDataGenerator::LoadFont(wxString family, wxString style, float size, wxUint32 encoding)
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
