/* the main function of outputs. will give us the files 
of the entry symbols, extern symbols and the machine code file.
entry symbol and extern symbol files would be as an output if
there are entry symbols and extern symbols */
void output(pross_table *file_tables, int ic, int dc);
/* checks how many nodes are in entry table 
if zero then return without doing anythings . otherwise 
will print the entry table */
void entTables(symbol_table *table);
/*printing the entry symbols to the ps.ent file 
would not print anything if there arnt any appropriate symbols
thanks to the func entTables */
void printEntTables(symbol_table *table);
/* checks if the extern symbols tabels is empty. if it is do nothing  */ 
void exTables(symbol_table *ExSymbol);
/*printing the extern symbols to the ps.ent file
would not print anything if there arnt any appropriate symbols 
thanks to the func exTables */
void printExTables(symbol_table *ExSymbol);
/* printing the '.' and '/' to the "ps.ob" file */
int encoding(table* out1, table* out2, int arr1Len , int arr2Len);
