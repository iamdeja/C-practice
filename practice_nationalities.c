#include "stdlib.h"
#include "stdio.h"
#include "string.h"

char names[] = "John, Mary, James, Elizabeth, Richard";
const char* pNationalities[] = { "Great Britain", "USA", "Jamaica", "USA", "Canada" };

typedef struct { char* pName; const char* pNationality; } STUDENT;

STUDENT* Exam(char* pNames, const char** ppNationalities, const char* pNationalityToStudy);

int main() {

	STUDENT* pResult = Exam(names, pNationalities, "Canada");
	if (pResult) {
		printf("%s %s\n", pResult->pName, pResult->pNationality);
	}
	else {
		printf("something went wrong");
	}

	return 0;
}

STUDENT* Exam(char* pNames, const char** ppNationalities, const char* pNationalityToStudy) {
	//error checking first!!!
	if (pNames == 0 || ppNationalities == 0 || pNationalityToStudy == 0) {
		return 0;
	}
	if (*pNames == 0) {
		return 0;
	}

	//calculate the number of names
	int nNames = 1;
	for (int i = 0; i < strlen(pNames); i++) {
		if (*(pNames + i) == ',') {
			nNames++;
		}
	}
	//printf("%d", nNames);

	//error checking again
	for (int i = 0; i < nNames; i++) {
		if (*ppNationalities == 0) {
			return 0;
		}
	}

	//save cordinates of names
	int* nameCords = (int*)malloc(nNames * sizeof(int));
	int n = 1;
	*nameCords = 0;
	for (int i = 0; i < strlen(pNames); i++) {
		if (*(pNames + i) == ',') {
			*(nameCords + n) = i + 2;
			n++;
		}
	}
	/*for (int i = 0; i < nNames; i++) {
		printf("%d ", *(nameCords + i));
	}*/

	//save name lengths
	int* nameLength = (int*)malloc(nNames * sizeof(int));
	for (int i = 0; i < nNames; i++) {
		for (int j = 0; ; j++) {
			if (*(pNames + *(nameCords + i) + j) == ',' || *(pNames + *(nameCords + i) + j) == 0) {
				*(nameLength + i) = j;
				break;
			}
		}
	}
	/*for (int i = 0; i < nNames; i++) {
		printf("%d ", *(nameLength + i));
	}*/

	//find the first matching name
	int found = 0;
	for (int i = 0; i < nNames; i++) {
		int match = strcmp(ppNationalities[i], pNationalityToStudy);
		if (match == 0) {
			//printf("%d ", i);
			found = i + 1;
			break;
		}
	}
	if (found == 0) {
		return 0;
	}
	found--;

	//create pointer for name
	char* name = (char*)malloc(*(nameLength + found) * sizeof(char));
	int x = 0;
	for (x; x < *(nameLength + found); x++) {
		*(name + x) = *(pNames + *(nameCords + found) + x);
	}
	*(name + x) = 0;

	//return value
	STUDENT* output = (STUDENT*)malloc(sizeof(STUDENT));
	output->pName = name;
	output->pNationality = ppNationalities[found];
	return output;

}