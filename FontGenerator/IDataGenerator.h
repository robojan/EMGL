#ifndef _IDATAGENERATOR_H_
#define _IDATAGENERATOR_H_

#include <wx/wx.h>
#include <wx/filename.h>

class CharMap;

class IDataGenerator
{
public:
	virtual wxString GetWildCardString() = 0;
	virtual bool IsFilenameSupported(const wxFileName &fileName) = 0;

	virtual void SaveCharmap(const CharMap &charmap, bool compress, const wxString &fileName) = 0;
};

#endif
