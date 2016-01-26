#pragma once
#include "IDataGenerator.h"
#include "font.h"
#include <wx/wx.h>

class CSourceDataGenerator :
	public IDataGenerator
{
public:
	CSourceDataGenerator();
	~CSourceDataGenerator();

	virtual wxString GetWildCardString() override;

	virtual bool IsFilenameSupported(const wxFileName &fileName) override;

	virtual void SaveCharmap(const CharMap &charmap, bool compress, enum OutputFormat format, const wxString &fileName) override;

private:
	typedef struct {
		int16_t advanceX;
		int16_t advanceY;
		int16_t bitmapLeft;
		int16_t bitmapTop;
		uint16_t bitmapWidth;
		uint16_t bitmapHeight;
		uint32_t bitmapSize;
		uint8_t *bitmapData;
	} EMGL_glyph_t;

	typedef struct {
		uint32_t startCode;
		uint32_t endCode;
		char name[17];
		EMGL_glyph_t **glyphPtrs;
	} EMGL_codePage_t;

	typedef struct {
		int16_t ascender;
		uint8_t bpp;
		uint8_t numCodepages;
		EMGL_codePage_t *codePagePtrs;
	} EMGL_font_t;

	void LoadFont(wxString family, wxString style, float size, wxUint32 encoding);
	static wxString MakeCorrectName(const wxString &x);
	EMGL_font_t *LoadEMGLFont(const CharMap &charmap, bool compress, enum OutputFormat format);
	void DeleteEMGLFont(EMGL_font_t *font);
	// header file
	wxString GetHeaderFile(wxString name);
	wxString GetHeaderIncludes();
	wxString GetStructureDeclaration();
	wxString GetFontDeclaration(const wxString &name);
	// source file
	wxString GetSourceIncludes(const wxString &name);
	wxString GetGlyphDataDefinitions(const EMGL_font_t *font);
	wxString GetGlyphDefinitions(const EMGL_font_t *font);
	wxString GetGlyphTableDefinition(const EMGL_font_t *font);
	wxString GetCodepageDefinitions(const EMGL_font_t *font);
	wxString GetFontDefinition(const EMGL_font_t *font, const wxString &name);
	wxString GetSourceFile(const EMGL_font_t *font, wxString name);
	Font m_loadedFont;
};

