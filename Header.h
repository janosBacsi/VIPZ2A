#pragma once
struct books {
	char afn[60];
	char aln[60];
	char nob[100];
	int year;
	int nop;
	double price;
	struct books* next;
};

void printline(int);
void sortList(struct books* p);
int isString(char test[]);
int isnum(char test[]);
void addNewNode(struct books** p);
void readFromFile(char* address, struct books** p, struct books** start);
void removeElement(struct books** p, struct books** pp, struct books** start);
void delSmall(struct books** p);
void printList(struct books* p);
void writeToFile(struct books* p);
