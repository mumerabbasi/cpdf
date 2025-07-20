#include"cpdf-structure.h"
using namespace std;

//get pages reference numbers
void cpdf::extractPages(void)
{

	string tempString{};
	int temp{}, i{};
	vector<string> tempVec;
	vector<string> tempVec1;
	stringstream ss;
	char *tempChar = new char;

	//Extracting offset of catalog from trailor.
	temp = eofLocaction;
	tempString.clear();
	while (true)
	{
	
		temp = eofLocaction + i;
		pdfFile.seekg(temp, ios::end);
		pdfFile.get(*tempChar);
		i = i - 1;
		if (*tempChar == '/')
		{
			tempString = getline(pdfFile);
			if (tempString.substr(0,4) == "Root")
				break;
		}
	}
	ss << tempString;
	ss >> tempString >> tempString;
	catalogRefNum = stoi(tempString); //Reference_Num is entry number in xref table starting from 0.
	//Reading Catalog
	tempString = readObj(catalogRefNum);
	//getting obj_num of /type/Pages
	ss.str("");
	ss.clear();
	tempString = tempString.substr(tempString.find("/Pages") + 1, tempString.length());
	ss << tempString;
	ss >> tempString >> tempString;
	pdfPages.pagesRefNum = stoi(tempString);
	//Reading Pages
	tempString = readObj(pdfPages.pagesRefNum);
	//Extracting Kids reference number.
	ss.str("");
	ss.clear();
	tempString = tempString.substr(tempString.find("/Kids") + 1, tempString.length());
	tempString = tempString.substr(tempString.find('[') + 1, tempString.find_first_of(']')- tempString.find('[')-1);
	// Kids [ 4 0 R] and Kids[4 0 R]
	if (tempString[0] == ' ')
	{
		tempString = tempString.substr(tempString.find(' ') + 1, tempString.length() - 2);
	}
	ss << tempString;
	temp = 1;
	//Extracting pages  page numbers, reference numbers and generation numbers.
	while(!ss.eof())
	{
		pdfPages.pdfPage.resize(temp);
		pdfPages.pdfPage[temp-1].pageNum = temp;
		ss >> pdfPages.pdfPage[temp - 1].refNum;
		ss >> pdfPages.pdfPage[temp - 1].generationNum;
		ss >> tempString;
		temp++;
	}
}

