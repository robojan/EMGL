#ifndef _FONTMANAGER_H_
#define _FONTMANAGER_H_

#include <wx/wx.h>
#include <map>
#include <vector>
#include <set>
#include "font.h"

//forward decleration
struct FT_LibraryRec_;
//struct FTC_ManagerRec_;

struct FontStyleInfo
{
	int pathIndex;
	int faceIndex;
	std::set<float> sizes;
	bool scaleable;
};

class FontEntry{
public:
	FontEntry();
	FontEntry(const wxString &family);
	~FontEntry();

	void SetFamily(const wxString &family);
	wxString GetFamily() const;

	bool GetStylePath(const wxString &style, wxString &file, int &face) const;

	void AddStyle(const wxString &style, const wxString &path, int face, bool scaleable);
	void AddFixedSize(const wxString &style, float size);
	void SetScaleable(const wxString &style, bool scaleable);
	wxArrayString GetStyles() const;
	wxArrayDouble GetSizes(const wxString &style) const;
	bool IsScaleable(const wxString &style) const;

private:
	
	wxString m_family;
	std::map<wxString, FontStyleInfo> m_styles;
	std::vector<wxString> m_paths;
};

class FontManager
{
public:
	static void Init();
	static void Release();

	static const std::map<wxString, FontEntry> &GetInstalledFonts();
	static wxArrayString GetInstalledFamilies();
	static wxArrayString GetStyles(const wxString &family);
	static wxArrayDouble GetSizes(const wxString &family, const wxString &style);
	static bool IsScaleable(const wxString &family, const wxString &style);
	static wxString GetDefaultStyle(const wxString &family);

	static Font GetFont(const wxString &family, const wxString &style, float size);
	static struct FT_LibraryRec_ *GetFTLibrary();

	static wxString GetEncodingName(wxUint32 encoding);
	static void AddFont(const wxString &path);
	static void AddFont(const wxString &path, int num_faces, const wxString &familyName = "Unknown");
	static bool IsOK();

private:
	static void GetSystemFonts(std::map<wxString, wxString> &fonts);
	static void BuildFontsDB();

	static struct FT_LibraryRec_ *m_lib;
	//static struct FTC_ManagerRec_ *m_manager;
	static std::map<wxString, FontEntry> m_installed;
};

#endif