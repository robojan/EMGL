#include "fontManager.h"


#include <Windows.h>

#include <ft2build.h>
#include FT_FREETYPE_H
//#include FT_CACHE_H

#include "memdbg.h"

FontEntry::FontEntry()
{
}

FontEntry::FontEntry(const wxString &family)
{
	m_family = family;
}

FontEntry::~FontEntry()
{

}

void FontEntry::SetFamily(const wxString &family)
{
	m_family = family;
}

wxString FontEntry::GetFamily() const
{
	return m_family;
}

bool FontEntry::GetStylePath(const wxString &style, wxString &file, int &face) const
{
	std::map<wxString, FontStyleInfo>::const_iterator fstyle = m_styles.find(style);
	if (fstyle == m_styles.end())
	{
		return false;
	}
	file = m_paths.at(fstyle->second.pathIndex);
	face = fstyle->second.faceIndex;
	return true;
}

void FontEntry::AddStyle(const wxString &style, const wxString &path, int face, bool scaleable)
{
	// Search for path
	int pathIndex = -1;
	for (size_t i = 0; i < m_paths.size(); i++)
	{
		if (m_paths[i] == path)
		{
			pathIndex = i;
		}
	}
	// Add path to list if it didn't already exist
	if (pathIndex == -1)
	{
		pathIndex = m_paths.size();
		m_paths.push_back(path);
	}
	// Add the style
	FontStyleInfo info;
	info.faceIndex = face;
	info.pathIndex = pathIndex;
	info.scaleable = scaleable;
	m_styles[style] = info;
}

wxArrayString FontEntry::GetStyles() const
{
	wxArrayString result;
	for (std::map<wxString, FontStyleInfo>::const_iterator it = m_styles.cbegin();
		it != m_styles.cend(); ++it)
	{
		result.Add(it->first);
	}
	return result;
}

void FontEntry::AddFixedSize(const wxString &style, float size)
{
	if (m_styles.find(style) == m_styles.end())
	{
		return;
	}
	if (m_styles[style].sizes.find(size) != m_styles[style].sizes.end())
	{
		return;
	}
	m_styles[style].sizes.insert(size);
}

void FontEntry::SetScaleable(const wxString &style, bool scaleable)
{
	if (m_styles.find(style) == m_styles.end())
	{
		return;
	}
	m_styles[style].scaleable = scaleable;
}

int doubleCompareFunc(double *lhs, double *rhs)
{
	return *(double *)lhs - *(double *)rhs;
}

wxArrayDouble FontEntry::GetSizes(const wxString &style) const
{
	wxArrayDouble result;
	if (m_styles.find(style) == m_styles.end())
	{
		return result;
	}
	if (IsScaleable(style))
	{
		// Add some default font sizes
		result.Add(8);
		result.Add(9);
		result.Add(10);
		result.Add(11);
		result.Add(12);
		result.Add(14);
		result.Add(16);
		result.Add(18);
		result.Add(20);
		result.Add(22);
		result.Add(24);
		result.Add(26);
		result.Add(28);
		result.Add(36);
		result.Add(48);
		result.Add(72);
	}
	for (std::set<float>::const_iterator it = m_styles.at(style).sizes.cbegin();
		it != m_styles.at(style).sizes.cend(); ++it)
	{
		if (result.Index(*it) != wxNOT_FOUND)
		{
			continue;
		}
		result.Add(*it);
	}

	result.Sort(doubleCompareFunc);
	return result;
}

bool FontEntry::IsScaleable(const wxString &style) const
{
	if (m_styles.find(style) == m_styles.end())
	{
		return false;
	}
	return m_styles.at(style).scaleable;
}

struct FT_LibraryRec_ *FontManager::m_lib = NULL;
//struct FTC_ManagerRec_ *FontManager::m_manager = NULL;
std::map<wxString, FontEntry> FontManager::m_installed;

const std::map<wxString, FontEntry> &FontManager::GetInstalledFonts()
{
	return m_installed;
}

wxArrayString FontManager::GetInstalledFamilies()
{
	wxArrayString result;
	for (std::map<wxString, FontEntry>::const_iterator it = m_installed.cbegin();
		it != m_installed.cend(); ++it)
	{
		result.Add(it->first);
	}
	return result;
}

wxArrayString FontManager::GetStyles(const wxString &family)
{
	wxArrayString result;
	std::map<wxString, FontEntry>::const_iterator font = m_installed.find(family);
	if (font == m_installed.end())
	{
		return result;
	}

	return font->second.GetStyles();
}

wxArrayDouble FontManager::GetSizes(const wxString &family, const wxString &style)
{
	wxArrayDouble result;
	std::map<wxString, FontEntry>::const_iterator font = m_installed.find(family);
	if (font == m_installed.end())
	{
		return result;
	}

	return font->second.GetSizes(style);
}

bool FontManager::IsScaleable(const wxString &family, const wxString &style)
{
	std::map<wxString, FontEntry>::const_iterator font = m_installed.find(family);
	if (font == m_installed.end())
	{
		return false;
	}

	return font->second.IsScaleable(style);
}

Font FontManager::GetFont(const wxString &family, const wxString &style, float size)
{
	std::map<wxString, FontEntry>::const_iterator ffamily = m_installed.find(family);
	if (ffamily == m_installed.end())
	{
		return Font();
	}

	wxString styleName = style;
	if (styleName.IsEmpty())
	{
		wxArrayString styles = ffamily->second.GetStyles();
		if (styles.IsEmpty())
		{
			return Font();
		}
		else {
			styleName = GetDefaultStyle(family);
		}
	}

	wxString path;
	int fi;
	if (!ffamily->second.GetStylePath(styleName, path, fi))
	{
		return Font();
	}

	return Font(path, fi, size);
}

struct FT_LibraryRec_ *FontManager::GetFTLibrary()
{
	return m_lib;
}

wxString FontManager::GetEncodingName(wxUint32 encoding)
{
	switch (encoding)
	{
	case FT_ENCODING_UNICODE:
		return _("Unicode");
	case FT_ENCODING_MS_SYMBOL:
		return _("Microsoft Symbol");
	case FT_ENCODING_SJIS:
		return _("Japanese SJIS");
	case FT_ENCODING_GB2312:
		return _("Simplified Chinese (GB2312)");
	case FT_ENCODING_BIG5:
		return _("Traditional Chinese (Big5)");
	case FT_ENCODING_WANSUNG:
		return _("Wansung");
	case FT_ENCODING_JOHAB:
		return _("Johab");
	case FT_ENCODING_ADOBE_LATIN_1:
		return _("Latin-1");
	case FT_ENCODING_ADOBE_STANDARD:
		return _("Adobe Standard");
	case FT_ENCODING_ADOBE_EXPERT:
		return _("Adobe Expert");
	case FT_ENCODING_ADOBE_CUSTOM:
		return _("Adobe Custom");
	case FT_ENCODING_APPLE_ROMAN:
		return _("Apple roman");
	case FT_ENCODING_OLD_LATIN_2:
		return _("Old Latin-2");
	default:
		return _("Unknown");
	}
}

void FontManager::AddFont(const wxString &path)
{
	FT_Face face;
	if (FT_New_Face(m_lib, path.c_str(), -1, &face) != FT_Err_Ok)
	{
		wxLogError(_("Could not open font from file: \"%s\""), path);
		return;
	}
	int numFaces = face->num_faces;
	FT_Done_Face(face);
	AddFont(path, numFaces);
}

void FontManager::AddFont(const wxString &path, int numFaces, const wxString &familyName)
{
	FT_Face face;
	FT_Error error;
	// Iterate all faces
	for (int fi = 0; fi < numFaces; fi++)
	{
		if ((error = FT_New_Face(m_lib, path.c_str(), fi, &face)) != FT_Err_Ok)
		{
			wxLogWarning(_("Could not open font face \"%s\": %d"), familyName,
				fi);
			continue;
		}

		wxString family =
			face->family_name == NULL ? familyName : face->family_name;
		wxString style =
			face->style_name == NULL ? "Regular" : face->style_name;
		bool scaleable = face->face_flags & FT_FACE_FLAG_SCALABLE;

		if (m_installed.find(family) == m_installed.end())
		{
			// Font not yet in list add new
			m_installed[family] = FontEntry(family);
		}

		m_installed[family].AddStyle(style, path, fi, scaleable);

		for (int i = 0; i < face->num_fixed_sizes; i++)
		{
			FT_Bitmap_Size *size = &face->available_sizes[i];

			m_installed[family].AddFixedSize(style, size->y_ppem/64.0);
		}

		FT_Done_Face(face);
	}
}

wxString FontManager::GetDefaultStyle(const wxString &family)
{
	wxString style;
	std::map<wxString, FontEntry>::const_iterator ffamily = m_installed.find(family);
	if (ffamily == m_installed.end())
	{
		return style;
	}

	wxArrayString styles = ffamily->second.GetStyles();
	if (!styles.IsEmpty())
	{
		int index = styles.Index("Regular", false);
		if (index == wxNOT_FOUND)
		{
			index = styles.Index("Normal", false);
			if (index == wxNOT_FOUND)
			{
				index = 0;
			}
		}
		style = styles[index];
	}
	return style;
}


void FontManager::Init()
{
	FT_Error error = FT_Init_FreeType(&m_lib);
	if (error != FT_Err_Ok)
	{
		wxLogFatalError(_("Could not initialize FreeType library"));
		return;
	}

	// Build library of all system fonts
	BuildFontsDB();
}

void FontManager::Release()
{
	FT_Done_FreeType(m_lib);
	m_lib = NULL;
}

void FontManager::GetSystemFonts(std::map<wxString, wxString> &fonts)
{

	wxString fontDir;
	TCHAR buffer[MAX_PATH];

	// Get windows dir
	int length = GetWindowsDirectory(buffer, MAX_PATH);
	if (length <= 0)
	{
		wxLogError(_("Could not get windows directory: 0x%08X"), GetLastError());
		return;
	}

	// Create Font dir
	fontDir = buffer;
	fontDir += "\\Fonts\\";

	// Get installed fonts
	DWORD retCode;
	HKEY fontsKey;
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"),
		0, KEY_READ, &fontsKey))
	{
		wxLogError(_("Could not get font registry key: 0x%08X"), GetLastError());
		return;
	}

	DWORD cValues;
	DWORD maxValueNameLength;
	DWORD maxValueLength;
	retCode = RegQueryInfoKey(fontsKey, NULL, NULL, NULL, NULL, NULL,
		NULL, &cValues, &maxValueNameLength, &maxValueLength,
		NULL, NULL);
	if (retCode == ERROR_SUCCESS)
	{
		TCHAR *name = new TCHAR[maxValueNameLength + 1];
		TCHAR *value = new TCHAR[maxValueNameLength + 1];
		// Enumerate through all the values
		for (DWORD i = 0; i < cValues; i++)
		{
			DWORD nameLength = maxValueNameLength;
			DWORD valueLength = maxValueLength;
			DWORD type;
			retCode = RegEnumValue(fontsKey, i, name, &nameLength,
				NULL, &type, (LPBYTE) value, &valueLength);
			if (retCode != ERROR_SUCCESS || REG_SZ != type)
			{
				continue;
			}

			// Check for null termination
			if (value[valueLength / sizeof(TCHAR) - 1] != L'\0')
			{
				value[valueLength / sizeof(TCHAR) - 1] = L'\0';
			}

			// Make absolute path
			wxString path = value;
			if (!wxIsAbsolutePath(path))
			{
				path = fontDir;
				path += value;
			}

			fonts[name] = path;
		}
		delete [] name;
		delete [] value;
	}
	else {
		wxLogError(_("Could not get registry key information: 0x%08X"),
			GetLastError());
	}

	RegCloseKey(fontsKey);
}

void FontManager::BuildFontsDB()
{
	// Clear current buffer
	m_installed.clear();

	// Get installed fonts
	std::map<wxString, wxString> installedFonts;
	FontManager::GetSystemFonts(installedFonts);

	// Combine font files together
	for (std::map<wxString, wxString>::iterator it = installedFonts.begin();
		it != installedFonts.end(); ++it)
	{
		// Get font info
		// Get number of faces and if the face is supported
		FT_Face face;
		if (FT_New_Face(m_lib, it->second, -1, &face))
		{
			// error not supported
			wxLogMessage(_("Font not supported: %s"), it->first);
			continue;
		}
		int numFaces = face->num_faces;
		FT_Done_Face(face);
		AddFont(it->second, numFaces, it->first);
	}
}

bool FontManager::IsOK()
{
	return m_lib != NULL;
}
