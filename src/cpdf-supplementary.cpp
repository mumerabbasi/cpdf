#include "cpdf-structure.h"
using namespace std;

// PDF version of getline that obeys PDF newline conventions \n \r or \r\n and returns the next string in PDF
std::string cpdf::getline(fstream &source)
{
	string nextLine{};
	char tempChar{};
	while (source.get(tempChar))
	{
		nextLine += tempChar;
		if (tempChar == '\n')
			break;
		if (tempChar == '\r') {
			if (!source.eof() && source.peek() == '\n')
				nextLine += source.get();
			break;
		}
	}
	return nextLine;
}
std::string cpdf::readObj(int refNum)
{
	std::string tempString;
	pdfFile.seekg(stoi(pdfXref[refNum].offSet), ios::beg);
	//storing whole catalog obj in string (replacing new line chars with space.)
	while (true)
	{
		tempString += getline(pdfFile);
		if (tempString.find("endobj") != -1 || tempString.find("stream") != -1)
			break;
		if (tempString.substr(tempString.length() - 2, tempString.length()) == "\r\n")
		{
			tempString = tempString.substr(0, tempString.length() - 1);
			tempString[tempString.length() - 1] = ' ';
		}
		if (tempString[tempString.length() - 1] == '\n')
			tempString[tempString.length() - 1] = ' ';
	}
	return tempString;
}

BOOL cpdf::SwapRedBlue32a(FIBITMAP* dib)
{
	if (FreeImage_GetImageType(dib) != FIT_BITMAP) {
		return FALSE;
	}

	const unsigned bytesperpixel = FreeImage_GetBPP(dib) / 8;
	if (bytesperpixel > 4 || bytesperpixel < 3) {
		return FALSE;
	}

	const unsigned height = FreeImage_GetHeight(dib);
	const unsigned pitch = FreeImage_GetPitch(dib);
	const unsigned lineSize = FreeImage_GetLine(dib);

	BYTE* line = FreeImage_GetBits(dib);
	for (unsigned y = 0; y < height; ++y, line += pitch) {
		for (BYTE* pixel = line; pixel < line + lineSize; pixel += bytesperpixel) {
			INPLACESWAP(pixel[0], pixel[2]);
		}
	}

	return TRUE;
}
