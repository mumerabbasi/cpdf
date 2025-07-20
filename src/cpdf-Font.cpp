#include"cpdf-structure.h"
using namespace std;

void cpdf::extractFont()
{
	double temp{}, R_location{};
	string tempString{};
	vector<string> tempVec;
	stringstream ss;

	//Loop for all pages of pdf file.
	for (int pNum = 0; pNum < pdfPages.pdfPage.size(); pNum++)
	{
		tempString.clear();
		tempVec.clear();
		temp = 1;
		//Reading obj Page
		tempString = readObj(stoi(pdfPages.pdfPage[pNum].refNum));
		ss.str("");
		ss.clear();
		//Check if any font is used in page i.e. if it has any text.
		if (tempString.find("/Font") > tempString.length() || tempString.find("/Font") < 0)
			continue;
		tempString = tempString.substr(tempString.find("/Font") + 1, tempString.length());
		tempString = tempString.substr(tempString.find("<<") + 2, tempString.find(">>") - tempString.find("<<") - 2 - 2);
		if (tempString[0] == ' ')
			tempString = tempString.substr(1, tempString.length() - 2);
		ss << tempString;
		//Storing ref Num of FONTS in temp Vec
		while (!ss.eof())
		{
			ss >> tempString;
			ss >> tempString;
			tempVec.push_back(tempString);
			ss >> tempString;
			ss >> tempString;
		}
		pdfPages.pdfPage[pNum].pageFont.resize(tempVec.size());
		//Copying font references from string stream to obj font.
		for (int i = 0; i < pdfPages.pdfPage[pNum].pageFont.size(); i++)
		{
			pdfPages.pdfPage[pNum].pageFont[i].refNum = tempVec[i];
		}
		//Loop for all fonts of a page.
		for (int fontNum = 0; fontNum < pdfPages.pdfPage[pNum].pageFont.size(); fontNum++)
		{
			temp = stoi(pdfPages.pdfPage[pNum].pageFont[fontNum].refNum);
			//Reading obj FONT
			tempString = readObj(temp);
			//Extracting BaseFont
			tempString = tempString.substr(tempString.find("BaseFont"), tempString.length());
			tempString = tempString.substr(tempString.find("/") + 1, tempString.length());
			tempString = tempString.substr(0, tempString.find(' '));
			pdfPages.pdfPage[pNum].pageFont[fontNum].BaseFont = tempString;
			//Extracting FontName
			if (pdfVersion[7]=='6'|| pdfVersion[7] =='7')
			{
				tempString = readObj(temp);
				tempString = tempString.substr(tempString.find("FontDescriptor"), tempString.length());
				tempString = tempString.substr(tempString.find(' ') + 1, tempString.length());
				tempString = tempString.substr(0, tempString.find(' '));
				//Extracting offset of FontDescriptor
				//Reading FontDescriptor Object
				tempString = readObj(stoi(tempString));
				tempString = tempString.substr(tempString.find("FontName"), tempString.length());
				tempString = tempString.substr(tempString.find("/") + 1, tempString.length());
				tempString = tempString.substr(0, tempString.find(' '));
				pdfPages.pdfPage[pNum].pageFont[fontNum].Name = tempString;
			}
			else
			{
				//Reading obj FONT
				tempString = readObj(temp);
				//Extracting BaseFont
				tempString = tempString.substr(tempString.find("Name"), tempString.length());
				tempString = tempString.substr(tempString.find("/") + 1, tempString.length());
				tempString = tempString.substr(0, tempString.find(' '));
				pdfPages.pdfPage[pNum].pageFont[fontNum].Name = tempString;
			}
		}
	}

}