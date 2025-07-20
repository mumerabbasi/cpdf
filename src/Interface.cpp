#include"cpdf-structure.h"
using namespace std;

void Header()
{
	//Some decor although console specific.
	printf("\t\t\t\t\t  **************************************\n", 186, 186);
	printf("\t\t\t\t\t%c |P| |D| |F|    |V| |I| |E| |W| |E| |R| %c\n", 186, 186);
	printf("\t\t\t\t\t%c                                        %c\n", 186, 186);
	printf("\t\t\t\t\t  **************************************\n\n\n", 186, 186);
}
void main(void)
{
	system("Color 0B");
	Header();

	char *pathToFile = new char;
	int choice;
	int pageNum;

	cout << "Please Enter the complete path of your file (including .pdf): ";
	cin >> pathToFile;
	try {
		//Initializing a c-pdf object.
		cpdf FILE(pathToFile);

	MAIN:
		system("cls");
		Header();
		cout << "Please Enter:";
		cout << "\n1 for viewing total number of Pages:";
		cout << "\n2 for viewing Resources of a Page ( Fonts & Images): ";
		cout << "\n3 for Exit.";
		cout << "\nInput: ";
		cin >> choice;

		system("cls");
		Header();
		switch (choice)
		{
		case 1:
			cout << "Total Number of Pages in this pdf file are: " << FILE.getTotalPages();
			cout << "\n\n";
			system("pause");
			goto MAIN;
		case 2:
			cout << "Enter Page Number whose resources you want to get: ";
			cin >> pageNum;
			FILE.getResources(pageNum);
			cout << "\n\n";
			system("pause");
			goto MAIN;
		case 3:
			return;
		default:
			cout << "You did give an ivalid input.\n\n";
			system("pause");
			goto MAIN;

		}
	}
	catch (string str)
	{
		cout << str << "\n\n";
	}
	system("pause");
}