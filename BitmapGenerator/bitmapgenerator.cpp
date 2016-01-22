
#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/progdlg.h>
#include <wx/filename.h.>
#include <wx/ffile.h>
#include <vector>
#include "bitmapgenerator.h"
#include "ids.h"
#include "image.h"
#include "ImagePropertiesDialog.h"


wxIMPLEMENT_APP(BitmapGenerator);

bool BitmapGenerator::OnInit()
{
	BitmapGeneratorFrame *frame = new BitmapGeneratorFrame(_("Bitmap Generator"), 
		wxDefaultPosition, wxDefaultSize);
	frame->Show(true);
	wxInitAllImageHandlers();
	return true;
}

wxBEGIN_EVENT_TABLE(BitmapGeneratorFrame, wxFrame)
EVT_BUTTON(ID_OPENIMAGE, OnOpenImageButtonClick)
EVT_LIST_END_LABEL_EDIT(ID_IMAGELIST,OnImageNameEdit)
EVT_LIST_ITEM_FOCUSED(ID_IMAGELIST, OnImageListSelectionChanged)
EVT_LIST_ITEM_RIGHT_CLICK(ID_IMAGELIST, OnImageListRightClick)
EVT_LIST_DELETE_ITEM(ID_IMAGELIST, OnImageDeleted)
EVT_MENU(ID_IMAGEPROPERTIES, OnImageProperties)
EVT_MENU(ID_IMAGERENAME, OnImageRename)
EVT_MENU(ID_IMAGEREMOVE, OnImageRemove)
EVT_BUTTON(ID_SAVEBITMAPS, OnGenerateFiles)
wxEND_EVENT_TABLE()

static const wxString modeChoices [] = {
	_("EmGL mode"),
	_("C header files mode"),
	_("C header & source files mode"),
};

static const wxString originChoices [] = {
	_("Top left origin"), 
	_("Bottom left origin"),
	_("Top right origin"),
	_("Bottom right origin"),
};

BitmapGeneratorFrame::BitmapGeneratorFrame(const wxString& title, 
	const wxPoint& pos, const wxSize& size)
		: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	m_contextMenuItem = NULL;
	m_framePanel = new wxPanel(this);

	wxSizer *sizerH1 = new wxBoxSizer(wxHORIZONTAL);
	wxSizer *sizerOptions = new wxBoxSizer(wxVERTICAL);
	wxSizer *sizerV1 = new wxBoxSizer(wxVERTICAL);
	sizerOptions->SetMinSize(wxSize(170,200));
	m_imagePanel = new wxImagePanel(m_framePanel, wxID_ANY);
	m_imagePanel->SetMinSize(wxSize(200, 200));
	m_imagePanel->SetBackgroundColour(*wxBLACK);
	sizerV1->Add(m_imagePanel, 1, wxEXPAND, 3);

	wxSizer *sizerInfo = new wxBoxSizer(wxHORIZONTAL);
	wxString label = _("Image size: 0px * 0px(0 pixels)\n"
		"RGBA8888 size: 0 bytes");
	m_imageSizeText = new wxStaticText(m_framePanel, wxID_ANY, label);
	sizerInfo->Add(m_imageSizeText);
	sizerInfo->AddSpacer(20);
	label = _("Number of Images: 0\nTotal RGBA8888 size: 0 bytes");
	m_statsText = new wxStaticText(m_framePanel, wxID_ANY, label);
	sizerInfo->Add(m_statsText);

	sizerV1->Add(sizerInfo, 0, wxEXPAND, 3);


	wxButton *selectImageButton = new wxButton(m_framePanel, ID_OPENIMAGE, _("Open image"));
	sizerOptions->Add(selectImageButton, 0, wxEXPAND, 3);
	m_imageListView = new wxListView(m_framePanel, ID_IMAGELIST,
		wxDefaultPosition, wxDefaultSize, wxLC_ICON | wxLC_EDIT_LABELS | wxBORDER_THEME);
	m_imageListView->SetImageList(new wxImageList(96,96,false), wxIMAGE_LIST_NORMAL);
	sizerOptions->Add(m_imageListView, 1, wxEXPAND, 3);
	m_originChoice = new wxChoice(m_framePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
		wxArrayString(sizeof(originChoices) / sizeof(wxString), originChoices));
	m_originChoice->SetSelection(1);
	sizerOptions->Add(m_originChoice, 0, wxEXPAND, 3);
	m_modeChoice = new wxChoice(m_framePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
		wxArrayString(sizeof(modeChoices) / sizeof(wxString), modeChoices));
	m_modeChoice->SetSelection(0);
	sizerOptions->Add(m_modeChoice, 0, wxEXPAND, 3);

		

	wxButton *saveButton = new wxButton(m_framePanel, ID_SAVEBITMAPS, _("Generate files"));
	sizerOptions->Add(saveButton, 0, wxEXPAND, 3);

	sizerH1->Add(sizerOptions, 0, wxEXPAND, 3);
	sizerH1->Add(sizerV1, 1, wxEXPAND, 3);
	m_framePanel->SetSizer(sizerH1);
	sizerH1->SetSizeHints(this);
}

void BitmapGeneratorFrame::AddImage(Image *image)
{
	static int imageIndex = 0;
	m_images[imageIndex] = image;

	// Update list
	wxListItem item;
	item.SetText(image->GetName());
	wxImageList *imList = m_imageListView->GetImageList(wxIMAGE_LIST_NORMAL);
	wxImage icon = image->GetImage().Copy();
	icon.Rescale(96, 96);
	int iconIndex = imList->Add(wxBitmap(icon));
	item.SetImage(iconIndex);
	item.SetId(0);
	item.SetData(imageIndex);
	m_imageListView->InsertItem(item);
	imageIndex++;
}

void BitmapGeneratorFrame::RecalculateStats()
{
	int size = 0;
	int images = 0;


	for (std::map<int, Image *>::iterator it = m_images.begin();
		it != m_images.end(); ++it)
	{
		images++;
		wxSize imageSize = it->second->GetImage().GetSize();
		size += imageSize.GetWidth()*imageSize.GetHeight()*4;
	}

	wxString label = wxString::Format(
		_("Number of Images: %d\nTotal RGBA8888 size: %d bytes"),
		images, size);
	m_statsText->SetLabel(label);
	m_framePanel->GetSizer()->RecalcSizes();
}

void BitmapGeneratorFrame::OnOpenImageButtonClick(wxCommandEvent &evt)
{
	wxString imageWildcard = _("Images") + wxImage::GetImageExtWildcard();
	wxFileDialog openFileDialog(this, _("Open image files"), wxEmptyString,
		wxEmptyString, imageWildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);

	// Show the file dialog
	if (openFileDialog.ShowModal() == wxID_CANCEL)
	{
		// Canceled
		return;
	}


	wxArrayString paths;
	openFileDialog.GetPaths(paths);
	wxGenericProgressDialog busy(_("Loading images"), _("Loading images"), paths.size());
	int i =0;
	for (wxArrayString::const_iterator path = paths.begin();
		path != paths.end(); path++)
	{
		Image *image = new Image(*path);
		if (image->GetImage().IsOk())
		{
			AddImage(image);
		}
		else {
			delete image;
		}
		i++;
		busy.Update(i);
	}
	RecalculateStats();
}

void BitmapGeneratorFrame::OnImageNameEdit(wxListEvent &evt)
{
	int id = evt.GetItem().GetData();
	m_images.at(id)->SetName(evt.GetLabel());
	m_imageListView->Arrange();
}

void BitmapGeneratorFrame::OnImageListSelectionChanged(wxListEvent &evt)
{
	int id = evt.GetItem().GetData();
	Image *image = m_images.at(id);
	UpdateDisplayedImage(image);
}

void BitmapGeneratorFrame::OnImageDeleted(wxListEvent &evt)
{
	int id = evt.GetItem().GetData();
	delete m_images.at(id);
	m_images.erase(id);
}

void BitmapGeneratorFrame::OnImageListRightClick(wxListEvent &evt)
{
	wxMenu menu;
	menu.Append(ID_IMAGEPROPERTIES, _("&Properties"));
	menu.Append(ID_IMAGERENAME, _("&Rename"));
	menu.Append(ID_IMAGEREMOVE, _("Remove"));
	m_contextMenuItem = evt.GetIndex();
	PopupMenu(&menu, evt.GetPoint());
}

void BitmapGeneratorFrame::OnImageProperties(wxCommandEvent &evt)
{
	int index = m_imageListView->GetItemData(m_contextMenuItem);
	Image *image = m_images.at(index);
	ImagePropertiesDialog dialog(image,this, wxID_ANY, _("Properties"));

	dialog.ShowModal();
	UpdateDisplayedImage(image);
	m_imageListView->SetItemText(m_contextMenuItem, image->GetName());
	RecalculateStats();
}

void BitmapGeneratorFrame::OnImageRename(wxCommandEvent &evt)
{
	m_imageListView->EditLabel(m_contextMenuItem);
}

void BitmapGeneratorFrame::OnImageRemove(wxCommandEvent &evt)
{
	if (wxMessageBox(_("Are you sure you want to remove those images?"),
		_("Are you sure?"), wxYES_NO | wxCENTRE, this) == wxNO)
	{
		return;
	}

	std::vector<long> items;
	long item = -1;
	while (1)
	{
		item = m_imageListView->GetNextSelected(item);
		if (item == -1)
		{
			break;
		}
		items.push_back(item);
	}

	for (int i = items.size()-1; 0<=i; i--)
	{
		m_imageListView->DeleteItem(items[i]);
	}
	m_imageListView->Arrange();
	RecalculateStats();
}

void BitmapGeneratorFrame::OnGenerateFiles(wxCommandEvent &evt)
{
	std::map<int, Image*>::iterator it1;
	std::map<int, Image*>::iterator it2;
	// Check for duplicate names
	for (it1 = m_images.begin(); it1 != m_images.end(); it1++)
	{
		for (it2 = m_images.begin(); it2 != m_images.end(); it2++)
		{
			if (it1 == it2)
			{
				continue;
			}

			if (it1->second->GetName() == it2->second->GetName())
			{
				wxLogError(_("Duplicate name detected: %s\n"
					"Non alphanumeric characters will be converted to '_'"), 
					it1->second->GetName());
				return;
			}
		}
	}

	switch (m_modeChoice->GetSelection())
	{
		case 0:
			GenerateFilesEMGL();
			break;
		case 1: 
			GenerateFilesHeaderOnly();
			break;
		case 2:
			GenerateFilesHeaderAndSource();
			break;
		default:
			wxLogError(_("Unknown mode selected"));
			break;
	}
}

void BitmapGeneratorFrame::GenerateFilesEMGL()
{
	wxFileDialog saveFileDialog(this, _("Save EMGL files"), wxEmptyString,
		wxEmptyString, _T(""), wxFD_SAVE);
	saveFileDialog.SetMessage(_("Filename will be a prefix to the output files.\n"
		"e.g. bitmap will become bitmap.c and bitmap.h"));
	// Show the file dialog
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
	{
		// Canceled
		return;
	}

	wxFileName selectedFilePath = wxFileName(saveFileDialog.GetPath());
	wxFileName sourceFilePath = selectedFilePath;
	wxFileName headerFilePath = selectedFilePath;

	sourceFilePath.SetExt(_T("c"));
	headerFilePath.SetExt(_T("h"));
	if (sourceFilePath.FileExists())
	{
		wxString msg = wxString::Format(_("File \"%s\" already exists.\n"
			"Do you want to overwrite it?"), sourceFilePath.GetFullPath());
		if (wxMessageBox(msg, _("File exists"), wxYES_NO | wxCENTRE) == wxNO)
		{
			return;
		}
	}
	if (headerFilePath.FileExists())
	{
		wxString msg = wxString::Format(_("File \"%s\" already exists.\n"
			"Do you want to overwrite it?"), headerFilePath.GetFullPath());
		if (wxMessageBox(msg, _("File exists"), wxYES_NO | wxCENTRE) == wxNO)
		{
			return;
		}
	}

	wxGenericProgressDialog busy(_("Generating files"), _("Generating files"), 1);
	wxFFile *sourceFile = new wxFFile(sourceFilePath.GetFullPath(), _T("w"));
	if (!sourceFile->IsOpened())
	{
		wxLogError(_("Could not open source file for writing"));
		return;
	}
	wxFFile *headerFile = new wxFFile(headerFilePath.GetFullPath(), _T("w"));
	if (!headerFile->IsOpened())
	{
		wxLogError(_("Could not open header file for writing"));
		return;
	}

	wxString sourceFileHeader = wxString::Format(_(
		"// This file was generated with Bitmap Generator\n"
		"#include \"%s\"\n"
		"#include <emgl/color.h>\n\n"
		"#ifndef emgl_colorConvFromRGBA8888\n"
		"#warning emgl_colorConvFromRGBA8888 not defined!\n"
		"#define _C(x) (x)\n"
		"#else\n"
		"#define _C(x) emgl_colorConvFromRGBA8888(x)\n"
		"#endif\n\n"
		),
		headerFilePath.GetFullName()
		);
	wxString sourceFileFooter = wxString::Format(_(
		""
		));

	wxString headerFileHeader = wxString::Format(_(
		"#ifndef _%s_%s_\n"
		"#define _%s_%s_\n\n"
		"#include <emgl/color.h>\n\n"
		"#ifndef __BITMAP_DATA_T_\n"
		"#define __BITMAP_DATA_T_\n"
		"typedef struct _bitmap_data {\n"
		"\tcoord_t width;\n"
		"\tcoord_t height;\n"
		"\temgl_color_t const *const data;\n"
		"} bitmap_data_t;\n"
		"#endif\n\n"
		),
		Image::MakeNameValid(headerFilePath.GetName().MakeUpper()),
		Image::MakeNameValid(headerFilePath.GetExt().MakeUpper()),
		Image::MakeNameValid(headerFilePath.GetName().MakeUpper()),
		Image::MakeNameValid(headerFilePath.GetExt().MakeUpper())
		);
	wxString headerFileFooter = wxString::Format(_(
		"#endif\n"
		));

	// Start the files
	headerFile->Write(headerFileHeader);
	sourceFile->Write(sourceFileHeader);

	// Process the content of the files
	for (std::map<int, Image *>::iterator it = m_images.begin();
		it != m_images.end(); it++)
	{
		wxString def = wxString::Format(_(
			"extern const bitmap_data_t %s_%s;\n"), 
			Image::MakeNameValid(selectedFilePath.GetName().Lower()),
			it->second->GetName());
		headerFile->Write(def);

		wxString impHeader = wxString::Format(_(
			"const emgl_color_t %s_%s_data[] = {"),
			Image::MakeNameValid(selectedFilePath.GetName().Lower()),
			it->second->GetName());

		wxString impFooter = wxString::Format(_(
			"};\n\n"
			"const bitmap_data_t %s_%s = {\n"
			"\t%d,%d, %s_%s_data\n"
			"};\n\n"
			),
			Image::MakeNameValid(selectedFilePath.GetName().Lower()),
			it->second->GetName(),
			it->second->GetImage().GetWidth(),
			it->second->GetImage().GetHeight(),
			Image::MakeNameValid(selectedFilePath.GetName().Lower()),
			it->second->GetName());

		sourceFile->Write(impHeader);

		// Write All the data of one image
		wxImage image = it->second->GetConvertedImage();
		switch (m_originChoice->GetSelection())
		{
			case 0: // Do nothing
				break;
			case 1: // Flip Vertically
				image = image.Mirror(false);
				break;
			case 2: // Flip Horizontally
				image = image.Mirror(true);
				break;
			case 3: // Flip Horizontally and Vertically
				image = image.Mirror(false);
				image = image.Mirror(true);
				break;
		}
		unsigned char *imageData = image.GetData();
		unsigned char *imageAlpha = image.GetAlpha();
		int length = image.GetWidth()*image.GetHeight();
		busy.SetRange(length+1);
		for (int i = 0; i < length; i++)
		{
			if ((i % 5) == 0)
			{
				sourceFile->Write(_("\n"));
			}
			if ((i % 3000) == 0)
			{
				busy.Update(i);
			}
			
			unsigned char alpha;
			if(imageAlpha == NULL)
			{
				alpha = 0xFF;
			}
			else {
				alpha = *imageAlpha;
				imageAlpha++;
			}
			wxString data = wxString::Format(_(
				"_C(0x%02x%02x%02x%02x)%c "),
				alpha, 
				imageData[0],
				imageData[1], 
				imageData[2],
				i != length-1 ? ',' : '\n');
			imageData+=3;
			sourceFile->Write(data);
		}
		sourceFile->Write(impFooter);
	}

	// End the files
	headerFile->Write(headerFileFooter);
	headerFile->Close();
	sourceFile->Write(sourceFileFooter);
	sourceFile->Close();
	busy.Close();
}

void BitmapGeneratorFrame::GenerateFilesHeaderOnly()
{
	wxFileDialog saveFileDialog(this, _("Save bitmap file"), wxEmptyString,
		wxEmptyString, _T("Header files(*.h)|*.h"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	// Show the file dialog
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
	{
		// Canceled
		return;
	}
	
	wxFileName headerFilePath = wxFileName(saveFileDialog.GetPath());
	
	wxGenericProgressDialog busy(_("Generating files"), _("Generating files"), 1);
	
	wxFFile *headerFile = new wxFFile(headerFilePath.GetFullPath(), _T("w"));
	if (!headerFile->IsOpened())
	{
		wxLogError(_("Could not open header file for writing"));
		return;
	}

	wxString headerFileHeader = wxString::Format(_(
		"#ifndef _%s_%s_\n"
		"#define _%s_%s_\n\n"
		"#ifndef __BITMAP_DATA_T_\n"
		"#define __BITMAP_DATA_T_\n"
		"typedef struct _bitmap_data {\n"
		"\tint width;\n"
		"\tint height;\n"
		"\tunsigned int const *const data;\n"
		"} bitmap_data_t;\n"
		"#endif\n\n"
		"#ifndef _C\n"
		"//You can use this to convert the image data to an seperate format\n"
		"#define _C(x) (x)\n"
		"#endif\n"
		),
		Image::MakeNameValid(headerFilePath.GetName().MakeUpper()),
		Image::MakeNameValid(headerFilePath.GetExt().MakeUpper()),
		Image::MakeNameValid(headerFilePath.GetName().MakeUpper()),
		Image::MakeNameValid(headerFilePath.GetExt().MakeUpper())
		);
	wxString headerFileFooter = wxString::Format(_(
		"#endif\n"
		));

	// Start the files
	headerFile->Write(headerFileHeader);

	// Process the content of the files
	for (std::map<int, Image *>::iterator it = m_images.begin();
		it != m_images.end(); it++)
	{
		wxString impHeader = wxString::Format(_(
			"const unsigned int %s_%s_data[] = {"),
			Image::MakeNameValid(headerFilePath.GetName().Lower()),
			it->second->GetName());

		wxString impFooter = wxString::Format(_(
			"};\n\n"
			"const bitmap_data_t %s_%s = {\n"
			"\t%d,%d, %s_%s_data\n"
			"};\n\n"
			),
			Image::MakeNameValid(headerFilePath.GetName().Lower()),
			it->second->GetName(),
			it->second->GetImage().GetWidth(),
			it->second->GetImage().GetHeight(),
			Image::MakeNameValid(headerFilePath.GetName().Lower()),
			it->second->GetName());

		headerFile->Write(impHeader);

		// Write All the data of one image
		wxImage image = it->second->GetConvertedImage();
		switch (m_originChoice->GetSelection())
		{
		case 0: // Do nothing
			break;
		case 1: // Flip Vertically
			image.Rescale(image.GetWidth(), -image.GetHeight(),
				wxIMAGE_QUALITY_HIGH);
			break;
		case 2: // Flip Horizontally
			image.Rescale(-image.GetWidth(), image.GetHeight(),
				wxIMAGE_QUALITY_HIGH);
			break;
		case 3: // Flip Horizontally and Vertically
			image.Rescale(-image.GetWidth(), -image.GetHeight(),
				wxIMAGE_QUALITY_HIGH);
			break;
		}
		unsigned char *imageData = image.GetData();
		unsigned char *imageAlpha = image.GetAlpha();
		int length = image.GetWidth()*image.GetHeight();
		busy.SetRange(length +1);
		for (int i = 0; i < length; i++)
		{
			if ((i % 5) == 0)
			{
				headerFile->Write(_("\n"));
			}
			if ((i % 3000) == 0)
			{
				busy.Update(i);
			}

			unsigned char alpha;
			if (imageAlpha == NULL)
			{
				alpha = 0xFF;
			}
			else {
				alpha = *imageAlpha;
				imageAlpha++;
			}
			wxString data = wxString::Format(_(
				"_C(0x%02x%02x%02x%02x)%c "),
				alpha,
				imageData[0],
				imageData[1],
				imageData[2],
				i != length - 1 ? ',' : '\n');
			imageData += 3;
			headerFile->Write(data);
		}
		headerFile->Write(impFooter);
	}

	// End the files
	headerFile->Write(headerFileFooter);
	headerFile->Close();
	busy.Close(true);
}

void BitmapGeneratorFrame::GenerateFilesHeaderAndSource()
{
	wxFileDialog saveFileDialog(this, _("Save bitmap files"), wxEmptyString,
		wxEmptyString, _T(""), wxFD_SAVE);
	saveFileDialog.SetMessage(_("Filename will be a prefix to the output files.\n"
		"e.g. bitmap will become bitmap.c and bitmap.h"));
	// Show the file dialog
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
	{
		// Canceled
		return;
	}

	wxFileName selectedFilePath = wxFileName(saveFileDialog.GetPath());
	wxFileName sourceFilePath = selectedFilePath;
	wxFileName headerFilePath = selectedFilePath;

	sourceFilePath.SetExt(_T("c"));
	headerFilePath.SetExt(_T("h"));
	if (sourceFilePath.FileExists())
	{
		wxString msg = wxString::Format(_("File \"%s\" already exists.\n"
			"Do you want to overwrite it?"), sourceFilePath.GetFullPath());
		if (wxMessageBox(msg, _("File exists"), wxYES_NO | wxCENTRE) == wxNO)
		{
			return;
		}
	}
	if (headerFilePath.FileExists())
	{
		wxString msg = wxString::Format(_("File \"%s\" already exists.\n"
			"Do you want to overwrite it?"), headerFilePath.GetFullPath());
		if (wxMessageBox(msg, _("File exists"), wxYES_NO | wxCENTRE) == wxNO)
		{
			return;
		}
	}

	wxGenericProgressDialog busy(_("Generating files"), _("Generating files"), 1);
	wxFFile *sourceFile = new wxFFile(sourceFilePath.GetFullPath(), _T("w"));
	if (!sourceFile->IsOpened())
	{
		wxLogError(_("Could not open source file for writing"));
		return;
	}
	wxFFile *headerFile = new wxFFile(headerFilePath.GetFullPath(), _T("w"));
	if (!headerFile->IsOpened())
	{
		wxLogError(_("Could not open header file for writing"));
		return;
	}

	wxString sourceFileHeader = wxString::Format(_(
		"// This file was generated with Bitmap Generator\n"
		"#include \"%s\"\n"
		"#ifndef _C\n"
		"//You can use this to convert the image data to an sepperate format\n"
		"#define _C(x) (x)\n"
		"#endif\n"
		),
		headerFilePath.GetFullName()
		);
	wxString sourceFileFooter = wxString::Format(_(
		""
		));

	wxString headerFileHeader = wxString::Format(_(
		"#ifndef _%s_%s_\n"
		"#define _%s_%s_\n\n"
		"#ifndef __BITMAP_DATA_T_\n"
		"#define __BITMAP_DATA_T_\n"
		"typedef struct _bitmap_data {\n"
		"\tint width;\n"
		"\tint height;\n"
		"\tunsigned int const *const data;\n"
		"} bitmap_data_t;\n"
		"#endif\n\n"
		),
		Image::MakeNameValid(headerFilePath.GetName().MakeUpper()),
		Image::MakeNameValid(headerFilePath.GetExt().MakeUpper()),
		Image::MakeNameValid(headerFilePath.GetName().MakeUpper()),
		Image::MakeNameValid(headerFilePath.GetExt().MakeUpper())
		);
	wxString headerFileFooter = wxString::Format(_(
		"#endif\n"
		));

	// Start the files
	headerFile->Write(headerFileHeader);
	sourceFile->Write(sourceFileHeader);

	// Process the content of the files
	for (std::map<int, Image *>::iterator it = m_images.begin();
		it != m_images.end(); it++)
	{
		wxString def = wxString::Format(_(
			"extern const bitmap_data_t %s_%s;\n"),
			Image::MakeNameValid(selectedFilePath.GetName().Lower()),
			it->second->GetName());
		headerFile->Write(def);

		wxString impHeader = wxString::Format(_(
			"const unsigned int %s_%s_data[] = {"),
			Image::MakeNameValid(selectedFilePath.GetName().Lower()),
			it->second->GetName());

		wxString impFooter = wxString::Format(_(
			"};\n\n"
			"const bitmap_data_t %s_%s = {\n"
			"\t%d,%d, %s_%s_data\n"
			"};\n\n"
			),
			Image::MakeNameValid(selectedFilePath.GetName().Lower()),
			it->second->GetName(),
			it->second->GetImage().GetWidth(),
			it->second->GetImage().GetHeight(),
			Image::MakeNameValid(selectedFilePath.GetName().Lower()),
			it->second->GetName());

		sourceFile->Write(impHeader);

		// Write All the data of one image
		wxImage image = it->second->GetConvertedImage();
		switch (m_originChoice->GetSelection())
		{
		case 0: // Do nothing
			break;
		case 1: // Flip Vertically
			image.Rescale(image.GetWidth(), -image.GetHeight(),
				wxIMAGE_QUALITY_HIGH);
			break;
		case 2: // Flip Horizontally
			image.Rescale(-image.GetWidth(), image.GetHeight(),
				wxIMAGE_QUALITY_HIGH);
			break;
		case 3: // Flip Horizontally and Vertically
			image.Rescale(-image.GetWidth(), -image.GetHeight(),
				wxIMAGE_QUALITY_HIGH);
			break;
		}
		unsigned char *imageData = image.GetData();
		unsigned char *imageAlpha = image.GetAlpha();
		int length = image.GetWidth()*image.GetHeight();
		busy.SetRange(length+1);
		for (int i = 0; i < length; i++)
		{
			if ((i % 5) == 0)
			{
				sourceFile->Write(_("\n"));
			}
			if ((i % 3000) == 0)
			{
				busy.Update(i);
			}

			unsigned char alpha;
			if (imageAlpha == NULL)
			{
				alpha = 0xFF;
			}
			else {
				alpha = *imageAlpha;
				imageAlpha++;
			}
			wxString data = wxString::Format(_(
				"_C(0x%02x%02x%02x%02x)%c "),
				alpha,
				imageData[0],
				imageData[1],
				imageData[2],
				i != length - 1 ? ',' : '\n');
			imageData += 3;
			sourceFile->Write(data);
		}
		sourceFile->Write(impFooter);
	}

	// End the files
	headerFile->Write(headerFileFooter);
	headerFile->Close();
	sourceFile->Write(sourceFileFooter);
	sourceFile->Close();
	busy.Close();
}

void BitmapGeneratorFrame::UpdateDisplayedImage(Image *image)
{
	m_imagePanel->SetImage(image);
	wxSize imageSize = image->GetImage().GetSize();
	wxString label = wxString::Format(_("Image size: %dpx * %dpx(%d pixels)\n"
		"RGBA8888 size: %d bytes"),
		imageSize.GetWidth(), imageSize.GetHeight(),
		imageSize.GetWidth()*imageSize.GetHeight(),
		imageSize.GetWidth()*imageSize.GetHeight() * 4);
	m_imageSizeText->SetLabel(label);
	m_framePanel->GetSizer()->RecalcSizes();
}
