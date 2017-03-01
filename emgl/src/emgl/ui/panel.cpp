#include <emgl/emgl_ui.h>

namespace emgl
{

	Panel::Panel(Element *parent, Point pos, Size size) :
		Element(parent, pos, size), _foregroundColor(EMGL_DEFAULT_FOREGROUND_COLOR),
		_backgroundColor(EMGL_DEFAULT_BACKGROUND_COLOR)
	{

	}

	Panel::~Panel()
	{

	}

	void Panel::setForeground(emgl_color_t color)
	{
		_foregroundColor = color;
	}

	void Panel::setBackground(emgl_color_t color)
	{
		_backgroundColor = color;
	}

	emgl_color_t Panel::getForeground() const
	{
		return _foregroundColor;
	}

	emgl_color_t Panel::getBackground() const
	{
		return _backgroundColor;
	}

	void Panel::Draw(PaintContext &PC, Rectangle &area)
	{
		PC.drawFilledRectangle(area, _backgroundColor);
	}
}
