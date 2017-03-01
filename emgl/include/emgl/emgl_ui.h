/*
 * emgl_ui.h
 *
 * Created: 25-2-2017 21:13:21
 *  Author: Robojan
 */ 


#ifndef EMGL_UI_H_
#define EMGL_UI_H_

#include <emgl/emgl.h>
#include <set>
#include <vector>

namespace emgl {

	enum Alignment {
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT,
		ALIGN_TOP = ALIGN_LEFT,
		ALIGN_BOTTOM = ALIGN_RIGHT,
	};

	class Point {
	public:
		emgl_coord_t x;
		emgl_coord_t y;

		Point(emgl_coord_t x, emgl_coord_t y);

		void setPos(emgl_coord_t x, emgl_coord_t y);

		Point operator-() const;
		Point &operator+=(const Point &other);
		Point &operator-=(const Point &other);
		Point operator+(const Point &other) const;
		Point operator-(const Point &other) const;
	};

	class Size {
	public:
		emgl_length_t width;
		emgl_length_t height;

		Size(emgl_length_t width, emgl_length_t height);
		
		void setSize(emgl_length_t width, emgl_length_t height);
	};

	class Rectangle {
	public:
		emgl_coord_t x;
		emgl_coord_t y;
		emgl_length_t width;
		emgl_length_t height;

		Rectangle(emgl_coord_t x, emgl_coord_t y, emgl_length_t width, emgl_length_t height);
		Rectangle(Point pos, Size size);

		Size getSize() const;
		Point getPos() const;

		Rectangle operator+(const Point &other) const;
		Rectangle operator-(const Point &other) const;
	};

	class TextLayout {
	public:
		class GlyphPos {
		public:
			GlyphPos(emgl_U32 glyph, Point point, Size size) :
				glyph(glyph), pos(point), size(size) {}
			emgl_U32 glyph;
			Point pos;
			Size size;
		};

		void reset();
		void doLayout(const char *str, const EMGL_font_t *font, Size size = Size(-1,-1), 
			Alignment hAlignment = ALIGN_LEFT, Alignment vAlignment = ALIGN_BOTTOM, bool wrap = false);
		Size getSize() const;

		std::vector<GlyphPos>::iterator begin();
		std::vector<GlyphPos>::iterator end();
		std::vector<GlyphPos>::const_iterator cbegin() const;
		std::vector<GlyphPos>::const_iterator cend() const;

	private:
		void doAlign(Alignment hAlignment, emgl_length_t width, const std::set<int> &linebreaks);
		void doVerticalShift(Alignment vAlignment, emgl_length_t height);

		std::vector<GlyphPos> _layout;
	};

	class PaintContext {
	public:
		PaintContext(Point origin, Size size);
		PaintContext(Rectangle area);
		~PaintContext();

		void offsetOrigin(Point offset);
		void setOrigin(Point origin);
		void setSize(Size size);

		void drawFilledRectangle(const Rectangle &rect, emgl_color_t color);
		void drawText(const EMGL_font_t *font, const TextLayout &layout, emgl_color_t foreColor, 
			emgl_color_t backColor);

	private:
		Point _origin;
		Size _elementSize;
	};

	class Element {
	public:
		Element(Element *parent, Point pos, Size size);
		virtual ~Element();

		void AddChild(Element *child);
		virtual void Refresh();

		virtual void SetPos(Point pos);
		virtual void SetSize(Size size);

		virtual Point GetPos() const;
		Point GetScreenPos() const;
		virtual Size GetSize() const;
		virtual Rectangle GetRect() const;

	protected:
		virtual void Draw(PaintContext &PC, Rectangle &area);
		void Refresh(PaintContext & PC, Rectangle & area);

		void removeChildren();
	private:

		Element *_parent;
		std::set<Element *> _children;
		Point _pos;
		Size _size;
	};

	class Panel : public Element {
	public:
		Panel(Element *parent, Point pos, Size size);
		virtual ~Panel();

		void setForeground(emgl_color_t color);
		void setBackground(emgl_color_t color);
		emgl_color_t getForeground() const;
		emgl_color_t getBackground() const;

		virtual void Draw(PaintContext &PC, Rectangle &area) override;
	private:
		emgl_color_t _foregroundColor;
		emgl_color_t _backgroundColor;
	};


	class Text : public Element {
	public:
		Text(Element *parent, Point pos, Size size, const char *text, Alignment hAlignment = ALIGN_LEFT, 
			Alignment vAlignment = ALIGN_TOP, bool wrap = false);
		virtual ~Text();

		void setForeground(emgl_color_t color);
		void setBackground(emgl_color_t color);
		void setFont(const EMGL_font_t *font);
		emgl_color_t getForeground() const;
		emgl_color_t getBackground() const;
		const EMGL_font_t *getFont() const;

		const char *getText() const;
		void setText(const char *text);

		virtual void Draw(PaintContext &PC, Rectangle &area) override;
	protected:
		void calculateLayout();

	private:
		char *_text;
		TextLayout _layout;
		emgl_color_t _foregroundColor;
		emgl_color_t _backgroundColor;
		const EMGL_font_t *_font;
		Alignment _hAlignment;
		Alignment _vAlignment;
		bool _wrap; 
	};
}


#endif /* EMGL_UI_H_ */