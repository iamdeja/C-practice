#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define SUCCESS 0
#define MEMFL 1 // memory allocation failed
#define RALLOCFL 2 // reallocation memory failure
#define NADA 10 // empty
#define MISMATCH 11 // string not found
int err_code = SUCCESS;

char* pErrCode(int);

typedef struct {
	int day;
	char month[4];
	int year;
} DATE;

char* Exam(char*, DATE*, const char*);
int NameCount(char*);
int SolCount(DATE*, char*, int);

int main()
{
	char names[] = "John, Mary, James, Elizabeth, Richard";
	DATE birthdates[] = { { 2, "Jun", 2000 }, {27, "Jul", 2001 }, { 12, "Jun", 1999 }, { 15, "Sep", 1998 }, { 16, "Jun", 2000 } };
	char* pResult = Exam(names, birthdates, "Jun");

	if (pResult)
	{
		printf("%s", pResult);
		free(pResult);
		pResult = NULL;
	}
	else
	{
		switch (err_code)
		{
		case NADA:
			printf("Inputs cannot be left empty!");
			break;
		case MISMATCH:
			printf("No people born in that month found.");
			break;
		case MEMFL:
			printf("Memory allocation failed.");
			break;
		case RALLOCFL:
			printf("Memory reallocation failed");
			break;
		case SUCCESS:
			printf("No Issues Found.");
			break;
		default:
			printf("Please check input.");
			break;
		}
	}
	return 0;
}

char* Exam(char* pNames, DATE* pBirthDates, const char* pMonthToStudy)
{
	// initialise
	if (!pNames || !*pNames || !pBirthDates || !pMonthToStudy || !*pMonthToStudy) return pErrCode(NADA);
	int nNames = NameCount(pNames);
	int maxSols = SolCount(pBirthDates, pMonthToStudy, nNames); // number of names in output string, update string length
	if (!maxSols) return pErrCode(MISMATCH);

	// variables and counters
	char* pIndex = pNames;
	int nSols = 0; // counter for number of names in output string
	int len = 0; // length of word
	int pfs = 0; // position from start
	int finish = 0; // boolean

	// allocations
	int* pStrLen = (int*)calloc(maxSols, sizeof(int));
	char* pOut = (char*)calloc(strlen(pNames), sizeof(char)); // immediate buffer to avoid multiple reallocs
	if (!pStrLen || !pOut) return pErrCode(MEMFL);

	// fill pointers
	for (int i = 0; i < nNames && !finish; i++)
	{
		char* pOutdex = strchr(pIndex, ',');
		if (strstr(pBirthDates[i].month, pMonthToStudy))
		{
			nSols++;
			pfs = len + pfs;
			if (pOutdex && nSols == maxSols)
			{
				len = pOutdex - pIndex; // skip comma and space
				finish++;
			}
			else if (pOutdex)
				len = pOutdex - pIndex + 2; // add comma and space
			else
			{
				len = strlen(pIndex); // end of string
				finish++;
			}
			pStrLen[nSols - 1] = len;
			strncpy_s(pOut + pfs, len + 1, pIndex, len);
		}
		if (!finish)
			pIndex = pOutdex + 2; // skip comma and space
	}
	pIndex = NULL;
	// --- preventing heap reallocation failure, remediates warning C6308
	char* tmp = NULL;
	tmp = realloc(pOut, Sum(pStrLen, maxSols));
	if (!tmp) return pErrCode(RALLOCFL);
	pOut = tmp;
	// ---
	free(pStrLen);
	pStrLen = NULL;

	return pOut;
}

int NameCount(char* pInput)
{
	int n = 0;
	char* pSearch = strchr(pInput, ' ');
	while (pSearch)
	{
		n++;
		pSearch = strchr(pSearch + 1, ' ');
	}
	return ++n;
}

int SolCount(DATE* pInput, char* pMatch, int max)
{
	int n = 0;
	for (int i = 0; i < max; i++)
	{
		if (strstr(pInput[i].month, pMatch))
			n++;
	}
	return n;
}

int Sum(int* pInput, int nFields)
{
	int n = 0;
	for (int i = 0; i < nFields; i++)
		n += pInput[i];
	return ++n;
}

char* pErrCode(int n)
{
	err_code = n;
	return NULL;
}