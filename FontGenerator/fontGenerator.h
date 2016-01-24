#ifndef _FONTGENERATOR_H_
#define _FONTGENERATOR_H_

#include <wx/wx.h>
#include <wx/fontpicker.h>
#include "fontPreview.h"
#include "fontManager.h"
#include "font.h"
#include "fontFamilyCombobox.h"
#include "charmapWidget.h"
#include "IDataGenerator.h"

class FontGenerator: public wxApp
{
private:
	virtual bool OnInit();
	virtual int OnExit();
};

class FontGeneratorFrame: public wxFrame
{
public:
	FontGeneratorFrame(const wxString &title, const wxPoint &pos,
		const wxSize &size);
	~FontGeneratorFrame();

private:
	wxPanel *CreateFontChooser(wxWindow *parent);
	wxPanel *CreateNewCodePagePanel(wxWindow *parent);
	void SetStyles(const wxString &family);
	void SetSizes(const wxString &family, const wxString &style);
	void SetEncodings(const wxString &family, const wxString &style);
	Font LoadSelectedFont();
	void GetSelectedCodePage(CodePage &out, const struct encodingDef *preset);
	void OnFontFamilyChanged(wxCommandEvent &evt);
	void OnFontStyleChanged(wxCommandEvent &evt);
	void OnOpenFont(wxCommandEvent &evt);
	void OnLoadFont(wxCommandEvent &evt);
	void OnImportFont(wxCommandEvent &evt);
	void OnCharmapSelection(wxGridEvent &evt);
	void OnLoadAllFont(wxCommandEvent &evt);
	void OnLoadPresetFont(wxCommandEvent &evt);
	void OnImportAllFont(wxCommandEvent &evt);
	void OnImportPresetFont(wxCommandEvent &evt);
	void OnCreateCodepage(wxCommandEvent &evt);
	void OnMapCellRightClick(wxGridEvent &evt);
	void OnFontMapDrag(wxGridEvent &evt);
	void OnRemoveGlyph(wxCommandEvent &evt);
	void OnRemoveCharmap(wxCommandEvent &evt);
	void OnRenameCharmapClick(wxCommandEvent &evt);
	void OnSetCharmapOffset(wxCommandEvent &evt);
	void OnImportGlyph(wxCommandEvent &evt);
	void OnSaveOutputButton(wxCommandEvent &evt);
	void AdvanceCharmapCursor(CharMapWidget *widget);

	void SetSizeFromString(const wxString &str, Font &font);

	GlyphInfoPanel *m_glyphInfoPanel;
	CharMapWidget *m_charmapWidget;
	CharMapWidget *m_fontCharmapWidget;

	FontFamilyComboBox *m_fontFamilyPicker;
	wxChoice *m_fontStylePicker;
	wxComboBox *m_fontSizePicker;
	//wxChoice *m_encodingPicker;

	//wxChoice *m_outputEncodingPicker;
	wxCheckBox *m_compressionCheckbox;

	wxTextCtrl *m_addCodepageStartTextbox;
	wxTextCtrl *m_addCodepageEndTextbox;
	wxTextCtrl *m_addCodepageNameTextbox;

	std::vector<IDataGenerator *> m_dataGenerators;

	wxDECLARE_EVENT_TABLE();
};


#endif