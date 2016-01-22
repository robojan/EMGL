#include "fontPreview.h"

#include "fontManager.h"

#include "memdbg.h"

GlyphInfoPanel::GlyphInfoPanel(wxWindow *parent, 
	wxWindowID id /*= wxID_ANY*/, const wxPoint &pos /*= wxDefaultPosition*/, 
	const wxSize &size /*= wxDefaultSize*/, long style /*= 0*/) : 
	wxPanel(parent, id, pos, size, style)
{
	wxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	m_preview = new GlyphPreviewWindow(this, wxID_ANY, wxDefaultPosition, wxSize(size.GetWidth(), size.GetWidth()));
	m_preview->SetMinSize(wxSize(size.GetWidth(), size.GetWidth()));
	sizer->Add(m_preview, 1, wxSHAPED | wxCENTER, 3);

	wxSizer *sizerInfo = new wxGridSizer(2, 4, 4);

	m_codeLabel = new wxStaticText(this, wxID_ANY, wxEmptyString);
	sizerInfo->Add(m_codeLabel, 0, wxEXPAND, 3);
	m_fontLabel = new wxStaticText(this, wxID_ANY, wxEmptyString);
	sizerInfo->Add(m_fontLabel, 0, wxEXPAND, 3);
	m_sizeLabel = new wxStaticText(this, wxID_ANY, wxEmptyString);
	sizerInfo->Add(m_sizeLabel, 0, wxEXPAND, 3);
	m_dataSizeLabel = new wxStaticText(this, wxID_ANY, wxEmptyString);
	sizerInfo->Add(m_dataSizeLabel, 0, wxEXPAND, 3);

	UpdateInfo();

	sizer->Add(sizerInfo, 0, wxEXPAND, 3);

	SetSizerAndFit(sizer);
}

void GlyphInfoPanel::SetPreviewGlyph(const CharMapEntry &entry)
{
	m_entry = entry;
	m_preview->SetPreviewGlyph(entry);
	UpdateInfo();
}

void GlyphInfoPanel::UpdateInfo()
{
	if (m_entry.IsOK())
	{
		Font font = FontManager::GetFont(m_entry.GetFamily(), 
			m_entry.GetStyle(), m_entry.GetSize());
		font.SelectEncoding(m_entry.GetEncodingID());
		wxUint32 glyph = font.GetGlyphIndex(m_entry.GetCode());
		m_codeLabel->SetLabel(wxString::Format(_("%s, 0x%x"),
			m_entry.GetEncoding(), m_entry.GetCode()));
		m_fontLabel->SetLabel(wxString::Format(_("%s, %s, %g"), 
			font.GetFamily(), font.GetStyle(), font.GetSize()));
		wxSize size = font.GetGlyphSize(glyph);
		int width, height;
		int dataSize = font.GetGlyphBitmap(glyph, NULL, &width, &height);
		m_sizeLabel->SetLabel(wxString::Format(_("Size: %dpx x %dpx"), 
			width, height));
		m_dataSizeLabel->SetLabel(wxString::Format(_("Data size: %d bytes"), dataSize));

	}
	else {
		m_codeLabel->SetLabel(_(""));
		m_fontLabel->SetLabel(_(""));
		m_sizeLabel->SetLabel(_("Size: "));
		m_dataSizeLabel->SetLabel(_("Data size: "));
	}
}

GlyphPreviewWindow::GlyphPreviewWindow(wxWindow *parent, 
	wxWindowID id /*= wxID_ANY*/, const wxPoint &pos /*= wxDefaultPosition*/, 
	const wxSize &size /*= wxDefaultSize*/, long style /*= wxBORDER_THEME*/) :
	wxPanel(parent, id, pos, size, style | wxFULL_REPAINT_ON_RESIZE)
{
	SetBackgroundColour(*wxWHITE);
	SetMinSize(wxSize(50,50));

	Bind(wxEVT_PAINT, &GlyphPreviewWindow::OnPaint, this);
}

void GlyphPreviewWindow::SetPreviewGlyph(const CharMapEntry &entry)
{
	m_entry = entry;
	Refresh();
	if (entry.IsOK())
	{
		m_font = FontManager::GetFont(entry.GetFamily(), 
			entry.GetStyle(), entry.GetSize());
		m_font.SelectEncoding(entry.GetEncodingID());
	}
}

void GlyphPreviewWindow::OnPaint(wxPaintEvent& evt)
{
	wxPaintDC dc(this);
	if (m_entry.IsOK() && m_font.IsOk())
	{
		wxUint32 glyph = m_font.GetGlyphIndex(m_entry.GetCode());
		wxSize glyphSize = m_font.GetGlyphSize(glyph);
		wxSize clientSize = GetClientSize();

		if (glyphSize.GetHeight() != 0 && glyphSize.GetWidth() != 0)
		{
			float scale;
			if (glyphSize.GetWidth() > glyphSize.GetHeight())
			{
				scale = (float)clientSize.GetWidth()/glyphSize.GetWidth();
			}
			else {
				scale = (float) clientSize.GetHeight() / glyphSize.GetHeight();
			}
			scale *= 0.9f;
			int size = m_font.GetGlyphBitmap(glyph, NULL, NULL, NULL);
			if (size > 0)
			{
				int width, height;
				wxUint8 *bitmap = new wxUint8[size];
				wxSize glyphSize = m_font.GetGlyphSize(glyph);
				wxPoint bearing = m_font.GetGlyphBearing(glyph);
				m_font.GetGlyphBitmap(glyph, bitmap, &width, &height);

				wxImage image(width, height);
				unsigned char *rgb = image.GetData();
				for (int y = 0; y < height; y++)
				{
					for (int x = 0; x < width; x++)
					{
						int offset = ((y) * image.GetWidth() + x) * 3;
						wxASSERT(offset+2 < image.GetWidth()*image.GetHeight()*3);
						wxASSERT(offset >= 0);
						rgb[0 + offset] = 255 - bitmap[y * width + x];
						rgb[1 + offset] = 255 - bitmap[y * width + x];
						rgb[2 + offset] = 255 - bitmap[y * width + x];
					}
				}
				image.Rescale(width*scale, height*scale);
				
				wxPoint origin((clientSize.GetWidth() - glyphSize.GetWidth()*scale)/2,
					(clientSize.GetHeight() - glyphSize.GetHeight()*scale)/2);
				origin.y += bearing.y*scale;

				wxPoint glyphPos = origin + wxPoint(bearing.x, -bearing.y)*scale;
				dc.DrawBitmap(wxBitmap(image), glyphPos);
				dc.SetPen(*wxRED_PEN);
				dc.SetBrush(*wxTRANSPARENT_BRUSH);
				dc.DrawRectangle(glyphPos, wxSize(image.GetWidth(), image.GetHeight()));
				dc.SetPen(*wxGREEN_PEN);
				dc.DrawLine(0, origin.y, clientSize.GetWidth(), origin.y);
				dc.DrawLine(origin.x, 0, origin.x, clientSize.GetHeight());
				wxPoint advancePoint = origin += m_font.GetGlyphAdvance(glyph)*scale;
				dc.SetPen(*wxBLUE_PEN);
				dc.DrawLine(advancePoint.x - 10, advancePoint.y, advancePoint.x + 10, advancePoint.y);
				dc.DrawLine(advancePoint.x, advancePoint.y - 10, advancePoint.x, advancePoint.y + 10);


				delete [] bitmap;
			}
		}
	}
}