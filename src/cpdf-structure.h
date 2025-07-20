#ifndef CPDF_STRUCTURE_H
#define CPDF_STRUCTURE_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <string> 
#include <sstream>
#include"Free Image\FreeImage.h"
#include"Free Image\FreeImage-Utilities.h"

struct Font
{
	std::string refNum;
	std::string Name;
	std::string BaseFont;
};
struct Image
{
	long width;
	long height;
	long length;
	int BitsperComponent;
	std::string decodingFilter;
	std::string ColorSpace;
	std::string refNum;
};
struct cpdfXrefEntry
{
	std::string offSet;
	std::string genNum;
	std::string type;
};
struct page
{
	int pageNum;
	std::string generationNum;
	std::string refNum; 
	std::vector<Font> pageFont;
	std::vector<Image> pageImage;
};
struct pages
{
	int pagesRefNum;
	std::vector<page> pdfPage;
};
class cpdf
{
private:
	std::fstream pdfFile;
	char pdfVersion[9];
	const char * pdfPath;
	std::vector<cpdfXrefEntry> pdfXref;
	pages pdfPages;
	int eofLocaction; //offset from end.
	int catalogRefNum;

public:
	//Constructor and Destructor.
	cpdf(const char * pathToFile);
	virtual ~cpdf();
	//Intialization functions.
	void init();
	bool performEOFCheck();
	void loadXref(size_t); //Initialize the pdf \Xref
	void extractPages(void); //get pages reference numbers.
	void extractFont(); //get Font used in page.
	bool extractImages(int pNum); //Extract images of a page. targetPath for saving images.
	std::string readObj(int); //Read an entire object.
	std::string getline(std::fstream&); //Overrloade getline() for pdf.
	static BOOL SwapRedBlue32a(FIBITMAP* dib); //For swapping colors.
	//Output Functions.
	int getTotalPages();
	void getResources(int pagNum);
};

#endif
