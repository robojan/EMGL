#ifndef _FONTINFO_H_
#define _FONTINFO_H_

#include <wx/wx.h>

class Glyph;

class FontInfo
{
public:
	FontInfo(const wxFont &font);
	virtual ~FontInfo();

	const wxDC *GetDC() const;

	Glyph GetGlyph(wxUint32 c);

	wxSize GetMaxSize() const;
	wxSize GetAvgSize() const;
	bool IsTrueType() const;

private:
	void ReadFontData();

	wxFont m_font;
	wxMemoryDC m_dc;
	wxBitmap m_bitmap;
	wxSize m_maxSize;
	wxSize m_avgSize;
	int m_ascent;
	int m_descent;
	int m_intLeading;
	int m_extLeading;
	int m_weight;
	int m_overhang;
	int m_digitizedAspectX;
	int m_digitizedAspectY;
	wxUint32 m_firstChar;
	wxUint32 m_lastChar;
	wxUint32 m_defaultChar;
	wxUint32 m_breakChar;
	bool m_italic;
	bool m_underlined;
	bool m_struckOut;
	char m_pitchAndFamily; // todo converted in universal format
	wxFontEncoding m_charSet;

};

class Glyph
{
public:
	Glyph(const FontInfo *fi, wxUint32 c);
	virtual ~Glyph();

	wxSize GetSize() const;
	wxSize GetTotalSize() const;
	wxPoint GetOrigin() const;
	wxPoint GetNextPos() const;
	


private:
	void LoadGlyphData();

	const FontInfo *m_fi;
	wxUint32 m_c;
	wxSize m_size;
	int m_origin;
	int m_nextPos;
	wxUint8 *m_data;
	int m_dataSize;
};

#endif