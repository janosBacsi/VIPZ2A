#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>

#include "books.h"

int main() {
	struct sBooks* pCurrentBookPointer = NULL;
	char cInputFileAddress[] = "input.txt";
	if (readFromFile(cInputFileAddress, &pCurrentBookPointer) == 0) {
		return 0;
	}

	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); 

	MoveWindow(console, r.left, r.top, 1300, 600, TRUE);
	int iUserAnswer = 3;
	printf("Book store management software, by Chaus Ivan \n");

	while (iUserAnswer != 0) {
		printf(
			"\t(1)Add new element"
			"\n\t(2)Sort List"
			"\n\t(3)Print list"
			"\n\t(4)Print books with price less than a certain filter"
			"\n\t(5)Write to file"
			"\n\t(6)Delete an element by index"
			"\n\t(7)Save changes"
			"\n\t(8)Search by author's last name"
			"\n\t(9)Print two books with the most pages  "
			"\n\t(10)Edit existing entry"
			"\n\t(0)Exit\nYour answer:");

		iUserAnswer = readFoolProofInt();
		MoveWindow(console, r.left, r.top, 1000, 600, TRUE);
		switch (iUserAnswer) {
		case 1: {
			addNewNode(&pCurrentBookPointer);
			break;
		}
		case 2: {
			sortList(pCurrentBookPointer);
			printf("\nList successfully sorted by author's last name\n\n");
			break;
		}
		case 3: {
			printList(pCurrentBookPointer);
			break;
		}
		case 4: {
			lessThanCertainPrice(&pCurrentBookPointer);
			break;
		}
		case 5: {
			writeToFile(pCurrentBookPointer);
			break;
		}
		case 6: {
			deleteSingleElement(&pCurrentBookPointer);
			break;
		}
		case 7: {
			saveChanges(pCurrentBookPointer);
			break;
		}
		case 8: {
			searchByAuthorsLastName(&pCurrentBookPointer);
			break;
		}
		case 9:
			booksWithTheMostPages(pCurrentBookPointer);
			break;
		case 10: {
			editElementByIndex(pCurrentBookPointer);
			break;
		}
		case 0: {
			return 0;
			break;
		}
		default: {
			printf("\nYou have entered the wrong number. Please try again\n");
		}
		}
	}
}