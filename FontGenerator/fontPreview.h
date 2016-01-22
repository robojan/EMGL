#ifndef _FONTPREVIEW_H_
#define _FONTPREVIEW_H_

#include "font.h"
#include "charmap.h"
#include <wx/wx.h>

class GlyphPreviewWindow : public wxPanel
{
public:
	GlyphPreviewWindow(wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize, long style = wxBORDER_THEME);

	void SetPreviewGlyph(const CharMapEntry &entry);

private:
	void OnPaint(wxPaintEvent& evt);

	CharMapEntry m_entry;
	Font m_font;
};

class GlyphInfoPanel : public wxPanel
{
public:
	GlyphInfoPanel(wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize,
		long style = 0);

	void SetPreviewGlyph(const CharMapEntry &entry);

private:
	void UpdateInfo();
	
	wxStaticText *m_codeLabel;
	wxStaticText *m_fontLabel;
	wxStaticText *m_sizeLabel;
	wxStaticText *m_dataSizeLabel;
	GlyphPreviewWindow *m_preview;
	CharMapEntry m_entry;
};

#endif // ! _FONTPREVIEW_H_
