#include "proto.h"
#include "dataStructure.h"
#include "first_pass.h"
#include "second_pass.h"
#include "macro.h"
#include "output.h"
/*#include "first_pass.c"
#include "second_pass.c"
#include "macro.c"*/
int IC = 0, DC = 0;  /* global intergers */
bool assembler(char *name); /* func prototyp
/* main function, first the preassembler pass and then another 2 passes on the file */
int main( int argc, char *argv[])
{	
	int arg_i; /* argument index*/
	int numberOfArguments = argc;
	FILE *fpNew = NULL;
	FILE *fp = NULL; /* file pointer */	
	char kelet[30];
	char afterMac[30];

	/* check fora right number of arguments */
	if (numberOfArguments == 1) 
	{
		printf("number of arguments should be at least 2,\n");
		printf("1 for the program name,\nand the others for  the file name.\n");
		return -1;
	}
	for (arg_i = 1 ;arg_i < numberOfArguments ;arg_i++ )
	{
	strcpy(kelet,argv[arg_i]);
	strcat(kelet,".as");
	strcpy(afterMac,argv[arg_i]);
	strcat(afterMac,".am");
	fp = fopen(kelet, "r");
	
	if (fp == NULL)
	{	
		printf("reading failed\n");
        return -1;   
	} 
	fpNew = fopen (afterMac, "w");
	if (fpNew == NULL)
	{	
		printf("reading failed\n");
        return -1;   
	} 
	
	mcrFunc(fp,fpNew); /*preassembler*/
	assembler(afterMac);
}
return 0;	

}
/* the assembler that goes for the 2 passes */
bool assembler(char *name)
{
    /* boolean for present : if it has succeeded so far */
    bool is_success = true;
    /* assembly src namefile */ 
    /* assembly src file */
    FILE* fp;   
    /* to check the line that is limited to a maximum of LINELEN characters, with 2 more places for '\n' and '\0' */
    char current_line[LINELEN + 2]; 
    /* contains information on the current line */
    info_of_line line_info;
    /* the tables needed to process the file */
    pross_table file_tables;
    /* Reset all the Pointers */
    memset(&file_tables, 0, sizeof(table));
    fp = fopen(name, "r");
    if(fp == NULL)
    {
        printf("error opening file");
        return false;
    }
    /* Go over all the lines in the source file */
    printf("Iterating all the lines\n");
    for(line_info.line_num = 1, line_info.file_name = name; 
        (line_info.text = fgets(current_line, LINELEN , fp)) != NULL; line_info.line_num ++) 
    {
        is_success = first_pass(line_info, &file_tables) && is_success;

    }
    
    printf("%d\n",IC);
    IC = 0;
    printf("%d\n",IC);
    rewind(fp);
   	dump_symTable(&file_tables.symbolTable);
    if (is_success){
    for(line_info.line_num = 1, line_info.file_name = name; 
        (line_info.text = fgets(current_line, LINELEN , fp)) != NULL; line_info.line_num ++) 
        {
       
        is_success = second_pass(line_info, &file_tables) && is_success;
         printf("line.info = %ld\nIC = %d\n",line_info.line_num,IC);
        }
	}
	if(is_success){
		output(&file_tables, IC, DC);
	}
    printf("Try to delete\n");
    print_table((file_tables.code_table), IC);
    print_table((file_tables.data_table), DC);
	printf("exSymbol\n");
	dump_symTable(&(file_tables.ExSymbol));
    del_symbol_table(&(file_tables.ExSymbol));    
	printf("Symbol\n");
	dump_symTable(&(file_tables.symbolTable));
    del_symbol_table(&(file_tables.symbolTable));/**************************/
    printf("Deleted Success!\n");
    return is_success;
    
}
