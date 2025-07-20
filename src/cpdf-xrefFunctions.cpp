#include "cpdf-structure.h"
using namespace std;

void cpdf::loadXref(size_t xrefPos)
{
	pdfFile.seekg(xrefPos, ios::beg);
	string line;
	line = getline(pdfFile);
	line = getline(pdfFile);
	line = line.substr(line.find_first_of(' '),line.find_first_of("\r\t\n")-1);
	int tempSize;
	string tempPosStr{};
	int tempPos{};

	tempSize= stoi(line);
	pdfXref.resize(tempSize);

	for (int i = 0; i < tempSize; i++)
	{
		line = getline(pdfFile);
		pdfXref[i].offSet = line.substr(0, 10);
		pdfXref[i].genNum = line.substr(11, 5);
		pdfXref[i].type = line.substr(17, 1);
	}
}
