#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "conio.h"

#define SUCCESS 0
#define EMPTY 10
#define MISMATCH 11
int err_msg = SUCCESS;

char** Exam(char*, char*);
int WordCount(char*);

int main()
{
	char buf1[81];
	char buf2[81];

	printf("Enter a list of first names, separated by comma and space: ");
	gets_s(buf1, 81);
	printf("Enter a list of last names, separated by comma and space: ");
	gets_s(buf2, 81);

	char** ppOut = Exam(buf1, buf2);
	if (ppOut)
	{
		int nr = WordCount(buf1);
		for (int i = 0; i < nr; i++)
			printf("%s\n", ppOut[i]);
		free(ppOut);
	}
	else {
		switch (err_msg) {
			case EMPTY:
				printf("String cannot be empty!");
				break;
			case MISMATCH:
				printf("Strings must contain the same numer of words!");
				break;
			default:
				printf("Check your input!");
				break;
		}
	}
	while (_kbhit())
		_getch();
	printf("\nPress any key to close this window . . .\n");
	_getch();

	return 0;
}

char** Exam(char* pFirstNames, char* pLastNames)
{
	// validation
	int words = WordCount(pFirstNames);
	if (words == 0) {
		return 0;
	}
	else if (words != WordCount(pLastNames)) {
		err_msg = MISMATCH;
		return 0;
	}

	char** ppOut = (char**)calloc(words, sizeof(char*));

	char *p1 = pFirstNames, *q1 = pLastNames;
	int wLen1 = 0, wLen2 = 0;
	int last = 0; // boolean if last word or not

	for (int i = 0; i < words; i++)
	{
		// wordlengths
		char* p2 = strchr(p1, ' ');
		char* q2 = strchr(q1, ' ');
		if (p2 && q2) {
			wLen1 = p2 - p1; // word index delta, comma will turn into space
			wLen2 = q2 - q1; // comma will turn into terminating zero
		}
		else if (!p2 && !q2) {
			wLen1 = strlen(p1); // if last word
			wLen2 = strlen(q1);
			last = 1;
		}
		else {
			return 0;
		}
		if (!last) {
			ppOut[i] = (char*)calloc(wLen1 + wLen2, sizeof(char)); // sizeof(char) = 1, calloc sets terminating zero
			strncpy_s(ppOut[i], wLen1 + 1, p1, wLen1); // second paramter in gets_s is a backup for calloc: terminating zero
			ppOut[i][wLen1 - 1] = ' '; // comma to space
			strncpy_s(ppOut[i] + wLen1, wLen2, q1, wLen2 - 1); // do not copy comma
			p1 = p2 + 1;
			q1 = q2 + 1;
		}
		else {
			ppOut[i] = (char*)calloc(wLen1 + wLen2 + 2, sizeof(char)); // +2 for the space and terminating zero
			strncpy_s(ppOut[i], wLen1 + 1, p1, wLen1);
			ppOut[i][wLen1] = ' ';
			strncpy_s(ppOut[i] + wLen1 + 1, wLen2 + 1, q1, wLen2);
		}
	}
	return ppOut;
}

int WordCount(char* pInput)
{
	if (!pInput || !*pInput) {
		err_msg = EMPTY;
		return 0;
	}
	int count = 0;
	char* pSearch = strchr(pInput, ' ');
	while (pSearch)
	{
		count++;
		pSearch = strchr(pSearch + 1, ' ');
	}
	free(pSearch);
	return ++count;
}