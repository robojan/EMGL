#ifndef _CHARMAPWIDGET_H_
#define _CHARMAPWIDGET_H_

#include <wx/wx.h>
#include <wx/grid.h>
#include <vector>
#include <set>
#include "charmap.h"
#include "font.h"
#define CHARMAPCHAR_TYPENAME wxT("charmapchar")

class CharGridRenderer : public wxGridCellRenderer
{
public:
	CharGridRenderer();
	virtual wxGridCellRenderer *Clone() const;
	virtual void Draw(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, const wxRect &rect, int row, int col, bool isSelected);
	virtual wxSize GetBestSize(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, int row, int col);
	virtual wxSize GetBestHeight(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, int row, int col, int width);
	virtual wxSize GetBestWidth(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, int row, int col, int height);
	
protected:
	virtual ~CharGridRenderer();
	void LoadFont(wxString family, wxString style, float size, wxUint32 encoding);
	Font m_loadedFont;
};

class CharGridEditor : public wxGridCellEditor
{
public:
	CharGridEditor();
	virtual void BeginEdit(int row, int col, wxGrid *grid);
	virtual wxGridCellEditor *Clone() const;
	virtual void Create(wxWindow *parent, wxWindowID id, wxEvtHandler *evtHandler);
	virtual bool EndEdit(int row, int col, const wxGrid *grid, const wxString &oldval, wxString *newval);
	virtual void ApplyEdit(int row, int col, wxGrid *grid);
	virtual void HandleReturn(wxKeyEvent &event);
	virtual void Reset();
	virtual wxString GetValue() const;

protected:
	virtual ~CharGridEditor();
private:
	wxPanel *m_panel;
	wxString m_lastEdit;
	wxString m_oldValue;
	int m_lastRow, m_lastCol;
};

class CharMapGridTable : public wxGridTableBase
{
public:
	CharMapGridTable();
	virtual ~CharMapGridTable();

	virtual int GetNumberRows();
	virtual int GetNumberCols();
	virtual bool IsEmptyCell(int row, int col);
	virtual wxString GetValue(int row, int col);
	virtual void SetValue(int row, int col, const wxString &value);
	virtual wxString GetTypeName(int row, int col);
	virtual bool CanGetValueAs(int row, int col, const wxString &typeName);
	virtual bool CanSetValueAs(int row, int col, const wxString &typeName);
	virtual long GetValueAsLong(int row, int col);
	virtual double GetValueAsDouble(int row, int col);
	virtual bool GetValueAsBool(int row, int col);
	virtual void *GetValueAsCustom(int row, int col, const wxString &typeName);
	virtual void SetValueAsLong(int row, int col, long value);
	virtual void SetValueAsDouble(int row, int col, double value);
	virtual void SetValueAsBool(int row, int col, bool value);
	virtual void SetValueAsCustom(int row, int col, const wxString &typeName, void *value);

	virtual void Clear();
	virtual bool InsertRows(size_t pos = 0, size_t numRows = 1);
	virtual bool AppendRows(size_t numRows = 1);
	virtual bool DeleteRows(size_t pos = 0, size_t numRows = 1);
	virtual bool InsertCols(size_t pos = 0, size_t numCols = 1);
	virtual bool AppendCols(size_t numCols = 1);
	virtual bool DeleteCols(size_t pos = 0, size_t numCols = 1);

	virtual wxString GetRowLabelValue(int row);
	virtual wxString GetColLabelValue(int col);
	virtual void SetRowLabelValue(int row, const wxString &label);
	virtual void SetColLabelValue(int col, const wxString &label);

	bool AddCodePage(const CodePage &codePage);
	void RemoveCodePage(int row);
	void RemoveCodePage(const CodePage &codePage);
	std::pair<int, int> GetCodePageRange(const CodePage &codePage);

	bool IsTitleRow(int row);
	CodePage *GetCodePage(int row);
	int GetCodePageStartRow(int row);
	wxUint32 GetCharMapCode(int row, int col);
	bool IsRangeEmpty(wxUint32 start, wxUint32 end);
private:

	CharMap m_charmap;
};


class CharMapDataObject : public wxDataObjectSimple
{
public:
	static const wxString CharMapDataFormatID;
	CharMapDataObject();
	virtual ~CharMapDataObject();

	virtual size_t GetDataSize() const override;
	virtual bool GetDataHere(void *buf) const override;
	virtual bool SetData(size_t len, const void *buf) override;

	int GetNumGlyphs() const;
	wxString GetGlyphString(int index) const;
	void AddGlyphString(const wxString &str);


private:
	std::vector<char> m_glyphs;
};

class GridCellCoordsCompare {
public:
	bool operator() (const wxGridCellCoords &x, const wxGridCellCoords &y) const {
		return x.GetRow() == y.GetRow() ? x.GetCol() < y.GetCol() : x.GetRow() < y.GetRow();
	}
};


class CharMapWidget : public wxGrid
{
public:
	CharMapWidget(wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize, long style = wxVSCROLL, 
		const wxString &name = _("CharMapWidget"));

	void AddCodePage(const CodePage &codePage);
	void AddCharmap(const CharMap &map);
	CharMapEntry *GetEntry(int row, int col);
	CharMapGridTable *GetCharmapTable();
	bool IsTitleRow(int row) const;

private:
	void OnRangeSelect(wxGridRangeSelectEvent &evt);
	void OnCellSelect(wxGridEvent &evt);
	

	std::set<wxGridCellCoords, GridCellCoordsCompare> m_selectedCells;
	CharMapGridTable *m_table;
	CharGridRenderer *m_renderer;
	CharGridEditor *m_editor;
};
#endif