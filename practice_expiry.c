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
	int quantity, write_off;
} ITEM;

ITEM* Exam(char*, char*);
int isOld(int, int);

int main()
{
	char pInput[] = "[ Name: Makerel Quantity: 1000  Expires: 10-2018] "
		"[  Quantity: 500 Name: Sardine Expires: 12-2017  ]"
		" [Expires: 1-2020  Quantity: 10000 Name: Tuna  ]";
	char pItemName[] = "Sardine";

	ITEM* pOutput = Exam(pInput, pItemName);

	if (pOutput)
	{
		printf("Name: %s, Quantity: %d, Expired: ", pOutput->pName, pOutput->quantity);
		if (pOutput->write_off)
			printf("yes");
		else
			printf("no");

		// freeing and reseting pointers
		free(pOutput->pName);
		pOutput->pName = NULL;
		free(pOutput);
		pOutput = NULL;
	}
	else
		switch (err_code)
		{
		case MEMFL:
			printf("Memory allocation failure");
			break;
		default:
			printf("Please check your input.");
			break;
		}
	while (_kbhit())
		_getch();
	printf("\nPress any key to close this window . . .\n");
	_getch();

	return 0;
}

ITEM* Exam(char* pInput, char* pItemName)
{
	// Validation
	if (!pInput || !*pInput) return 0;
	if (!pItemName || !*pItemName) return 0;

	// Name
	char* pIndex = strstr(pInput, pItemName); // get index of the item name
	if (!pIndex) return 0;
	int len = strlen(pItemName);

	ITEM* pOut = (ITEM*)calloc(1, sizeof(ITEM)); // allocate memory for the output
	if (!pOut) return ErrCode(MEMFL); // in case memory allocation fails
	pOut->pName = (char*)calloc(len + 1, sizeof(char)); // allocate memory for the string at pOut.pName, +1 for terminating zero with calloc
	if (!pOut->pName) return ErrCode(MEMFL);
	strncpy_s(pOut->pName, len + 1, pItemName, len); // copy the item name to the struct (len + 1, as fallback for terminating zero with _s)

	// Detect Segment Start
	char* pSegStart = strchr(pInput, '['); // first segment
	while (1)
	{
		char* pTemp = strchr(pSegStart + 1, '[');
		if (pTemp && pTemp < pIndex) // if the next bracket is before the word we're looking for: if the word is in the segment
			pSegStart = strchr(pTemp, '[');
		else // found the bracket corresponding to the segment start
			break;
	}

	// Detect Quantity
	pIndex = strstr(pSegStart, "Quantity:");
	if (!pIndex) return 0;
	pIndex = strpbrk(pIndex, "0123456789"); // find the number after 'Quantity:'
	pOut->quantity = atoi(pIndex);

	// Detect Date
	pIndex = strstr(pSegStart, "Expires:");
	if (!pIndex) return 0;
	pIndex = strpbrk(pIndex, "0123456789"); // find number after 'Expires:' but before the dash
	int month = atoi(pIndex);
	pIndex = strchr(pSegStart, '-'); // find separator
	pIndex = strpbrk(pIndex, "0123456789"); // find number after separator
	int year = atoi(pIndex);
	pOut->write_off = isOld(month, year); // determine if expired and add result to pOut.write_off

	return pOut;
}

int isOld(int m, int y)
{
	int cM = 1, cY = 2019;
	if (cY > y || (cY == y && cM > m))
		return 1;
	else
		return 0;
}

int ErrCode(int n)
{
	err_code = n;
	return 0;
}