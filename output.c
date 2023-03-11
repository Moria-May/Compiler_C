#include "proto.h"
#include "dataStructure.h"
#include "output.h"
/* the main function of outputs. will give us the files 
of the entry symbols, extern symbols and the machine code file.
entry symbol and extern symbol files would be as an output if
there are entry symbols and extern symbols */
void output(pross_table *file_tables, int ic, int dc)
{
	
	entTables(&(file_tables->symbolTable));
	exTables(&(file_tables->ExSymbol));
	encoding((file_tables->code_table), (file_tables->data_table), IC , DC);


}
/* checks how many nodes are in entry table 
if zero then return without doing anythings . otherwise 
will print the entry table */
void entTables(symbol_table *table){
	int count =0;	
	symbol* node;
	node = *table;
	while (node != NULL)
    {
        if(node->type == ENTRY_Sym )
			count++;
        node = node->next;
    }
	if(count == 0)
		return;
	else
	{
		printEntTables(table);
	}


}
/*printing the entry symbols to the ps.ent file 
would not print anything if there arnt any appropriate symbols
thanks to the func entTables */
void printEntTables(symbol_table *table){
	symbol* node;
	
	FILE *ent = fopen("ps.ent", "w");
	node = *table;
	while (node != NULL)
    {
		if(node->type == ENTRY_Sym )
        	fprintf(ent,"%s   %d\n",node->name,((node->address)+100));
        node = node->next;
    }
}
/* checks if the extern symbols tabels is empty. if it is do nothing  */ 
void exTables(symbol_table *ExSymbol)
{
    
    if (ExSymbol == NULL || *ExSymbol == NULL)
    {
        printf("the Table is not initated yet.\n");
        return;
    }
	printExTables(ExSymbol);
	
}
/*printing the extern symbols to the ps.ent file
would not print anything if there arnt any appropriate symbols 
thanks to the func exTables */
void printExTables(symbol_table *ExSymbol){
	symbol* node;	
	FILE *ext = fopen("ps.ext", "w");
    node = *ExSymbol;

    while (node != NULL)
    {
        fprintf(ext,"%s   %d\n",node->name,((node->address)+100));
        node = node->next;
    }
}

/* printing the '.' and '/' to the "ps.ob" file */
int encoding(table* out1, table* out2, int arr1Len , int arr2Len){
int i,j;
FILE *f = fopen("ps.ob", "w");
int address = 100;

if (f == NULL)
{	
	printf("reading failed\n");
        return -1;   
} 
fprintf(f,"%d  %d  ",arr1Len , arr2Len);

    for(i=0;i<arr1Len; i++){
        fprintf(f, "\n0%d   ",address++);
		for (j = 13; j >= 0; j--) {
			if ((out1[i].code) & (1 << j))
				fprintf(f, "/");
            		else
               			fprintf(f,"."); 
		}
	}
    for(i=0;i<arr2Len; i++){
        fprintf(f, "\n0%d   ",address++);
		for (j = 13; j >= 0; j--) {
			if ((out2[i].code) & (1 << j))
				fprintf(f, "/");
            		else
               			fprintf(f,"."); 
		}
	}
    return 0;
}


