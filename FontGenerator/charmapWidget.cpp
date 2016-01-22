
#include "charmapWidget.h"
#include "fontManager.h"
#include "font.h"

#include "memdbg.h"


CharMapWidget::CharMapWidget(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
	: wxGrid(parent, id, pos, size, style, wxGridNameStr)
{
	m_table = new CharMapGridTable();
	m_renderer = new CharGridRenderer();
	m_editor = new CharGridEditor();
	SetTable(m_table, true);
	HideColLabels();
	HideRowLabels();
	RegisterDataType(CHARMAPCHAR_TYPENAME, m_renderer, m_editor);
	//DisableCellEditControl();

	wxSize maxSize = wxSize(m_table->GetNumberCols()*65, -1); // TODO: calculate width
	wxSize minSize = wxSize(60 * 8 + wxSystemSettings::GetMetric(wxSYS_VSCROLL_X), -1);
	SetMaxSize(maxSize);
	SetMinSize(minSize);

	EnableDragCell(true);
	DisableDragColMove();
	DisableDragGridSize();

	this->Bind(wxEVT_GRID_RANGE_SELECT, &CharMapWidget::OnRangeSelect, this);
	this->Bind(wxEVT_GRID_SELECT_CELL, &CharMapWidget::OnCellSelect, this);
	//this->Bind(wxEVT_GRID_CELL_LEFT_CLICK, &CharMapWidget::GridEventDoNothing, this);
}

void CharMapWidget::AddCodePage(const CodePage &codePage)
{
	m_table->AddCodePage(codePage);
}

void CharMapWidget::AddCharmap(const CharMap &map)
{
	for (std::set<CodePage>::iterator it = map.Begin();
		it != map.End(); ++it)
	{
		AddCodePage(*it);
	}
}

CharMapEntry *CharMapWidget::GetEntry(int row, int col)
{
	CodePage *page = m_table->GetCodePage(row);
	if (page == NULL)
		return NULL;
	return page->GetCharMapEntry(m_table->GetCharMapCode(row, col));
}

CharMapGridTable * CharMapWidget::GetCharmapTable()
{
	return m_table;
}

bool CharMapWidget::IsTitleRow(int row) const
{
	wxASSERT(m_table != NULL);
	return m_table->IsTitleRow(row);
}

void CharMapWidget::OnRangeSelect(wxGridRangeSelectEvent &evt)
{
	for (int r = evt.GetTopRow(); r > 0 && r <= evt.GetBottomRow(); r++) {
		for (int c = evt.GetLeftCol(); c > 0 && c <= evt.GetRightCol(); c++) {
			if (IsInSelection(r, c)) {
				m_selectedCells.insert(wxGridCellCoords(r, c));
			}
			else {
				m_selectedCells.erase(wxGridCellCoords(r, c));
			}
		}
	}
	for (std::set<wxGridCellCoords, GridCellCoordsCompare>::iterator 
		it = m_selectedCells.begin(); it != m_selectedCells.end(); ) {
		if (!IsInSelection(*it)) {
			it = m_selectedCells.erase(it);
		}
		else {
			it++;
		}
	}
}

void CharMapWidget::OnCellSelect(wxGridEvent &evt)
{
	wxEventType type = evt.GetEventType();
	if (m_table->IsTitleRow(evt.GetRow())) {
		return;
	}
	if (evt.ShiftDown()) {
		SelectBlock(m_currentCellCoords, wxGridCellCoords(evt.GetRow(), evt.GetCol()), evt.ControlDown());
	}
	else {
		this->SetCurrentCell(evt.GetRow(), evt.GetCol());
		if (IsInSelection(evt.GetRow(), evt.GetCol())) {
			DeselectCell(evt.GetRow(), evt.GetCol());
		}
		else {
			SelectBlock(evt.GetRow(), evt.GetCol(), evt.GetRow(), evt.GetCol(), evt.ControlDown());
		}
	}
}

const wxString CharMapDataObject::CharMapDataFormatID = "DF_Font_Glyphs";

CharMapDataObject::CharMapDataObject() :
	wxDataObjectSimple(wxDataFormat(CharMapDataFormatID))
{

}

CharMapDataObject::~CharMapDataObject()
{
}

size_t CharMapDataObject::GetDataSize() const
{
	return m_glyphs.size();
}

bool CharMapDataObject::GetDataHere(void *buf) const
{
	if (m_glyphs.size() > 0) {		
		memcpy(buf, m_glyphs.data(), m_glyphs.size());
	}
	return true;
}

bool CharMapDataObject::SetData(size_t len, const void *buf)
{
	m_glyphs.clear();
	m_glyphs.resize(len);	
	memcpy(m_glyphs.data(), buf, len);
	return true;
}

int CharMapDataObject::GetNumGlyphs() const
{
	int numGlyphs = 0;
	if (m_glyphs.size() == 0) {
		return numGlyphs;
	}
	for (unsigned int i = 0; i < m_glyphs.size(); i++) {
		if (m_glyphs.data()[i] == '\0') {
			numGlyphs++;
		}
	}
	return numGlyphs;
}

wxString CharMapDataObject::GetGlyphString(int index) const
{
	if (m_glyphs.size() == 0) {
		return wxString();
	}
	const char *ptr = m_glyphs.data();
	unsigned int offset = 0;
	for (int i = 0; i < index; i++) {
		while (ptr[offset] != '\0') offset++;
		if (offset >= m_glyphs.size()) {
			return wxString();
		}
	}
	return wxString(ptr);
}

void CharMapDataObject::AddGlyphString(const wxString &str)
{
	int oldSize = m_glyphs.size();
	const wxScopedCharBuffer strData = str.To8BitData();
	m_glyphs.resize(oldSize + strData.length() + 1);
	strcpy(m_glyphs.data() + oldSize, strData.data());
}
