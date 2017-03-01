#include <emgl/emgl_ui.h>

namespace emgl
{

	Element::Element(Element *parent, Point pos, Size size) :
		_pos(pos), _size(size), _parent(parent)
	{
		if (parent) {
			parent->AddChild(this);
		}
	}

	Element::~Element()
	{
		removeChildren();
	}

	Size Element::GetSize() const
	{
		return _size;
	}

	Point Element::GetPos() const
	{
		return _pos;
	}

	Point Element::GetScreenPos() const
	{
		if (_parent) {
			return _parent->GetScreenPos() + GetPos();
		}
		else {
			return GetPos();
		}
	}

	Rectangle Element::GetRect() const
	{
		return Rectangle(_pos, _size);
	}

	void Element::SetSize(Size size)
	{
		_size = size;
	}

	void Element::SetPos(Point pos)
	{
		_pos = pos;
		if (_parent) {
			_parent->Refresh();
		}
		else {
			Refresh();
		}
	}

	void Element::Refresh(PaintContext &PC, Rectangle &area)
	{
		this->Draw(PC, area);
		Size selfSize = GetSize();
		for (Element * child : _children)
		{
			Point pos = child->GetPos();
			PC.offsetOrigin(pos);
			Size size = child->GetSize();
			if (pos.x + size.width > selfSize.width)  size.width = selfSize.width - pos.x;
			if (pos.y + size.height > selfSize.height)  size.height = selfSize.height - pos.y;
			PC.setSize(size);
			child->Refresh(PC, area - pos);
			PC.offsetOrigin(-pos);
		}
	}

	void Element::Refresh()
	{
		Point pos = GetPos();
		Refresh(PaintContext(GetScreenPos(), GetSize()), GetRect() - pos);
	}

	void Element::Draw(PaintContext &PC, Rectangle &area)
	{

	}

	void Element::AddChild(Element *child)
	{
		_children.insert(child);
	}

	void Element::removeChildren()
	{
		for (Element *child : _children) {
			child->removeChildren();
			delete child;
		}
		_children.clear();
	}
}

