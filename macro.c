#include "proto.h"
#include "macro.h"
/* this function save the names of the macros in a dynamic array
for each name well reallocate another space for it
well expend also the contents array for each name well define */
void saveName(char* buffer, char*** macNames , char*** macConts,int* maci,int i , int* mcrBool) 
{
	
	int j;

	*mcrBool = 1;
	(*maci)++;
	/* extending the array for each macro */
	*macNames = realloc(*macNames,((*maci)+1)*sizeof(char*));
	*macConts = realloc(*macConts,((*maci)+1)*sizeof(char*));
	(*macConts)[*maci] =(char*)malloc((strlen(buffer)+1)*sizeof(char*));
	(*macNames)[*maci] =(char*)malloc((strlen(buffer)+1)*sizeof(char));

	i+=3; /* skipping the word mcr */
			
	for (; isspace(buffer[i]) ; i++); /* arrived to the macro name */
	for (j = 0; !isspace(buffer[i]) ;j++, i++) /* saving macro name */
	(*macNames)[(*maci)][j] = buffer[i]; /* to my array of strings */ 
}
/* this function save the contents of the macros in a dynamic array
for each content line we'll reallocate another space for it,
as the size of the line*/
void saveCon(char* buffer , char*** macConts,int *maci,int i,int* mcrBool)
{
	if(strncmp(buffer + i, "endmcr", 6)) /* not endmcr */
	{/* inserting the content of the macro to maccontent */
		(*macConts)[*maci] = realloc((*macConts)[*maci],(strlen((*macConts)[*maci]+1)+(strlen(buffer)+1))*sizeof(char*));
		strcat((*macConts)[*maci],buffer);
	 
	}
	else /* endmcr */
	{
		*mcrBool = 0;
	}
}
/*printing to a temp file the requested */				
void printing(char* buffer,FILE *fp, FILE *fpNew ,char** macNames , char** macConts, int *maci)	
{	
	int i;
	int equal = 0;
	char firstRead[LINELEN];
	sscanf(buffer,"%s",firstRead); /* initialzing first word in line */
	/*printing to the file, with macros as should be */
	for ( i = 0 ; i <= (*maci) ; i ++ )
	{
		/*checking if the first word in the line is equal
		to one of the macro names*/
		/* if equal*/

		if((strcmp(firstRead,macNames[i]) == 0))
		{
		
		equal = 1;
		fputs(macConts[i],fpNew); 
		}	
	}
	/* if not equal*/
	if ((equal) == 0)
	{
	fputs(buffer,fpNew);
	}			
}		
/* this function is the main algorithm of the macro stage
its using savename, savecon, and printing. at the end , 
it frees the pointers */
void mcrFunc(FILE *fp, FILE *fpNew)		
{
	int i = 0;
	int mcrBool = 0;
	int maci = -1; /* macro  index */
	char **macConts = NULL; /*dynamicly initializing size of string array */
	char **macNames = NULL;/*dynamicly initializing size of string array */
	char buffer[LINELEN];
	/* saving macros name in macNames array of strings
	and also saving macro contents in maccontencts array of strings */ 
	while (fgets(buffer,LINELEN, fp))  
	{ 
		/* going to the macro name with i */
		for (i = 0; isspace(buffer[i]) ; i++); /* going to the first word in line */
		/* identify mcr in this if statement */
		if (buffer[i] == 'm' && buffer[i+1] == 'c' && buffer[i+2] == 'r' && mcrBool == 0) 
		{
			/*saving the name of it*/
			saveName(buffer,  &macNames ,  &macConts,&maci,i,&mcrBool);	
		}
		/*reading a macro*/
		else if ( mcrBool == 1) 
		{
			saveCon(buffer,&macConts,&maci,i,&mcrBool);	 
		}
		/*not reading a macro and is not a new macro definition line */
		else if (!(buffer[i] == 'm' && buffer[i+1] == 'c' && buffer[i+2] == 'r') && mcrBool == 0)
		{	
			printing(buffer,fp, fpNew , macNames , macConts,&maci); 
		}			
	}

	for (i=0;i<=maci;i++)
	free(macNames[i]);
	free(macNames);
	for (i=0;i<=maci;i++)
	free(macConts[i]);
	free(macConts);
	fclose(fp);
	fp = NULL;
	fclose(fpNew);
	fpNew = NULL;	
}	

