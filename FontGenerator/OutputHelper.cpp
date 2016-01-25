#include "OutputHelper.h"

#include <vector>

size_t Convert8BppTo1Bpp(wxUint8 *buffer, int bufferSize)
{
	for (int i = 0; i < bufferSize; ++i) {
		int idx = i / 8;
		int bit = i & 7;
		wxUint8 value = buffer[i] / 128;
		buffer[idx] = buffer[idx] & ~(1 << bit) | (value << bit);
	}
	return (bufferSize + 7) / 8;
}

size_t Convert8BppTo2Bpp(wxUint8 *buffer, int bufferSize)
{
	for (int i = 0; i < bufferSize; ++i) {
		int idx = i / 4;
		int bit = (i & 3) << 1;
		wxUint8 value = buffer[i] / 64;
		buffer[idx] = buffer[idx] & ~(3 << bit) | (value << bit);
	}
	return (bufferSize + 3) / 4;
}

size_t Convert8BppTo4Bpp(wxUint8 *buffer, int bufferSize)
{
	for (int i = 0; i < bufferSize; ++i) {
		int idx = i / 2;
		int bit = (i & 1) << 2;
		wxUint8 value = buffer[i] / 16;
		buffer[idx] = buffer[idx] & ~(0xF << bit) | (value << bit);
	}
	return (bufferSize + 1) / 2;
}

size_t ConvertToFormat(wxUint8 *buffer, int bufferSize, enum OutputFormat format)
{
	switch (format) {
	case FMT_1BPP:
		return Convert8BppTo1Bpp(buffer, bufferSize);
	case FMT_2BPP:
		return Convert8BppTo2Bpp(buffer, bufferSize);
	case FMT_4BPP:
		return Convert8BppTo4Bpp(buffer, bufferSize);
	default:
		return bufferSize;
	}
}

#define _GET_PIXEL_VALUE_BIT(pixel, bpp) \
		((((pixel) % (8 / (bpp))) * (bpp)) & 7)
#define _GET_PIXEL_VALUE_IDX(pixel, bpp) \
		((pixel) / (8 / (bpp)))
#define GET_PIXEL_VALUE(buffer, pixel, bpp, mask) \
		(((buffer)[_GET_PIXEL_VALUE_IDX(pixel, bpp)] & ((mask) << _GET_PIXEL_VALUE_BIT(pixel, bpp))) >> _GET_PIXEL_VALUE_BIT(pixel, bpp))

void CompressBitmap(wxUint8 **buffer, int *bufferSize, int numPixels, enum OutputFormat format)
{
	wxASSERT(buffer != NULL);
	wxASSERT(bufferSize != NULL);
	wxASSERT_MSG((*bufferSize & (1 << 31)) == 0, "Tried to compress already compressed bitmap");
	int bpp;
	wxUint8 mask;
	switch (format) {
	case FMT_1BPP:
		bpp = 1;
		mask = 0x1;
		break;
	case FMT_2BPP:
		bpp = 2;
		mask = 0x3;
		break;
	case FMT_4BPP:
		bpp = 4;
		mask = 0xf;
		break;
	default:
		bpp = 8;
		mask = 0xff;
		break;		
	}
	std::vector<wxUint8> outBuffer;
	int outBitPos = 0;
	int outElementSize = 4 + bpp;
	outBuffer.reserve(numPixels * outElementSize / (8 * 2));
	for (int i = 0; i < numPixels; ) {
		wxUint8 value = GET_PIXEL_VALUE(*buffer, i, bpp, mask);
		wxUint8 valueCount = 0;
		while (i < numPixels && value == GET_PIXEL_VALUE(*buffer, i, bpp, mask) && valueCount <= 16) {
			i++;
			valueCount++;
		}
		wxUint16 outValue = ((valueCount - 1) & 0xF | (value << 4));
		outBuffer.resize((outBitPos + outElementSize + 7) / 8);
		int outIdx = outBitPos / 8;
		int outBit = outBitPos & 7;
		int bitsWritten = 0;
		int outElementBytes = (outBit + outElementSize + 7) / 8;
		for (int iOut = 0; iOut < outElementBytes; ++iOut) {
			int bitsFree = 8 - outBit;
			int bitsToWrite = (outElementSize - bitsWritten) > bitsFree ? bitsFree : (outElementSize - bitsWritten);
			wxUint8 outValueMask = (0xFF >> (8 - bitsToWrite));
			wxUint8 outElement = (outValue & (outValueMask << bitsWritten)) >> bitsWritten;
			outElement <<= outBit;
			wxUint8 out = outBuffer[outIdx + iOut];
			out &= ~(outValueMask << outBit);
			out |= outElement;
			outBuffer[outIdx + iOut] = out;		
			outBit += bitsToWrite;
			outBit &= 7;
			bitsWritten += bitsToWrite;
		}
		outBitPos += outElementSize;
		
	}
	if ((int)outBuffer.size() < *bufferSize) {
		// When compressing reduced the size overwrite the buffer
		wxUint8 *newBuffer = new wxUint8[outBuffer.size()];
		memcpy(newBuffer, outBuffer.data(), outBuffer.size());
		delete[] * buffer;
		*buffer = newBuffer;
		*bufferSize = (1 << 31) | (int)outBuffer.size();
	}
}

void DecompressBitmap(wxUint8 **buffer, int *bufferSize, int numPixels, enum OutputFormat format)
{
	wxASSERT(buffer != NULL);
	wxASSERT(bufferSize != NULL);
	if ((*bufferSize & (1 << 31)) == 0) {
		// Not compressed
		return;
	}
	int bpp;
	wxUint8 mask;
	switch (format) {
	case FMT_1BPP:
		bpp = 1;
		mask = 0x1;
		break;
	case FMT_2BPP:
		bpp = 2;
		mask = 0x3;
		break;
	case FMT_4BPP:
		bpp = 4;
		mask = 0xf;
		break;
	default:
		bpp = 8;
		mask = 0xff;
		break;
	}

	int outBufferSize = (numPixels * bpp + 7) / 8;
	wxUint8 *outBuffer = new wxUint8[outBufferSize];
	int bitPos = 0;
	int outBitPos = 0;
	int elementSize = 4 + bpp;
	for (int i = 0; i < numPixels; i++) {
		int idx = bitPos / 8;
		int bit = bitPos & 7;
		int elementSizeBytes = (bit + elementSize + 7) / 8;
		wxUint16 element = 0;
		int elementBitsRead = 0;
		for (unsigned int i = 0; i < elementSizeBytes; i++) {
			int bitsFree = 8 - bit;
			int bitsToRead = (elementSize - elementBitsRead) > bitsFree ? bitsFree : (elementSize - elementBitsRead);
			wxUint8 elementMask = 0xFF >> (8 - bitsToRead);
			wxUint8 elementPart = (*buffer)[idx] & (elementMask << bit);
			element |= elementPart << elementBitsRead;
		}
		int repeat = element & 0xF;
		wxUint8 value = (element >> 4) & mask;
		for (int i = 0; i < repeat; ++i) {
			int outIdx = outBitPos / 8;
			int outBit = outBitPos & 7;
			outBuffer[outIdx] = (outBuffer[outIdx] & ~(mask << outBit)) | (value << outBit);
		}
	}
	delete[] * buffer;
	*buffer = outBuffer;
	*bufferSize = outBufferSize;
}
