#include"cpdf-structure.h"
using namespace std;

int cpdf::getTotalPages()
{
	return pdfPages.pdfPage.size();
}

void cpdf::getResources(int pageNum)
{
	cout << "PAGE " << pageNum<<" :\n\n\n";
	for (int i = 0; i < pdfPages.pdfPage[pageNum-1].pageFont.size(); i++) //Because page 1 is the 0th object in vector.
	{
		cout << "Font Name: " << pdfPages.pdfPage[pageNum - 1].pageFont[i].Name << endl;
		cout << "Base Font: " << pdfPages.pdfPage[pageNum - 1].pageFont[i].BaseFont << endl << endl;
	}
	int temp = extractImages(pageNum);
	if (temp == 1)
	{
		cout << "\n\nIMAGES OF THIS PAGE HAVE BEEN SUCCESSFULLY SAVED IN THE ROOT DIRECTORY.";
	}
	else
	{
		cout << "\n\nNO IMAGES FOUND IN THIS PAGE.";
	}
}