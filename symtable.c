/* symtable.c - hash symbol table
 * programmer 백지수, 송주은, 정연우
 * date 05/07/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"

#define STsize 1000	//size of string table
#define HTsize 100	//size of hash table
#define maxLen 10 //identifier 유효 글자수
#define MIN(a,b) (((a)<(b))?(a):(b))

typedef struct HTentry* HTpointer;
typedef struct HTentry {
	int index;		//index of identifier in ST
	HTpointer next;	//pointer to next identifier
} HTentry;

HTpointer HT[HTsize];
char ST[STsize];

char input;
int found;			//identifier 중복 판단 플래그 변수
extern int nextid;
int nextfree = 0;
int hashcode;
int stidx;			//identifier 중복시 기존 identifier의 ST 인덱스 저장 변수

//ReadIO - Read identifier directly into ST(append it to the previous identifier).
//		   An identifier is a string of letters and digits, starting with a letter.
void ReadID() {
	int min_i = yyleng;
	nextid = nextfree;

	//identifier의 유효길이는 최대 maxLen을 따른다
    min_i = MIN(yyleng,maxLen);
	for(int i=0; i<min_i; i++){
	        input = yytext[i];
		//ST사이즈 초과시 에러
		if (nextfree == STsize) {
            		//error 관리 추가
			//err = overst;
			//PrintError(overst);
		}
		//소문자로 통일하여 삽입
		if (input >= 'A' && input <= 'Z') input += 32;
		ST[nextfree++] = input;
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

// SymbolTableManagement
int SymbolTable(){
    //ST 추가
    ReadID();
    ST[nextfree++] = '\0';
    //해싱
    ComputeHS(nextid,nextfree);
    //lookup
    LookupHS(nextid,nextfree);
    //add HT
    if(!found){
        ADDHT(hashcode);
        return nextid;
    }
    else{
        nextfree = nextid;
		ST[nextid] = '\0';
        return stidx;
    }
}
