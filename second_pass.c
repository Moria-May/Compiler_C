#include "proto.h"
#include "dataStructure.h"
#include "second_pass.h"
/* checking for blank lines */
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





/*static struct search_inst search_inst_table[] = {
    {"data", DATA_INST},
    {"string", STRING_INST},
    {"entry", ENTRY_INST},
    {"extern", EXTERN_INST},
    {NULL, NONE_INST}
};*/



/** Register r0-r15 type */
/*static char registers[8][3] =
{
    "r0",
    "r1",
    "r2",
    "r3",
    "r4",
    "r5",
    "r6",
    "r7",
  
};*/
/* a main procedure the second pass!
will go through the line sent from the file and determine 
for each word if its a label , a register , an instruction ..
and will send it to the right functin to keep the coding process */
bool second_pass(info_of_line line_info, pross_table *file_tables)
{	
	instruction current_inst;
	char word[LINELEN];
    printf("get to second_pass@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    SKIP_WHITESPACE(line_info.text);/*go to the first place in the line with char*/
    while((sscanf(line_info.text, "%s", word)) != '\n')
    {
	 
         printf("line: %s\n", line_info.text);
       	/* check for a blank line */
      	 if (blankCheck(line_info,word))
		 {
     	   return true;
    	 }  
    	 if( word[0] == ';' )
    	 {
            return true;
         }
       
         if ((*(word + (strlen(word) - 1)) == ':'))
         {
          
        	  line_info.text += (strlen(word));
         	  SKIP_WHITESPACE(line_info.text);
          continue;
          }
    
          if(*word == '.')
          { 
            current_inst = findInst(word+1);
       	    printf("\nthe inst is = %d\n" , current_inst );
            if (current_inst == ENTRY_INST)
            {
            	  line_info.text += (strlen(word));
        		  SKIP_WHITESPACE(line_info.text);
            	  sscanf(line_info.text, "%s", word);
            	  printf(" the ENTRY IS %s\n" , word);
            	  return checkEntry2(line_info,word,&(file_tables->symbolTable));
            }
            else return true;
          }
          printf("%s\n", "dec888888888888") ;
          return kidudSymbol(line_info,file_tables, word);
 	}
 	return true;
}
 
/* A function that returns the address of the label , if not found returns -1 */
int labAdd(symbol_table* table, char* name)
{	

	symbol* node;
   	 if (table == NULL || *table == NULL)
   	 {    	 
     	   return -1;
   	 }
   	
    	node = *table;
 
   	 while (node!= NULL)
    	{
    	
    	if((!strcmp(node->name,name))){
    	return node->address;}
    	else    	
      	  node = node->next;      	        	  
   	 }
   	 return -1;
}
/* coding the symbol to the table */
bool kidudSymbol(info_of_line line_info,pross_table* file_table, char  word[])
{
	bool found = false;
    int sym;
	symbol *exsymbol;
    char* tok;
    char label[30]; 
	int i,j;
    value addrs1;
    value addrs2;
    addrs_method address;
    addrs1.adr = NONE_ADDRS;
    addrs2.adr = NONE_ADDRS;
 	
    
   printf("get to kidudsymbol^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  
    
    /***************************************************************************/
    /********************** check for 2 operands *******************************/
    for(i=0;i<(sizeof(opCodeVal2Op) / sizeof(struct opCode2));i++)
    {
        if (!strcmp(word,opCodeVal2Op[i].commandName))
        {
          
        IC++;
        line_info.text += (strlen(word)); /* going to the next word */
        SKIP_WHITESPACE(line_info.text);
                tok = strtok(line_info.text, ",");
                while (tok != NULL)
                {  
               
                    address = addressType(tok);
                    if (!found)
                    {
                    addrs1.adr = address;
                    addrs1.opName = tok;
                   printf("%s_______________________________\n", addrs1.opName);
 					found = true;
 					
                    }
                   else
                    {
                     
                     addrs2.adr = address;
                     addrs2.opName = tok;  
                     printf("%s_______________________________\n", addrs2.opName);       
                            
                                              
					}
					tok = strtok(NULL, ",");
                            
                }
                if (strchr(addrs2.opName,'\n'))
                {
                *strchr(addrs2.opName,'\n') = '\0';
                }

                if ( addrs1.adr == DIRECT_REG && addrs2.adr == DIRECT_REG )
                {   
                    IC++;
                    
                    return true;
                    }
                    
                else
                {
              	  if ( addrs1.adr == DIRECT )
              	  {
              	     sym = labAdd((&(file_table->symbolTable)), addrs1.opName); 
              	     if (sym == -1) /* symbol_table*/
             	     {
   					   printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
   	 				   printf("Symbol doesnt exist since it hasnt been added in first pass\n");
              	 	   return false;
            	     }
            	     if (find_symbol(&(file_table->symbolTable),  addrs1.opName)->type == EXTERNAL_Sym )
                 	 {
						IC++;
 						exsymbol = create_symbol( addrs1.opName, EXTERNAL_Sym, IC);
                    	add_symbol((&(file_table->ExSymbol)), exsymbol);
                 	    file_table->code_table[IC].code |= EX_SYM ;
                 	    
               	     }
             	     else
              	     {   IC++;         	        
              	        sym = labAdd((&(file_table->symbolTable)), addrs1.opName);
              	        file_table->code_table[IC].code = CODE_Sym;
               	        file_table->code_table[IC].code |= (sym+LINENUM_INITIAL) << ARE_SKIP;
               	        
              	     }
              	  }
              	  else 
              	  {
             	   IC++;
             	 
              	  }
              	  
                  if ( addrs2.adr == DIRECT )
                  {  
                     sym = labAdd((&(file_table->symbolTable)), addrs2.opName);
              	     if (sym ==-1)
                      { 
                      	printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
   	 				    printf("Symbol doesnt exist since it hasnt been added in first pass\n");
                        return false;
                      }
                     else
                     {
						
                     file_table->code_table[IC].code = CODE_Sym;
                     file_table->code_table[IC].code |= (sym+LINENUM_INITIAL) << ARE_SKIP;
                     IC++;
                     }
                  }
                  else 
                  {
                    IC++;
                  }
                }
         return true;       
        }
        
    }
        /***************************************************************************/
    /********************** check for 1 operands *******************************/
    for(i=0;i<(sizeof(opCodeVal1Op) / sizeof(struct opCode1));i++)
    {
        if (!strcmp(word,opCodeVal1Op[i].commandName))
        {
        	line_info.text += (strlen(word)); /* going to the next word */ 
       		SKIP_WHITESPACE(line_info.text);
        	address = addressType(line_info.text); 
printf("the IC is: %d\n", IC); 
       		IC++;
printf("the IC is: %d\n", IC); 
                for(j=0;j <= opCodeVal1Op[i].opDestinLen ;j++)
            {
                if (address == opCodeVal1Op[i].opDestination[j])
                {
                    found = true; 
            	    addrs2.adr = address;
         		    addrs2.opName = line_info.text;
         		    if (strchr(addrs2.opName,'\n'))
                	{
                		*strchr(addrs2.opName,'\n') = '\0';
                	}
                
                printf("the addrs2 is: %d\n",addrs2.adr);
         		printf("the addrs2 is: %s\n",addrs2.opName);  
         	    }
            }
              if(addrs2.adr == IMMEDIATE)
              {
              	IC++;
              	return true; 
              }
              if(addrs2.adr == DIRECT)
        	  {
          	  	if (!strcmp(opCodeVal1Op[i].commandName,"jmp") || !strcmp(opCodeVal1Op[i].commandName,"jsr") || !strcmp(opCodeVal1Op[i].commandName,"bne"))
                {
           
        	  
        		for(i=0 ;(*(line_info.text) != '\0') && (*(line_info.text) != '(') ;line_info.text++,i++)
        		{
        			label[i] = *(line_info.text);        			
        		}
				label[i] = '\0';
				
				
                if (*(line_info.text) == '\0')
                {
               
                    if    (find_symbol(&(file_table->symbolTable), label) == NULL) /* symbol_table*/
                	{
                	   printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
   	 				   printf("Symbol doesnt exist since it hasnt been added in first pass\n");
                	   return false;
                	}
                   if (find_symbol(&(file_table->symbolTable), label)->type == EXTERNAL_Sym )
                   {
						
						exsymbol = create_symbol(label, EXTERNAL_Sym, IC);
                    	add_symbol((&(file_table->ExSymbol)), exsymbol);
                    	file_table->code_table[IC].code |=EX_SYM ;
                    	IC++;
                    	return true;
                   }
                   else
                   {
					
                	sym = labAdd((&(file_table->symbolTable)), addrs2.opName);
                	file_table->code_table[IC].code = CODE_Sym;
                	file_table->code_table[IC].code |= (sym+LINENUM_INITIAL) << ARE_SKIP;
                	IC++;
                    return true;
               		}
                }
                else if (*(line_info.text) == '(') 
                {
                	if    (find_symbol(&(file_table->symbolTable), label) == NULL)
                    { 
                	   printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
   	 				   printf("Symbol doesnt exist since it hasnt been added in first pass\n");
                       return false;
                    }

               	   else
                   {
                    sym = labAdd((&(file_table->symbolTable)), label);
                    
                    if (find_symbol(&(file_table->symbolTable), label)->type == EXTERNAL_Sym )
                    {
                    	
						exsymbol = create_symbol(label, EXTERNAL_Sym, IC);
                    	add_symbol((&(file_table->ExSymbol)), exsymbol);
                        file_table->code_table[IC].code |= EX_SYM ;
                     	IC++;
                    }
                    else
                    {
                      
				      printf("IVE GOT HERE!!!!!!!!!!!!!!!!!!!!!%s\n%d\n",label,sym);
                	  file_table->code_table[IC].code = CODE_Sym;
                	  file_table->code_table[IC].code |= (sym+LINENUM_INITIAL) << ARE_SKIP;
                	  IC++;
                    }
               		
                    tok = strtok((line_info.text+1), ",");
                    found =false;
                    while (tok != NULL)
                    {

                    	address = addressType(tok);
                    	if (!found)
                    	{
                    	addrs1.adr = address;
                    	addrs1.opName = tok;
 						found = true;
                    	}
                   		else
                    	{
                            if (strchr(tok,')'))
                			{
                				*strchr(tok,')') = '\0';
                			}
                			printf("%s888888888888\n",tok);
                       		printf("%s888888888888\n",tok);
                       		address = addressType(tok);
                            addrs2.adr = address;
                            addrs2.opName = tok;                           
						}
					tok = strtok(NULL, ",");         
                    }
                if ( addrs1.adr == DIRECT_REG && addrs2.adr == DIRECT_REG )
                {
                    IC++;
                    return true;
                }
                else
                {
              	  if ( addrs1.adr == DIRECT )
              	  {
              	  	if    (find_symbol(&(file_table->symbolTable), addrs1.opName) == NULL) 
             	  	{
             	  		printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
   	 				    printf("Symbol doesnt exist since it hasnt been added in first pass\n");
              	  		return false;
            	    }
            	       if (find_symbol(&(file_table->symbolTable), addrs1.opName)->type == EXTERNAL_Sym )
                 	  {
                 		   
							exsymbol = create_symbol(addrs1.opName, EXTERNAL_Sym, IC);
                    		add_symbol((&(file_table->ExSymbol)), exsymbol);
                  		   file_table->code_table[IC].code |= EX_SYM ;
                  		   IC++;
                  		   
                 	  }
             	      else
              	  	  {
						
              	   		sym = labAdd((&(file_table->symbolTable)), addrs1.opName);
              	  		file_table->code_table[IC].code = CODE_Sym;
               	 		file_table->code_table[IC].code |= (sym+LINENUM_INITIAL) << ARE_SKIP;
               	 		IC++;
              	  	  }
              	  }
              	  else 
              	  {
             	    IC++;
              	  }
                }
                if ( addrs2.adr == DIRECT )
                {
                	if    (find_symbol(&(file_table->symbolTable), addrs2.opName) == NULL) 
                	{   
                		printf("%s", addrs1.opName);
                		printf("%d",addrs2.adr);            		
                		printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
   	 					printf("Symboldfdf5555 doesnt exist since it hasnt been added in first pass\n");
                		return false;
                	}
                	if (find_symbol(&(file_table->symbolTable), addrs2.opName)->type == EXTERNAL_Sym )
                	{
						
						exsymbol = create_symbol(addrs2.opName, EXTERNAL_Sym, IC);
                    	add_symbol((&(file_table->ExSymbol)), exsymbol);
                 		file_table->code_table[IC].code |= EX_SYM ;
                   		IC++;
                	}
                	else
                	{
						
                		sym = labAdd((&(file_table->symbolTable)), addrs2.opName);
                		file_table->code_table[IC].code = CODE_Sym;
                		file_table->code_table[IC].code |= (sym+LINENUM_INITIAL)<< ARE_SKIP;
                		IC++;
                    	return true;
                	}
                }
                else 
                {
                	IC++;
                return true;
                }

                    }   
        	    }
            }
            else 
            {
            
                  if    (find_symbol(&(file_table->symbolTable),addrs2.opName) == NULL) /* symbol_table*/
                {
              			printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
   	 					printf("Symbol doesnt exist since it hasnt been added in first pass\n");
                		return false;
                }
                 else  if (find_symbol(&(file_table->symbolTable), addrs2.opName)->type == EXTERNAL_Sym )
                 {
					
						exsymbol = create_symbol(addrs2.opName, EXTERNAL_Sym, IC);
                    	add_symbol((&(file_table->ExSymbol)), exsymbol);
                   	    file_table->code_table[IC].code |= EX_SYM ;
                    	IC++;
                 }
                else
                {
					
                   sym = labAdd((&(file_table->symbolTable)), addrs2.opName);
                   file_table->code_table[IC].code = CODE_Sym;
                   file_table->code_table[IC].code |= (sym+LINENUM_INITIAL) << ARE_SKIP;
                   IC++;
                   return true;
                }
            }

        }
              
                return true;
       }
   }
      
    /***************************************************************************/
    /********************** check for 0 operands *******************************/
    for(i=0;i<(sizeof(opCodeValZOp) / sizeof(struct opCode0));i++)
    {
      if (!strcmp(word,opCodeValZOp[i].commandName))
      {      	
           IC++;
           return true;
      }
    }
	printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
	printf("ERROR: The input String is Wrong because of a nonexisting opcode\n");    	
	return false; 
}
/* this functions checks if the ENTRY label is  in the symbol linked list 
and updates the node if so, aftet that will return true . else will return  false*/
bool checkEntry2(info_of_line line_info,char *word,symbol_table *table)
{

	char name[LINELEN];
    symbol* node;
   if ((sscanf(word, "%s",name) == '\n'))
   {
    printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
	printf("ERROR: entry field must contain a legit label\n");  
   return false;
   }
   else
   {
   
    node = *table;
    
    while (node != NULL)
    {
        if (!strcmp(node->name, name)) /* if found, return the pointer */
        {
        node->type = ENTRY_Sym;
        printf("DONE:)");
            return true;
            
        }
        node = node->next;
    }
    
	}
	printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
	printf("ERROR: cant find the entry label on the label table\n");  
	return false; /* if not found, return false */
  }

