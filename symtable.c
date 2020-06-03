/* symtable.c - hash symbol table
 * programmer 백지수, 송주은, 정연우
 * date 05/07/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"

#define maxLen 10 //identifier 유효 글자수
#define MIN(a,b) (((a)<(b))?(a):(b))

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
			if(found){
				cur_ID = pt;
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
	cur_ID = new_entry;
	new_entry->index = nextid;
	new_entry->line_num = line;
	new_entry->type = 0; //default
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
				//identifier (variable type, line num) 출력
				switch(ptr->type){
					//case parse_error: 
					case void_function: printf("\t %s (function name, return type = void, line%d) \t", &ST[sidx],ptr->line_num); break;
					case int_function: printf("\t %s (function name, return type = int, line%d) \t", &ST[sidx],ptr->line_num); break;
					case float_function: printf("\t %s (function name, return type = float, line%d) \t", &ST[sidx],ptr->line_num); break;
					case int_scalar: printf("\t %s (integer scalar variable, line%d) \t", &ST[sidx],ptr->line_num); break;
					case float_scalar: printf("\t %s (float scalar variable, line%d) \t", &ST[sidx],ptr->line_num); break;
					case int_array: printf("\t %s (integer array variable, line%d) \t", &ST[sidx],ptr->line_num); break;
					case float_array: printf("\t %s (float array variable, line%d) \t", &ST[sidx],ptr->line_num); break;
				}
					
				
			}
			printf("\n");
		}
	}
	// ST에 지금까지 입력된 글자수 출력
	printf("\n<%d characters are used in the string table>\n", nextfree);
}

// UpdateHT - 해시테이블의 id type을 parser에서 classify해준 type으로 업데이트한다.
/*
void UpdateHT(int classified_type){
	//토큰 하나 스캐닝하고 parsing 할 경우에는
	//어차피 맨 앞에 있는 id type이 최근 것이므로 
	//next, hashcode 계산할 필요없음 (현재 저장된 hashcode가 보고 있는 토큰일테니까..?)
	HT[hashcode]->type = classified_type;
}
*/

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
