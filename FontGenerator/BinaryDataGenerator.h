#pragma once
#include "IDataGenerator.h"
#include "font.h"
class BinaryDataGenerator :
	public IDataGenerator
{
public:
	BinaryDataGenerator();
	~BinaryDataGenerator();

	virtual wxString GetWildCardString() override;

	virtual bool IsFilenameSupported(const wxFileName &fileName) override;

	virtual void SaveCharmap(const CharMap &charmap, bool compress, const wxString &fileName) override;

private:
	void LoadFont(wxString family, wxString style, float size, wxUint32 encoding);
	Font m_loadedFont;
};

