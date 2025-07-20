#include "cpdf-structure.h"
using namespace std;

void cpdf::init() //Initialize the PDF
{
	size_t xrefLocation{}; // Main document cross reference table location
	pdfFile.seekg(0, ios::end); // seek to the end of the file

	// Performing EOF check
	if (!performEOFCheck())
		throw "Unable to find %%EOF tag. Probably invalid PDF file.";

	// Performing search for startxref flag that'll point to xref location
	char charTemp{ '\0' };
	pdfFile.seekg(0, ios::cur);
	do
	{
		pdfFile.seekg(-2, ios::cur);
		if (charTemp == 'f') //startxref ends in f
		{
			break;
		}
		pdfFile.get(charTemp);
	} while (true);

	string str=	getline(pdfFile); // Move to next line
	string xrefLocationStr{};
	xrefLocationStr = getline(pdfFile);
	xrefLocation = stoi(xrefLocationStr);

	if (xrefLocation == 0)
	{
		throw "Unable to find Xref location. Most probably an invalid PDF file.";
	}

	// Start loading xref (assuming uncompressed)
	loadXref(xrefLocation);
	// Extracting Page DATA
	extractPages();
	extractFont();
}