
#include "fontGenerator.h"
#include "id.h"
#include "fontInfo.h"
#include "charmap.h"
#include <wx/wx.h>
#include <wx/progdlg.h>
#include <wx/dnd.h>
#include <wx/msgdlg.h>
#include <wx/numdlg.h>
#include "types.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "memdbg.h"

wxIMPLEMENT_APP(FontGenerator);

bool FontGenerator::OnInit()
{
	FontManager::Init();
	FontGeneratorFrame *frame = new FontGeneratorFrame(_("EMGL font generator"), 
		wxDefaultPosition, wxSize(900, 500));
	frame->Show(true);
	return true;
}

int FontGenerator::OnExit()
{
	FontManager::Release();
	return 0;
}

wxBEGIN_EVENT_TABLE(FontGeneratorFrame, wxFrame)
wxEND_EVENT_TABLE();

FontGeneratorFrame::FontGeneratorFrame(const wxString &title, 
	const wxPoint &pos, const wxSize &size): 
	wxFrame(NULL, wxID_ANY, title, pos, size)
{
	wxPanel *mainPanel = new wxPanel(this);
	wxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);

	wxPanel *optionsPanel = new wxPanel(mainPanel);
	optionsPanel->SetMinSize(wxSize(300, -1));
	wxSizer *optionsSizer = new wxBoxSizer(wxVERTICAL);

	m_glyphInfoPanel = new GlyphInfoPanel(optionsPanel, wxID_ANY, wxDefaultPosition, wxSize(210, -1));
	wxPanel *fontPicker = CreateFontChooser(optionsPanel);
	wxPanel *addCodepage = CreateNewCodePagePanel(optionsPanel);
	m_compressionCheckbox = new wxCheckBox(optionsPanel, wxID_ANY, _("Use compression"));
	m_compressionCheckbox->SetValue(true);
	wxButton *saveOutputButton = new wxButton(optionsPanel, wxID_ANY, _("Generate fonts"));
	
	wxSizer *useSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton *useButton = new wxButton(optionsPanel, wxID_ANY, _("<"));
	useSizer->Add(useButton, 0, wxSHAPED, 3);

	optionsSizer->Add(m_glyphInfoPanel, 0, wxEXPAND | wxALIGN_CENTER, 3);
	optionsSizer->Add(fontPicker, 0, wxEXPAND | wxALIGN_CENTER, 3);
	optionsSizer->Add(addCodepage, 0, wxEXPAND | wxALIGN_CENTER, 3);
	optionsSizer->Add(useSizer, 0, wxEXPAND, 3);
	optionsSizer->AddStretchSpacer(1);
	optionsSizer->Add(m_compressionCheckbox, 0, wxEXPAND, 3);
	optionsSizer->Add(saveOutputButton, 0, wxEXPAND, 3);


	optionsPanel->SetSizerAndFit(optionsSizer);

	wxSizer *previewSizer = new wxBoxSizer(wxVERTICAL);
	m_charmapWidget = new CharMapWidget(mainPanel);
	m_charmapWidget->ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_ALWAYS);
	m_charmapWidget->Bind(wxEVT_GRID_SELECT_CELL, 
		&FontGeneratorFrame::OnCharmapSelection, this);
	m_charmapWidget->Bind(wxEVT_GRID_CELL_RIGHT_CLICK,
		&FontGeneratorFrame::OnMapCellRightClick, this);
	wxStaticText *freetypeLabel = new wxStaticText(mainPanel, wxID_ANY, 
		_("Portions of this software are copyright \u00A9 2015\n"
		"The FreeType Project(www.freetype.org). All rights reserved."));

	previewSizer->Add(m_charmapWidget, 1, wxEXPAND, 3);
	previewSizer->Add(freetypeLabel, 0, wxEXPAND, 0);
	
	m_fontCharmapWidget = new CharMapWidget(mainPanel);
	m_fontCharmapWidget->ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_ALWAYS);
	m_fontCharmapWidget->Bind(wxEVT_GRID_SELECT_CELL,
		&FontGeneratorFrame::OnCharmapSelection, this);
	m_fontCharmapWidget->Bind(wxEVT_GRID_CELL_BEGIN_DRAG,
		&FontGeneratorFrame::OnFontMapDrag, this);

	
	mainSizer->Add(previewSizer, 0, wxEXPAND, 2);
	mainSizer->Add(optionsPanel, 0, wxEXPAND, 2);
	mainSizer->Add(m_fontCharmapWidget, 0, wxEXPAND, 2);
	mainPanel->SetSizerAndFit(mainSizer);
	this->SetSize(wxSize(mainPanel->GetBestSize().GetWidth()+20, 600));
}

wxPanel *FontGeneratorFrame::CreateNewCodePagePanel(wxWindow *parent)
{
	wxPanel *panel = new wxPanel(parent); 

	wxBoxSizer *sizer = new wxStaticBoxSizer(wxVERTICAL, panel, _("New codepage"));
	wxGridSizer *sizer2 = new wxGridSizer(2, 3, 3);

	wxStaticText *startLabel = new wxStaticText(panel, wxID_ANY, _("Start:"));
	sizer2->Add(startLabel);
	m_addCodepageStartTextbox = new wxTextCtrl(panel, wxID_ANY, _("0"), 
		wxDefaultPosition, wxSize(100,-1), wxTE_PROCESS_ENTER, 
		wxTextValidator(wxFILTER_DIGITS));
	sizer2->Add(m_addCodepageStartTextbox);
	wxStaticText *endLabel = new wxStaticText(panel, wxID_ANY, _("End:"));
	sizer2->Add(endLabel);
	m_addCodepageEndTextbox = new wxTextCtrl(panel, wxID_ANY, _("255"),
		wxDefaultPosition, wxSize(100, -1), wxTE_PROCESS_ENTER,
		wxTextValidator(wxFILTER_DIGITS));
	sizer2->Add(m_addCodepageEndTextbox);
	wxStaticText *nameLabel = new wxStaticText(panel, wxID_ANY, _("Name:"));
	sizer2->Add(nameLabel);
	m_addCodepageNameTextbox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(100, -1), wxTE_PROCESS_ENTER);
	sizer2->Add(m_addCodepageNameTextbox);
	sizer->Add(sizer2);
	wxButton *addCodepageButton = new wxButton(panel, wxID_ANY, _("Add codepage"));
	sizer->Add(addCodepageButton);
	panel->SetSizerAndFit(sizer);

	addCodepageButton->Bind(wxEVT_BUTTON, &FontGeneratorFrame::OnCreateCodepage, this);

	return panel;
}

wxPanel *FontGeneratorFrame::CreateFontChooser(wxWindow *parent)
{
	const std::map<wxString, FontEntry> installedFonts = FontManager::GetInstalledFonts();
	wxArrayString fontFamilies = FontManager::GetInstalledFamilies();
	wxPanel *fontPicker = new wxPanel(parent);

	wxBoxSizer *sizer1 = new wxStaticBoxSizer(wxVERTICAL, fontPicker, _("Select font"));
	wxGridSizer *sizer2 = new wxGridSizer(2, 2, 2);
	m_fontFamilyPicker = new FontFamilyComboBox(fontPicker, wxID_ANY, 
		wxDefaultPosition, wxSize(100, -1));
	sizer2->Add(m_fontFamilyPicker, 0, wxEXPAND, 3);
	wxButton *addFontButton = new wxButton(fontPicker, wxID_ANY, _("Open file"));
	sizer2->Add(addFontButton, 0, wxEXPAND, 3);
	wxArrayString fontStyles;
	m_fontStylePicker = new wxChoice(fontPicker, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, fontStyles);
	sizer2->Add(m_fontStylePicker, 0, wxEXPAND, 3);
	wxArrayString fontSizes;
	m_fontSizePicker = new wxComboBox(fontPicker, wxID_ANY, "", wxDefaultPosition,
		wxDefaultSize, fontStyles, wxTE_PROCESS_ENTER);
	sizer2->Add(m_fontSizePicker, 0, wxEXPAND, 3);

	//wxStaticText *encodingLabel = new wxStaticText(fontPicker, wxID_ANY, _("Encoding:"));
	//sizer1->Add(encodingLabel, 0, wxEXPAND, 3);
	//wxArrayString encodings;
	//m_encodingPicker = new wxChoice(fontPicker, wxID_ANY, wxDefaultPosition,
	//	wxDefaultSize, encodings);
	//sizer1->Add(m_encodingPicker, 0, wxEXPAND, 3);

	wxButton *importFontButton = new wxButton(fontPicker, wxID_ANY, _("Import"));
	sizer2->Add(importFontButton, 0, wxEXPAND, 3);
	wxButton *loadFontButton = new wxButton(fontPicker, wxID_ANY, _("Load"));
	sizer2->Add(loadFontButton, 0, wxEXPAND, 3);
	sizer1->Add(sizer2);

	m_fontFamilyPicker->Bind(wxEVT_COMBOBOX, 
		&FontGeneratorFrame::OnFontFamilyChanged, this);
	m_fontStylePicker->Bind(wxEVT_CHOICE, 
		&FontGeneratorFrame::OnFontStyleChanged, this);
	addFontButton->Bind(wxEVT_BUTTON,
		&FontGeneratorFrame::OnOpenFont, this);
	importFontButton->Bind(wxEVT_BUTTON, 
		&FontGeneratorFrame::OnImportFont, this);
	loadFontButton->Bind(wxEVT_BUTTON,
		&FontGeneratorFrame::OnLoadFont, this);
	m_fontFamilyPicker->SetSelection(0);
	SetStyles(fontFamilies[0]);

	fontPicker->SetSizerAndFit(sizer1);
	return fontPicker;
}

void FontGeneratorFrame::SetStyles(const wxString &family)
{
	wxArrayString styles;
	styles = FontManager::GetStyles(family);
	wxString oldSelection = m_fontStylePicker->GetStringSelection();
	m_fontStylePicker->Clear();
	m_fontStylePicker->Append(styles);

	wxString style = FontManager::GetDefaultStyle(family);

	if (!oldSelection.IsEmpty())
	{
		if (m_fontStylePicker->SetStringSelection(oldSelection))
		{
			style = oldSelection;
		}
	}

	m_fontStylePicker->SetStringSelection(style);
	SetSizes(family, style);
	SetEncodings(family, style);
}

void FontGeneratorFrame::SetSizes(const wxString &family, const wxString &style)
{
	wxString oldSize = m_fontSizePicker->GetStringSelection();
	
	m_fontSizePicker->Clear();
	wxArrayDouble sizes = FontManager::GetSizes(family, style);
	wxArrayString sizesStr;
	for (wxArrayDouble::iterator it = sizes.begin(); it != sizes.end(); ++it)
	{
		sizesStr.Add(wxString::Format(_("%g"), *it));
	}
	m_fontSizePicker->Clear();
	m_fontSizePicker->Append(sizesStr);
	if (oldSize.IsEmpty())
	{
		m_fontSizePicker->SetSelection(0);
	}
	else {
		if (!m_fontSizePicker->SetStringSelection(oldSize))
		{
			if (FontManager::IsScaleable(family, style))
			{
				m_fontSizePicker->SetValue(oldSize);
			}
			else {
				m_fontSizePicker->SetSelection(0);
			}
		}
	}
}

void FontGeneratorFrame::SetEncodings(const wxString &family, const wxString &style)
{
	Font font = FontManager::GetFont(family,style, 10.0f);
	if (font.IsOk())
	{
		wxUint32 *encodingCodes;
		wxArrayString encodings = font.GetEncodings(&encodingCodes);
		//m_encodingPicker->Clear();
		//m_encodingPicker->Append(encodings, (void **) encodingCodes);
		//m_encodingPicker->SetStringSelection(font.GetSelectedEncoding());
		delete [] encodingCodes;
	}
	else {
		//m_encodingPicker->Clear();
	}
}

void FontGeneratorFrame::OnFontFamilyChanged(wxCommandEvent &evt)
{
	wxArrayString families = m_fontFamilyPicker->GetStrings();
	wxString family = families[m_fontFamilyPicker->GetSelection()];

	SetStyles(family);
}

void FontGeneratorFrame::OnFontStyleChanged(wxCommandEvent &evt)
{
	wxString family = m_fontFamilyPicker->GetStringSelection();
	wxArrayString styles = m_fontStylePicker->GetStrings();
	wxString style = styles[m_fontStylePicker->GetSelection()];


	SetSizes(family, style);
	SetEncodings(family, style);
}

void FontGeneratorFrame::SetSizeFromString(const wxString &str, Font &font)
{
	wxString sizeStr = str;
	double size = 10;
	sizeStr.Trim(false);
	sizeStr.Trim(true);
	bool pixelMode = false;
	int index = sizeStr.find_first_not_of("0123456789", 0);
	wxString sizeNum = sizeStr.substr(0, index);
	sizeNum.ToDouble(&size);
	if (index != -1)
	{
		sizeStr = sizeStr.Mid(index);
		sizeStr.Trim(false);
		sizeStr = sizeStr.Upper();
		if (sizeStr.StartsWith(_("PX")))
		{
			pixelMode = true;
		}
	}
	if (font.IsOk())
	{
		if (pixelMode)
		{
			if (!font.SetPixelSize(static_cast<int>(size)))
			{
				wxLogWarning(_("Could not set size: %gpx"), size);
			}
		}
		else {
			if (!font.SetSize(size))
			{
				wxLogWarning(_("Could not set size: %gpx"), size);
			}

		}
	}
}

Font FontGeneratorFrame::LoadSelectedFont()
{
	wxString family = m_fontFamilyPicker->GetStringSelection();
	wxString style = m_fontStylePicker->GetStringSelection();
	wxString sizeStr = m_fontSizePicker->GetValue();
	Font font = FontManager::GetFont(family, style, 10.0f);
	if (font.IsOk())
	{
		SetSizeFromString(sizeStr, font);
		//wxUint32 code = (wxUint32) m_encodingPicker->GetClientData(m_encodingPicker->GetSelection());
		//font.SelectEncoding(code);
	}
	return font;
}

void FontGeneratorFrame::GetSelectedCodePage(CodePage &out, const struct encodingDef *preset)
{
	const struct encodingCharDef *codeptr = preset->codes;
	wxUint32 minCode = codeptr->code;
	wxUint32 maxCode = codeptr->code;
	while (codeptr->valid != ENCODING_VALID_END)
	{
		if (codeptr->code < minCode)
			minCode = codeptr->code;
		if (codeptr->code > maxCode)
			maxCode = codeptr->code;
		codeptr++;
	}
	out = CodePage(minCode, maxCode, preset->name);
	codeptr = preset->codes;

	Font font = LoadSelectedFont();
	unsigned char validMask = 0;
	ArrayUint32 encodingIDs = font.GetEncodingIDs();
	for (ArrayUint32::iterator it = encodingIDs.begin();
	it != encodingIDs.end(); ++it)
	{
		switch (*it)
		{
		case FT_ENCODING_APPLE_ROMAN:
			validMask |= ENCODING_APPLEROMAN_VALID;
			break;
		case FT_ENCODING_MS_SYMBOL:
			validMask |= ENCODING_MSSYMBOL_VALID;
			break;
		case FT_ENCODING_UNICODE:
			validMask |= ENCODING_UNICODE_VALID;
			break;
		}
	}
	while (codeptr->valid != ENCODING_VALID_END)
	{
		unsigned char flags = codeptr->valid & validMask;
		if (flags & ENCODING_UNICODE_VALID)
		{
			out[codeptr->code] = CharMapEntry(font.GetFamily(),
				font.GetStyle(), font.GetSize(), FT_ENCODING_UNICODE,
				codeptr->unicode);
		}
		else if (flags & ENCODING_MSSYMBOL_VALID) {
			out[codeptr->code] = CharMapEntry(font.GetFamily(),
				font.GetStyle(), font.GetSize(), FT_ENCODING_MS_SYMBOL,
				codeptr->msSymbol);
		}
		else if (flags & ENCODING_APPLEROMAN_VALID) {
			out[codeptr->code] = CharMapEntry(font.GetFamily(),
				font.GetStyle(), font.GetSize(), FT_ENCODING_APPLE_ROMAN,
				codeptr->appleRoman);
		}
		codeptr++;
	}
}

void FontGeneratorFrame::OnFontMapDrag(wxGridEvent &evt) {
	wxDropSource dndSource;
	CharMapDataObject dndData;
	CharMapWidget *widget = dynamic_cast<CharMapWidget *>(evt.GetEventObject());
	wxGridCellCoordsArray tl = widget->GetSelectionBlockTopLeft();
	wxGridCellCoordsArray br = widget->GetSelectionBlockBottomRight();
	wxGrid::wxGridSelectionModes mode = widget->GetSelectionMode();
	wxGridCellCoordsArray selection = widget->GetSelectedCells();
	for (size_t i = 0; i < selection.size(); i++) {
		wxString glyph = widget->GetCharmapTable()->GetValue(selection[i].GetRow(), 
			selection[i].GetCol());
		dndData.AddGlyphString(glyph);
	}
	
	
	wxLogMessage(_("Drag evt cell %d,%d"), evt.GetRow(), evt.GetCol());
}

void FontGeneratorFrame::OnRemoveGlyph(wxCommandEvent &evt)
{
	wxVariant *variant = dynamic_cast<wxVariant *>(evt.GetEventUserData());
	wxASSERT(variant->GetType() == wxT("void*"));
	CharMapWidget &widget =	*(CharMapWidget *)variant->GetVoidPtr();
	wxGridCellCoordsArray &cells(widget.GetAllSelectedCells());
	cells.push_back(wxGridCellCoords(widget.GetGridCursorRow(), 
		widget.GetGridCursorCol()));
	for (size_t i = 0; i < cells.size(); ++i) {
		widget.RemoveGlyph(cells[i].GetRow(), cells[i].GetCol());
	}
}

void FontGeneratorFrame::OnRemoveCharmap(wxCommandEvent &evt)
{
	wxVariant *variant = dynamic_cast<wxVariant *>(evt.GetEventUserData());
	wxASSERT(variant->GetType() == wxT("void*"));
	CharMapWidget &widget = *(CharMapWidget *)variant->GetVoidPtr();
	int selectedRow = widget.GetCursorRow();
	wxASSERT(widget.GetCharmapTable()->IsTitleRow(selectedRow));
	CodePage *page = widget.GetCharmapTable()->GetCodePage(selectedRow);
	wxASSERT(page != NULL);
	int answer = wxMessageBox("Are you sure you want to remove the codepage?", "Confirm", wxYES_NO, this);
	if (answer == wxYES) {
		widget.GetCharmapTable()->RemoveCodePage(*page);
	}
}

void FontGeneratorFrame::OnRenameCharmapClick(wxCommandEvent &evt)
{
	wxVariant *variant = dynamic_cast<wxVariant *>(evt.GetEventUserData());
	wxASSERT(variant->GetType() == wxT("void*"));
	CharMapWidget &widget = *(CharMapWidget *)variant->GetVoidPtr();
	int selectedRow = widget.GetCursorRow();
	wxASSERT(widget.GetCharmapTable()->IsTitleRow(selectedRow));
	widget.SetGridCursor(selectedRow, 0);
	if (widget.CanEnableCellControl()) {
		widget.EnableCellEditControl(true);
	}
}

void FontGeneratorFrame::OnSetCharmapOffset(wxCommandEvent &evt)
{
	wxVariant *variant = dynamic_cast<wxVariant *>(evt.GetEventUserData());
	wxASSERT(variant->GetType() == wxT("void*"));
	CharMapWidget &widget = *(CharMapWidget *)variant->GetVoidPtr();
	int selectedRow = widget.GetCursorRow();
	wxASSERT(widget.GetCharmapTable()->IsTitleRow(selectedRow));
	CodePage *page = widget.GetCharmapTable()->GetCodePage(selectedRow);
	wxASSERT(page != NULL);
	int pageSize = page->GetSize();
	int answer = wxGetNumberFromUser("Set the starting offset of the codepage.", "Starting offset:", 
		"Set offset", page->GetRangeStart(), 0, INT32_MAX - pageSize, this);
	if (answer != -1) {
		CodePage pageBackup = *page;
		widget.GetCharmapTable()->RemoveCodePage(*page);
		if (!widget.GetCharmapTable()->IsRangeEmpty(answer, answer + pageSize - 1)) {
			wxMessageBox(_("Could not change offset, because there is no space at the location specified."), _("Error"));
		}
		else {
			pageBackup.Shift(answer - pageBackup.GetRangeStart());
		}
		widget.AddCodePage(pageBackup);
	}
}

void FontGeneratorFrame::OnOpenFont(wxCommandEvent &evt)
{
	wxFileDialog openDialog(this, _("Open font from file"), wxEmptyString, 
		wxEmptyString, "Font files|*.ttf;*.pfa;*.pfb;*.bdf;*.cff;*.pcf;*.fnt;*.pfr|All files|*.*",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	wxString path = openDialog.GetPath();
	FontManager::AddFont(path);

	m_fontFamilyPicker->UpdateContent();
}

void FontGeneratorFrame::OnLoadFont(wxCommandEvent &evt)
{
	wxMenuItem *item;
	wxMenu menu;

	int id = wxID_HIGHEST + 100;
	item = menu.Append(id, _("All"), _("Load the complete font"));
	menu.Bind(wxEVT_MENU, &FontGeneratorFrame::OnLoadAllFont, this,id);
	id++;
	
	const struct encodingDef **encodings = defaultEncodings;
	while (*encodings != NULL)
	{
		menu.Append(id, (*encodings)->name);
		menu.Bind(wxEVT_MENU, &FontGeneratorFrame::OnLoadPresetFont, this,id,
			wxID_ANY, new wxVariant((void*)*encodings));
		encodings++;
		id++;
	}
	this->PopupMenu(&menu);
	m_fontCharmapWidget->ClearGrid();
}

void FontGeneratorFrame::OnImportFont(wxCommandEvent &evt)
{
	wxMenuItem *item;
	wxMenu menu;

	int id = wxID_HIGHEST + 100;
	item = menu.Append(id, _("All"), _("Import the complete font"));
	menu.Bind(wxEVT_MENU, &FontGeneratorFrame::OnImportAllFont, this, id);
	id++;

	const struct encodingDef **encodings = defaultEncodings;
	while (*encodings != NULL)
	{
		menu.Append(id, (*encodings)->name);
		menu.Bind(wxEVT_MENU, &FontGeneratorFrame::OnImportPresetFont, this, id,
			wxID_ANY, new wxVariant((void*)*encodings));
		encodings++;
		id++;
	}
	this->PopupMenu(&menu);
	m_fontCharmapWidget->ClearGrid();
}

void FontGeneratorFrame::OnMapCellRightClick(wxGridEvent &evt) {
	CharMapWidget *widget = dynamic_cast<CharMapWidget *>(evt.GetEventObject());
	wxWindow *window = dynamic_cast<wxWindow *>(evt.GetEventObject());
	wxPoint point = evt.GetPosition();
	wxMenu menu;

	widget->SetFocus();

	int id = wxID_HIGHEST + 100;
	bool isCodepageCell = widget->GetCharmapTable()->IsTitleRow(evt.GetRow());
	if (isCodepageCell) {
		widget->SetGridCursor(evt.GetRow(), evt.GetCol());
		menu.Append(id + 0, _("Remove"));
		menu.Bind(wxEVT_MENU, &FontGeneratorFrame::OnRemoveCharmap, this, id + 0,
			wxID_ANY, new wxVariant((void *)widget));
		menu.Append(id + 1, _("Rename"));
		menu.Bind(wxEVT_MENU, &FontGeneratorFrame::OnRenameCharmapClick, this, id + 1,
			wxID_ANY, new wxVariant((void *)widget));
		menu.Append(id + 2, _("Set offset"));
		menu.Bind(wxEVT_MENU, &FontGeneratorFrame::OnSetCharmapOffset, this, id + 2,
			wxID_ANY, new wxVariant((void *)widget));
	}
	else {
		if (!widget->IsSelection()) {
			widget->SetGridCursor(evt.GetRow(), evt.GetCol());
		}
		menu.Append(id + 0, _("Remove"));
		menu.Bind(wxEVT_MENU, &FontGeneratorFrame::OnRemoveGlyph, this, id + 0,
			wxID_ANY, new wxVariant((void *)widget));
	}
	window->PopupMenu(&menu, point);
}

void FontGeneratorFrame::OnCharmapSelection(wxGridEvent &evt)
{
	CharMapWidget *widget = dynamic_cast<CharMapWidget *>(evt.GetEventObject());
	CharMapEntry *entry = widget->GetEntry(evt.GetRow(), evt.GetCol());
	if (entry != NULL)
	{
		m_glyphInfoPanel->SetPreviewGlyph(*entry);
	}
	else {
		m_glyphInfoPanel->SetPreviewGlyph(CharMapEntry());
	}
}

void FontGeneratorFrame::OnLoadAllFont(wxCommandEvent &evt)
{
	wxProgressDialog progress(_("Loading font..."), _("Loading font ..."));
	progress.Show();
	progress.Pulse();
	Font font = LoadSelectedFont();
	progress.Pulse();
	CharMap map = font.GetCharMap();
	progress.Pulse();
	m_fontCharmapWidget->AddCharmap(map);
}

void FontGeneratorFrame::OnLoadPresetFont(wxCommandEvent &evt)
{
	wxVariant *variant = dynamic_cast<wxVariant *>(evt.GetEventUserData());
	wxASSERT(variant->GetType() == wxT("void*"));
	const struct encodingDef *preset = 
		(const struct encodingDef *)variant->GetVoidPtr();
	wxASSERT(preset != NULL);
	CodePage page;
	GetSelectedCodePage(page, preset);
	m_fontCharmapWidget->AddCodePage(page);
}

void FontGeneratorFrame::OnImportAllFont(wxCommandEvent &evt)
{
	wxProgressDialog progress(_("Importing font..."), _("Importing font ..."));
	progress.Show();
	progress.Pulse();
	Font font = LoadSelectedFont();
	progress.Pulse();
	CharMap map = font.GetCharMap();
	progress.Pulse();
	m_charmapWidget->AddCharmap(map);
}

void FontGeneratorFrame::OnImportPresetFont(wxCommandEvent &evt)
{
	wxVariant *variant = dynamic_cast<wxVariant *>(evt.GetEventUserData());
	wxASSERT(variant->GetType() == wxT("void*"));
	const struct encodingDef *preset =
		(const struct encodingDef *)variant->GetVoidPtr();
	wxASSERT(preset != NULL);
	CodePage page;
	GetSelectedCodePage(page, preset);
	m_charmapWidget->AddCodePage(page);
}

void FontGeneratorFrame::OnCreateCodepage(wxCommandEvent &evt)
{
	long start, end;
	if (!m_addCodepageStartTextbox->GetValue().ToLong(&start))
	{
		wxLogError(_("Start value not valid!"));
		return;
	}
	if (!m_addCodepageEndTextbox->GetValue().ToLong(&end))
	{
		wxLogError(_("End value not valid!"));
		return;
	}
	if (end <= start || start < 0) {
		wxLogError(_("Invallid start or end value"));
		return;
	}
	wxString name = m_addCodepageNameTextbox->GetValue();
	if (name.IsEmpty())
	{
		name = wxString::Format(_("Codepage(0x%x - 0x%x)"), start, end);
	}
	m_charmapWidget->AddCodePage(CodePage(start, end, name));
}
