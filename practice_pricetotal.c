#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define SUCCESS 0
#define MEMFL 1 // memory allocation failed
int err_code = SUCCESS;

int ErrCode(int);

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
	if (pOutput) {
		for (int i = 0; i < nItems; i++)
		{
			if (pOutput[i].pName) {
				printf("Name: %s, Quantity: %d, Price: %d, Total: %d.\n", pOutput[i].pName, pOutput[i].Quantity, pOutput[i].Price, pOutput[i].Total);
				free(pOutput[i].pName);
				pOutput[i].pName = NULL;
			}
			else
				printf("Something went wrong.");
		}
		free(pOutput);
		pOutput = NULL;
	}
	else
		switch (err_code)
		{
		case MEMFL:
			printf("Memory allocation failed.");
			break;
		default:
			printf("Please check input");
			break;
		}

	while (_kbhit())
		_getch();
	printf("\nPress any key to close this window . . .\n");
	_getch();

	return 0;
}

ITEM* Exam(char* pInput, int* pnItems)
{
	if (!pInput || !*pInput) return 0;

	// initialise
	*pnItems = CharFreq(pInput, ';'); // get the amount of trios
	ITEM* pOutput = (ITEM*)calloc(*pnItems, sizeof(ITEM)); // initialise as much structs as trios
	if (!pOutput) return ErrCode(MEMFL);

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
		if (!pOutput[i].pName) return ErrCode(MEMFL);
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

int ErrCode(int n)
{
	err_code = n;
	return 0;
}