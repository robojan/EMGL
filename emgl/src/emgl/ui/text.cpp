#include <emgl/emgl_ui.h>
#include <emgl/debug.h>
#include <emgl/font.h>

#include <cstring>

extern const EMGL_font_t *EMGL_DEFAULT_FONT;

namespace emgl {
	
	void TextLayout::reset()
	{
		_layout.clear();
	}

	void TextLayout::doLayout(const char *str, const EMGL_font_t *font, Size size /*= Size(-1, -1) */,
		Alignment hAlignment /*= ALIGN_LEFT*/, Alignment vAlignment /*= ALIGN_TOP*/, 
		bool wrap /*= false*/)
	{
		emgl_coord_t x = 0;
		emgl_coord_t y = -font->ascender;
		std::set<int> linebreaks;

		const char *ptr = str;
		while (*ptr != '\0') {
			if (*ptr == '\n') {
				x = 0;
				y -= font->ascender;
				if (_layout.size() > 0) {
					linebreaks.insert(_layout.size() - 1);
				}
			}
			else if (*ptr == '\t') {
				int remainder = x % EMGL_TAB_SIZE;
				x += remainder == 0 ? 0 : (EMGL_TAB_SIZE - remainder);
			}
			else {
				const EMGL_glyph_t *glyph = emgl_getGlyph(font, *ptr);
				if (glyph) {
					if (x + glyph->bitmapWidth > size.width && size.width > 0 && wrap) {
						x = 0;
						y -= font->ascender;
						linebreaks.insert(_layout.size()-1);
					}
					_layout.push_back(GlyphPos(*ptr, Point(x,y), Size(glyph->bitmapWidth, glyph->bitmapHeight)));
					x += glyph->advanceX;
					y -= glyph->advanceY;
				}
			}
			ptr++;
		}
		linebreaks.insert(_layout.size()-1);
		if (size.width > 0) {
			doAlign(hAlignment, size.width, linebreaks);
		}
		doVerticalShift(vAlignment, size.height);
	}

	void TextLayout::doAlign(Alignment hAlignment, emgl_length_t width, const std::set<int> &linebreaks) {
		EMGL_ASSERT("linebreaks must have at least 1 element!", linebreaks.size() > 0);
		EMGL_ASSERT("width must be > 0!", width > 0);
		int lineWidth;
		int lineStart = 0;
		int lineEnd = 0;
		auto it = linebreaks.begin();
		do {
			lineEnd = *it;
			auto lastGlyph = _layout.at(lineEnd);
			lineWidth = lastGlyph.pos.x + lastGlyph.size.width + 2 ;
			emgl_coord_t offset;
			switch(hAlignment) {
			default:
			case ALIGN_LEFT:
				offset = 0;
				break;
			case ALIGN_CENTER:
				offset = (width - lineWidth) / 2;
				break;
			case ALIGN_RIGHT:
				offset = width - lineWidth;
				break;
			}
			for (int i = lineStart; i <= lineEnd; i++) {
				_layout[i].pos.x += offset;
			}
			lineStart = lineEnd+1;
			it++;
		} while (it != linebreaks.end());
	}

	void TextLayout::doVerticalShift(Alignment vAlign, emgl_length_t height)
	{
		if (_layout.size() == 0) {
			return;
		}
		emgl_length_t size = -_layout.at(_layout.size() - 1).pos.y;
		emgl_coord_t offset = size;
		if (height > 0)
		{
			switch (vAlign) {
			default:
			case ALIGN_TOP:
				offset = height;
				break;
			case ALIGN_CENTER:
				offset = size + (height - size) / 2;
				break;
			case ALIGN_BOTTOM:
				offset = size;
				break;
			}
		}
		for (auto &glyph : _layout) {
			glyph.pos.y += offset;
		}
	}

	Size TextLayout::getSize() const
	{
		emgl_length_t width = 0;
		emgl_length_t height = 0;

		for (GlyphPos element : _layout)
		{
			if (element.pos.x + element.size.width > width) {
				width = element.pos.x + element.size.width;
			}
			if (element.pos.y + element.size.height > height) {
				height = element.pos.y + element.size.height;
			}
		}

		return Size(width, height);
	}

	std::vector<TextLayout::GlyphPos>::iterator TextLayout::begin() {
		return _layout.begin();
	}

	std::vector<TextLayout::GlyphPos>::iterator TextLayout::end() {
		return _layout.end();
	}

	std::vector<TextLayout::GlyphPos>::const_iterator TextLayout::cbegin() const {
		return _layout.cbegin();
	}

	std::vector<TextLayout::GlyphPos>::const_iterator TextLayout::cend() const {
		return _layout.cend();
	}

	Text::Text(Element *parent, Point pos, Size size, const char *text, Alignment hAlignment,
		Alignment vAlignment, bool wrap) :
		Element(parent, pos, size), _backgroundColor(EMGL_DEFAULT_BACKGROUND_COLOR),
		_foregroundColor(EMGL_DEFAULT_FOREGROUND_COLOR), _font(EMGL_DEFAULT_FONT),
		_text(nullptr), _hAlignment(hAlignment), _vAlignment(vAlignment), _wrap(wrap)
	{
		EMGL_ASSERT("text != nullptr", text != nullptr);
		int textLen = strlen(text);
		_text = new char[textLen + 1];
		memcpy(_text, text, textLen + 1);
		calculateLayout();
	}

	Text::~Text()
	{
		if (_text) {
			delete[] _text;
			_text = nullptr;
		}
	}

	void Text::setForeground(emgl_color_t color)
	{
		_foregroundColor = color;
	}

	void Text::setBackground(emgl_color_t color)
	{
		_backgroundColor = color;
	}

	void Text::setFont(const EMGL_font_t *font)
	{
		_font = font;
		calculateLayout();
	}

	emgl_color_t Text::getForeground() const
	{
		return _foregroundColor;
	}

	emgl_color_t Text::getBackground() const
	{
		return _backgroundColor;
	}

	const EMGL_font_t *Text::getFont() const
	{
		return _font;
	}

	const char *Text::getText() const
	{
		return _text;
	}

	void Text::setText(const char *text)
	{
		EMGL_ASSERT("text != nullptr", text != nullptr);
		int textLen = strlen(text);
		delete[] _text;
		_text = new char[textLen + 1];
		memcpy(_text, text, textLen + 1);
		calculateLayout();
	}

	void Text::calculateLayout()
	{
		_layout.reset();
		_layout.doLayout(_text, _font, GetSize(), _hAlignment, _vAlignment, _wrap);
	}

	void emgl::Text::Draw(PaintContext &PC, Rectangle &area)
	{
		PC.drawFilledRectangle(area, _backgroundColor);

		PC.drawText(_font, _layout, _foregroundColor, _backgroundColor);
	}

}