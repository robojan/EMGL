
#include "fontFamilyCombobox.h"

#include "memdbg.h"

FontFamilyComboBox::FontFamilyComboBox(wxWindow *parent, 
	wxWindowID id, const wxPoint &pos, const wxSize &size, long style, 
	const wxValidator &validator, const wxString &name) : 
	wxOwnerDrawnComboBox(parent, id, wxEmptyString, pos, size, 
		wxArrayString(), style | wxCB_READONLY, validator, name), 
	m_fontSize(15)
{

	UpdateContent();
}

FontFamilyComboBox::~FontFamilyComboBox()
{

}

void FontFamilyComboBox::UpdateContent()
{
	wxArrayString families = FontManager::GetInstalledFamilies();

	wxString currentSelection = wxItemContainer::GetStringSelection();
	wxItemContainer::Clear();
	wxItemContainer::Append(families);
	if (currentSelection.IsEmpty())
	{
		SetSelection(0);
	}
	else {
		wxItemContainer::SetStringSelection(currentSelection);
	}
}

wxString FontFamilyComboBox::GetStringSelection() const
{
	return wxItemContainer::GetStringSelection();
}

void FontFamilyComboBox::OnDrawItem(wxDC &dc, const wxRect &rect, int item, int flags) const
{
	if ((flags & wxODCB_PAINTING_CONTROL) != 0 || item == wxNOT_FOUND)
	{
		wxOwnerDrawnComboBox::OnDrawItem(dc, rect, item, flags);
		return;
	}
	dc.SetTextForeground(GetForegroundColour());
	wxString family = GetString(item);
	Font font = FontManager::GetFont(family, wxString(), 10);
	font.SetPixelSize(m_fontSize);
	int itemHeight = rect.GetHeight();
	wxPoint origin = rect.GetLeftTop();
	origin.x += 5;
	if (font.IsOk())
	{
		int fontHeight = font.GetHeight();
		origin.y += itemHeight/2 - fontHeight/2;
		if (font.HasNonDrawableSymbols(family))
		{
			dc.DrawText(family, origin);			
		}
		else {
			font.DrawText(dc, origin, family);
		}
	}
	else {
		dc.DrawText(_("Error"), origin);
	}

}

wxCoord FontFamilyComboBox::OnMeasureItem(size_t item) const
{
	return m_fontSize + 10;
}

wxCoord FontFamilyComboBox::OnMeasureItemWidth(size_t item) const
{
	return 300;
}

