
#include <wx/wx.h>
#include <wx/filename.h>
#include "image.h"

Image::Image(const wxString &path, const wxString &name)
{
	m_hFlip = false;
	m_vFlip = false;
	m_Image = new wxImage(path);
	m_width = m_Image->GetWidth();
	m_height = m_Image->GetHeight();
	if (name.IsEmpty())
	{
		wxFileName filename = wxFileName(path);
		SetName(filename.GetName());
	}
	else {
		SetName(name);
	}
}

Image::~Image()
{
	delete m_Image;
}

wxImage &Image::GetImage()
{
	return *m_Image;
}

wxImage Image::GetConvertedImage() const
{
	wxImage image = m_Image->Copy();
	image.Rescale(GetWidth(), GetHeight(), wxIMAGE_QUALITY_HIGH);
	if (GetHFlip())
	{
		image = image.Mirror(true);
	}
	if (GetVFlip())
	{
		image = image.Mirror(false);
	}
	return image;
}


const wxString &Image::GetName() const
{
	return m_Name;
}

void Image::SetName(const wxString &name)
{
	m_Name = MakeNameValid(name);
}

bool Image::GetHFlip() const
{
	return m_hFlip;
}

bool Image::GetVFlip() const
{
	return m_vFlip;
}

void Image::SetHFlip(bool hflip)
{
	m_hFlip = hflip;
}

void Image::SetVFlip(bool vflip)
{
	m_vFlip = vflip;
}

wxString Image::MakeNameValid(const wxString &str)
{
	wxString out = str;
	for (wxString::iterator it = out.begin(); it != out.end(); it++)
	{
		if (wxIsalpha(*it) || wxIsdigit(*it) || *it == '_')
		{
			// Correct character
		}
		else {
			// Character needs to be changed
			*it = '_';
		}
	}
	return out;
}

int Image::GetWidth() const
{
	return m_width;
}

int Image::GetHeight() const
{
	return m_height;
}

void Image::SetWidth(int w)
{
	m_width = w;
}

void Image::SetHeight(int h)
{
	m_height = h;
}
