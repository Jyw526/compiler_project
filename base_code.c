#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "testdata.txt"
#define STsize 1000	//size of string table
#define HTsize 100	//size of hash table

// more define variables��
typedef struct HTentry *HTpointer;
typedef struct HTentry {
	int index;		//index of identifier in ST
	HTpointer next;	//pointer to next identifier
} HTentry;

enum errorTypes { noerror, illsp, illid, overst };
typedef enum errorTypes ERRORtypes;

char seperators[] = " .,;:?!\t\n";

HTpointer HT[HTsize];
char ST[STsize];

// more global variables��
ERRORtypes err;

FILE *fp; //to be a pointer to FILE
char input;

int found;
int nextid;
int nextfree = 0;
int hashcode;

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
	return ((input >= 'a'&&input <= 'z') || (input >= 'A'&&input <= 'Z') || input == '_') ? 1 : 0;
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
	case overst: printf("***Error***		OVERFLOW\n"); break; abort();
	case illid: printf("***Error***	%c		illegal identifier\n", input); break;
	case illsp: printf("***Error***	%c		illegal seperator\n", input); break;
	}
}

// Skip Seperators - skip over strings of spaces,tabs,newlines, . , ; : ? !
//					 if illegal seperators,print out error message.
void SkipSeperators() {
	while (input != EOF) {
		if (isSeperator()) input = fgetc(fp);
		else if (!(isDigit() || isCharacter())) PrintError(illsp);
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

}

// ComputeHS - Compute the hash code of identifier by summing the ordinal values of its
//			   characters and then taking the sum modulo the size of HT.
void ComputeHS(int nid, int nfree) {
	hashcode = 0;
	for (int i = nid; i < nfree - 1; i++)
		hashcode += ((ST[i] >= 'A') && (ST[i] <= 'Z')) ? (int)ST[i] + 32 : (int)ST[i];
	if (hashcode >= HTsize) hashcode %= HTsize;
}

// LookupHS - For each identifier,Look it up in the hashtable for previous occurrence
//			  of the identifier.If find a match, set the found flag as true.
//			  Otherwise flase.
//			  If find a match, save the starting index of ST in same id.
void LookupHS(int nid, int hscode) {

}

// ADDHT - Add a new identifier to the hash table.
//		   If list head ht[hashcode] is null, simply add a list element with
//		   starting index of the identifier in ST.
//		   IF list head is not a null , it adds a new identifier to the head of the chain
void ADDHT(int hscode) {
	HTentry new_entry = {nextid,NULL};
	if(HT[hscode]==NULL){
		HT[hscode]=&new_entry;
	}
	else{
		HTentry* temp_p = HT[hscode];
		while(temp_p->next!=NULL){
			temp_p = temp_p->next;
		}
		temp_p->next = &new_entry;
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
		if (input != EOF && err != illid) {
			if (nextfree == STsize) {
				// print error message
				err = overst;
				PrintError(err);
			}
			ST[nextfree++] = '\0';
			ComputeHS(nextid, nextfree);
			LookupHS(nextid, hashcode);
			if (!found) {
				// print message
				ADDHT(hashcode);
			}
			else {
				// print message
			}
		}
	}
	PrintHStable();
	getchar();
}