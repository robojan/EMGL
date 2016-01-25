#ifndef _IDATAGENERATOR_H_
#define _IDATAGENERATOR_H_

#include <wx/wx.h>
#include <wx/filename.h>

class CharMap;

#define FLAG_COMPRESSED 0x80

enum OutputFormat {
	FMT_1BPP,
	FMT_2BPP,
	FMT_4BPP,
	FMT_8BPP
};

class IDataGenerator
{
public:
	virtual wxString GetWildCardString() = 0;
	virtual bool IsFilenameSupported(const wxFileName &fileName) = 0;

	virtual void SaveCharmap(const CharMap &charmap, bool compress, 
		enum OutputFormat format, const wxString &fileName) = 0;
};

#endif
