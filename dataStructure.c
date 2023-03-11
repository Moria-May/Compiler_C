#include "proto.h"
#include "dataStructure.h"






/*opcode type: 2 operators array*/
static opCode2 opCodeVal2Op[]= 
{
    {0 ,"mov",3,{IMMEDIATE,DIRECT,DIRECT_REG},2,{DIRECT,DIRECT_REG}},
    {1 ,"cmp",3,{IMMEDIATE,DIRECT,DIRECT_REG},3,{IMMEDIATE,DIRECT_REG,DIRECT}},
    {2 ,"add",3,{IMMEDIATE,DIRECT,DIRECT_REG},2,{DIRECT,DIRECT_REG}},
    {3 ,"sub",3,{IMMEDIATE,DIRECT,DIRECT_REG},2,{DIRECT,DIRECT_REG}},
    {6 ,"lea",1,{DIRECT},2,{DIRECT,DIRECT_REG}},

};
/*opcode type 1 operator array*/
static opCode1 opCodeVal1Op[] = 
{
    {4 ,"not",2,{DIRECT,DIRECT_REG}},
    {5 ,"clr",2,{DIRECT,DIRECT_REG}},
    {7 ,"inc",2,{DIRECT,DIRECT_REG}},
    {8 ,"dec",2,{DIRECT,DIRECT_REG}},
    {9 ,"jmp",3,{DIRECT,JUMP,DIRECT_REG}},
    {10 ,"bne",3,{DIRECT,JUMP,DIRECT_REG}},
    {11 ,"red",2,{DIRECT,DIRECT_REG}},
    {12 ,"prn",4,{IMMEDIATE,DIRECT,DIRECT_REG}},
    {13 ,"jsr",3,{DIRECT,JUMP,DIRECT_REG}},
};
/*opcode type: 0 operators array*/
static opCode0 opCodeValZOp[] = 
{
    {14 ,"rts"},
    {15 ,"stop"},
};
/* a structure for searcing the instance of the word after the '.' operator */ 
static struct search_inst search_inst_table[] = {
    {"data", DATA_INST},
    {"string", STRING_INST},
    {"entry", ENTRY_INST},
    {"extern", EXTERN_INST},
    {NULL, NONE_INST}
};
/* freeing a node */
void free_symbol(symbol *sym)
{
    if (sym == NULL)
        return;

    else
    free(sym);
}

/* freeing the link list , step by step */ 
void del_symbol_table(symbol_table *table)
{
    symbol* node;
    symbol* prev;


    if (table == NULL || *table == NULL)
        return;

    node = *table;
    prev = node;

    /* Go over all the symbols in the table */
    while (node != NULL)
    {
        prev = node;
        node = node->next;

        free_symbol(prev);
    }
}

/* test func. delete before submission */
void print_table(table* out, int size)
{
	int i, j;
    for (i = 0; i < size; i++)
    {
        printf("address = %d:\n", out[i].address);
		for (j = 13; j >= 0; j--) {
			if ((out[i].code) & (1 << j)) 
                printf("%d ", 1);
            else
                printf("%d ", 0);
			
        }
        printf("\n");
    }
}
/* printing the sumbol name, address and type */
void printSymbol(symbol *sym)
{
    printf("Name = %s : Address = %d : Type = %d.\n", sym->name, sym->address, sym->type);
}
/* test func. delete before submission */
void dump_symTable(symbol_table* table)
{
    symbol* node;
    if (table == NULL || *table == NULL)
    {
        printf("the Table is not initated yet.\n");
        return;
    }
    node = *table;

    while (node != NULL)
    {
        printSymbol(node);
        node = node->next;
    }

}

/* check if the symbol is on the symbol table, if not returns NULL */
symbol* find_symbol(symbol_table *table, char *name)
{
	
    symbol* node;
    if (table == NULL || name == NULL || *table == NULL)
    {
        return NULL;
    }
    node = *table;
    while (node != NULL)
    { printf("%s name\tnode name %s\n",name, node->name);
   
        if (!strcmp(node->name, name) ) {/* if found, return the pointer */
         
            return node;
        }
        node = node->next;
    }
    
    return NULL; /* if not found, return NULL */
}
/* creating a node which represents a symbol 
using tp as a type and ad as an address. the name will be the name too */
symbol *create_symbol(char *name, sym_type tp, unsigned int ad)
{
    symbol *sym = (symbol*) malloc(sizeof(symbol));
    /* set the values */
    strncpy(sym->name, name,LINELEN);
     if (strchr(sym->name,'\n'))
                {
                *strchr(sym->name,'\n') = '\0';
                }
    sym->type = tp;
    sym->address = ad;
    sym->next = NULL;
    return sym;
}
/* adding a node to the end of the linked list of the symbols */
void add_symbol(symbol_table *table, symbol *sym)
{
    symbol *node;
    /* if the table isn't initated, init the table */
    if (*table == NULL)
    {
        *table = sym;
        return;
    }
    node = *table;
    while (node->next != NULL)
    {
        if (node == sym) /* prevent nasty loops */
            return;
        node = node->next;
    }
    node->next = sym; /* add the symbol to the table */
}



/* returns the type of the operand*/
addrs_method addressType(char* word){
    if (*(word) == '#'){
        return IMMEDIATE;
    }
    /*checking for register*/
    if((*(word) == 'r' && atoi(word+1) >= 0 && atoi(word+1) <=7 && *(word+2)== '\0' )||((*(word+2)== '\n') && (*(word) == 'r') && atoi(word+1) >= 0 && atoi(word+1) <=7))
    {
        return DIRECT_REG;
    }
    if (*(word) == '(' && *(word+1) == '#')
    {
    	return JUMP;
    }
    /*checking for label*/
    else  
    if (isdigit(*(word))|| strlen(word) > 30)
    {
   
    return NONE_ADDRS;
    }
    
        return DIRECT; 

}

/*
****************************************************************************************
  end code 
****************************************************************************************
*/
/* using the structure searc_inst in order to find the instance of the word
after the '.' operand */
instruction findInst(char *str)
{
    search_inst *inst;

    /* Search the Instruction Type */
    for(inst = search_inst_table; inst->name_inst != NULL; inst++)
    {
        if(strcmp(str, inst->name_inst) == 0)
        {
            return inst->value_inst;
        }
    }
    /* The last String in the array is: inst->name_inst == NULL */
    return inst->value_inst; /* == NONE_INST (NULL) */
}

/* checks if the symbol is legal ************************** THIS FUNCTION NEEDS HELP **/ 
int CheckTheLabelName(char *labelName)
{ 

    int i ;
    for(i=0;i<(sizeof(opCodeVal2Op) / sizeof(struct opCode2));i++){
        if (!strcmp(labelName,opCodeVal2Op[i].commandName)){
            printf("ERROR");
            return 0;
        }
    }
    for(i=0;i<(sizeof(opCodeVal1Op) / sizeof(struct opCode1));i++){
        if (!strcmp(labelName,opCodeVal1Op[i].commandName)){
            printf("ERROR");
            return 0;
        }
    }
    for(i=0;i<(sizeof(opCodeValZOp) / sizeof(struct opCode0));i++){
        if (!strcmp(labelName,opCodeValZOp[i].commandName)){
            printf("ERROR");
            return 0;
        }
    }

    return 1; 
}




