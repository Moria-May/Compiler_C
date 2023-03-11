#include <stdbool.h>
#define MAX_SYMBOL_LENGTH 100
#define SKIP_WHITESPACE(str) \
        while(*(str) && (*(str) == ' ' || *(str) == '\t')) \
        (str)++;

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAXSIZE 1000
#define LINELEN 81
#define LINENUM_INITIAL 100
#define EX_SYM 1
#define ARE_SKIP 2
#define REG_NUMBER 8
#include <math.h>
/*global integers*/
extern int IC , DC ;   
/*macro prototypes */

void mcrFunc(FILE *fp, FILE *fpNew);
/*printing to a temp file the requested */		
void printing(char* buffer,FILE *fp, FILE *fpNew ,char** macNames , char** macConts, int* maci);
/* this function save the contents of the macros in a dynamic array
for each content line we'll reallocate another space for it,
as the size of the line*/
void saveCon(char* buffer , char*** macConts,int *maci,int i,int *mcrBoolPtr);
/* this function save the names of the macros in a dynamic array
for each name well reallocate another space for it
well expend also the contents array for each name well define */
void saveName(char* buffer, char*** macNames , char*** macConts,int* maci, int i, int* mcrBool);

