#ifndef _BITMAPGENERATOR_H_
#define _BITMAPGENERATOR_H_

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <map>
#include "image.h"
#include "imagePanel.h"


class BitmapGenerator:public wxApp
{
public:
	virtual bool OnInit();
};

class BitmapGeneratorFrame :public wxFrame
{
public:
	BitmapGeneratorFrame(const wxString &title, const wxPoint &pos, 
		const wxSize &size);
	
private:
	void OnOpenImageButtonClick(wxCommandEvent &evt);
	void OnImageNameEdit(wxListEvent &evt);
	void OnImageListSelectionChanged(wxListEvent &evt);
	void OnImageListRightClick(wxListEvent &evt);
	void OnImageDeleted(wxListEvent &evt);
	void OnImageProperties(wxCommandEvent &evt);
	void OnImageRename(wxCommandEvent &evt);
	void OnImageRemove(wxCommandEvent &evt);
	void OnGenerateFiles(wxCommandEvent &evt);
	void AddImage(Image *image);
	void RecalculateStats();
	void GenerateFilesEMGL();
	void GenerateFilesHeaderOnly();
	void GenerateFilesHeaderAndSource();
	void UpdateDisplayedImage(Image *image);
	
	std::map<int,Image *> m_images;
	wxImagePanel *m_imagePanel;
	wxListView *m_imageListView;
	wxStaticText *m_imageSizeText;
	wxStaticText *m_statsText;
	wxPanel *m_framePanel;

	wxChoice *m_modeChoice;
	wxChoice *m_originChoice;
	int m_contextMenuItem;

	wxDECLARE_EVENT_TABLE();
};

#endif