#ifndef _FONTFAMILYCOMBOBOX_H_
#define _FONTFAMILYCOMBOBOX_H_

#include <wx/wx.h>
#include <wx/odcombo.h>
#include "fontManager.h"
#include "font.h"

class FontFamilyComboBox : public wxOwnerDrawnComboBox
{
public:
	FontFamilyComboBox(wxWindow *parent, wxWindowID id, 
		const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize, long style = 0,
		const wxValidator &validator = wxDefaultValidator,
		const wxString &name= _("FontFamilyComboBox"));
	virtual ~FontFamilyComboBox();

	void UpdateContent();
	virtual wxString GetStringSelection() const;

protected:
	virtual void OnDrawItem(wxDC &dc, const wxRect &rect, int item, int flags) const;
	virtual wxCoord OnMeasureItem(size_t item) const;
	virtual wxCoord OnMeasureItemWidth(size_t item) const;

private:
	int m_fontSize;

};

#endif