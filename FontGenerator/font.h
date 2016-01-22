#ifndef _FONT_H_
#define _FONT_H_

struct FT_FaceRec_;
class FontManager;

#include <wx/wx.h>
#include "types.h"
#include "charmap.h"

class Font
{
public:
	Font();
	Font(const Font &other);
	Font(const wxString &path, int fi, float size);
	~Font();

	bool IsOk();
	wxUint32 GetGlyphIndex(wxUint32 code) const;
	ArrayUint32 GetGlyphIndices(const wxString &str) const;
	void DrawText(wxDC &dc, const wxPoint &pos, const wxString &str);
	void DrawGlyph(wxDC &dc, wxPoint &pos, wxUint32 glyph);
	wxPoint GetGlyphAdvance(const wxString &c);
	wxPoint GetGlyphAdvance(wxUint32 glyph);
	wxString GetUserDescription() const;
	int GetHeight() const;
	wxArrayString GetEncodings(wxUint32 **codes = NULL) const;
	ArrayUint32 GetEncodingIDs() const;
	wxString GetSelectedEncoding() const;
	wxUint32 GetSelectedEncodingId() const;
	void SelectEncoding(wxUint32 encoding);
	bool SetSize(float size, int dpix = 72, int dpiy = 72);
	bool SetPixelSize(int size);
	bool HasNonDrawableSymbols(const wxString &str) const;
	float GetSize() const;
	wxSize GetGlyphSize(wxUint32 glyph) const;
	Font &operator=(const Font &other);

	CharMap GetCharMap() const;
	wxString GetFamily() const;
	wxString GetStyle() const;
	wxString GetGlyphName(wxUint32 glyph);
	wxPoint GetGlyphBearing(wxUint32 glyph) const;

	int GetGlyphBitmap(wxUint32 glyph, wxUint8 *data, int *width, int *height);
private:
	wxUint8 GetBitmapGrayscale(int x, int y) const;
	int m_dpix, m_dpiy;
	FT_FaceRec_ *m_face;
	int m_refcnt;
};

#endif