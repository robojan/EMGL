#pragma once

#include <wx/wx.h>

#include "IDataGenerator.h"

size_t Convert8BppTo1Bpp(wxUint8 *buffer, int bufferSize); // 8 pixels per byte
size_t Convert8BppTo2Bpp(wxUint8 *buffer, int bufferSize); // 4 pixels per byte
size_t Convert8BppTo4Bpp(wxUint8 *buffer, int bufferSize); // 2 pixels per byte
size_t ConvertToFormat(wxUint8 *buffer, int bufferSize, enum OutputFormat format);

void CompressBitmap(wxUint8 **buffer, int *bufferSize, int numPixels, enum OutputFormat format);
void DecompressBitmap(wxUint8 **buffer, int *bufferSize, int numPixels, enum OutputFormat format);