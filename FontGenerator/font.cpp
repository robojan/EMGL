#include <wx/wx.h>
#include <wx/rawbmp.h>
#include "font.h"
#include "fontManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "memdbg.h"

Font::Font()
{
	m_face = NULL;
	m_dpix = 72;
	m_dpiy = 72;
}

Font::Font(const wxString &path, int fi, float size)
{
	m_face = NULL;
	m_refcnt = 1;
	FT_Error error = FT_New_Face(FontManager::GetFTLibrary(), path, fi, &m_face);
	if (error != FT_Err_Ok)
	{
		m_face = NULL;
		return;
	}

	SetSize(size);

	// Check if a char map is selected
	if (m_face->charmap == NULL && m_face->num_charmaps > 0)
	{
		error = FT_Select_Charmap(m_face, m_face->charmaps[0]->encoding);
		if (error != FT_Err_Ok)
		{
			return;
		}
	}
}

Font::Font(const Font &other)
{
	m_face = other.m_face;
	m_dpix = other.m_dpix;
	m_dpiy = other.m_dpiy;
	m_face = other.m_face;
	if (m_face != NULL)
	{
		FT_Reference_Face(m_face);
	}
}

Font::~Font()
{
	if (m_face != NULL && FontManager::IsOK())
	{
		FT_Done_Face(m_face);
		m_face = NULL;
	}
}

bool Font::IsOk()
{
	return m_face != NULL;
}

wxUint32 Font::GetGlyphIndex(wxUint32 code) const
{
	if (m_face->charmap->encoding == FT_ENCODING_MS_SYMBOL && code <= 0xFF)
	{
		code += 0xF000;
	}
	return FT_Get_Char_Index(m_face, code);
}

ArrayUint32 Font::GetGlyphIndices(const wxString &str) const
{
	wxASSERT(m_face != NULL);
	
	ArrayUint32 result;
	wxMBConv *conv = new wxMBConvUTF32;
	const wchar_t * const rawStr = str.c_str();
	size_t rawLength = str.length();
	size_t maxLen = conv->FromWChar(NULL, 0, rawStr, rawLength);
	if (maxLen == wxCONV_FAILED)
	{
		delete conv;
		return result;
	}
	wxUint32 *str32 = new wxUint32[maxLen/sizeof(wxUint32)];
	size_t utf32Len = conv->FromWChar(reinterpret_cast<char *>(str32),
		maxLen, rawStr, rawLength);
	if (utf32Len == wxCONV_FAILED)
	{
		delete conv;
		delete[] str32;
		return result;
	}
	for (size_t i = 0; i < utf32Len / sizeof(wxUint32); i++)
	{
		wxUint32 c = str32[i];
		FT_UInt index = GetGlyphIndex(c);
		result.Add(index);
	}
	delete conv;
	delete[] str32;
	return result;
}

void Font::DrawText(wxDC &dc, const wxPoint &pos, const wxString &str)
{
	wxASSERT(m_face != NULL);
	ArrayUint32 indices = GetGlyphIndices(str);
	wxPoint penPos = pos;

	// Move to origin
	penPos.y += m_face->size->metrics.ascender>>6;
	
	for (ArrayUint32::iterator it = indices.begin(); it != indices.end(); ++it)
	{
		DrawGlyph(dc, penPos, *it);
	}
}

void Font::DrawGlyph(wxDC &dc, wxPoint &pos, wxUint32 glyph)
{
	wxASSERT(m_face != NULL);
	wxASSERT(dc.IsOk());
	FT_Error error;
	error = FT_Load_Glyph(m_face, glyph, FT_LOAD_DEFAULT);
	if (error != FT_Err_Ok)
	{
		return;
	}

	error = FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);
	if (error != FT_Err_Ok)
	{
		return;
	}

	FT_Bitmap *glyphBitmap = &m_face->glyph->bitmap;
	FT_GlyphSlot slot = m_face->glyph;
	wxColour colour = dc.GetTextForeground();
	if (glyphBitmap->width != 0 && glyphBitmap->rows != 0)
	{
		wxImage image(glyphBitmap->width, glyphBitmap->rows);
		image.InitAlpha();
		unsigned char *rgb = image.GetData();
		unsigned char *a = image.GetAlpha();
		for (unsigned int y = 0; y < glyphBitmap->rows; y++)
		{
			for (unsigned int x = 0; x < glyphBitmap->width; x++)
			{
				rgb[0] = colour.Red();
				rgb[1] = colour.Green();
				rgb[2] = colour.Blue();
				wxUint8 p = GetBitmapGrayscale(x, y);
				*a = colour.Alpha()*p / 255;
				rgb += 3;
				a += 1;
			}
		}
		wxBitmap bmp(image);
		wxCoord x = pos.x + slot->bitmap_left;
		wxCoord y = pos.y - slot->bitmap_top;
		dc.DrawBitmap(bmp,x, y);
	}
	pos.x += slot->advance.x >> 6;
	pos.y += slot->advance.y >> 6;
}

wxUint8 Font::GetBitmapGrayscale(int x, int y) const
{
	wxASSERT(m_face != NULL);
	FT_Bitmap *glyphBitmap = &m_face->glyph->bitmap;
	wxUint8 result = 0;
	int index;

	switch (glyphBitmap->pixel_mode)
	{
	case FT_PIXEL_MODE_MONO:
		index = y * ((glyphBitmap->width+7)& ~0x7) + x;
		if ((glyphBitmap->buffer[index / 8] & (0x80 >> (index & 7))) != 0)
			result = 0xFF;
		else
			result = 0x00;
		break;
	case FT_PIXEL_MODE_GRAY:
		index = (y * glyphBitmap->pitch + x);
		result = glyphBitmap->buffer[index];
		break;
	default:
		wxLogError(_("Unknown font pixel mode: %d"), glyphBitmap->pixel_mode);
		break;		
	}
	return result;
}

wxPoint Font::GetGlyphAdvance(const wxString &c)
{
	wxASSERT(m_face != NULL);
	ArrayUint32 index = GetGlyphIndices(c);
	return GetGlyphAdvance(index[0]);
}

wxPoint Font::GetGlyphAdvance(wxUint32 glyph)
{
	wxASSERT(m_face != NULL);
	FT_Error error;
	error = FT_Load_Glyph(m_face, glyph, FT_LOAD_DEFAULT);
	if (error != FT_Err_Ok)
	{
		return wxPoint(0,0);
	}
	return wxPoint(m_face->glyph->advance.x>>6, m_face->glyph->advance.y>>6);
}

int Font::GetHeight() const
{
	wxASSERT(m_face != NULL);
	return m_face->size->metrics.height>>6;
}

wxArrayString Font::GetEncodings(wxUint32 **codes) const
{
	wxASSERT(m_face != NULL);
	wxArrayString result;
	if (codes != NULL)
	{
		*codes = new wxUint32[m_face->num_charmaps];
	}
	for (int i = 0; i < m_face->num_charmaps; i++)
	{
		FT_Encoding code = m_face->charmaps[i]->encoding;
		wxString encoding = FontManager::GetEncodingName(code);
		result.Add(encoding);
		if (codes != NULL)
		{
			(*codes)[i] = code;
		}
	}
	return result;
}

ArrayUint32 Font::GetEncodingIDs() const
{
	ArrayUint32 result;
	for (int i = 0; i < m_face->num_charmaps; i++)
	{
		FT_Encoding code = m_face->charmaps[i]->encoding;
		result.Add(code);
	}
	return result;
}

wxUint32 Font::GetSelectedEncodingId() const
{
	wxASSERT(m_face != NULL);
	wxASSERT(m_face->charmap != NULL);
	return m_face->charmap->encoding;
}

wxString Font::GetSelectedEncoding() const
{
	return FontManager::GetEncodingName(GetSelectedEncodingId());
}

void Font::SelectEncoding(wxUint32 encoding)
{
	wxASSERT(m_face != NULL);
	FT_Error error = FT_Select_Charmap(m_face, (FT_Encoding)encoding);
	if (error != FT_Err_Ok)
	{
		wxLogError(_("Could not select encoding"));
	}
}

wxString Font::GetUserDescription() const
{
	wxASSERT(m_face != NULL);
	wxString result;
	if (m_face == NULL)
	{
		return _("Not loaded");
	}
	if (m_face->family_name != NULL)
	{
		result += m_face->family_name;
	}
	else {
		result += _("Unknown");
	}
	if (m_face->style_name != NULL)
	{
		result += _(" ") + m_face->style_name;
	}
	float size = GetSize();
	result += wxString::Format(_(" %g"), size);
	return result;
}

bool Font::SetSize(float size, int dpix, int dpiy)
{
	m_dpix = dpix;
	m_dpiy = dpiy;
	FT_Error error = FT_Set_Char_Size(m_face, 0, static_cast<FT_F26Dot6>(size * 64.0), dpix, dpiy);
	if (error != FT_Err_Ok)
	{
		return false;
	}
	return true;
}

bool Font::SetPixelSize(int size)
{
	FT_Error error = FT_Set_Pixel_Sizes(m_face, 0, size);
	if (error != FT_Err_Ok)
	{
		return false;
	}
	return true;
}

Font & Font::operator=(const Font &other)
{
	m_face = other.m_face;
	m_dpix = other.m_dpix;
	m_dpiy = other.m_dpiy;
	m_face = other.m_face;
	if (m_face != NULL)
	{
		FT_Reference_Face(m_face);
	}
	return *this;
}

bool Font::HasNonDrawableSymbols(const wxString &str) const
{
	wxASSERT(m_face != NULL);
	ArrayUint32 indices = GetGlyphIndices(str);
	bool nondrawable = false;
	for (size_t i = 0; i < indices.size(); i++)
	{
		if (indices[i] == 0)
		{
			nondrawable = true;
		}
	}
	return nondrawable;
}

CharMap Font::GetCharMap() const
{
	wxString pageName = FontManager::GetEncodingName(m_face->charmap->encoding);
	FT_UInt index;
	FT_ULong code = FT_Get_First_Char(m_face, &index);
	if (index == 0)
	{
		return CharMap();
	}
	wxUint32 minChar = code;
	wxUint32 maxChar;
	CodePage page(minChar);
	CharMap result;
	while (index != 0)
	{
		maxChar = code;
		page[code] = CharMapEntry(GetFamily(), GetStyle(),
			GetSize(), m_face->charmap->encoding, code);
		code = FT_Get_Next_Char(m_face, code, &index);
		if (code - 10 > maxChar)
		{
			page.SetEnd(maxChar);
			page.SetName(wxString::Format(_("%s (0x%X-0x%X)"), pageName, minChar, maxChar));
			result.AddCodePage(page);
			minChar = code;
			page = CodePage(minChar);
		}
	}
	return result;
}

float Font::GetSize() const
{
	return m_face->size->metrics.y_ppem*72.0 / m_dpiy;
}

wxSize Font::GetGlyphSize(wxUint32 glyph) const
{
	wxASSERT(m_face != NULL);
	FT_Error error;
	error = FT_Load_Glyph(m_face, glyph, FT_LOAD_DEFAULT);
	if (error != FT_Err_Ok)
	{
		return wxSize(-1,-1);
	}
	/*
	error = FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);
	if (error != FT_Err_Ok)
	{
		return wxSize(-1, -1);
	}

	FT_Bitmap *glyphBitmap = &m_face->glyph->bitmap;
	return wxSize(glyphBitmap->width, glyphBitmap->rows);
	*/

	FT_Glyph_Metrics *metrics = &m_face->glyph->metrics;
	wxSize result;

	wxPoint bearing = GetGlyphBearing(glyph);
	int advance = metrics->horiAdvance >> 6;
	int height = metrics->height>>6;
	int width = metrics->width>>6;
	if (bearing.x < 0 && -bearing.x < width)
	{
		result.SetWidth(width);
	}
	else {
		if (bearing.x < 0)
		{
			result.SetWidth(-bearing.x + width);
		}
		else {
			result.SetWidth(bearing.x + width);
		}
	}
	if (advance > result.GetWidth())
	{
		result.SetWidth(advance);
	}
	if (bearing.y > 0)
	{
		if (bearing.y > height)
		{
			result.SetHeight(bearing.y);
		}
		else {
			result.SetHeight(height);
		}
	}
	else {
		result.SetHeight(-bearing.y + height);
	}

	return result;
}

wxPoint Font::GetGlyphBearing(wxUint32 glyph) const
{
	wxASSERT(m_face != NULL);
	FT_Error error;
	error = FT_Load_Glyph(m_face, glyph, FT_LOAD_DEFAULT);
	if (error != FT_Err_Ok)
	{
		return wxPoint(0, 0);
	}

	FT_Glyph_Metrics *metrics = &m_face->glyph->metrics;
	return wxPoint(metrics->horiBearingX>>6, metrics->horiBearingY>>6);
}

wxString Font::GetFamily() const
{
	wxASSERT(m_face != NULL);
	if (m_face->family_name)
	{
		return m_face->family_name;
	}
	return wxString();
}

wxString Font::GetStyle() const
{
	wxASSERT(m_face != NULL);
	if (m_face->style_name)
	{
		return m_face->style_name;
	}
	return wxString();
}

wxString Font::GetGlyphName(wxUint32 glyph)
{
	wxASSERT(m_face != NULL);
	FT_String str[256];
	FT_Error err = FT_Get_Glyph_Name(m_face, glyph, str, 256);
	if (err == FT_Err_Ok && str[0] != 0)
	{
		return str;
	}
	return wxEmptyString;
}

int Font::GetGlyphBitmap(wxUint32 glyph, wxUint8 *data, int *width, int *height)
{
	wxASSERT(m_face != NULL);
	FT_Error error;
	error = FT_Load_Glyph(m_face, glyph, FT_LOAD_DEFAULT);
	if (error != FT_Err_Ok)
	{
		return -1;
	}

	error = FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);
	if (error != FT_Err_Ok)
	{
		return -1;
	}

	FT_Bitmap *glyphBitmap = &m_face->glyph->bitmap;
	if (width)
	{
		*width = glyphBitmap->width;
	}
	if (height)
	{
		*height = glyphBitmap->rows;
	}
	if (data)
	{
		for (unsigned int y = 0; y < glyphBitmap->rows; y++)
		{
			for (unsigned int x = 0; x < glyphBitmap->width; x++)
			{
				data[y*glyphBitmap->width+x] = GetBitmapGrayscale(x,y);
			}
		}
	}
	return glyphBitmap->width*glyphBitmap->rows;
}

