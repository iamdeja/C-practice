#include "stdlib.h"
#include "stdio.h"
#include "string.h"

typedef struct {
	char* pFirstName, * pMiddleName, * pLastName;
	int Brutto, TaxCoefficient, Netto;
} EMPLOYEE;

EMPLOYEE* Exam(char*);
int MiddleName(char*);
void StructUpdate(char**, char**, int*, char*);

int main()
{
	char input[] = "John James Smith 15000";
	EMPLOYEE* pOutput = Exam(input);
	// %% is escape sequence for %, no space between %s%s as this is handled by pOutput
	printf("%s %s%s %d %d%% %d", pOutput->pFirstName, pOutput->pMiddleName, pOutput->pLastName, pOutput->Brutto, pOutput->TaxCoefficient, pOutput->Netto);

	// freeing stuff
	free(pOutput->pFirstName); pOutput->pFirstName = NULL;
	if (pOutput->pMiddleName != "") free(pOutput->pMiddleName);
	pOutput->pMiddleName = NULL;
	free(pOutput->pLastName); pOutput->pLastName = NULL;

	return 0;
}

EMPLOYEE* Exam(char* pInput) {
	int namebool = MiddleName(pInput); // boolean determine if there is a middle name
	EMPLOYEE* pOutput = (EMPLOYEE*)calloc(1, sizeof(EMPLOYEE)); // allocate memory for output struct
	char apb[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	char* pIndex = strpbrk(pInput, apb);
	char* pOutdex = strchr(pIndex, ' ');
	int len = pOutdex - pIndex;

	// first name
	pOutput->pFirstName = (char*)calloc(len + 1, sizeof(char)); // allocates memory for the name
	strncpy_s(pOutput->pFirstName, len + 1, pIndex, len); // copies the name to the struct

	// middle name | Recommendation: make a function to handle
	if (namebool) {
		StructUpdate(&pIndex, &pOutdex, &len, apb);
		len++; // counting the space, for modular display
		pOutput->pMiddleName = (char*)calloc(len + 1, sizeof(char));
		strncpy_s(pOutput->pMiddleName, len + 1, pIndex, len);
	}
	else
		pOutput->pMiddleName = ""; // no middle name

	// last name
	StructUpdate(&pIndex, &pOutdex, &len, apb);
	pOutput->pLastName = (char*)calloc(len + 1, sizeof(char));
	strncpy_s(pOutput->pLastName, len + 1, pIndex, len);
	pOutdex = NULL;

	// price
	pIndex = strpbrk(pInput, "0123456789");
	pOutput->Brutto = atoi(pIndex);
	if (atoi(pIndex) > 10000)
		pOutput->TaxCoefficient = 25;
	else if (atoi(pIndex) > 7500)
		pOutput->TaxCoefficient = 20;
	else if (atoi(pIndex) > 5000)
		pOutput->TaxCoefficient = 15;
	else
		pOutput->TaxCoefficient = 0;
	pOutput->Netto = (int)(pOutput->Brutto / (1 + pOutput->TaxCoefficient / 100.0)); // bypass integer division with float, cast to int
	pIndex = NULL;

	return pOutput;
}

int MiddleName(char* pInput)
{
	int n = 0;
	char* pSearch = strchr(pInput, ' ');
	while (pSearch)
	{
		n++;
		pSearch = strchr(pSearch + 1, ' ');
	}
	if (n == 2)
		return 0;
	else if (n == 3)
		return 1;
	else
		return -1;
}

void StructUpdate(char** pIndex, char** pOutdex, int* len, char* apb)
{
	*pIndex = strpbrk(*pIndex + 1, apb); // update start of name
	*pOutdex = strchr(*pIndex, ' '); // update end of name
	*len = *pOutdex - *pIndex; // update length of word
}