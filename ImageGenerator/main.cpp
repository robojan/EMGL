
#include <iostream>
#include <fstream>
#include <stdint.h>
#include "CImg.h"

using namespace std;
using namespace cimg_library;

void main(void)
{
	CImg<uint8_t> image("parrot.jpg");

	ofstream out = ofstream("parrot.h", ofstream::out);
	out << "#ifndef _BITMAP_" << "PARROT" << "_H_" << endl;
	out << "#define _BITMAP_" << "PARROT" << "_H_" << endl << endl;

	out << "struct _bitmap_" << "parrot" << "{" << endl;
	out << "\tcoord_t width;" << endl;
	out << "\tcoord_t heigth;" << endl;
	out << "\tconst emgl_color_t data[" << image.width() * image.height() * 4 << "];" << endl;
	out << "} bitmap_" << "parrot" << " = { " << image.width() << ", ";
	out << image.height() << ", {";
	
	uint8_t *data = image.data();
	
	uint32_t *imageData = new uint32_t[image.width()*image.height()];

	for (int x = 0; x < image.width(); x++)
	{
		for (int y = 0; y < image.height(); y++)
		{
			uint8_t r = image(x,y,0,0);
			uint8_t g = image(x,y,0,1);
			uint8_t b = image(x,y,0,2);
			uint8_t a = 0xFF;
			imageData[x + (image.height()-y-1)*image.width()] = (a << 24) | (r << 16) | (g << 8) | (b << 0); 
		}
	}

	char buffer[255];
	for (int i = 0; i < image.width()*image.height(); i++)
	{
		if (i % 5 == 0)
		{
			out << endl << "\t";
		}
		/*sprintf_s(buffer, "0x%x,0x%x,0x%x,0x%x,", 
			(imageData[i] >> 24) & 0xFF,
			(imageData[i] >> 16) & 0xFF,
			(imageData[i] >> 8) & 0xFF,
			(imageData[i] >> 0) & 0xFF);*/
		sprintf_s(buffer, "0x%x,",
			imageData[i]);
		out << buffer;
	}

	delete[] imageData;

	out << "}};" << endl;
	out << "#endif" << endl;
	out.close();
}
