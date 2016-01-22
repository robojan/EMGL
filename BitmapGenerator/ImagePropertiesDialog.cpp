
#include "ImagePropertiesDialog.h"
#include "ids.h"

#include <wx/valnum.h>

wxBEGIN_EVENT_TABLE(ImagePropertiesDialog, wxDialog)
EVT_TEXT(ID_PROPWSCALE, OnScaleWTextChange)
EVT_TEXT(ID_PROPHSCALE, OnScaleHTextChange)
EVT_CLOSE(OnClose)
EVT_BUTTON(wxID_OK, OnCloseButton)
EVT_BUTTON(wxID_CANCEL, OnCloseButton)
wxEND_EVENT_TABLE();

ImagePropertiesDialog::ImagePropertiesDialog(Image *image, wxWindow *parent,
	wxWindowID id, const wxString &title, const wxPoint &pos,
	const wxSize &size, long style, const wxString &name) :
	wxDialog(parent, id, title, pos, size, style, name)
{
	m_image = image;
	m_content = new wxPanel(this, wxID_ANY, wxDefaultPosition, size);
	wxSizer *sizerV1 = new wxBoxSizer(wxVERTICAL);

	// Renaming options
	wxStaticBoxSizer *sizerName = new wxStaticBoxSizer(wxHORIZONTAL,
		m_content, _("Name"));
	wxStaticText *label0 = new wxStaticText(m_content, wxID_ANY, 
		_("Name: "));
	sizerName->Add(label0, 0, wxEXPAND, 3);
	m_nameText = new wxTextCtrl(m_content, wxID_ANY, image->GetName());
	sizerName->Add(m_nameText, 1, wxEXPAND, 3);
	sizerV1->Add(sizerName, 0, wxEXPAND, 3);

	// Scaling options
	wxIntegerValidator<int> validator(NULL, wxNUM_VAL_DEFAULT);

	wxStaticBoxSizer *sizerScale = new wxStaticBoxSizer(wxVERTICAL, 
		m_content, _("Scaling"));
	m_fixAspectRatioCheckbox = new wxCheckBox(m_content, wxID_ANY, 
		_("Maintain aspect ratio"));
	m_fixAspectRatioCheckbox->SetValue(true);
	sizerScale->Add(m_fixAspectRatioCheckbox, 0, wxEXPAND, 3);
	wxStaticText *label1 = new wxStaticText(m_content, wxID_ANY, 
		wxString::Format(_("Current image size: %dpx*%dpx"),
		image->GetImage().GetWidth(), 
		image->GetImage().GetHeight()));
	sizerScale->Add(label1, 0, wxEXPAND, 3);
	wxSizer *sizerH1 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *label2 = new wxStaticText(m_content, wxID_ANY,
		_("Size: "));
	sizerH1->Add(label2, 0, wxEXPAND, 3);
	m_scaleWText = new wxTextCtrl(m_content, ID_PROPWSCALE,
		wxString::Format(_("%d"), image->GetImage().GetWidth()),
		wxDefaultPosition, wxDefaultSize, 0, validator);
	sizerH1->Add(m_scaleWText, 0, wxEXPAND, 3);
	wxStaticText *label3 = new wxStaticText(m_content, wxID_ANY, 
		_("px * "));
	sizerH1->Add(label3, 0, wxEXPAND, 3);
	m_scaleHText = new wxTextCtrl(m_content, ID_PROPHSCALE,
		wxString::Format(_("%d"), image->GetImage().GetHeight()),
		wxDefaultPosition, wxDefaultSize, 0, validator);
	sizerH1->Add(m_scaleHText, 0, wxEXPAND, 3);
	wxStaticText *label4 = new wxStaticText(m_content, wxID_ANY,
		_("px"));
	sizerH1->Add(label4, 0, wxEXPAND, 3);
	sizerScale->Add(sizerH1, 0, wxEXPAND, 3);
	sizerV1->Add(sizerScale,0,wxEXPAND, 3);

	// Add Image options
	wxStaticBoxSizer *sizerOptions = new wxStaticBoxSizer(wxVERTICAL,
		m_content, _("Options"));
	m_hFlipCheckBox = new wxCheckBox(m_content, wxID_ANY, 
		_("Horizontal image mirror"));
	m_hFlipCheckBox->SetValue(image->GetHFlip());
	sizerOptions->Add(m_hFlipCheckBox, 0, wxEXPAND, 3);
	m_vFlipCheckBox = new wxCheckBox(m_content, wxID_ANY,
		_("Vertical image mirror"));
	m_vFlipCheckBox->SetValue(image->GetVFlip());
	sizerOptions->Add(m_vFlipCheckBox, 0, wxEXPAND, 3);
	sizerV1->Add(sizerOptions, 0, wxEXPAND, 3);

	// Add buttons
	sizerV1->AddStretchSpacer();
	
	
	wxSizer *sizerButtons = new wxBoxSizer(wxHORIZONTAL);
	sizerButtons->AddStretchSpacer(1);
	wxButton *okButton = new wxButton(m_content, wxID_OK, _("Ok"));
	this->SetAffirmativeId(wxID_OK);
	sizerButtons->Add(okButton, 0, wxEXPAND, 3);
	wxButton *cancelButton = new wxButton(m_content, wxID_CANCEL, 
		_("Cancel"));
	this->SetEscapeId(wxID_CANCEL);
	sizerButtons->Add(cancelButton, 0, wxEXPAND, 3);
	sizerV1->Add(sizerButtons, 0, wxEXPAND, 3);
	
	m_content->SetSizer(sizerV1);
	sizerV1->SetSizeHints(this);
}

void ImagePropertiesDialog::OnScaleWTextChange(wxCommandEvent &evt)
{
	if (!m_fixAspectRatioCheckbox->GetValue())
	{
		return;
	}
	long width, height;
	double ratio = ((double)m_image->GetImage().GetWidth())/
		m_image->GetImage().GetHeight();
	if (m_scaleWText->GetValue().ToLong(&width))
	{
		height = (long)(width / ratio);
		m_scaleHText->ChangeValue(wxString::Format(_("%d"), height));
	}

}

void ImagePropertiesDialog::OnScaleHTextChange(wxCommandEvent &evt)
{
	if (!m_fixAspectRatioCheckbox->GetValue())
	{
		return;
	}
	long width, height;
	double ratio = ((double) m_image->GetImage().GetWidth()) /
		m_image->GetImage().GetHeight();
	if (m_scaleHText->GetValue().ToLong(&height))
	{
		width = (long) (height * ratio);
		m_scaleWText->ChangeValue(wxString::Format(_("%d"), width));
	}
}

void ImagePropertiesDialog::OnClose(wxCloseEvent &evt)
{
	EndModal(wxID_CANCEL);
	ApplyChanges();
}

void ImagePropertiesDialog::OnCloseButton(wxCommandEvent &evt)
{
	wxButton *button = (wxButton *) evt.GetEventObject();
	EndModal(button->GetId());
	ApplyChanges();
}

void ImagePropertiesDialog::ApplyChanges(void)
{
	if (GetReturnCode() != wxID_OK)
	{
		return;
	}
	long width, height;
	m_scaleWText->GetValue().ToLong(&width);
	m_scaleHText->GetValue().ToLong(&height);

	m_image->SetWidth(width);
	m_image->SetHeight(height);
	m_image->SetName(m_nameText->GetValue());
	m_image->SetHFlip(m_hFlipCheckBox->GetValue());
	m_image->SetVFlip(m_vFlipCheckBox->GetValue());
}

