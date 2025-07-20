#include "cpdf-structure.h"
using namespace std;

cpdf::cpdf(const char * pathToFile)
{
	pdfPath = pathToFile;
	pdfFile.open(pathToFile, ios::in | ios::binary); // Open in read and binary mode
	if (pdfFile.is_open())
	{
		pdfFile.seekg(0, ios::beg);
		pdfFile.read(pdfVersion, 8); // Write the initial 8 bytes buffer which contains PDF version
		pdfVersion[8] = '\n';
		init(); // Start initialization
	}
	else
	{
		string str{ "NO PDF FOUND!!!" };
		throw str;
	}
}

cpdf::~cpdf() {}
