/* a main procedure the second pass!
will go through the line sent from the file and determine 
for each word if its a label , a register , an instruction ..
and will send it to the right functin to keep the coding process */
bool second_pass(info_of_line line_info, pross_table *file_tables);
/* A function that returns the address of the label , if not found returns -1 */
int labAdd(symbol_table* table, char* name);
/* coding the symbol to the table */
bool kidudSymbol(info_of_line line_info,pross_table* file_table, char  word[]);
/* this functions checks if the ENTRY label is  in the symbol linked list 
and updates the node if so, aftet that will return true . else will return  false*/
bool checkEntry2(info_of_line line_info,char *word,symbol_table *table);
/* checking for blank lines */
bool blankCheck(info_of_line line_info,char * word);
