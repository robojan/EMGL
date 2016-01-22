#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <wx/wx.h>

class Image {
public:
	Image(const wxString &file, const wxString &name = "");
	~Image();

	wxImage &GetImage();
	wxImage GetConvertedImage() const;
	const wxString &GetName() const;
	bool GetHFlip() const;
	bool GetVFlip() const;
	int GetWidth() const;
	int GetHeight() const;
	void SetName(const wxString &name);
	void SetHFlip(bool hflip);
	void SetVFlip(bool vflip);
	void SetWidth(int w);
	void SetHeight(int h);
	static wxString MakeNameValid(const wxString &str);

private:
	wxImage *m_Image;
	wxString m_Name;
	bool m_hFlip;
	bool m_vFlip;
	int m_width;
	int m_height;
};

#endif