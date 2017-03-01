#include <emgl/color.h>

emgl_color_t emgl_colorBlend1BPP(emgl_color_t color1, emgl_color_t color2, emgl_U8 mix)
{
	return mix >= 128 ? color2 : color1;
}

emgl_color_t emgl_colorBlend8BPP(emgl_color_t color1, emgl_color_t color2, emgl_U8 mix)
{
	return (color1  * (emgl_U16)mix + color2 * (emgl_U16)(0xFF - mix)) / 0xFF;
}

emgl_color_t emgl_colorBlendRGB332(emgl_color_t color1, emgl_color_t color2, emgl_U8 mix)
{
	emgl_U8 r1 = color1 & 0x3;
	emgl_U8 g1 = (color1 & 0x1C) >> 2;
	emgl_U8 b1 = (color1 & 0xE0) >> 5;
	emgl_U8 r2 = color2 & 0x3;
	emgl_U8 g2 = (color2 & 0x1C) >> 2;
	emgl_U8 b2 = (color2 & 0xE0) >> 5;
	emgl_U8 r = ((r1 * (emgl_U16)mix) + (r2 * (emgl_U16)(0xFF - mix))) / 0xFF;
	emgl_U8 g = ((g1 * (emgl_U16)mix) + (g2 * (emgl_U16)(0xFF - mix))) / 0xFF;
	emgl_U8 b = ((b1 * (emgl_U16)mix) + (b2 * (emgl_U16)(0xFF - mix))) / 0xFF;
	return r | (g << 2) | (b << 5);
}

emgl_color_t emgl_colorBlendRGB565(emgl_color_t color1, emgl_color_t color2, emgl_U8 mix)
{
	emgl_U8 r1 = (color1 & 0x001F);
	emgl_U8 g1 = (color1 & 0x07E0) >> 5;
	emgl_U8 b1 = (color1 & 0xF800) >> 11;
	emgl_U8 r2 = (color2 & 0x001F);
	emgl_U8 g2 = (color2 & 0x07E0) >> 5;
	emgl_U8 b2 = (color2 & 0xF800) >> 11;
	emgl_U8 r = ((r1 * (emgl_U16)mix) + (r2 * (emgl_U16)(0xFF - mix))) / 0xFF;
	emgl_U8 g = ((g1 * (emgl_U16)mix) + (g2 * (emgl_U16)(0xFF - mix))) / 0xFF;
	emgl_U8 b = ((b1 * (emgl_U16)mix) + (b2 * (emgl_U16)(0xFF - mix))) / 0xFF;
	return r | (g << 5) | (b << 11);
}

emgl_color_t emgl_colorBlendRGBA5551(emgl_color_t color1, emgl_color_t color2, emgl_U8 mix)
{
	emgl_U8 r1 = (color1 & 0x001F);
	emgl_U8 g1 = (color1 & 0x07E0) >> 5;
	emgl_U8 b1 = (color1 & 0xF800) >> 11;
	emgl_U8 r2 = (color2 & 0x001F);
	emgl_U8 g2 = (color2 & 0x07E0) >> 5;
	emgl_U8 b2 = (color2 & 0xF800) >> 11;
	emgl_U8 r = ((r1 * (emgl_U16)mix) + (r2 * (emgl_U16)(0xFF - mix))) / 0xFF;
	emgl_U8 g = ((g1 * (emgl_U16)mix) + (g2 * (emgl_U16)(0xFF - mix))) / 0xFF;
	emgl_U8 b = ((b1 * (emgl_U16)mix) + (b2 * (emgl_U16)(0xFF - mix))) / 0xFF;
	emgl_U16 a = mix >= 128u ? (color2 & 0x8000) : (color1 & 0x8000);
	return r | (g << 5) | (b << 10) | a;
}

emgl_color_t emgl_colorBlendRGBA8888(emgl_color_t color1, emgl_color_t color2, emgl_U8 mix)
{
	emgl_U8 r1 = (color1 & 0x000000FF);
	emgl_U8 g1 = (color1 & 0x0000FF00) >> 8;
	emgl_U8 b1 = (color1 & 0x00FF0000) >> 16;
	emgl_U8 a1 = (color1 & 0xFF000000) >> 24;
	emgl_U8 r2 = (color2 & 0x000000FF);
	emgl_U8 g2 = (color2 & 0x0000FF00) >> 8;
	emgl_U8 b2 = (color2 & 0x00FF0000) >> 16;
	emgl_U8 a2 = (color2 & 0xFF000000) >> 24;
	emgl_U8 r = ((r1 * (emgl_U16)mix) + (r2 * (emgl_U16)(0xFF - mix))) / 0xFF;
	emgl_U8 g = ((g1 * (emgl_U16)mix) + (g2 * (emgl_U16)(0xFF - mix))) / 0xFF;
	emgl_U8 b = ((b1 * (emgl_U16)mix) + (b2 * (emgl_U16)(0xFF - mix))) / 0xFF;
	emgl_U8 a = ((a1 * (emgl_U16)mix) + (a2 * (emgl_U16)(0xFF - mix))) / 0xFF;
	return r | (g << 8) | (b << 16) | (a << 24);
}

emgl_color_t emgl_colorLighter(emgl_color_t color)
{
	emgl_U32 c = emgl_colorConvToRGBA8888(color);
	emgl_U8 r = (c & 0x000000FF);
	emgl_U8 g = (c & 0x0000FF00) >> 8;
	emgl_U8 b = (c & 0x00FF0000) >> 16;
	emgl_U8 a = (c & 0xFF000000) >> 24;
	r = (r >= 0xcc) ? 0xff : r + r/4;
	g = (g >= 0xcc) ? 0xff : g + g/4;
	b = (b >= 0xcc) ? 0xff : b + b/4;
	return emgl_colorConvFromRGBA8888(r | (g << 8) | (b << 16) | (a << 24));
}

emgl_color_t emgl_colorDarker(emgl_color_t color)
{
	emgl_U32 c = emgl_colorConvToRGBA8888(color);
	emgl_U8 r = (c & 0x000000FF);
	emgl_U8 g = (c & 0x0000FF00) >> 8;
	emgl_U8 b = (c & 0x00FF0000) >> 16;
	emgl_U8 a = (c & 0xFF000000) >> 24;
	r = r - r/4;
	g = g - g/4;
	b = b - b/4;
	return emgl_colorConvFromRGBA8888(r | (g << 8) | (b << 16) | (a << 24));
}

emgl_color_t emgl_colorDoubleLighter(emgl_color_t color)
{
	emgl_U32 c = emgl_colorConvToRGBA8888(color);
	emgl_U8 r = (c & 0x000000FF);
	emgl_U8 g = (c & 0x0000FF00) >> 8;
	emgl_U8 b = (c & 0x00FF0000) >> 16;
	emgl_U8 a = (c & 0xFF000000) >> 24;
	r = (r >= 0xAA) ? 0xff : r + r / 2;
	g = (g >= 0xAA) ? 0xff : g + g / 2;
	b = (b >= 0xAA) ? 0xff : b + b / 2;
	return emgl_colorConvFromRGBA8888(r | (g << 8) | (b << 16) | (a << 24));
}

emgl_color_t emgl_colorDoubleDarker(emgl_color_t color)
{
	emgl_U32 c = emgl_colorConvToRGBA8888(color);
	emgl_U8 r = (c & 0x000000FF);
	emgl_U8 g = (c & 0x0000FF00) >> 8;
	emgl_U8 b = (c & 0x00FF0000) >> 16;
	emgl_U8 a = (c & 0xFF000000) >> 24;
	r = r - r / 2;
	g = g - g / 2;
	b = b - b / 2;
	return emgl_colorConvFromRGBA8888(r | (g << 8) | (b << 16) | (a << 24));
}