#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define line 98

#define SETPRICE 50

void printline(int);

void sortList(struct books* p);

struct books {
	char afn[60];
	char aln[60];
	char nob[100];
	int year;
	int nop;
	double price;
	struct books* next;
};

int isString(char test[])
{
	for (int i = 0; i < strlen(test); i++)
	{
		if (isalpha(test[i]) == 0)
		{
			return 1;
		}
	}
	return 0;
}


void addNewNode(struct books** p)
{
	struct books* start = *p;
	char buffer[20];
	while ((*p)->next)
	{
		*p = (*p)->next;
	}
	(*p)->next = (books*)malloc(sizeof(books));
	(*p) = (*p)->next;
	(*p)->next = NULL;
	printf("Please enter new name in format <AUTHOR'S FIRST NAME> <AUTHOR'S LAST NAME> \"<NAME OF THE BOOK> \" <YEAR> <NUMBER OF PAGES> <PRICE>");
	scanf("%s %s %c %[^\"]s", &(*p)->afn, &(*p)->aln, &(*p)->nob, &(*p)->nob);
	scanf("%*c%d", &(*p)->year);
	scanf("%*c%d", &(*p)->nop);
	scanf("%*c%lf", &(*p)->price);
	while(isString((*p)->afn)==1)
	{
		printf("Author's last name does not correspond to the format. Please enter the name again:  ");
		scanf("%s", &(*p)->afn);
	}
	while (isString((*p)->aln) == 1)
	{
		printf("Author's first name does not correspond to the format. Please enter the name again:  ");
		scanf("%s", &(*p)->aln);
	}
	*p = start;
	sortList(*p);
	
}

void readFromFile(char* address, struct books **p, struct books **start)
{
	FILE* fptr;
	fptr = fopen(address, "r");
	struct books* pp = NULL;
	*p= (struct books*)malloc(sizeof(struct books));
	*start=*p;
	while (getc(fptr) != EOF)
	{
		pp = *p;
		if (!*start)
		{
			*start = *p;
		} 
		fscanf(fptr, "%s %s %c %[^\"]s", &(*p)->afn, &(*p)->aln, &(*p)->nob, &(*p)->nob);
		fscanf(fptr, "%*c%d", &(*p)->year);
		fscanf(fptr, "%*c%d", &(*p)->nop);
		fscanf(fptr, "%*c%lf", &(*p)->price);
		(*p)->next = (struct books*)malloc(sizeof(struct books));
		(*p) = (*p)->next;

	}
	free(*p);
	pp->next = NULL;
	*p=*start;
	fclose(fptr);
}
void removeElement(struct books** p, struct books** pp, struct books**start)
{
	struct books* temp;
	if (!(*pp))
	{
		temp = *p;
		*p = (*p)->next;
		*start = *p;
		free(temp);
		
	}
	else
	{
		(*pp)->next = (*p)->next;
		free(*p);
		*p = *pp;
	}
}

void delSmall(struct books **p)
{
	struct books *start = *p;
	struct books* prev = NULL;
	while (*p)
	{
		if ((*p)->price < SETPRICE)
		{
			removeElement(p, &prev, &start);

		}
			prev = *p;
			*p = (*p)->next;	
	}
	*p = start;
}

void printList(struct books* p)
{
	printline(line);
	printf("|FIRST NAME | LAST NAME  |           NAME OF THE BOOK              | YEAR   |PAGES   |PRICE      |\n");
	printline(line);
	struct books* start=p;
	while (p)
	{
		
		printf("|%10s |%10s  |%40s |    %4d|    %4d|    %6.2lf |\n", p->afn, p->aln, p->nob, p->year, p->nop, p->price);
		p = p->next;
		printline(line);
		
		

	}
}


void printline(int amount)
{
	for (int i = 0; i < amount; i++)
		printf("-");
	printf("\n");
}

void sortList(struct books*p)
{
	char safn[50];
	char saln[50];
	char snob[60];
	int syear;
	int snop;
	double sprice;
	struct books*  p2=p;
	struct books* start= p;
	while(p)
	{
		p2 = p->next;
		while(p2)
		{
			if (strcmp((p)->aln, ((p2)->aln))>0)
			{
				strcpy(safn, p->afn);
				strcpy(saln, p->aln);
				strcpy(snob, p->nob);
				syear = p->year;
				snop = p->nop;
				sprice = p->price;

				strcpy(p->afn, p2->afn);
				strcpy(p->aln, p2->aln);
				strcpy(p->nob, p2->nob);
				p->year = p2->year;
				p->nop = p2->nop;
				p->price = p2->price;

				strcpy(p2->afn, safn);
				strcpy(p2->aln, saln);
				strcpy(p2->nob, snob);
				p2->year = syear;
				p2->nop = snop;
				p2->price = sprice;
			}
			p2 = p2->next;
		}
		p = p->next;
		
	}

}

void writeToFile(struct books* p)
{
	char path[50];
	printf("Enter a path to the file you want to write your data to:  ");
	getchar();

	scanf("%[^\n]s", path);

	FILE* write;
	write = fopen(path, "w");
	fprintf(write, " ");
	while (p)
	{
		fprintf(write, "%10s %10s \"%40s\" %4d %4d %6.2lf", p->afn, p->aln, p->nob, p->year, p->nop, p->price);
		
		if (p->next)
		{
			fprintf(write, "\n");
		}
		p = p->next;
	}
	fclose(write);
}


int main()
{

	struct books* p=NULL;
	struct books* start = NULL;
	char file[] = "input.txt";
	readFromFile(file, &p, &start);
	sortList(p);
	char answer=' ';
	while (answer != 'e')
	{
		printf("Do you want to add(a), print list(p), remove(r), write to file(w) or exit(e): \n");
		scanf("%c", &answer);

		if (answer == 'a')
		{
			addNewNode(&p);
			sortList(p);
		}
		else if (answer == 'r')
		{
			delSmall(&p);
		}
		else if (answer == 'w')
		{
			writeToFile(p);
		}
		else if (answer == 'p')
		{
			printList(p);
		}
		getchar();
	}

}