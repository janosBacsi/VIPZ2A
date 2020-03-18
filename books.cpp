#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
#include "books.h"

#define NUMBER_OF_LINES \
115  //значення ширини роздільної лініїї в таблицях

//виводить елемент списку у вигляді рядку таблиці на екран консолі
//приймає два параметри: вказівник на елемент, який треба видрукувати
//та порядковий номер
void printComponent(struct sBooks* pSCurrentBooksPointer, int iListOrder) {
  printf("| %2d|%14s|%15s|%49s|  %6d| %8d| %7.2lf |\n", iListOrder,
         pSCurrentBooksPointer->chAuthorsFirsName,
         pSCurrentBooksPointer->chAuthorsLastName,
         pSCurrentBooksPointer->chNameOfTheBook,
         pSCurrentBooksPointer->iYearPublished,
         pSCurrentBooksPointer->iNumberOfPages,
         pSCurrentBooksPointer->iPriceOfTheBook);
  vPrintLine(NUMBER_OF_LINES);
}
//------------------------------------------------------------------------------
//виводить заголовок таблиці на консоль
void printHeader() {
  vPrintLine(NUMBER_OF_LINES);
  printf(
      "| # |  FIRST NAME  |   LAST NAME   |                "
      "NAME OF THE BOOK                 |  YEAR  |  PAGES  |  PRICE  |\n");
  vPrintLine(NUMBER_OF_LINES);
}
//------------------------------------------------------------------------------
int nIsString(char chTestedString[]) {
  if (strlen(chTestedString) > 1) {
    for (int i = 0; i < strlen(chTestedString); i++) {
      if (isalpha(chTestedString[i]) == 0) {
        return 0;
      }
    }
    return 1;
  }
  return 0;
}
//------------------------------------------------------------------------------
int nIsRealNumber(char chTestedNumberString[]) {
  if (strlen(chTestedNumberString) > 0) {
    for (int i = 0; i < strlen(chTestedNumberString); i++) {
      if (isdigit(chTestedNumberString[i]) == 0 &&
          chTestedNumberString[i] != '.') {
        return 0;
      }
    }
    return 1;
  }
  return 0;
}
//------------------------------------------------------------------------------
int nIsNum(char chTestedNumberString[]) {
  if (strlen(chTestedNumberString) > 0) {
    for (int i = 0; i < strlen(chTestedNumberString); i++) {
      if (isdigit(chTestedNumberString[i]) == 0) {
        return 0;
      }
    }
    return 1;
  }
  return 0;
}
//------------------------------------------------------------------------------
void vAddNewNode(struct sBooks **pSCurrentBooksPointer) {
  struct sBooks *pSStartingPointer = *pSCurrentBooksPointer;
  char сBuffer[50];
  while ((*pSCurrentBooksPointer)->pNext) {
    *pSCurrentBooksPointer = (*pSCurrentBooksPointer)->pNext;
  }
  (*pSCurrentBooksPointer)->pNext = (sBooks*)malloc(sizeof(sBooks));
  (*pSCurrentBooksPointer) = (*pSCurrentBooksPointer)->pNext;
  (*pSCurrentBooksPointer)->pNext = NULL;
  printf("Please enter new entry\n");
  printf("Enter author's first name:");

  vReadFoolProofNameString((*pSCurrentBooksPointer)->chAuthorsFirsName);
  getchar();
  printf("Enter author's last name: ");
  vReadFoolProofNameString((*pSCurrentBooksPointer)->chAuthorsLastName);

  printf("Enter name of the book: ");
  getchar();
  scanf("%[^\n]s", &(*pSCurrentBooksPointer)->chNameOfTheBook);
  while (strlen((*pSCurrentBooksPointer)->chNameOfTheBook) < 2) {
    printf("You cannot skip this step. Please enter the name of the book:\t");
    getchar();
    scanf("%[^\n]s", &(*pSCurrentBooksPointer)->chNameOfTheBook);
  }

  printf("Enter the year: ");
  getchar();
  (*pSCurrentBooksPointer)->iYearPublished = iReadFoolProofInt();

  printf("Enter the number of pages: ");
  (*pSCurrentBooksPointer)->iNumberOfPages = iReadFoolProofInt();

  printf("Enter the price: ");
  (*pSCurrentBooksPointer)->iPriceOfTheBook = dReadFoolProofDouble();

  *pSCurrentBooksPointer = pSStartingPointer;

  printf("Do you want to resort the list: yes(1) no(0) ");
  if (iReadFoolProofInt() == 1) {
    vSortList(pSStartingPointer);
    printf("List is resorted\n");
  }
}
//------------------------------------------------------------------------------
int nReadFromFile(char *cFileOutPutAddress, struct sBooks **pCurrentElement) {
  FILE* pInputFile;

  pInputFile = fopen(cFileOutPutAddress, "r");
  if (nIfFileEmpty(pInputFile) == 1) {
    printf(
        "The file is emptry. Please add some information"
        "to the list first(at least 1 item)\n");
    return 0;
  }
  struct sBooks *pPreviousElement = NULL;
  struct sBooks *pStartingElement;
  *pCurrentElement = (struct sBooks*)malloc(sizeof(struct sBooks));
  pStartingElement = *pCurrentElement;
  while (getc(pInputFile) != EOF) {
    pPreviousElement = *pCurrentElement;
    if (!pStartingElement) {
      pStartingElement = *pCurrentElement;
    }
    fscanf(pInputFile, "%s %s %c %[^\"]s",
           &(*pCurrentElement)->chAuthorsFirsName,
           &(*pCurrentElement)->chAuthorsLastName,
           &(*pCurrentElement)->chNameOfTheBook,
           &(*pCurrentElement)->chNameOfTheBook);
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
void vRemoveElement(struct sBooks **pCurrentBook, struct sBooks **pPreviousBook,
                   struct sBooks **pStartBook) {
  struct sBooks *pTempBook = NULL;
  if (!(*pPreviousBook)) {
    pTempBook = *pCurrentBook;
    *pCurrentBook = (*pCurrentBook)->pNext;
    *pStartBook = *pCurrentBook;
    free(pTempBook);
    *pPreviousBook = NULL;
  } else {
    (*pPreviousBook)->pNext = (*pCurrentBook)->pNext;
    free(*pCurrentBook);
    *pCurrentBook = *pPreviousBook;
  }
}
//------------------------------------------------------------------------------
void vLessThanCertainPrice(struct sBooks **pListStart) {
  printf("Please enter the price filter:");
  double dPriceFilter = dReadFoolProofDouble();

  struct sBooks *pCurrentBook = *pListStart;
  printHeader();
  struct sBooks *start = pCurrentBook;
  int iElementIndex = 1;
  while (pCurrentBook) {
    if (pCurrentBook->iPriceOfTheBook < dPriceFilter) {
      printComponent(pCurrentBook, iElementIndex);
      iElementIndex++;
    }
    pCurrentBook = pCurrentBook->pNext;
  }
  if (iElementIndex > 1) {
    printf(
        "Do you want to remove these"
        " elements from the list?: yes(1), no(0):");
    int answer;
    answer = iReadFoolProofInt();

    if (answer == 1) {
      struct sBooks *pTempListStart = *pListStart;
      struct sBooks *pPreviousBook = NULL;
      while (*pListStart) {
        if ((*pListStart)->iPriceOfTheBook < dPriceFilter) {
          vRemoveElement(pListStart, &pPreviousBook, &pTempListStart);
        } else {
          pPreviousBook = *pListStart;

          *pListStart = (*pListStart)->pNext;
        }
      }
      *pListStart = pTempListStart;
      printf(
          "All items with a price of less than %.2lf UAH "
          "were removed successfully \n",
          dPriceFilter);
    }
  } else {
    printf(
        "|                     All books cost more than %.2lf UAH"
        "                                              | \n",
        dPriceFilter);
    vPrintLine(NUMBER_OF_LINES);
    printf("Press enter to continute");
  }
}
//------------------------------------------------------------------------------
void vPrintList(struct sBooks *pListStart) {
  system("cls");
  printHeader();
  int iElementIndex = 1;
  while (pListStart) {
    printComponent(pListStart, iElementIndex);
    pListStart = pListStart->pNext;
    iElementIndex++;
  }
}
//------------------------------------------------------------------------------
void vPrintLine(int iNumberOfLines) {
  for (int i = 0; i < iNumberOfLines; i++) printf("-");
  printf("\n");
}
//------------------------------------------------------------------------------
void vSortList(struct sBooks *pListStart) {
  char cTempAuthorsFirstName[50];
  char cTempAuthorsLastName[50];
  char cTempNameOfBook[60];
  int iTempYearPublished;
  int iTempNumberOfPages;
  double sprice;
  struct sBooks* pMovingBook = pListStart;
  while (pListStart) {
    pMovingBook = pListStart->pNext;
    while (pMovingBook) {
      if (strcmp((pListStart)->chAuthorsLastName,
                 ((pMovingBook)->chAuthorsLastName)) > 0) {
        strcpy(cTempAuthorsFirstName, pListStart->chAuthorsFirsName);
        strcpy(cTempAuthorsLastName, pListStart->chAuthorsLastName);
        strcpy(cTempNameOfBook, pListStart->chNameOfTheBook);
        iTempYearPublished = pListStart->iYearPublished;
        iTempNumberOfPages = pListStart->iNumberOfPages;
        sprice = pListStart->iPriceOfTheBook;

        strcpy(pListStart->chAuthorsFirsName, pMovingBook->chAuthorsFirsName);
        strcpy(pListStart->chAuthorsLastName, pMovingBook->chAuthorsLastName);
        strcpy(pListStart->chNameOfTheBook, pMovingBook->chNameOfTheBook);
        pListStart->iYearPublished = pMovingBook->iYearPublished;
        pListStart->iNumberOfPages = pMovingBook->iNumberOfPages;
        pListStart->iPriceOfTheBook = pMovingBook->iPriceOfTheBook;

        strcpy(pMovingBook->chAuthorsFirsName, cTempAuthorsFirstName);
        strcpy(pMovingBook->chAuthorsLastName, cTempAuthorsLastName);
        strcpy(pMovingBook->chNameOfTheBook, cTempNameOfBook);
        pMovingBook->iYearPublished = iTempYearPublished;
        pMovingBook->iNumberOfPages = iTempNumberOfPages;
        pMovingBook->iPriceOfTheBook = sprice;
      }
      pMovingBook = pMovingBook->pNext;
    }
    pListStart = pListStart->pNext;
  }
}
//------------------------------------------------------------------------------
void vWriteToFile(struct sBooks *pListStart) {
  char cWritingPath[50];
  printf("Enter a path to the file you want to write your data to:  ");
  getchar();

  scanf("%[^\n]s", cWritingPath);

  FILE* pOutputFile;
  pOutputFile = fopen(cWritingPath, "w");
  fprintf(pOutputFile, " ");
  while (pListStart) {
    fprintf(pOutputFile, "%10s %10s \"%40s\" %4d %4d %6.2lf",
            pListStart->chAuthorsFirsName, pListStart->chAuthorsLastName,
            pListStart->chNameOfTheBook, pListStart->iYearPublished,
            pListStart->iNumberOfPages, pListStart->iPriceOfTheBook);

    if (pListStart->pNext) {
      fprintf(pOutputFile, "\n");
    }
    pListStart = pListStart->pNext;
  }
  fclose(pOutputFile);
}
//------------------------------------------------------------------------------
void vDeleteSingleElement(struct sBooks **pCurrentPointer) {
  int iIndexDelEl;
  printf("Enter elements index that you want to delete:");
  iIndexDelEl = iReadFoolProofInt();

  int iCurrIndexEl = 1;
  struct sBooks *pListStart = *pCurrentPointer;
  struct sBooks *pPreviousBook = NULL;
  while (*pCurrentPointer) {
    if (iCurrIndexEl == iIndexDelEl) {
      vRemoveElement(pCurrentPointer, &pPreviousBook, &pListStart);
      printf("Element with index number %d has been successfully deleted.\n",
             iIndexDelEl);
      break;
    }
    pPreviousBook = *pCurrentPointer;
    *pCurrentPointer = (*pCurrentPointer)->pNext;
    iCurrIndexEl++;
    if (!*pCurrentPointer) {
      printf("There is no element with index %d.\n", iIndexDelEl);
    }
  }
  *pCurrentPointer = pListStart;
}
//------------------------------------------------------------------------------
void vSaveChanges(struct sBooks *pCurrentElement) {
  FILE* pSaveFile;
  pSaveFile = fopen("input.txt", "w");
  fprintf(pSaveFile, " ");
  while (pCurrentElement) {
    fprintf(pSaveFile, "%10s %10s \"%40s\" %4d %4d %6.2lf",
            pCurrentElement->chAuthorsFirsName,
            pCurrentElement->chAuthorsLastName,
            pCurrentElement->chNameOfTheBook, pCurrentElement->iYearPublished,
            pCurrentElement->iNumberOfPages, pCurrentElement->iPriceOfTheBook);

    if (pCurrentElement->pNext) {
      fprintf(pSaveFile, "\n");
    }
    pCurrentElement = pCurrentElement->pNext;
  }
  fclose(pSaveFile);
  printf("Changes applied\n");
}
//------------------------------------------------------------------------------
void vSearchByAuthorsLastName(struct sBooks **pListStart) {
  char cSearchedLastName[60] = "";
  printf("Please enter author's last name to find books written by him/her:");
  vReadFoolProofNameString(cSearchedLastName);

  struct sBooks* pCurrentBook = *pListStart;
  printHeader();
  int i = 1;
  int iBookCounter = 0;
  while (pCurrentBook) {
    if (strcmp(pCurrentBook->chAuthorsLastName, cSearchedLastName) == 0) {
      printComponent(pCurrentBook, i);
      iBookCounter++;
    }
    pCurrentBook = pCurrentBook->pNext;
    i++;
  }
  if (iBookCounter > 1) {
    printf(
        "Do you want to remove these elements from the list?: yes(1), "
        "no(0):");
    getchar();
    int answer = iReadFoolProofInt();
    if (answer == 1) {
      struct sBooks *start = *pListStart;
      struct sBooks *prev = NULL;
      while (*pListStart) {
        if (strcmp((*pListStart)->chAuthorsLastName, cSearchedLastName) == 0) {
          vRemoveElement(pListStart, &prev, &start);
        }
        prev = *pListStart;
        *pListStart = (*pListStart)->pNext;
      }
      *pListStart = start;
      printf("All items written by %s were deleted\n", cSearchedLastName);
    }
  } else {
    printf(
        "|                      %14s wrote none of the books in the list"
        "                                         | \n",
        cSearchedLastName);
    vPrintLine(NUMBER_OF_LINES);
    getchar();
  }
}
//------------------------------------------------------------------------------
int nIfFileEmpty(FILE *pCheckedFile) {
  long savedOffset = ftell(pCheckedFile);
  fseek(pCheckedFile, 0, SEEK_END);

  if (ftell(pCheckedFile) == 0) {
    return 1;
  }
  fseek(pCheckedFile, savedOffset, SEEK_SET);
  return 0;
}
//------------------------------------------------------------------------------
void vBooksWithTheMostPages(struct sBooks *pListStart) {
  struct sBooks *pbookWithTheMostPages = pListStart;
  struct sBooks *pbookWithSecondToMostPages = pListStart;
  int iLargestBooksIndex[2] = {1, 1};
  int iCurrentBookIndex = 0;
  while (pListStart) {
    iCurrentBookIndex++;
    if (pListStart->iNumberOfPages >
        pbookWithSecondToMostPages->iNumberOfPages) {
      if (pListStart->iNumberOfPages > pbookWithTheMostPages->iNumberOfPages) {
        pbookWithSecondToMostPages = pbookWithTheMostPages;
        pbookWithTheMostPages = pListStart;
        iLargestBooksIndex[1] = iLargestBooksIndex[0];
        iLargestBooksIndex[0] = iCurrentBookIndex;
      } else {
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
void vEditElement(struct sBooks* psCurrentBooksPointer) {
  char cTempStringBuffer[100];
  printf(
      "Do you want to edit author's first name?: "
      "%s(press enter if you wanna skip this step):",
      psCurrentBooksPointer->chAuthorsFirsName);

  gets_s(cTempStringBuffer, 100);
  if (strlen(cTempStringBuffer) > 2) {
    while (nIsString(cTempStringBuffer) == 0) {
      printf("Please reenter the string:");
      gets_s(cTempStringBuffer, 100);
    }
    strcpy(psCurrentBooksPointer->chAuthorsFirsName, cTempStringBuffer);
  }

  printf(
      "Do you want to edit author's last name?: "
      "%s(press enter if you wanna skip this step):",
      psCurrentBooksPointer->chAuthorsLastName);

  gets_s(cTempStringBuffer, 100);
  if (strlen(cTempStringBuffer) > 0) {
    while (nIsString(cTempStringBuffer) == 0) {
      printf("Please reenter the string:");
      gets_s(cTempStringBuffer, 100);
    }
    strcpy(psCurrentBooksPointer->chAuthorsLastName, cTempStringBuffer);
  }

  printf(
      "Do you want to edit name of the book?:"
      " %s(press enter if you wanna skip this step):",
      psCurrentBooksPointer->chNameOfTheBook);
  gets_s(cTempStringBuffer, 100);
  if (strlen(cTempStringBuffer) > 0) {
    strcpy(psCurrentBooksPointer->chNameOfTheBook, cTempStringBuffer);
  }

  printf(
      "Do you want to edit the year book was published?: %d(press enter "
      "if you wanna skip this step):",
      psCurrentBooksPointer->iYearPublished);
  gets_s(cTempStringBuffer, 100);
  if (strlen(cTempStringBuffer) > 0) {
    while (nIsNum(cTempStringBuffer) == 0) {
      printf("Please reenter the number:");
      gets_s(cTempStringBuffer, 100);
    }
    psCurrentBooksPointer->iYearPublished = atoi(cTempStringBuffer);
  }

  printf(
      "Do you want to edit the number of pages?: %d"
      "(press enter if you wanna skip this step):",
      psCurrentBooksPointer->iNumberOfPages);
  gets_s(cTempStringBuffer, 100);
  if (strlen(cTempStringBuffer) > 0) {
    while (nIsNum(cTempStringBuffer) == 0) {
      printf("Please reenter the number:");
      gets_s(cTempStringBuffer, 100);
    }
    psCurrentBooksPointer->iNumberOfPages = atoi(cTempStringBuffer);
  }

  printf(
      "Do you want to edit the price?: %.2lf(press enter if you wanna "
      "skip this step):",
      psCurrentBooksPointer->iPriceOfTheBook);
  gets_s(cTempStringBuffer, 100);
  if (strlen(cTempStringBuffer) > 1) {
    while (nIsRealNumber(cTempStringBuffer) == 0) {
      printf("Please reenter the number:");
      gets_s(cTempStringBuffer, 100);
    }
    psCurrentBooksPointer->iPriceOfTheBook = atof(cTempStringBuffer);
  }
}
//------------------------------------------------------------------------------
void vEditElementByIndex(struct sBooks *pListStart) {
  int iIndexOfEdElement = 0;
  struct sBooks *start = pListStart;
  printf("Enter index of an element you want to edit:");
  iIndexOfEdElement = iReadFoolProofInt();

  int iCurrElementIndex = 1;
  while (pListStart) {
    if (iIndexOfEdElement == iCurrElementIndex) {
      vEditElement(pListStart);
      break;
    }
    iCurrElementIndex++;
    pListStart = pListStart->pNext;
    if (!pListStart) {
      printf("There is no element with index %d\n", iIndexOfEdElement);
      return;
    }
  }

  printf("Do you want to resort the list: yes(1) no(0): ");
  if (iReadFoolProofInt() == 1) {
    vSortList(start);
    printf("List is resorted\n");
  }
  printf("Element successfully edited.\n");
}
//------------------------------------------------------------------------------
void vReadFoolProofNameString(char testString[]) {
  scanf("%[^\n]s", testString);
  while (nIsString(testString) == 0) {
    printf(
        "The string you entered does not correspond to name string "
        "format(no spaces or numbers). Please enter a new string:");
    getchar();
    scanf("%[^\n]s", testString);
  }
}
//------------------------------------------------------------------------------
int iReadFoolProofInt() {
  char buffer[60];
  gets_s(buffer, 59);
  while (nIsNum(buffer) == 0) {
    printf("Reenter the interger number: ");
    gets_s(buffer, 59);
  }
  return atoi(buffer);
}
//------------------------------------------------------------------------------
double dReadFoolProofDouble() {
  char buffer[60];
  gets_s(buffer, 59);
  while (nIsRealNumber(buffer) == 0) {
    printf("Reenter the floating point number: ");
    gets_s(buffer, 59);
  }
  return atof(buffer);
}
//------------------------------------------------------------------------------