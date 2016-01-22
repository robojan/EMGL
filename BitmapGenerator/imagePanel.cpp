#include "imagePanel.h"

wxBEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
EVT_PAINT(OnPaint)
wxEND_EVENT_TABLE();

wxImagePanel::wxImagePanel(wxWindow *parent, wxWindowID id,
	const wxPoint &pos,	const wxSize &size,
	long style,	const wxString &name)
	: wxPanel(parent, id, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name)
{
	m_image = NULL;
}

void wxImagePanel::SetImage(Image *image)
{
	m_image = image;
	this->Refresh();
}

Image *wxImagePanel::GetImage()
{
	return m_image;
}

void wxImagePanel::OnPaint(wxPaintEvent &evt)
{
	wxPaintDC dc(this);
	if (m_image != NULL && m_image->GetImage().IsOk())
	{
		wxMemoryDC imageDC;
		imageDC.SelectObject(wxBitmap(m_image->GetConvertedImage()));
		
		wxSize imageSize = wxSize(m_image->GetWidth(), m_image->GetHeight());
		wxSize panelSize = this->GetClientSize();
		wxDouble xScale = ((wxDouble) panelSize.GetWidth()) / imageSize.GetWidth();
		wxDouble yScale = ((wxDouble) panelSize.GetHeight()) / imageSize.GetHeight();
		wxSize resultSize;
		wxPoint resultPos;
		if (xScale > yScale)
		{
			resultSize = imageSize*yScale;
			resultPos.y = 0;
			resultPos.x = (panelSize.GetWidth() - resultSize.GetWidth())/2;
		}
		else {
			resultSize = imageSize*xScale;
			resultPos.y = (panelSize.GetHeight() - resultSize.GetHeight())/2;
			resultPos.x = 0;
		}
		dc.StretchBlit(resultPos, resultSize, &imageDC, 
			wxPoint(0,0), imageSize);
	}
}