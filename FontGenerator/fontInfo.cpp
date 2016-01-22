#include "fontInfo.h"
#include <wx/wx.h>

#ifdef WIN32

#include <Windows.h>

#endif

#include "memdbg.h"

Glyph::Glyph(const FontInfo *fi, wxUint32 c)
{
	m_fi = fi;
	m_c = c;
	m_data = NULL;
	m_dataSize = 0;

	int height = fi->GetMaxSize().GetHeight();
	if (fi->IsTrueType())
	{
		ABC abc;
		if (GetCharABCWidths(fi->GetDC()->GetHDC(), c, c, &abc))
		{
			m_size = wxSize(abc.abcB, height);
			m_origin = abc.abcA;
			m_nextPos = abc.abcB + abc.abcC;
		}
		else{
			// Error
		}
	}
	else {
		int width;
		if (GetCharWidth32(fi->GetDC()->GetHDC(), c, c, &width))
		{
			m_size = wxSize(width, height);
			m_origin = 0;
			m_nextPos = width;
		}
	}
	LoadGlyphData();
}

Glyph::~Glyph()
{
	if (m_data != NULL)
	{
		free(m_data);
		m_data = NULL;
		m_dataSize = 0;
	}
}

wxSize Glyph::GetSize() const 
{
	return m_size;
}

wxPoint Glyph::GetOrigin() const
{
	return wxPoint(m_origin, 0);
}

wxPoint Glyph::GetNextPos() const
{
	return wxPoint(m_origin + m_nextPos, 0);
}

wxSize Glyph::GetTotalSize() const
{
	return wxSize(m_origin + m_nextPos, m_fi->GetMaxSize().GetHeight());
}

void Glyph::LoadGlyphData()
{
	if (m_data != NULL)
	{
		free(m_data);
		m_data = NULL;
		m_dataSize = 0;
	}

	int size = GetFontData(m_fi->GetDC()->GetHDC(), 0, 0, NULL,0);
	m_data = (wxUint8 *)malloc(size);
	int result = GetFontData(m_fi->GetDC()->GetHDC(), 0, 0, m_data, size);
	if (m_fi->IsTrueType())
	{
		MAT2 mat2 = {{ 0, 1 }, { 0, 0 }, { 0, 0 }, {0 ,1 } };
		GLYPHMETRICS metrics;
		int size = GetGlyphOutline(m_fi->GetDC()->GetHDC(), m_c, 
			GGO_GRAY8_BITMAP, &metrics, 0, NULL, &mat2);
		if (size <= 0)
		{
			// Error
			return;
		}
		m_data = (wxUint8 *)malloc(size);
		m_dataSize = size;

		if (GetGlyphOutline(m_fi->GetDC()->GetHDC(), m_c, 
			GGO_GRAY8_BITMAP, &metrics, m_dataSize, m_data, &mat2) == GDI_ERROR)
		{
			// Error 
			free(m_data);
			m_data = NULL;
			m_dataSize = 0;
			return;
		}
	}
	else {
		MAT2 mat2 = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };
		GLYPHMETRICS metrics;
		int size = GetGlyphOutline(m_fi->GetDC()->GetHDC(), m_c,
			GGO_GRAY8_BITMAP, &metrics, 0, NULL, &mat2);
		if (size <= 0)
		{
			// Error
			return;
		}
		m_data = (wxUint8 *) malloc(size);
		m_dataSize = size;

		if (GetGlyphOutline(m_fi->GetDC()->GetHDC(), m_c,
			GGO_GRAY8_BITMAP, &metrics, m_dataSize, m_data, &mat2) == GDI_ERROR)
		{
			// Error 
			free(m_data);
			m_data = NULL;
			m_dataSize = 0;
			return;
		}

	}
}

FontInfo::FontInfo(const wxFont &font)
{
	m_font = font;
	m_bitmap = wxBitmap(512,512,-1);
	m_dc.SelectObject(m_bitmap);
	m_dc.SetFont(font);
	m_dc.Clear();

	ReadFontData();
}

FontInfo::~FontInfo()
{

}

const wxDC *FontInfo::GetDC() const
{
	if (m_dc.IsOk())
	{
		return &m_dc;
	}
	else {
		return NULL;
	}
}

Glyph FontInfo::GetGlyph(wxUint32 c)
{
	return Glyph(this, c);
}

void FontInfo::ReadFontData()
{
	TEXTMETRIC metrics;
	if (!GetTextMetrics(m_dc.GetHDC(), &metrics))
	{
		wxLogFatalError("Could not get text metrics");
		return;
	}

	m_maxSize = wxSize(metrics.tmMaxCharWidth, metrics.tmHeight);
	m_avgSize = wxSize(metrics.tmAveCharWidth, metrics.tmHeight);
	m_ascent = metrics.tmAscent;
	m_descent = metrics.tmDescent;
	m_intLeading = metrics.tmInternalLeading;
	m_extLeading = metrics.tmExternalLeading;
	m_weight = metrics.tmWeight;
	m_overhang = metrics.tmOverhang;
	m_digitizedAspectX = metrics.tmDigitizedAspectX;
	m_digitizedAspectY = metrics.tmDigitizedAspectY;
	m_firstChar = metrics.tmFirstChar;
	m_lastChar = metrics.tmLastChar;
	m_defaultChar = metrics.tmDefaultChar;
	m_breakChar = metrics.tmBreakChar;
	m_italic = metrics.tmItalic != 0;
	m_underlined = metrics.tmUnderlined != 0;
	m_struckOut = metrics.tmStruckOut != 0;
	m_pitchAndFamily = metrics.tmPitchAndFamily;
	m_charSet = (wxFontEncoding)metrics.tmCharSet;
}

wxSize FontInfo::GetMaxSize() const
{
	return m_maxSize;
}

wxSize FontInfo::GetAvgSize() const
{
	return m_avgSize;
}

bool FontInfo::IsTrueType() const
{
	return (m_pitchAndFamily & TMPF_TRUETYPE) != 0;
}