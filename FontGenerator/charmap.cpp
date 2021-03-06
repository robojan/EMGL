#include "charmap.h"
#include "fontManager.h"
#include <wx/tokenzr.h>

#include "memdbg.h"

CharMapEntry::CharMapEntry(wxString family, wxString style, float size, wxUint32 encoding, wxUint32 code)
{
	SetFamily(family);
	SetStyle(style);
	SetSize(size);
	SetEncoding(encoding);
	SetCode(code);
}

CharMapEntry::CharMapEntry()
{
	SetSize(-1.0f);
	SetEncoding(0);
	SetCode(0);
}

CharMapEntry::CharMapEntry(const wxString &str)
{
	wxStringTokenizer tokenizer(str, wxT(";"), wxTOKEN_RET_EMPTY_ALL);
	bool insideString = false;
	// Family
	if (tokenizer.HasMoreTokens())
	{
		m_family = tokenizer.GetNextToken();
	}
	// Style
	if (tokenizer.HasMoreTokens())
	{
		m_style = tokenizer.GetNextToken();
	}
	// Size
	if (tokenizer.HasMoreTokens())
	{
		double size;
		if (tokenizer.GetNextToken().ToDouble(&size))
		{
			m_size = size;
		}
	}
	// Encoding
	if (tokenizer.HasMoreTokens())
	{
		unsigned long encoding;
		if (tokenizer.GetNextToken().ToULong(&encoding))
		{
			m_encoding = encoding;
		}
	}
	// Code
	if (tokenizer.HasMoreTokens())
	{
		unsigned long code;
		if (tokenizer.GetNextToken().ToULong(&code))
		{
			m_code = code;
		}
	}
}

bool CharMapEntry::operator==(const CharMapEntry &rhs) const
{
	return m_encoding == rhs.m_encoding &&
		m_family == rhs.m_family &&
		m_code == rhs.m_code && 
		m_size == rhs.m_size && 
		m_style == rhs.m_style;
}

bool CharMapEntry::operator!=(const CharMapEntry &rhs) const
{
	return !operator==(rhs);
}

wxString CharMapEntry::GetFamily() const
{
	return m_family;
}

wxString CharMapEntry::GetStyle() const
{
	return m_style;
}

wxString CharMapEntry::GetEncoding() const
{
	return FontManager::GetEncodingName(m_encoding);
}

wxUint32 CharMapEntry::GetCode() const
{
	return m_code;
}

float CharMapEntry::GetSize() const
{
	return m_size;
}

wxUint32 CharMapEntry::GetEncodingID() const
{
	return m_encoding;
}

bool CharMapEntry::IsOK() const
{
	return !m_family.IsEmpty() && !m_style.IsEmpty();
}

wxString CharMapEntry::ToString() const
{
	return wxString::Format(_("%s;%s;%g;%u;%u"), 
		GetFamily(), GetStyle(), GetSize(), GetEncodingID(), GetCode());
}

void CharMapEntry::SetFamily(const wxString &family)
{
	m_family = family;
	m_family.Replace(wxT(";"), wxT(":"));
}

void CharMapEntry::SetStyle(const wxString &style)
{
	m_style = style;
	m_style.Replace(wxT(";"), wxT(":"));
}

void CharMapEntry::SetEncoding(wxUint32 code)
{
	m_encoding = code;
}

void CharMapEntry::SetCode(wxUint32 code)
{
	m_code = code;
}

void CharMapEntry::SetSize(float size)
{
	m_size = size;
}

CodePage::CodePage(wxUint32 start, wxUint32 end, const wxString &name)
{
	m_start = start;
	m_end = end;
	m_name = name;
}

CodePage::CodePage(const CodePage &other, wxUint32 start, wxUint32 end)
{
	m_name = other.m_name;
	m_start = start;
	m_end = end;
	std::map<wxUint32, CharMapEntry>::const_iterator it = other.m_map.cbegin();
	while (it != other.m_map.cend() && it->first < start) {
		++it;
	}
	if (it != other.m_map.cend()) {
		m_map.insert(it, other.m_map.cend());
	}
}

CodePage::CodePage(const CodePage &other)
{
	m_name = other.m_name;
	m_start = other.m_start;
	m_end = other.m_end;
	m_map.insert(other.m_map.cbegin(), other.m_map.cend());
}

bool CodePage::operator<(const CodePage &rhs) const
{
	return m_end < rhs.m_start;
}

bool CodePage::operator>(const CodePage &rhs) const
{
	return m_start > rhs.m_end;
}

bool CodePage::operator==(const CodePage &rhs) const
{
	if (&rhs == this) 
		return true;
	if (!(m_start == rhs.m_start &&
		m_end == rhs.m_end &&
		m_name == rhs.m_name))
	{
		return false;
	}
	std::map<unsigned int, CharMapEntry>::const_iterator lit, rit;
	for (lit = m_map.cbegin(); lit != m_map.cend(); ++lit);
	{
		rit = rhs.m_map.find(lit->first);
		if (rit != rhs.m_map.cend())
		{
			if (lit->second != rit->second)
			{
				return false;
			}
		} else {
			return false;
		}
	}
	return true;
}

bool CodePage::operator!=(const CodePage &rhs) const
{
	return !operator==(rhs);
}

const wxString & CodePage::GetName() const
{
	return m_name;
}

wxUint32 CodePage::GetRangeStart() const
{
	return m_start;
}

wxUint32 CodePage::GetRangeEnd() const
{
	return m_end;
}

wxUint32 CodePage::GetSize() const
{
	return m_end - m_start + 1;
}

CharMapEntry *CodePage::GetCharMapEntry(wxUint32 code)
{
	std::map<unsigned int, CharMapEntry>::const_iterator it = m_map.find(code);
	if (it == m_map.end())
	{
		return NULL;
	}
	return &m_map.at(code);
}

void CodePage::Remove(wxUint32 code)
{
	m_map.erase(code);
}

void CodePage::Shift(int shift)
{
	if (shift < 0) {
		wxUint32 negShift = -shift;
		if (negShift > m_start) {
			// Cannot shift this amount
			return;
		}
	}
	m_start += shift;
	m_end += shift;
	std::map<unsigned int, CharMapEntry> newMap;
	for (std::map<unsigned int, CharMapEntry>::const_iterator it = m_map.cbegin();
		it != m_map.cend(); it++) {
		newMap[it->first + shift] = it->second;
	}
	m_map.swap(newMap);
}

CharMapEntry & CodePage::at(wxUint32 i)
{
	return m_map.at(i);
}

CharMapEntry & CodePage::operator[](wxUint32 i)
{
	return m_map[i];
}

void CodePage::SetName(const wxString &name)
{
	m_name = name;
}

void CodePage::SetEnd(wxUint32 end)
{
	m_end = end;
}

void CodePage::SetStart(wxUint32 start)
{
	m_start = start;
}

CharMap::CharMap()
{

}

CharMap::CharMap(const CharMap &other)
{
	for (std::list<CodePage *>::const_iterator it = other.CBegin();
		it != other.CEnd(); ++it) {
		m_map.push_back(new CodePage(**it));
	}
}

CharMap::~CharMap()
{
	for (std::list<CodePage *>::iterator it = Begin(); it != End(); ++it) {
		delete *it;
	}
}

int CharMap::GetCountCodePages() const
{
	return m_map.size();
}

std::list<CodePage *>::iterator CharMap::Begin()
{
	return m_map.begin();
}

std::list<CodePage *>::iterator CharMap::End()
{
	return m_map.end();
}

std::list<CodePage *>::const_iterator CharMap::CBegin() const
{
	return m_map.cbegin();
}

std::list<CodePage*>::const_iterator CharMap::CEnd() const
{
	return m_map.cend();
}

void CharMap::Clear()
{
	m_map.clear();
}

void CharMap::RemoveCodePage(CodePage *page)
{
	m_map.remove(page);
	delete page;
}

void CharMap::RemoveCodePage(std::list<CodePage *>::const_iterator first, std::list<CodePage *>::const_iterator last)
{
	m_map.erase(first, last);
}

void CharMap::AddCodePage(const CodePage &page)
{
	wxASSERT(CanAddCodePage(page));
	std::list<CodePage *>::const_iterator it = CBegin();
	while (it != CEnd() && (*it)->GetRangeStart() <= page.GetRangeEnd()) {
		++it;
	}
	m_map.insert(it, new CodePage(page));
}

bool CharMap::CanAddCodePage(const CodePage &page)
{
	for (std::list<CodePage*>::const_iterator it = CBegin();
		it != CEnd(); ++it)
	{
		if (!(page > **it))
		{
			return page < **it;
		}
	}
	return true;
}

void CharMap::SplitCodePage(wxUint32 splitCodeFirst)
{
	CodePage *page = GetCodePage(splitCodeFirst);
	if (page == NULL || page->GetRangeStart() == splitCodeFirst) {
		return;
	}
	CodePage oldPage(*page);
	CodePage newPage(*page, splitCodeFirst, page->GetRangeEnd());
	RemoveCodePage(page);
	// Todo: delete only present glyphs
	for (wxUint32 i = splitCodeFirst; i <= oldPage.GetRangeEnd(); i++) {
		oldPage.Remove(i);
	}
	oldPage.SetEnd(splitCodeFirst - 1);
	AddCodePage(oldPage);
	AddCodePage(newPage);
}

CodePage *CharMap::GetCodePage(wxUint32 code)
{
	for (std::list<CodePage *>::iterator it = Begin();
		it != End(); ++it)
	{
		if (code >= (*it)->GetRangeStart() && code <= (*it)->GetRangeEnd())
		{
			return *it;
		}
	}
	return NULL;
}

