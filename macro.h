
/* this function save the names of the macros in a dynamic array
for each name well reallocate another space for it
well expend also the contents array for each name well define */
void saveName(char* buffer, char*** macNames , char*** macConts,int* maci,int i , int* mcrBool);

/* this function save the contents of the macros in a dynamic array
for each content line we'll reallocate another space for it,
as the size of the line*/
void saveCon(char* buffer , char*** macConts,int *maci,int i,int* mcrBool);

/*printing to a temp file the requested */				
void printing(char* buffer,FILE *fp, FILE *fpNew ,char** macNames , char** macConts, int *maci)	;
/* this function is the main algorithm of the macro stage, the PREASSEMBLER! 
its using savename, savecon, and printing. at the end , 
it frees the pointers */
void mcrFunc(FILE *fp, FILE *fpNew)	;
