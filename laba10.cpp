#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "books.h"


int main()
{
	struct sBooks* pBooksList=NULL;
	char cInputFileAddress[] = "input.txt";
	if (readFromFile(cInputFileAddress, &pBooksList) == 0)
	{
		return 0;
	}
	mainMenu(&pBooksList);
}