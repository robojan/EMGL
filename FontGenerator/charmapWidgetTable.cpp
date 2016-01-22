
#include "charmapWidget.h"
#include "fontManager.h"
#include "font.h"

#include "memdbg.h"

CharMapGridTable::CharMapGridTable()
{

}

CharMapGridTable::~CharMapGridTable()
{

}

bool CharMapGridTable::IsRangeEmpty(wxUint32 start, wxUint32 end)
{
	for (std::set<CodePage>::const_iterator it = m_charmap.Begin();
		it != m_charmap.End(); ++it) {
		if ((start >= it->GetRangeStart() && start <= it->GetRangeEnd()) ||
			(end >= it->GetRangeStart() && end <= it->GetRangeEnd())) {
			return false;
		}
	}
	return true;
}

std::pair<int, int> CharMapGridTable::GetCodePageRange(const CodePage &codePage)
{
	int start = -1;
	int end = -1;
	int pos = 0;
	for (std::set<CodePage>::const_iterator it = m_charmap.Begin();
		it != m_charmap.End(); ++it) {
		int size = (it->GetRangeEnd() - it->GetRangeStart() + GetNumberCols() - 1) /
			GetNumberCols() + 1;
		if (*it == codePage) {
			start = pos;
			end = pos + size - 1;
			return std::make_pair(start, end);
		}

		pos += size;
	}
	return std::make_pair(-1, -1);
}

int CharMapGridTable::GetNumberRows()
{
	int count = 0;

	for (std::set<CodePage>::const_iterator it = m_charmap.Begin();
		it != m_charmap.End(); ++it)
	{
		int chars = it->GetRangeEnd() - it->GetRangeStart();
		count += (chars + GetNumberCols() - 1) / GetNumberCols();
	}
	return count;
}

int CharMapGridTable::GetNumberCols()
{
	return 8;
}

bool CharMapGridTable::IsEmptyCell(int row, int col)
{
	return false;
}

wxString CharMapGridTable::GetValue(int row, int col)
{
	CodePage *page = GetCodePage(row);
	if (IsTitleRow(row))
	{
		return page->GetName();
	}
	else {
		wxUint32 code = GetCharMapCode(row, col);
		CharMapEntry *entry = page->GetCharMapEntry(code);
		if (entry == NULL)
		{
			return wxString();
		}
		else {
			return entry->ToString();
		}
	}
}

void CharMapGridTable::SetValue(int row, int col, const wxString &value)
{
	CodePage *page = GetCodePage(row);
	if (IsTitleRow(row))
	{
		return page->SetName(value);
	}
	else {
		wxUint32 code = GetCharMapCode(row, col);
		CharMapEntry *entry = page->GetCharMapEntry(code);
		if (entry == NULL)
		{
			(*page)[code] = CharMapEntry(value);
		}
		else {
			*entry = CharMapEntry(value);
		}
	}
}

wxString CharMapGridTable::GetTypeName(int row, int col)
{
	return CHARMAPCHAR_TYPENAME;
}

bool CharMapGridTable::CanGetValueAs(int row, int col, const wxString &typeName)
{
	return false;
}

bool CharMapGridTable::CanSetValueAs(int row, int col, const wxString &typeName)
{
	return false;
}

long CharMapGridTable::GetValueAsLong(int row, int col)
{
	return -1;
}

double CharMapGridTable::GetValueAsDouble(int row, int col)
{
	return NAN;
}

bool CharMapGridTable::GetValueAsBool(int row, int col)
{
	return false;
}

void *CharMapGridTable::GetValueAsCustom(int row, int col, const wxString &typeName)
{
	return NULL;
}

void CharMapGridTable::SetValueAsLong(int row, int col, long value)
{

}

void CharMapGridTable::SetValueAsDouble(int row, int col, double value)
{

}

void CharMapGridTable::SetValueAsBool(int row, int col, bool value)
{

}

void CharMapGridTable::SetValueAsCustom(int row, int col, const wxString &typeName, void *value)
{

}

void CharMapGridTable::Clear()
{

}

bool CharMapGridTable::InsertRows(size_t pos /*= 0*/, size_t numRows /*= 1*/)
{
	return false;
}

bool CharMapGridTable::AppendRows(size_t numRows /*= 1*/)
{
	return false;
}

bool CharMapGridTable::DeleteRows(size_t pos /*= 0*/, size_t numRows /*= 1*/)
{
	return false;
}

bool CharMapGridTable::InsertCols(size_t pos /*= 0*/, size_t numCols /*= 1*/)
{
	return false;
}

bool CharMapGridTable::AppendCols(size_t numCols /*= 1*/)
{
	return false;
}

bool CharMapGridTable::DeleteCols(size_t pos /*= 0*/, size_t numCols /*= 1*/)
{
	return false;
}

wxString CharMapGridTable::GetRowLabelValue(int row)
{
	return wxString();
}

wxString CharMapGridTable::GetColLabelValue(int col)
{
	return wxString();
}

void CharMapGridTable::SetRowLabelValue(int row, const wxString &label)
{

}

void CharMapGridTable::SetColLabelValue(int col, const wxString &label)
{

}

bool CharMapGridTable::AddCodePage(const CodePage &codePage)
{
	int rows = 0;
	int numRows = 0;
	if (!m_charmap.CanAddCodePage(codePage))
	{
		return false;
	}
	if (GetView())
	{
		for (std::set<CodePage>::const_iterator it = m_charmap.Begin();
			it != m_charmap.End(); ++it)
		{
			if (it->GetRangeEnd() > codePage.GetRangeStart())
			{
				break;
			}
			rows += 1 + (it->GetSize() + GetNumberCols() - 1) / GetNumberCols();
		}
		numRows = (codePage.GetSize() + GetNumberCols() - 1) / GetNumberCols() + 1;
	}
	m_charmap.AddCodePage(codePage);
	if (GetView())
	{
		wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_INSERTED, rows, numRows);
		GetView()->ProcessTableMessage(msg);
		for (int y = 1; y < numRows; y++) {
			for (int x = 0; x < GetNumberCols(); x++) {
				GetView()->SetReadOnly(rows + y, x, true);
			}
		}
		GetView()->SetCellSize(rows, 0, 1, GetNumberCols());
		GetView()->AutoSizeRows(true);
		GetView()->AutoSizeColumns();
	}
	return true;
}

void CharMapGridTable::RemoveCodePage(const CodePage &codePage)
{
	std::pair<int, int> range = GetCodePageRange(codePage);
	if (range.first == -1 || range.second == -1) {
		return;
	}
	m_charmap.RemoveCodePage(codePage);
	if (GetView()) {
		wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_DELETED, range.first, range.second - range.first + 1);
		GetView()->ProcessTableMessage(msg);
	}
}

void CharMapGridTable::RemoveCodePage(int row)
{
	if (!IsTitleRow(row)) {
		return;
	}
	RemoveCodePage(*GetCodePage(row));
}

bool CharMapGridTable::IsTitleRow(int row)
{
	int currentRow = 0;
	for (std::set<CodePage>::const_iterator it = m_charmap.Begin();
		it != m_charmap.End(); ++it)
	{
		if (row == currentRow)
		{
			return true;
		}
		if (row < currentRow)
		{
			return false;
		}
		currentRow += 1 + (it->GetSize() + GetNumberCols() - 1) / GetNumberCols();
	}
	return false;
}

CodePage * CharMapGridTable::GetCodePage(int row)
{
	int currentRow = 0;
	for (std::set<CodePage>::iterator it = m_charmap.Begin();
		it != m_charmap.End(); ++it)
	{
		int size = 1 + (it->GetSize() + GetNumberCols() - 1) / GetNumberCols();
		if (row >= currentRow && row < currentRow + size)
		{
			return const_cast<CodePage *>(&(*it));
		}
		currentRow += size;
	}
	return NULL;
}

int CharMapGridTable::GetCodePageStartRow(int row)
{
	int currentRow = 0;
	for (std::set<CodePage>::iterator it = m_charmap.Begin();
		it != m_charmap.End(); ++it)
	{
		int size = 1 + (it->GetSize() + GetNumberCols() - 1) / GetNumberCols();
		if (row >= currentRow && row < currentRow + size)
		{
			return currentRow;
		}
		currentRow += size;
	}
	return -1;
}

wxUint32 CharMapGridTable::GetCharMapCode(int row, int col)
{
	return (row - GetCodePageStartRow(row) - 1) * GetNumberCols() +
		col + GetCodePage(row)->GetRangeStart();
}

CharGridRenderer::CharGridRenderer()
{

}

wxGridCellRenderer * CharGridRenderer::Clone() const
{
	return new CharGridRenderer();
}

void CharGridRenderer::Draw(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, const wxRect &rect, int row, int col, bool isSelected)
{
	CharMapGridTable *table = dynamic_cast<CharMapGridTable *>(grid.GetTable());
	CodePage *page = table->GetCodePage(row);
	int startrow = table->GetCodePageStartRow(row);
	wxColour backgroundColour = grid.GetBackgroundColour();
	wxBrush backgroundBrush = wxBrush(backgroundColour);

	if (page == NULL)
	{
		dc.SetBrush(backgroundBrush);
		dc.SetPen(*wxTRANSPARENT_PEN);
		dc.DrawRectangle(rect);
		return;
	}
	if (row == startrow)
	{
		dc.SetBrush(backgroundBrush);
		dc.SetPen(*wxTRANSPARENT_PEN);
		dc.DrawRectangle(rect);
		wxSize labelSize = dc.GetTextExtent(page->GetName());
		dc.SetTextBackground(backgroundColour);
		dc.DrawText(page->GetName(), (rect.GetWidth() - labelSize.GetWidth()) / 2 + rect.GetLeft(),
			(rect.GetHeight() - labelSize.GetHeight()) / 2 + rect.GetTop());
		return;
	}
	else {
		wxUint32 charmapCode = table->GetCharMapCode(row, col);
		CharMapEntry *entry = page->GetCharMapEntry(charmapCode);
		if (charmapCode > page->GetRangeEnd() || entry == NULL)
		{
			dc.SetBrush(backgroundBrush);
			dc.SetPen(*wxTRANSPARENT_PEN);
			dc.DrawRectangle(rect);
			dc.SetTextBackground(backgroundColour);
			dc.DrawText(wxString::Format(_("%X"), charmapCode), rect.GetLeftTop() + wxPoint(2, 2));
			return;
		}
		wxGridCellRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);
		wxColour cellBackgroundColour;
		if (isSelected)
		{
			cellBackgroundColour = grid.GetSelectionBackground();
		}
		else {
			cellBackgroundColour = attr.GetBackgroundColour();
		}
		dc.SetTextBackground(cellBackgroundColour);
		dc.DrawText(wxString::Format(_("%X"), charmapCode), rect.GetLeftTop() + wxPoint(2, 2));

		LoadFont(entry->GetFamily(), entry->GetStyle(), entry->GetSize(), entry->GetEncodingID());
		if (m_loadedFont.IsOk())
		{
			wxUint32 glyph = m_loadedFont.GetGlyphIndex(entry->GetCode());
			wxPoint pos = rect.GetTopLeft();
			int glyphWidth, glyphHeight;
			m_loadedFont.GetGlyphBitmap(glyph, NULL, &glyphWidth, &glyphHeight);
			wxPoint offset((rect.GetWidth() - glyphWidth) / 2,
				(rect.GetHeight() + glyphHeight) / 2);
			m_loadedFont.DrawGlyph(dc, pos + offset, glyph);
		}
	}
}

wxSize CharGridRenderer::GetBestSize(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, int row, int col)
{
	CharMapGridTable *table = dynamic_cast<CharMapGridTable *>(grid.GetTable());
	wxSize fontSize = attr.GetFont().GetPixelSize();
	int height = table->IsTitleRow(row) ? fontSize.GetHeight() + 10 : 50;
	return wxSize(50, height);
}

wxSize CharGridRenderer::GetBestHeight(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, int row, int col, int width)
{
	return wxSize(width, width);
}

wxSize CharGridRenderer::GetBestWidth(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, int row, int col, int height)
{
	return wxSize(height, height);
}

CharGridRenderer::~CharGridRenderer()
{

}

void CharGridRenderer::LoadFont(wxString family, wxString style, float size, wxUint32 encoding)
{
	if (!m_loadedFont.IsOk() || m_loadedFont.GetFamily() != family || m_loadedFont.GetStyle() != style)
	{
		// Load different file
		m_loadedFont = FontManager::GetFont(family, style, 10.0f);
	}
	if (m_loadedFont.GetSize() != size)
	{
		m_loadedFont.SetSize(size);
	}
	if (m_loadedFont.GetSelectedEncodingId() != encoding)
	{
		m_loadedFont.SelectEncoding(encoding);
	}
}

CharGridEditor::CharGridEditor() :
wxGridCellEditor(), m_lastCol(0), m_lastRow(0)
{
}

void CharGridEditor::BeginEdit(int row, int col, wxGrid *grid)
{
	m_oldValue = grid->GetCellValue(row, col);
	m_lastRow = row; 
	m_lastCol = col;
	wxTextCtrl *text = dynamic_cast<wxTextCtrl *>(m_control);

	text->SetValue(grid->GetTable()->GetValue(row, col));
	text->SetInsertionPointEnd();
	text->SelectAll();
	text->SetFocus();
}

wxGridCellEditor * CharGridEditor::Clone() const
{
	CharGridEditor *clone = new CharGridEditor();
	clone->m_lastCol = m_lastCol;
	clone->m_lastRow = m_lastRow;
	clone->m_lastEdit = m_lastEdit;
	clone->m_oldValue = m_oldValue;
	clone->m_panel = m_panel;
	return clone;
}

void CharGridEditor::Create(wxWindow *parent, wxWindowID id, wxEvtHandler *evtHandler)
{
	int style = wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB | wxNO_BORDER;

	wxTextCtrl* const text = new wxTextCtrl(parent, id, wxEmptyString,
		wxDefaultPosition, wxDefaultSize,
		style);
	text->SetMargins(0, 0);
	m_control = text;

	wxGridCellEditor::Create(parent, id, evtHandler);
}

bool CharGridEditor::EndEdit(int row, int col, const wxGrid *grid, const wxString &oldval, wxString *newval)
{
	const CharMapWidget *widget = dynamic_cast<const CharMapWidget *>(grid);
	if (widget == NULL || !widget->IsTitleRow(row) || m_control == NULL || 
		m_lastCol !=col || m_lastRow != row) {
		return false;
	}
	wxTextCtrl *text = dynamic_cast<wxTextCtrl *>(m_control);
	wxASSERT(text != NULL);
	m_lastEdit = text->GetValue();
	*newval = m_lastEdit;
	return true;
}

void CharGridEditor::ApplyEdit(int row, int col, wxGrid *grid)
{
	if (m_lastCol != col || m_lastRow != row) {
		return;
	}
	CharMapWidget *widget = dynamic_cast<CharMapWidget *>(grid);
	widget->GetCharmapTable()->GetCodePage(row)->SetName(m_lastEdit);
}

void CharGridEditor::HandleReturn(wxKeyEvent &event)
{

}

void CharGridEditor::Reset()
{

}

wxString CharGridEditor::GetValue() const
{
	wxTextCtrl *text = dynamic_cast<wxTextCtrl *>(m_control);
	wxASSERT(text != NULL);
	return text->GetValue();
}

CharGridEditor::~CharGridEditor()
{
}
