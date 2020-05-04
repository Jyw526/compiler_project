#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "testdata.txt"
#define STsize 1000	//size of string table
#define HTsize 100	//size of hash table

// more define variables¡¦
typedef struct HTentry* HTpointer;
typedef struct HTentry {
	int index;		//index of identifier in ST
	HTpointer next;	//pointer to next identifier
} HTentry;

enum errorTypes { noerror, illsp, illid, overst };
typedef enum errorTypes ERRORtypes;

char seperators[] = " .,;:?!\t\n";

HTpointer HT[HTsize];
char ST[STsize];

// more global variables¡¦
ERRORtypes err;

FILE* fp;			//to be a pointer to FILE
char input;

int found;			//identifier 중복 판단 플래그 변수
int nextid;
int nextfree = 0;
int hashcode;
int stidx;			//identifier 중복시 기존 identifier의 ST 인덱스 저장 변수
int checkLen = 1;	//identifier 길이 판단 플래그 변수
int printlen;		//identifier 길이 저장 변수(출력시 사용)

//Initialize - open input file
void initialize() {
	fp = fopen(FILE_NAME, "r");
	input = fgetc(fp);
}

void PrintHeading() {
	printf("--------------		------------\n");
	printf(" Index in ST		 identifier\n");
	printf("--------------		------------\n");
}

// isCharacter - 입력된 문자가 알파벳 또는 _(언더바)인지 판단
int isCharacter() {
	return ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z') || input == '_') ? 1 : 0;
}

// isDigit - 입력된 문자가 숫자인지 판단
int isDigit() {
	return (input >= '0' && input <= '9') ? 1 : 0;
}

//isSeperator - 입력된 문자가 미리 정의된 seperator에 속하는지 판단
int isSeperator() {
	return (strchr(seperators, input) == NULL) ? 0 : 1;
}

// PrintHStable - Prints the hash table.write out the hashcode and the list of identifiers
//				  associated with each hashcode,but only for non-empty lists.
//				  Print out the number of characters used up in ST.
void PrintHStable() {
	HTpointer ptr;
	int hidx = 0, sidx = 0;

	printf("\n\n[[ HASH TABLE ]]\n\n");

	// HT의 각 인덱스를 순회하며 해당 인덱스에 리스트가 존재하면 그 리스트의 모든 element를 출력
	for (hidx = 0; hidx < HTsize; hidx++) {
		if (HT[hidx] != NULL) {
			// 해시코드 값 출력 후 해당 인덱스의 identifier 목록을 출력한다.
			printf("Hash Code %3d : ", hidx);
			for (ptr = HT[hidx]; ptr != NULL; ptr = ptr->next) {
				sidx = ptr->index;
				printf("%s ", &ST[sidx]);
			}
			printf("\n");
		}
	}
	// ST에 지금까지 입력된 글자수 출력
	printf("\n<%d characters are used in the string table>\n", nextfree);
}

// PrintError - Print out error messages
//				overst : overflow in ST
//				print the hashtable and abort by calling the function "abort()".
//				illid : illegal identifier
//				illsp :illegal seperator
void PrintError(ERRORtypes err) {
	switch (err) {
	case overst: printf(" ***Error***		OVERFLOW\n"); PrintHStable(); abort();
	case illid: printf(" ***Error***			");
		printlen = 0;
		// 출력 포맷을 맞추기 위한 코드
		for (int i = nextid; i < nextfree; i++, printlen++) printf("%c", ST[i]);
		for (int i = 0; i < 15 - printlen; i++) printf(" ");
		printf("	start with digit\n"); break;
	case illsp: printf(" ***Error***			%c		illegal seperator\n", input); break;
	}
}

// Skip Seperators - skip over strings of spaces,tabs,newlines, . , ; : ? !
//					 if illegal seperators,print out error message.
void SkipSeperators() {
	while (input != EOF) {
		// seperator인 경우 다음 문자를 입력받는다.
		// checkLen은 후에 각 identifier의 길이를 확인하게 만들기 위한 플래그 변수이다.
		if (isSeperator()) { input = fgetc(fp); checkLen = 1; }
		// seperator, digit, character 전부 아닌 경우 illegal seperator가 된다.
		else if (!(isDigit() || isCharacter())) {
			PrintError(illsp);
			input = fgetc(fp);
			checkLen = 1;
		}
		else return;
	}
}

//ReadIO - Read identifier from the input file the string table ST directly into
//		   ST(append it to the previous identifier).
//		   An identifier is a string of letters and digits, starting with a letter.
//		   If first letter is digit, print out error message.
void ReadID() {
	nextid = nextfree;

	//첫글자 숫자인 경우 error
	if (isDigit()) {
		err = illid;
	}
	while (input != EOF && (isCharacter() || isDigit())) {

		//ST사이즈 초과시 에러
		if (nextfree == STsize) {
			err = overst;
			PrintError(overst);
		}
		//10번째인 경우 checkLen = false 
		if (nextfree - nextid == 10) {
			checkLen = 0;
		}
		//identifier의 길이가 10글자 이하인 경우 ST에 삽입
		//이때 소문자로 통일하여 삽입 
		if (checkLen) {
			if (input >= 'A' && input <= 'Z')
				input += 32;
			ST[nextfree++] = input;
		}

		input = fgetc(fp);
	}
	if (err == illid) {
		PrintError(err);
		nextfree = nextid;
		ST[nextid] = '\0';
	}

}

// ComputeHS - Compute the hash code of identifier by summing the ordinal values of its
//			   characters and then taking the sum modulo the size of HT.
void ComputeHS(int nid, int nfree) {
	hashcode = 0;
	// 입력된 identifier의 각 글자 값을 더해서 hashcode를 계산한다.
	for (int i = nid; i < nfree - 1; i++) hashcode += (int)ST[i];
	// HT에 저장해야 하므로 hashcode가 HT의 최대 인덱스보다 큰 경우 modulo 연산으로 인덱스를 계산한다.
	if (hashcode >= HTsize) hashcode %= HTsize;
}

// LookupHS - For each identifier,Look it up in the hashtable for previous occurrence
//			  of the identifier.If find a match, set the found flag as true.
//			  Otherwise flase.
//			  If find a match, save the starting index of ST in same id.
void LookupHS(int nid, int hscode) {
	HTpointer pt;
	int curIdx, searchIdx;
	found = 0;
	//HT[hashcode]가 null이 아닌경우 
	if (HT[hashcode] != NULL) {
		pt = HT[hashcode];
		//동일한 identifier가 있는지 탐색 
		while (pt != NULL && found == 0) {
			found = 1; //동일한 identifier 존재
			curIdx = nid;
			searchIdx = pt->index;
			stidx = searchIdx;
			while (ST[curIdx] != '\0' && ST[searchIdx] != '\0' && found == 1) {
				if (ST[curIdx] != ST[searchIdx]) {
					found = 0;
				}
				else {
					curIdx++;
					searchIdx++;
				}
			}
			pt = pt->next;
		}
	}
}

// ADDHT - Add a new identifier to the hash table.
//		   If list head ht[hashcode] is null, simply add a list element with
//		   starting index of the identifier in ST.
//		   IF list head is not a null , it adds a new identifier to the head of the chain
void ADDHT(int hscode) {
	//새 entry 생성
	HTpointer new_entry = (HTpointer)malloc(sizeof(HTentry));
	new_entry->index = nextid;
	//HT[hscode]에 entry있는 경우
	if (HT[hscode] != NULL) {
		//HT[hscode] 맨 앞에 새로운 entry 삽입
		new_entry->next = HT[hscode]->next;
		HT[hscode]->next = new_entry;
	}
	//HT[hscode]에 entry없는 경우
	else {
		new_entry->next = NULL;
		HT[hscode] = new_entry;
	}
}

/* MAIN - Read the identifier from the file directly into ST.
		  Compute its hashcode.
		  Look up the idetifier in hashtable HT[hashcode]
		  If matched, delete the identifier from ST and print ST - index
		  of the matching identifier.
		  If not matched, add a new element to the list, pointing to new identifier.
		  Print the identifier, its index in ST, and whether it was entered or present.
		  Print out the hashtable, and number of characters used up in ST
*/
int main() {
	initialize();
	if (fp == NULL) {
		printf("Input file not found.\n");
		return -1;
	}
	PrintHeading();

	while (input != EOF) {
		err = noerror;
		SkipSeperators();
		ReadID();
		// EOF 후 공백 입력 안되게 하기 위해 조건 추가
		if (err != illid && (ST[nextid] != '\0' || input != EOF)) {

			if (nextfree >= STsize) {
				// print error message
				err = overst;
				PrintError(err);
			}
			ST[nextfree++] = '\0';
			ComputeHS(nextid, nextfree);
			LookupHS(nextid, hashcode);
			printlen = 0;
			//HT에 없는 경우 
			if (!found) {
				printf("%6d				", nextid);
				for (int i = nextid; i < nextfree; i++) {
					printf("%c", ST[i]);
					printlen++;
				}
				for (int i = 0; i < 15 - printlen; i++) {
					printf(" ");
				}
				printf("(entered)\n");
				ADDHT(hashcode);
			}
			//HT에 이미 존재하는 경우 
			else {
				printf("%6d				", stidx);
				for (int i = nextid; i < nextfree; i++) {
					printf("%c", ST[i]);
					printlen++;
				}
				for (int i = 0; i < 15 - printlen; i++) {
					printf(" ");
				}
				printf("(already existed)\n");
				nextfree = nextid;
				ST[nextid] = '\0';
			}
		}
	}
	PrintHStable();
	getchar();

	fclose(fp);
}