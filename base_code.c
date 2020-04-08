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

FILE* fp; //to be a pointer to FILE
char input;

int found;
int nextid;
int nextfree = 0;
int hashcode;
int stidx;
int checkLen = 1;
int printlen;

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

int isCharacter() {
	return ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z') || input == '_') ? 1 : 0;
}

int isDigit() {
	return (input >= '0' && input <= '9') ? 1 : 0;
}

int isSeperator() {
	return (strchr(seperators, input) == NULL) ? 0 : 1;
}

// PrintError - Print out error messages
//				overst : overflow in ST
//				print the hashtable and abort by calling the function "abort()".
//				illid : illegal identifier
//				illsp :illegal seperator
void PrintError(ERRORtypes err) {
	switch (err) {
	case overst: printf(" ***Error***		OVERFLOW\n"); break; abort();
	case illid: printf(" ***Error***			");
		printlen = 0;
		for (int i = nextid; i < nextfree; i++) {
			printf("%c", ST[i]);
			printlen++;
		}
		for (int i = 0; i < 15 - printlen; i++) {
			printf(" ");
		}
		printf("	start with digit\n"); break;
	case illsp: printf(" ***Error***			%c		illegal seperator\n", input); break;
	}
}

// Skip Seperators - skip over strings of spaces,tabs,newlines, . , ; : ? !
//					 if illegal seperators,print out error message.
void SkipSeperators() {
	while (input != EOF) {
		if (isSeperator()) { input = fgetc(fp); checkLen = 1; }
		else if (!(isDigit() || isCharacter())) {
			PrintError(illsp);
			input = fgetc(fp);
			checkLen = 1;
		}
		else return;
	}
}

// PrintHStable - Prints the hash table.write out the hashcode and the list of identifiers
//				  associated with each hashcode,but only for non-empty lists.
//				  Print out the number of characters used up in ST.
void PrintHStable() {
	HTpointer ptr;
	int hidx = 0, sidx = 0;

	printf("\n\n[[ HASH TABLE ]]\n\n");

	for (hidx = 0; hidx < HTsize; hidx++) {
		if (HT[hidx] != NULL) {
			printf("Hash Code %3d : ", hidx);
			for (ptr = HT[hidx]; ptr != NULL; ptr = ptr->next) {
				sidx = ptr->index;
				printf("%s ", &ST[sidx]);
			}
			printf("\n");
		}
	}
	printf("\n<%d characters are used in the string table>\n", nextfree);
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
		//10글자 초과할 시 
		if (nextfree - nextid == 10) {
			checkLen = 0;
		}
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
	}

}

// ComputeHS - Compute the hash code of identifier by summing the ordinal values of its
//			   characters and then taking the sum modulo the size of HT.
void ComputeHS(int nid, int nfree) {
	hashcode = 0;
	for (int i = nid; i < nfree - 1; i++) hashcode += (int)ST[i];
	if (hashcode >= HTsize) hashcode %= HTsize;
}

// LookupHS - For each identifier,Look it up in the hashtable for previous occurrence
//			  of the identifier.If find a match, set the found flag as true.
//			  Otherwise flase.
//			  If find a match, save the starting index of ST in same id.
void LookupHS(int nid, int hscode) {
	//HT[hashcode] is not nill, search the linked list for identifier
	HTpointer pt;
	int curIdx, searchIdx;
	found = 0;
	if (HT[hashcode] != NULL) {
		pt = HT[hashcode];
		while (pt != NULL && found == 0) {
			found = 1;
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
	HTpointer new_entry = (HTpointer)malloc(sizeof(HTentry));
	new_entry->index = nextid;
	new_entry->next = NULL;
	if (HT[hscode] != NULL) {
		HTpointer temp_p = HT[hscode];
		while (temp_p->next != NULL) {
			temp_p = temp_p->next;
		}
		temp_p->next = new_entry;
	}
	else {
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
		if (err != illid && (ST[nextid] != 0 || input != EOF)) {

			if (nextfree >= STsize) {
				// print error message
				err = overst;
				PrintError(err);
			}
			ST[nextfree++] = '\0';
			ComputeHS(nextid, nextfree);
			LookupHS(nextid, hashcode);
			printlen = 0;
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
			else {
				printf("%6d                        ", stidx);
				for (int i = nextid; i < nextfree; i++) {
					printf("%c", ST[i]);
					printlen++;
				}
				for (int i = 0; i < 15 - printlen; i++) {
					printf(" ");
				}
				printf("(already existed)\n");
				nextfree = nextid;
			}
		}
	}
	PrintHStable();
	getchar();

	fclose(fp);
}