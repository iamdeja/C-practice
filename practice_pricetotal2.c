#include "stdio.h"
#include "stdlib.h"
#include "string.h"

char names[] = "John, Mary, James, Elizabeth, Richard";
typedef struct { int day; char month[4]; int year; } DATE;
DATE birthdates[] = { {2, "Jun", 2000}, {27, "Jul", 2001}, {12, "Jun", 1999}, {15, "Sep", 1998}, {16, "Jun", 2000} };
char* Exam(char* pNames, DATE* pBirthdates, const char* pMonthToStudy);

int main() {
	char* pResult = Exam(names, birthdates, "Jun");

	printf("%s", pResult);

	free(pResult);
	pResult = NULL;

	return 0;
}

char* Exam(char* pNames, DATE* pBirthdates, const char* pMonthToStudy) {
	//calculate number of names
	int nNames = 1;
	for (int i = 0; i < strlen(pNames); i++) {
		if (*(pNames + i) == ',') {
			nNames++;
		}
	}
	//printf("%d", nNames);

	//check how many people were born this day
	int nBorn = 0;
	for (int i = 0; i < nNames; i++) {
		if (strstr(pBirthdates[i].month, pMonthToStudy)) {
			nBorn++;
		}
	}
	//printf("%d", nBorn);

	//saves cordinates of people born this month
	int* cords = (int*)malloc(nBorn * sizeof(int));
	int n = 0;
	for (int i = 0; i < nNames; i++) {
		if (strstr(pBirthdates[i].month, pMonthToStudy)) {
			*(cords + n) = i;
			n++;
		}
	}
	//printf("%d %d %d", *cords, *(cords + 1), *(cords + 2));

	//save name cordinates
	int* nameCords = (int*)malloc(nNames * sizeof(int));
	n = 1;
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

	//calculate name lengths
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

	//alocate enough space for output
	int requiredSpace = 0;
	for (int i = 0; i < nBorn; i++) {
		requiredSpace = requiredSpace + *(nameLength + *(cords + i)) + 2;
	}
	//printf("%d", requiredSpace); included
	char* output = (char*)malloc(requiredSpace * sizeof(char));

	//finally assign values
	n = 0;
	for (int i = 0; i < nBorn; i++) {
		for (int j = 0; j < *(nameLength + *(cords + i)); j++) {
			//printf("%c", *(pNames + *(nameCords + *(cords + i)) + j));
			*(output + n) = *(pNames + *(nameCords + *(cords + i)) + j);
			n++;
		}
		*(output + n) = ',';
		n++;
		//printf(",");
		*(output + n) = ' ';
		n++;
		//printf(" ");
	}
	*(output + n - 2) = 0;

	free(cords);
	cords = NULL;
	free(nameCords);
	nameCords = NULL;
	free(nameLength);
	nameLength = NULL;

	return output;
}