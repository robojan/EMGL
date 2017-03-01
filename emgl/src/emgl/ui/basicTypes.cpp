#include <emgl/emgl_ui.h>

namespace emgl
{

	Point::Point(emgl_coord_t x, emgl_coord_t y) :
		x(x), y(y)
	{
	}

	void Point::setPos(emgl_coord_t x, emgl_coord_t y)
	{
		this->x = x;
		this->y = y;
	}

	Point Point::operator-() const {
		return Point(-x, -y);
	}

	Point operator+(const Point &lhs, const Point &rhs)
	{
		return Point(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	Point operator-(const Point &lhs, const Point &rhs)
	{
		return Point(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	Point &Point::operator+=(const Point &other)
	{
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	Point &Point::operator-=(const Point &other)
	{
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}

	Point Point::operator+(const Point &other) const
	{
		return Point(this->x + other.x, this->y + other.y);
	}

	Point Point::operator-(const Point &other) const
	{
		return Point(this->x - other.x, this->y - other.y);
	}

	Size::Size(emgl_length_t width, emgl_length_t height) :
		width(width), height(height)
	{
	}

	void Size::setSize(emgl_length_t width, emgl_length_t height)
	{
		this->width = width;
		this->height = height;
	}

	Rectangle::Rectangle(emgl_coord_t x, emgl_coord_t y, emgl_length_t width, emgl_length_t height) :
		x(x), y(y), width(width), height(height)
	{
	}

	Rectangle::Rectangle(Point pos, Size size) :
		x(pos.x), y(pos.y), width(size.width), height(size.height)
	{

	}

	Rectangle Rectangle::operator+(const Point &other) const
	{
		return Rectangle(this->x + other.x, this->y + other.y, this->width, this->height);
	}

	Rectangle Rectangle::operator-(const Point &other) const
	{
		return Rectangle(this->x - other.x, this->y - other.y, this->width, this->height);
	}
	
	Size emgl::Rectangle::getSize() const
	{
		return Size(width, height);
	}

	Point emgl::Rectangle::getPos() const
	{
		return Point(x, y);
	}

}