#ifndef _CHARMAP_H_
#define _CHARMAP_H_

#include <wx/wx.h>
#include <map>
#include <list>

#define ENCODING_APPLEROMAN_VALID 0x01
#define ENCODING_MSSYMBOL_VALID 0x02
#define ENCODING_UNICODE_VALID 0x04
#define ENCODING_VALID_END 0x00

struct encodingCharDef {
	unsigned int code;
	unsigned char valid;
	unsigned char appleRoman;
	unsigned char msSymbol;
	unsigned int unicode;
};

struct encodingDef {
	wxString name;
	const struct encodingCharDef *codes;
};

extern const struct encodingDef *defaultEncodings [];

class CharMapEntry
{
public:
	CharMapEntry();
	CharMapEntry(wxString family, wxString style, float size, wxUint32 m_encoding, wxUint32 code);
	CharMapEntry(const wxString &str);
	bool operator==(const CharMapEntry &rhs) const;
	bool operator!=(const CharMapEntry &rhs) const;

	bool IsOK() const;
	wxString GetFamily() const;
	wxString GetStyle() const;
	wxString GetEncoding() const;
	wxUint32 GetEncodingID() const;
	wxUint32 GetCode() const;
	float GetSize() const;

	void SetFamily(const wxString &family);
	void SetStyle(const wxString &style);
	void SetEncoding(wxUint32 code);
	void SetCode(wxUint32 code);
	void SetSize(float size);

	wxString ToString() const;
private:
	wxString m_family;
	wxString m_style;
	wxUint32 m_encoding;
	wxUint32 m_code;
	float m_size;
};

class CodePage
{
public:
	CodePage(wxUint32 start = 0, wxUint32 end =0, const wxString &name = wxEmptyString);
	CodePage(const CodePage &other, wxUint32 start, wxUint32 end);
	CodePage(const CodePage &other);

	const wxString &GetName() const;
	void SetName(const wxString &name);
	void SetEnd(wxUint32 end);
	void SetStart(wxUint32 start);
	wxUint32 GetSize() const;
	wxUint32 GetRangeStart() const;
	wxUint32 GetRangeEnd() const;
	CharMapEntry &at(wxUint32 i);
	void Shift(int shift);

	bool operator<(const CodePage &rhs) const;
	bool operator>(const CodePage &rhs) const;
	bool operator==(const CodePage &rhs) const;
	bool operator!=(const CodePage &rhs) const;
	CharMapEntry &operator[](wxUint32 i);


	CharMapEntry *GetCharMapEntry(wxUint32 code);
	void Remove(wxUint32 code);
private:
	std::map<unsigned int, CharMapEntry> m_map;
	wxUint32 m_start;
	wxUint32 m_end;
	wxString m_name;
};

class CharMap
{
public:
	CharMap();
	CharMap(const CharMap &other);
	virtual ~CharMap();

	void RemoveCodePage(CodePage *codePage);
	void RemoveCodePage(std::list<CodePage*>::const_iterator first, std::list<CodePage*>::const_iterator last);
	void AddCodePage(const CodePage &page);
	bool CanAddCodePage(const CodePage &page);
	void SplitCodePage(wxUint32 splitCodeFirst);
	CodePage *GetCodePage(wxUint32 code);

	int GetCountCodePages() const;
	std::list<CodePage *>::iterator Begin();
	std::list<CodePage*>::iterator End();
	std::list<CodePage *>::const_iterator CBegin() const;
	std::list<CodePage*>::const_iterator CEnd() const;
	void Clear();
private:
	std::list<CodePage *> m_map;
};

#endif