/* checking for blank lines */
bool blankCheck(info_of_line line_info,char * word);
/* a main procedure the first pass!
will go through the line sent from the file and determine 
for each word if its a label , a register , an instruction ..
and will send it to the right functin to keep the coding process */
bool first_pass(info_of_line line_info, pross_table *file_tables);
/*on the first pass, if the label is entry were returning true; 
if its extern will check the label name, and create a symbol if there are no errors.
the last " return false " is unreachable because we covered all of the possible cases*/
bool checkExternAndEntry(instruction current_inst,info_of_line line_info, symbol_table *table);
/*check if the current word sent to the func is a data or a string */
bool checkDataAndStr(info_of_line line_info,instruction current_inst ,table* dataTable);
/*coding the string to the data table, and checking for any faulties */
int readStr(info_of_line line_info,table* dataTable);
/*coding the data to the data table, and checking for any faulties */
int readData(info_of_line line_info,table* dataTable);
/* coding the instructions ( such as mov .. jmp .. stop .. etc ) to the instruction table */
bool codePross(info_of_line line_info, char word[LINELEN], table *code_table);
/* coding the operators of 1 operator instruction type to the instructions table */  
void kidud1(value addrs2,int index, table *code_table);
/* coding the operators of jump operator instruction type to the instructions table */ 
void kidud_jump(value addrs1,value addrs2,value addrs3,int index,  table *code_table);
/* coding the operators of the 2 operator instruction type to the instructions table */ 
void kidud2 (value addrs1,value addrs2,int index,  table *code_table);

