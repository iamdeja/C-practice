#include "stdlib.h"
#include "stdio.h"
#include "string.h"

typedef struct item
{
	char* pName;
	int Quantity, Price, Total;
} ITEM;

ITEM* Exam(char*, int*);
int CharFreq(char*, char);
int Update(char**, char**, char);

int main()
{
	int nItems = 0;
	char buffer[81];
	printf("Enter the items in the following format: 'Item, Quantity, Price;'\n");
	gets_s(buffer, 81);

	ITEM* pOutput = Exam(buffer, &nItems);
	for (int i = 0; i < nItems; i++)
	{
		printf("Name: %s, Quantity: %d, Price: %d, Total: %d.\n", pOutput[i].pName, pOutput[i].Quantity, pOutput[i].Price, pOutput[i].Total);
		free(pOutput[i].pName);
		pOutput[i].pName = NULL;
	}
	free(pOutput);
	pOutput = NULL;

	while (_kbhit())
		_getch();
	printf("\nPress any key to close this window . . .\n");
	_getch();

	return 0;
}

ITEM* Exam(char* pInput, int* pnItems)
{
	if (!pInput || !*pInput)
		return 0;

	// initialise
	*pnItems = CharFreq(pInput, ';'); // get the amount of trios
	ITEM* pOutput = (ITEM*)calloc(*pnItems, sizeof(ITEM)); // initialise as much structs as trios

	char* pIndex = NULL;

	for (int i = 0; i < *pnItems; i++) // update structs with trios
	{
		if (i == 0)
		{
			pIndex = strpbrk(pInput, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"); // get index of first word
		}
		else
		{
			pIndex = strpbrk(pIndex + 1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"); // get index of first word of next trio
		}

		// Name
		char* pOutdex = strchr(pIndex, ',');
		int len = pOutdex - pIndex;	// length of first word
		pOutput[i].pName = (char*)calloc(len + 1, sizeof(char)); // allocate memory for the first word: name
		strncpy_s(pOutput[i].pName, len + 1, pIndex, len); // copies the name to the struct

		char* pBeg = NULL;

		// Quantity
		pOutput[i].Quantity = Update(&pBeg, &pOutdex, ','); // get the integer value, update pOutdex to next comma

		// Price
		pOutput[i].Price = Update(&pBeg, &pOutdex, ';'); // get the integer value, update pOutdex to semicolon

		// Total
		pOutput[i].Total = pOutput[i].Quantity * pOutput[i].Price;
	}

	return pOutput;
}

int CharFreq(char* pInput, char c)
{
	if (!pInput || !*pInput)
		return 0;
	int count = 0;
	char* pSearch = strchr(pInput, c);
	while (pSearch)
	{
		count++;
		pSearch = strchr(pSearch + 1, c);
	}
	return count;
}

int Update(char** pBeg, char** pOutdex, char c)
{
	*pBeg = *pOutdex + 2; // get the index of number, +2 because of comma and space
	*pOutdex = strchr(*pOutdex + 1, c); // update index of end of number, ending with ',' or ';', depending on 'c'
	return atoi(*pBeg); // convert the number into an int
}