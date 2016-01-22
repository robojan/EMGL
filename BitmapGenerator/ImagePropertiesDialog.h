#ifndef _IMAGEPROPERTIESDIALOG_H_
#define _IMAGEPROPERTIESDIALOG_H_

#include "image.h"

class ImagePropertiesDialog : public wxDialog
{
public:
	ImagePropertiesDialog(Image *image, wxWindow *parent, wxWindowID id, 
		const wxString &title, const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE,
		const wxString &name = wxDialogNameStr);
	
private:
	void OnScaleWTextChange(wxCommandEvent &evt);
	void OnScaleHTextChange(wxCommandEvent &evt);
	void OnCloseButton(wxCommandEvent &evt);
	void OnClose(wxCloseEvent &evt);
	void ApplyChanges(void);

	Image *m_image;
	wxPanel *m_content;
	wxCheckBox *m_fixAspectRatioCheckbox;
	wxCheckBox *m_hFlipCheckBox;
	wxCheckBox *m_vFlipCheckBox;
	wxTextCtrl *m_nameText;
	wxTextCtrl *m_scaleWText;
	wxTextCtrl *m_scaleHText;

	DECLARE_EVENT_TABLE();
};

#endif