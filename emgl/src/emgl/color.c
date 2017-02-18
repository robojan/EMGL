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
	emgl_U8 a = mix >= 128 ? (color2 & 0x8000) : (color1 & 0x8000);
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
