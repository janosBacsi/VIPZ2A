#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
#include "books.h"

#define iNumberOfLinesToPrint 115  //�������� ������ �������� ����� � ��������

void printComponent(struct sBooks* pSCurrentBooksPointer, int iListOrder){
	printf("| %2d|%14s|%15s|%49s|  %6d| %8d| %7.2lf |\n", iListOrder,
	pSCurrentBooksPointer->chAuthorsFirsName, 
	pSCurrentBooksPointer->chAuthorsLastName, 
	pSCurrentBooksPointer->chNameOfTheBook,
	pSCurrentBooksPointer->iYearPublished, 
	pSCurrentBooksPointer->iNumberOfPages,
	pSCurrentBooksPointer->iPriceOfTheBook);
	printline(iNumberOfLinesToPrint);
}
//------------------------------------------------------------------------------
void printHeader(){
	printline(iNumberOfLinesToPrint);
	printf("| # |  FIRST NAME  |   LAST NAME   |                "
	"NAME OF THE BOOK                 |  YEAR  |  PAGES  |  PRICE  |\n");
	printline(iNumberOfLinesToPrint);
}
//------------------------------------------------------------------------------
int isString(char chTestedString[]){
	if (strlen(chTestedString)>1)
	{
		for (int i = 0; i < strlen(chTestedString); i++)
		{
			if (isalpha(chTestedString[i]) == 0)
			{
				return 0;
			}
		}
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------
int isRealNumber(char chTestedNumberString[]){
	if (strlen(chTestedNumberString) > 0)
	{
		for (int i = 0; i < strlen(chTestedNumberString); i++)
		{
			if (isdigit(chTestedNumberString[i]) == 0 && chTestedNumberString[i] != '.')
			{
				return 0;
			}
		}
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------
int isnum(char chTestedNumberString[]){
	if (strlen(chTestedNumberString) > 0)
	{
		for (int i = 0; i < strlen(chTestedNumberString); i++)
		{
			if (isdigit(chTestedNumberString[i]) == 0)
			{
				return 0;
			}
		}
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------
void addNewNode(struct sBooks** pSCurrentBooksPointer)
{
	struct sBooks* pSStartingPointer = *pSCurrentBooksPointer;
	char �Buffer[50];
	while ((*pSCurrentBooksPointer)->pNext)
	{
		*pSCurrentBooksPointer = (*pSCurrentBooksPointer)->pNext;
	}
	(*pSCurrentBooksPointer)->pNext = (sBooks*)malloc(sizeof(sBooks));
	(*pSCurrentBooksPointer) = (*pSCurrentBooksPointer)->pNext;
	(*pSCurrentBooksPointer)->pNext = NULL;
	printf("Please enter new entry\n");
	printf("Enter author's first name:");
	
	readFoolProofNameString((*pSCurrentBooksPointer)->chAuthorsFirsName);
	getchar();
	printf("Enter author's last name: ");
	readFoolProofNameString((*pSCurrentBooksPointer)->chAuthorsLastName);
	

	printf("Enter name of the book: ");
	getchar();
	scanf("%[^\n]s", &(*pSCurrentBooksPointer)->chNameOfTheBook);
	while (strlen((*pSCurrentBooksPointer)->chNameOfTheBook)<2)
	{
		printf("You cannot skip this step. Please enter the name of the book:\t");
		getchar();
		scanf("%[^\n]s", &(*pSCurrentBooksPointer)->chNameOfTheBook);
	}

	printf("Enter the year: ");
	getchar();
	(*pSCurrentBooksPointer)->iYearPublished = readFoolProofInt();

	printf("Enter the number of pages: ");
	(*pSCurrentBooksPointer)->iNumberOfPages = readFoolProofInt();

	printf("Enter the price: ");
	(*pSCurrentBooksPointer)->iPriceOfTheBook = readFoolProofDouble();

	*pSCurrentBooksPointer = pSStartingPointer;

	printf("Do you want to resort the list: yes(1) no(0) ");
	if (readFoolProofInt() == 1)
	{
		sortList(pSStartingPointer);
		printf("List is resorted\n");
	}
}
//------------------------------------------------------------------------------
int readFromFile(char* cFileOutPutAddress, struct sBooks** pCurrentElement)
{
	FILE* pInputFile;
	
	pInputFile = fopen(cFileOutPutAddress, "r");
	if (ifFileEmpty(pInputFile) == 1)
	{
		printf("The file is emptry. Please add some information"
		"to the list first(at least 1 item)\n");
		return 0;
	}
	struct sBooks* pPreviousElement = NULL;
	struct sBooks* pStartingElement;
	*pCurrentElement = (struct sBooks*)malloc(sizeof(struct sBooks));
	pStartingElement = *pCurrentElement;
	while (getc(pInputFile) != EOF)
	{
		pPreviousElement = *pCurrentElement;
		if (!pStartingElement)
		{
			pStartingElement = *pCurrentElement;
		}
		fscanf(pInputFile, "%s %s %c %[^\"]s", &(*pCurrentElement)->chAuthorsFirsName,
		&(*pCurrentElement)->chAuthorsLastName,
		&(*pCurrentElement)->chNameOfTheBook, &(*pCurrentElement)->chNameOfTheBook);
		fscanf(pInputFile, "%*c%d", &(*pCurrentElement)->iYearPublished);
		fscanf(pInputFile, "%*c%d", &(*pCurrentElement)->iNumberOfPages);
		fscanf(pInputFile, "%*c%lf", &(*pCurrentElement)->iPriceOfTheBook);
		(*pCurrentElement)->pNext = (struct sBooks*)malloc(sizeof(struct sBooks));
		(*pCurrentElement) = (*pCurrentElement)->pNext;

	}
	free(*pCurrentElement);
	pPreviousElement->pNext = NULL;
	*pCurrentElement = pStartingElement;
	fclose(pInputFile);
	return 1;
}
//------------------------------------------------------------------------------
void removeElement(struct sBooks** pCurrentBook, struct sBooks** pPreviousBook,
struct sBooks** pStartBook){
	struct sBooks* pTempBook=NULL;
	if (!(*pPreviousBook))
	{
		pTempBook = *pCurrentBook;
		*pCurrentBook = (*pCurrentBook)->pNext;
		*pStartBook = *pCurrentBook;
		free(pTempBook);
		*pPreviousBook = NULL;
	}
	else
	{
		(*pPreviousBook)->pNext = (*pCurrentBook)->pNext;
		free(*pCurrentBook);
		*pCurrentBook = *pPreviousBook;
	}
}
//------------------------------------------------------------------------------
void lessThanCertainPrice(struct sBooks** pListStart)
{
	printf("Please enter the price filter:");
	double dPriceFilter = readFoolProofDouble();
	
	struct sBooks* pCurrentBook = *pListStart;
	printHeader();
	struct sBooks* start = pCurrentBook;
	int iElementIndex = 1;
	while (pCurrentBook)
	{
		if (pCurrentBook->iPriceOfTheBook < dPriceFilter)
		{
			printComponent(pCurrentBook, iElementIndex);
			iElementIndex++;
		}
		pCurrentBook = pCurrentBook->pNext;

	}
	if (iElementIndex > 1)
	{
		printf("Do you want to remove these"
		" elements from the list?: yes(1), no(0):");
		int answer;
		answer = readFoolProofInt();

		if (answer == 1)
		{
			struct sBooks* pTempListStart = *pListStart;
			struct sBooks* pPreviousBook = NULL;
			while (*pListStart)
			{
				if ((*pListStart)->iPriceOfTheBook < dPriceFilter)
				{
					removeElement(pListStart, &pPreviousBook, &pTempListStart);
					
				}
				else
				{
					pPreviousBook = *pListStart;

					*pListStart = (*pListStart)->pNext;
				}
				
				
				
			}
			*pListStart = pTempListStart;
			printf("All items with a price of less than %.2lf UAH "
			"were removed successfully \n", dPriceFilter);
		}
	}
	else
	{
		printf("|                     All books cost more than %.2lf UAH" 
		"                                              | \n", dPriceFilter);
		printline(iNumberOfLinesToPrint);
		printf("Press enter to continute");
	}
}
//------------------------------------------------------------------------------
void printList(struct sBooks* pListStart)
{
	system("cls");
	printHeader();
	int iElementIndex = 1;
	while (pListStart)
	{
		printComponent(pListStart, iElementIndex);
		pListStart = pListStart->pNext;
		iElementIndex++;
	}
}
//------------------------------------------------------------------------------
void printline(int iNumberOfLines)
{
	for (int i = 0; i < iNumberOfLines; i++)
		printf("-");
	printf("\n");
}//------------------------------------------------------------------------------
void sortList(struct sBooks* pListStart)
{
	char safn[50];
	char saln[50];
	char snob[60];
	int syear;
	int snop;
	double sprice;
	struct sBooks* pMovingBook = pListStart;
	while (pListStart)
	{
		pMovingBook = pListStart->pNext;
		while (pMovingBook)
		{
			if (strcmp((pListStart)->chAuthorsLastName, ((pMovingBook)->chAuthorsLastName)) > 0)
			{
				strcpy(safn, pListStart->chAuthorsFirsName);
				strcpy(saln, pListStart->chAuthorsLastName);
				strcpy(snob, pListStart->chNameOfTheBook);
				syear = pListStart->iYearPublished;
				snop = pListStart->iNumberOfPages;
				sprice = pListStart->iPriceOfTheBook;

				strcpy(pListStart->chAuthorsFirsName, pMovingBook->chAuthorsFirsName);
				strcpy(pListStart->chAuthorsLastName, pMovingBook->chAuthorsLastName);
				strcpy(pListStart->chNameOfTheBook, pMovingBook->chNameOfTheBook);
				pListStart->iYearPublished = pMovingBook->iYearPublished;
				pListStart->iNumberOfPages = pMovingBook->iNumberOfPages;
				pListStart->iPriceOfTheBook = pMovingBook->iPriceOfTheBook;

				strcpy(pMovingBook->chAuthorsFirsName, safn);
				strcpy(pMovingBook->chAuthorsLastName, saln);
				strcpy(pMovingBook->chNameOfTheBook, snob);
				pMovingBook->iYearPublished = syear;
				pMovingBook->iNumberOfPages = snop;
				pMovingBook->iPriceOfTheBook = sprice;
			}
			pMovingBook = pMovingBook->pNext;
		}
		pListStart = pListStart->pNext;

	}

}
//------------------------------------------------------------------------------
void writeToFile(struct sBooks* pListStart)
{
	char cWritingPath[50];
	printf("Enter a path to the file you want to write your data to:  ");
	getchar();

	scanf("%[^\n]s", cWritingPath);

	FILE* pOutputFile;
	pOutputFile = fopen(cWritingPath, "w");
	fprintf(pOutputFile, " ");
	while (pListStart)
	{
		fprintf(pOutputFile, "%10s %10s \"%40s\" %4d %4d %6.2lf", pListStart->chAuthorsFirsName, 
		pListStart->chAuthorsLastName, pListStart->chNameOfTheBook, pListStart->iYearPublished, 
		pListStart->iNumberOfPages, pListStart->iPriceOfTheBook);

		if (pListStart->pNext)
		{
			fprintf(pOutputFile, "\n");
		}
		pListStart = pListStart->pNext;
	}
	fclose(pOutputFile);
}
//------------------------------------------------------------------------------
void deleteSingleElement(struct sBooks** pCurrentPointer)
{
	int iIndexDelEl;
	printf("Enter elements index that you want to delete:");
	iIndexDelEl = readFoolProofInt();

	int iCurrIndexEl = 1;
	struct sBooks* pListStart = *pCurrentPointer;
	struct sBooks* pPreviousBook = NULL;
	while (*pCurrentPointer)
	{
		if (iCurrIndexEl==iIndexDelEl)
		{
			removeElement(pCurrentPointer, &pPreviousBook, &pListStart);
			printf("Element with index number %d has been successfully deleted."
			"Press enter to continue", iIndexDelEl);
			break;
		}
		pPreviousBook = *pCurrentPointer;
		*pCurrentPointer = (*pCurrentPointer)->pNext;
		iCurrIndexEl++;
		if (!*pCurrentPointer)
		{
			printf("There is no element with index %d.\n", iIndexDelEl);
		}
	}
	*pCurrentPointer = pListStart;
}
//------------------------------------------------------------------------------
void saveChanges(struct sBooks* pCurrentElement)
{
	FILE* pSaveFile;
	pSaveFile = fopen("input.txt", "w");
	fprintf(pSaveFile, " ");
	while (pCurrentElement)
	{
		fprintf(pSaveFile, "%10s %10s \"%40s\" %4d %4d %6.2lf", 
		pCurrentElement->chAuthorsFirsName, pCurrentElement->chAuthorsLastName, 
		pCurrentElement->chNameOfTheBook, pCurrentElement->iYearPublished, 
		pCurrentElement->iNumberOfPages, pCurrentElement->iPriceOfTheBook);

		if (pCurrentElement->pNext)
		{
			fprintf(pSaveFile, "\n");
		}
		pCurrentElement = pCurrentElement->pNext;
	}
	fclose(pSaveFile);
}
//------------------------------------------------------------------------------
void searchByAuthorsLastName(struct sBooks** pListStart)
{
	char cSearchedLastName[60]="";
	printf("Please enter author's last name to find books written by him/her:");
	readFoolProofNameString(cSearchedLastName);

	struct sBooks* pCurrentBook = *pListStart;
	printHeader();
	int i = 1;
	int iBookCounter = 0;
	while (pCurrentBook)
	{
		if (strcmp(pCurrentBook->chAuthorsLastName, cSearchedLastName)==0)
		{
			printComponent(pCurrentBook, i);
			iBookCounter++;
		}
		pCurrentBook = pCurrentBook->pNext;
		i++;

	}
	if (iBookCounter > 1)
	{
		printf("Do you want to remove these elements from the list?: yes(1), no(any):");
		getchar();
		int answer = readFoolProofInt();
		if (answer == 1)
		{
			struct sBooks* start = *pListStart;
			struct sBooks* prev = NULL;
			while (*pListStart)
			{
				if (strcmp((*pListStart)->chAuthorsLastName, cSearchedLastName) == 0)
				{
					removeElement(pListStart, &prev, &start);

				}
				prev = *pListStart;
				*pListStart = (*pListStart)->pNext;
			}
			*pListStart = start;
			printf("All items written by %s were deleted\n", cSearchedLastName);
		}
	}
	else
	{
		printf("|                      %14s wrote none of the books in the list"
		"                                         | \n", cSearchedLastName);
		printline(iNumberOfLinesToPrint);
		getchar();
	}
	

}
//------------------------------------------------------------------------------
int ifFileEmpty(FILE* pCheckedFile) {
	long savedOffset = ftell(pCheckedFile);
	fseek(pCheckedFile, 0, SEEK_END);

	if (ftell(pCheckedFile) == 0) {
		return 1;
	}
	fseek(pCheckedFile, savedOffset, SEEK_SET);
	return 0;
}
//------------------------------------------------------------------------------
void booksWithTheMostPages(struct sBooks* pListStart)
{
	struct sBooks* pbookWithTheMostPages=pListStart;
	struct sBooks* pbookWithSecondToMostPages=pListStart;
	int iLargestBooksIndex[2] = {1, 1};
	int iCurrentBookIndex=0;
	while (pListStart)
	{
		iCurrentBookIndex++;
		if (pListStart->iNumberOfPages > pbookWithSecondToMostPages->iNumberOfPages)
		{
			if (pListStart->iNumberOfPages > pbookWithTheMostPages->iNumberOfPages)
			{
				pbookWithSecondToMostPages = pbookWithTheMostPages;
				pbookWithTheMostPages = pListStart;
				iLargestBooksIndex[1] = iLargestBooksIndex[0];
				iLargestBooksIndex[0] = iCurrentBookIndex;

			}
			else
			{
				pbookWithSecondToMostPages = pListStart;
				iLargestBooksIndex[1] = iCurrentBookIndex;
			}
		}
		pListStart = pListStart->pNext;
	}
	printHeader();
	printComponent(pbookWithTheMostPages, iLargestBooksIndex[0]);
	printComponent(pbookWithSecondToMostPages, iLargestBooksIndex[1]);
}
//------------------------------------------------------------------------------
void editElement(struct sBooks* psCurrentBooksPointer)
{
	char cTempStringBuffer[100];
	printf("Do you want to edit author's first name?: "
	"%s(press enter if you wanna skip this step):", 
	psCurrentBooksPointer->chAuthorsFirsName);

	gets_s(cTempStringBuffer, 100);
	if(strlen(cTempStringBuffer) > 2)
	{
		while(isString(cTempStringBuffer) == 0)
		{
			printf("Please reenter the string:");
			gets_s(cTempStringBuffer, 100);
		}
		strcpy(psCurrentBooksPointer->chAuthorsFirsName, cTempStringBuffer);
	}
	
	printf("Do you want to edit author's last name?: "
	"%s(press enter if you wanna skip this step):",
	psCurrentBooksPointer->chAuthorsLastName);


	gets_s(cTempStringBuffer, 100);
	if (strlen(cTempStringBuffer) > 0)
	{
		while (isString(cTempStringBuffer) == 0)
		{
			printf("Please reenter the string:");
			gets_s(cTempStringBuffer, 100);
		}
		strcpy(psCurrentBooksPointer->chAuthorsLastName, cTempStringBuffer);
	}
	
	printf("Do you want to edit name of the book?:"
	" %s(press enter if you wanna skip this step):", 
	psCurrentBooksPointer->chNameOfTheBook);
	gets_s(cTempStringBuffer, 100);
	if (strlen(cTempStringBuffer) > 0)
	{
		strcpy(psCurrentBooksPointer->chNameOfTheBook, cTempStringBuffer);
	}

	printf("Do you want to edit the year book was published?: %d(press enter "
	"if you wanna skip this step):", psCurrentBooksPointer->iYearPublished);
	gets_s(cTempStringBuffer, 100);
	if (strlen(cTempStringBuffer) > 0)
	{
		while (isnum(cTempStringBuffer) == 0)
		{
			printf("Please reenter the number:");
			gets_s(cTempStringBuffer, 100);
		}
		psCurrentBooksPointer->iYearPublished = atoi(cTempStringBuffer);
	}

	printf("Do you want to edit the number of pages?: %d"
	"(press enter if you wanna skip this step):", 
	psCurrentBooksPointer->iNumberOfPages);
	gets_s(cTempStringBuffer, 100);
	if (strlen(cTempStringBuffer) > 0)
	{
		while (isnum(cTempStringBuffer) == 0)
		{
			printf("Please reenter the number:");
			gets_s(cTempStringBuffer, 100);
		}
		psCurrentBooksPointer->iNumberOfPages = atoi(cTempStringBuffer);
	}

	printf("Do you want to edit the price?: %.2lf(press enter if you wanna "
		"skip this step):", psCurrentBooksPointer->iPriceOfTheBook);
	gets_s(cTempStringBuffer, 100);
	if (strlen(cTempStringBuffer) > 1)
	{
		while (isRealNumber(cTempStringBuffer) == 0)
		{
			printf("Please reenter the number:");
			gets_s(cTempStringBuffer, 100);
		}
		psCurrentBooksPointer->iPriceOfTheBook = atof(cTempStringBuffer);
	}

}
//------------------------------------------------------------------------------
void editElementByIndex(struct sBooks* pListStart)
{
	int iIndexOfEdElement=0;
	struct sBooks* start = pListStart;
	printf("Enter index of an element you want to edit:");
	iIndexOfEdElement=readFoolProofInt();

	int iCurrElementIndex = 1;
	while (pListStart)
	{
		if (iIndexOfEdElement == iCurrElementIndex)
		{
			editElement(pListStart);
			break;
		}
		iCurrElementIndex++;
		pListStart = pListStart->pNext;
		if (!pListStart)
		{
			printf("There is no element with index %d\n", iIndexOfEdElement);
			return;
		}
	}

	printf("Do you want to resort the list: yes(1) no(0): ");
	if (readFoolProofInt() == 1)
	{
		sortList(start);
		printf("List is resorted\n");
	}
	printf("Element successfully edited.\n");
}
//------------------------------------------------------------------------------
void readFoolProofNameString(char testString[])
{
	scanf("%[^\n]s", testString);
	while (isString(testString) == 0)
	{
		printf("The string you entered does not correspond to name string "
			"format(no spaces or numbers). Please enter a new string:");
		getchar();
		scanf("%[^\n]s", testString);
	}
}
//------------------------------------------------------------------------------
int readFoolProofInt()
{
	char buffer[60];
	gets_s(buffer, 59);
	while (isnum(buffer) == 0)
	{
		printf("Reenter the interger number: ");
		gets_s(buffer, 59);
	}
	return atoi(buffer);
}
//------------------------------------------------------------------------------
double readFoolProofDouble()
{
	char buffer[60];
	gets_s(buffer, 59);
	while (isRealNumber(buffer) == 0)
	{
		printf("Reenter the floating point number: ");
		gets_s(buffer, 59);
	}
	return atof(buffer);
}
//------------------------------------------------------------------------------
void mainMenu(struct sBooks** currentList)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions

	MoveWindow(console, r.left, r.top, 1300, 600, TRUE);
	
	struct sBooks* pCurrentBookPointer = *currentList;
	int iUserAnswer = 3;
	printf("Book store management software, by Chaus Ivan");

	while (iUserAnswer != 0)
	{
		printf("\t(1)Add"
			"\n\t(2)Sort List"
			"\n\t(3)Print list"
			"\n\t(4)Print books with price less than certain price"
			"\n\t(5)Write to file"
			"\n\t(6)Delete a single element"
			"\n\t(7)Save   "
			"\n\t(8)Search by author's last name"
			"\n\t(9)Print two books with the most pages  "
			"\n\t(10)Edit existing entry"
			"\n\t(0)Exit\nYour answer:");

		iUserAnswer = readFoolProofInt();
		MoveWindow(console, r.left, r.top, 1000, 600, TRUE);
		switch (iUserAnswer)
		{
		case 1:
			addNewNode(&pCurrentBookPointer);
			break;
		case 2:
			sortList(pCurrentBookPointer);
			printf("\nList successfully sorted by author's last name\n\n");
			break;
		case 3:
			printList(pCurrentBookPointer);
			break;
		case 4:
			lessThanCertainPrice(&pCurrentBookPointer);
			break;
		case 5:
			writeToFile(pCurrentBookPointer);
			break;
		case 6:
			deleteSingleElement(&pCurrentBookPointer);
			break;
		case 7:
			saveChanges(pCurrentBookPointer);
			break;
		case 8:
			searchByAuthorsLastName(&pCurrentBookPointer);
			break;
		case 9:
			booksWithTheMostPages(pCurrentBookPointer);
			break;
		case 10:
			editElementByIndex(pCurrentBookPointer);
			break;
		case 0:
			return;
			break;
		default:
			printf("\nYou have entered the wrong number. Please try again\n");

		}
	}
}