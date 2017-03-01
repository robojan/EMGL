#include <emgl/emgl_ui.h>
#include <algorithm>

namespace emgl
{

	PaintContext::PaintContext(Point origin, Size size) :
		_origin(origin), _elementSize(size)
	{
	}

	PaintContext::PaintContext(Rectangle area) :
		_origin(area.getPos()), _elementSize(area.getSize())
	{
	}

	PaintContext::~PaintContext()
	{
	}

	void PaintContext::offsetOrigin(Point offset)
	{
		_origin += offset;
	}

	void PaintContext::setOrigin(Point origin)
	{
		_origin = origin;
	}

	void PaintContext::setSize(Size size)
	{
		_elementSize = size;
	}

	void PaintContext::drawFilledRectangle(const Rectangle & rect, emgl_color_t color)
	{
		emgl_coord_t x = std::max(rect.x, 0);
		emgl_coord_t y = std::max(rect.y, 0);
		emgl_coord_t width = std::max(std::min(rect.x + rect.width, _elementSize.width) - x, 0);
		emgl_coord_t height = std::max(std::min(rect.y + rect.height, _elementSize.height) - y, 0);

		emgl_drawFilledRectangle(_origin.x + x, _origin.y + y, width, height, color);
	}

	void PaintContext::drawText(const EMGL_font_t *font, const TextLayout &layout, 
		emgl_color_t foreColor, emgl_color_t backColor)
	{
		for (std::vector<TextLayout::GlyphPos>::const_iterator glyph = layout.cbegin(); 
			glyph != layout.cend(); glyph++) {
			emgl_coord_t x = _origin.x + glyph->pos.x;
			emgl_coord_t y = _origin.y + glyph->pos.y;
			if (x < _origin.x || y < _origin.y) continue;
			if (glyph->pos.x + glyph->size.width > _elementSize.width ||
				glyph->pos.y + glyph->size.height > _elementSize.height) continue;
			emgl_drawGlyph(font, glyph->glyph, &x, &y, foreColor, backColor);
		}
	}

}