
#define REG_NUMBER 8
#define LINELEN 81
/* Addressing mode type */
typedef enum addrs_method
{
    IMMEDIATE = 0,
    DIRECT = 1,
    JUMP = 2,
    DIRECT_REG = 3,
    NONE_ADDRS = -1
} addrs_method;

typedef struct value
{
	addrs_method adr;
	char * opName;
}value;
/*struct of opcode with 2 operators sorce and destination*/
typedef struct opCode2
{
	short commandNumber;
	char commandName[10];
    int opSourceLen; 
    addrs_method opSource[4];
    int opDestinLen;
    addrs_method opDestination[4];

}opCode2;

/*struct of opcode with 1 operator just destination*/

bool blankCheck(info_of_line line_info,char * word)
{
	int i;
	for( i = 0 ; i< strlen(line_info.text) ; i++)
        {
        if ( isspace(word[i] ))
       	    continue;
        else
        	break;
        }
        if (i == strlen(line_info.text)-1)
        {
        	return true;
        }
        return false;
}

typedef struct opCode1
{
	int commandNumber;
	char commandName[10];
	int opDestinLen;
    addrs_method opDestination[4];
    

}opCode1;

/*struct of opcode without operators */
typedef struct opCode0
{
	int commandNumber;
	char commandName[10];

}opCode0;

/** Define the symbol type */
typedef enum sym_type 
{
    ENTRY_Sym,
    DATA_Sym,
    CODE_Sym,
    EXTERNAL_Sym,
    NONE_Sym
} sym_type;


/* the definition of a linkedlist for the symbols named symbol */ 
typedef struct symbol
{
    /** The name of the symbol */
    char name[LINELEN];
    /** The type of the symbol */
    sym_type type;
    /** The symbol memory address, by base & offset */
    int address;
    /** The next Symbol entry */
    struct symbol* next;

} symbol;
/** Code type */
typedef enum code_type
{
    INSTRUCTION,
    OPCODE,
    DATA
} code_type;

/*pattern table */
typedef struct table{
    int address;
    short code;
}table;
/* definition of symbol */
typedef symbol *symbol_table;

typedef struct pross_table 
{
    table code_table[1000];
    table data_table[1000];
    /* Symbol_table is just pointer to symbol*/
    symbol_table symbolTable;
	symbol_table ExSymbol;

}pross_table;

/** Instruction type (.data/ .string/ .extern/ .entry) */
typedef enum instruction
{
    DATA_INST,
    STRING_INST,
    ENTRY_INST,
    EXTERN_INST,
    NONE_INST
} instruction;

/* Define structure & array to search for instruction */
typedef struct search_inst
{
    char *name_inst;
    instruction value_inst;
} search_inst;

typedef struct info_of_line
{
    /* the file name */
    char *file_name;
    
    /* the line number in the file */
    unsigned long line_num;
    
    /* pointer to a string that contains the contents of the line */
    char *text;
    
} info_of_line;



/* checks if the symbol is legal*/
int CheckTheLabelName(char *labelName);
/* using the structure searc_inst in order to find the instance of the word
after the '.' operand */
instruction findInst(char *str);
/* returns the type of the operand*/
addrs_method addressType(char* word);
/* adding a node to the end of the linked list of the symbols */
void add_symbol(symbol_table *table, symbol *sym);
/* creating a node which represents a symbol 
using tp as a type and ad as an address. the name will be the name too */
symbol *create_symbol(char *name, sym_type tp, unsigned int ad);

/* check if the symbol is on the symbol table, if not returns NULL */
symbol* find_symbol(symbol_table *table, char *name);
/* test func. delete before submission */
void dump_symTable(symbol_table* table);
/* printing the sumbol name, address and type */
void printSymbol(symbol *sym);
/* test func. delete before submission */
void print_table(table* out, int size);
/* freeing the link list , step by step */ 
void del_symbol_table(symbol_table *table);
/* freeing a node */
void free_symbol(symbol *sym);

