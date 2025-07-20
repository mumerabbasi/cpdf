#include"cpdf-structure.h"
using namespace std;

bool cpdf::extractImages(int pNum)
{
	pNum--;
	double temp{}, R_location{};
	string tempString{}, tempString1;
	vector<string> tempVec;
	stringstream ss;

	tempString.clear();
	tempVec.clear();
	temp = 1;
	//Reading obj Page
	tempString = readObj(stoi(pdfPages.pdfPage[pNum].refNum));
	ss.str("");
	ss.clear();
	//Check for if Page has any image or not.
	if (tempString.find("/XObject") > tempString.length() - 1 || tempString.find("/XObject") < 0) //Because in this case func will not return value in this range.
		return FALSE;
	tempString = tempString.substr(tempString.find("/XObject") + 1, tempString.length());
	tempString = tempString.substr(tempString.find("/"), tempString.find(">>") - tempString.find("/") - 1);
	ss << tempString;
	//Storing ref Num of Images in temp Vec
	while (!ss.eof())
	{
		ss >> tempString;
		ss >> tempString;
		tempVec.push_back(tempString);
		ss >> tempString;
		ss >> tempString;
	}
	pdfPages.pdfPage[pNum].pageImage.resize(tempVec.size());
	//Copying Image references from string stream to obj font.
	for (int i = 0; i < pdfPages.pdfPage[pNum].pageImage.size(); i++)
	{
		pdfPages.pdfPage[pNum].pageImage[i].refNum = tempVec[i];
	}
	//Loop for all Images of a page.
	for (int imageNum = 0; imageNum < pdfPages.pdfPage[pNum].pageImage.size(); imageNum++)
	{
		temp = stoi(pdfPages.pdfPage[pNum].pageImage[imageNum].refNum);
		//Reading obj Image
		tempString = readObj(temp);
		//Image Width
		ss.str("");
		ss.clear();
		ss << tempString.substr(tempString.find("/Width"), tempString.length());
		ss >> tempString1;
		ss >> pdfPages.pdfPage[pNum].pageImage[imageNum].width;
		//Image Height
		ss.str("");
		ss.clear();
		ss << tempString.substr(tempString.find("/Height"), tempString.length());
		ss >> tempString1;
		ss >> pdfPages.pdfPage[pNum].pageImage[imageNum].height;
		//Image Length
		ss.str("");
		ss.clear();
		ss << tempString.substr(tempString.find("/Length"), tempString.length());
		ss >> tempString1;
		ss >> pdfPages.pdfPage[pNum].pageImage[imageNum].length;
		//Image BitsPerComponent
		ss.str("");
		ss.clear();
		ss << tempString.substr(tempString.find("/BitsPerComponent"), tempString.length());
		ss >> tempString1;
		ss >> pdfPages.pdfPage[pNum].pageImage[imageNum].BitsperComponent;
		//Image ColorSpace
		ss.str("");
		ss.clear();
		tempString1 = tempString.substr(tempString.find("/ColorSpace") + 11, tempString.length());
		if (tempString1[0] == ' ') //Removing space if found.
			tempString1 = tempString1.substr(1, tempString.length());
		tempString1 = tempString1.substr(1, tempString.length()); //Removing '/' e.g /RGB
		ss << tempString1;
		ss >> pdfPages.pdfPage[pNum].pageImage[imageNum].ColorSpace;
		//Image decoding filter
		ss.str("");
		ss.clear();
		//Check if image have a filter or not.
		if (tempString.find("/Decode") < tempString.length() && tempString.find("/Decode") > 0)
		{
			pdfPages.pdfPage[pNum].pageImage[imageNum].decodingFilter = "FILTER";
		}
		else
		{
			pdfPages.pdfPage[pNum].pageImage[imageNum].decodingFilter = "NULL";
		}

		//Extracting Stream
		BYTE * stream = new BYTE[pdfPages.pdfPage[pNum].pageImage[imageNum].length];
		//Getting starting offset of stream.
		temp = stoi(pdfPages.pdfPage[pNum].pageImage[imageNum].refNum);
		int start = stoi(pdfXref[temp].offSet) + tempString.find("stream") + 8;
		//Seeking to start of stream in File.
		FILE *File;
		File = fopen(pdfPath, "rb");
		fseek(File, start, ios::beg);
		//Reading whole stream.
		fread(stream, 1, pdfPages.pdfPage[pNum].pageImage[imageNum].length, File);
		fclose(File);
		BYTE *temp = new BYTE;
		//Swapping 1st and 3rd element (accomodating just one filter). Filter specific.
		if (pdfPages.pdfPage[pNum].pageImage[imageNum].decodingFilter == "FILTER")
		{
			for (int i = 0; i < pdfPages.pdfPage[pNum].pageImage[imageNum].length - 2; i++)
			{
				stream[i] = *temp;
				stream[i] = stream[i + 2];
				stream[i + 2] = *temp;
			}
		}
		//Creating image from raw bits.
		FIBITMAP * pic = FreeImage_ConvertFromRawBits(stream, pdfPages.pdfPage[pNum].pageImage[imageNum].width, pdfPages.pdfPage[pNum].pageImage[imageNum].height, pdfPages.pdfPage[pNum].pageImage[imageNum].width * 3, pdfPages.pdfPage[pNum].pageImage[imageNum].BitsperComponent * 3, 0, 0, 0, 1);
		string bmp ="IMAGE" + to_string(pdfPages.pdfPage[pNum].pageImage[imageNum].length);
		bmp.append(".png");
		//Swapping red and blue.
		cpdf::SwapRedBlue32a(pic);
		//Saving final image.
		FreeImage_Save(FIF_PNG, pic, bmp.c_str());
	}
	return TRUE;
}