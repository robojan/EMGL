#ifndef _IMAGEPANEL_H_
#define _IMAGEPANEL_H_

#include <wx/wx.h>
#include "image.h"

class wxImagePanel : public wxPanel
{
public:
	wxImagePanel(wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint &pos = wxDefaultPosition, 
		const wxSize &size = wxDefaultSize, 
		long style = wxTAB_TRAVERSAL,
		const wxString &name = wxPanelNameStr);

	void SetImage(Image *image);
	Image *GetImage();

private:
	void OnPaint(wxPaintEvent &evt);
	Image *m_image;
	wxBitmap m_bitmap;

	DECLARE_EVENT_TABLE();
};

#endif