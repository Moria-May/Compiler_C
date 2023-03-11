#include "proto.h"
#include "dataStructure.h"
#include "first_pass.h"
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
    {1 ,"cmp",3,{IMMEDIATE,DIRECT,DIRECT_REG},3,{IMMEDIATE,DIRECT,DIRECT_REG}},
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









/** Register r0-r7 type */
static char registers[8][3] =
{
    "r0",
    "r1",
    "r2",
    "r3",
    "r4",
    "r5",
    "r6",
    "r7",
  
};
/* a main procedure the first pass!
will go through the line sent from the file and determine 
for each word if its a label , a register , an instruction ..
and will send it to the right functin to keep the coding process */
bool first_pass(info_of_line line_info, pross_table *file_tables)
{	
    char word[LINELEN];
    bool flagForLabel = false;
    char labelName[LINELEN];
    symbol *sym; 
    bool code;
    /*table *file_tables = file_tables;*/
    instruction current_inst;
    printf("Starting First pass\n");
    SKIP_WHITESPACE(line_info.text);/*go to the first place in the line with char*/
    while((sscanf(line_info.text, "%s", word)) != '\n')
    {
        printf("line: %s\n", line_info.text);
        printf("label: %s\n", labelName);
        /*if blank line or comment line. The line is OK */     
		if (blankCheck(line_info,word))
		{
     	   return true;
    	}      
        if( word[0] == ';' )
        {
           return true;
        }
        /* promotes 'line_info.text' to the next unread & not with character */
        line_info.text += (strlen(word));
        SKIP_WHITESPACE(line_info.text);
        printf("\nbefore if setting symbol \n");
        
        
        /*
        ***************************************************************************
        find label
        ***************************************************************************
        */
        if ((*(word + (strlen(word) - 1)) == ':')) /* 3.setting a symbol. skip to the next word */
        {
            printf("we have label\n");
            if(!CheckTheLabelName(word))
            { 
          		printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
        		printf("ERROR:the label name is wrong, return\n");   
                return false;
            }
            else
            {
                printf("The label name is legit\n");
                memcpy(labelName, word, strlen(word) - 1);
                *(labelName + strlen(word) - 1) = 0;/* labelname now have the label name */ 
                flagForLabel = true; 
                printf("\nthe bool is = %d\n", flagForLabel);
                /* promotes 'line_info.text' to the next unread & not white character */
                continue;
            }
        }



        /*
        ***************************************************************************
        find inst
        ***************************************************************************
        */
        if(*word == '.')
        { 
            current_inst = findInst(word+1);
            printf("\nthe inst is = %d\n" , current_inst );
            if((current_inst == ENTRY_INST || current_inst == EXTERN_INST) && !flagForLabel )
            {
                printf("\ninst ENTRY or EXTERN");
                return checkExternAndEntry(current_inst,line_info,&(file_tables->symbolTable));
            }
            else if ((current_inst != NONE_INST))
            {
                printf("inst data or str");
                if(flagForLabel)
                {
                    if(find_symbol(&(file_tables->symbolTable), labelName) != NULL)
                    {
          				printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
        				printf("ERROR:the label name has alreay been defined\n");   
                            return false;
                    }
                    else
                    {
                     
                        printf("\n");
                        sym = create_symbol(labelName, DATA_Sym, DC+IC);
                        add_symbol(&(file_tables->symbolTable), sym);  
                    }
                }
                return checkDataAndStr(line_info, current_inst ,(file_tables->data_table));
            }
                   
            else 
            {
            	printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
        		printf("ERROR:%c operator but no appropriate command\n",'.');
            	return false;
            }
        }


        /*
        ***************************************************************************
        code pross
        ***************************************************************************
        */
        else 
        {

            if(flagForLabel)
            {
                    if(find_symbol(&(file_tables->symbolTable), labelName) != NULL)
                    {
                       printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
        			   printf("ERROR:the label name has alreay been defined\n");   
                       return false;
                    }
                    else
                    {
                        sym = create_symbol(labelName, CODE_Sym, DC+IC);
                        add_symbol(&(file_tables->symbolTable), sym);
                    }
             }
            printf("code pross\n");
            code = codePross(line_info, word, (file_tables->code_table));
            printf("true or false : %d\n", code);
            return code;
        }
    printf("Going to next word\n");
    }
    /*empty line */
	return true;
}

/*
****************************************************************************************
  entry and extern 
****************************************************************************************
*/
/*on the first pass, if the label is entry were returning true; 
if its extern will check the label name, and create a symbol if there are no errors.
the last " return false " is unreachable because we covered all of the possible cases*/
bool checkExternAndEntry(instruction current_inst,info_of_line line_info, symbol_table *table)
{
    char word[LINELEN];
    symbol *sym;
   
    if (current_inst == EXTERN_INST)
    {
        printf("line: %s\n", line_info.text);
        /*check the label name after external*/
        if (!sscanf(line_info.text, "%s", word))
        {
        	printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
        	printf("ERROR:external whitout symbol\n");           
            return false;
        }
        else
        {
            do
            {        
           if(!CheckTheLabelName(word))
           	{
           		     printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
               		 printf("ERROR:the label name is illegal\n");
               		 return false;
         	      }
            else
            {/* if the label doesnt exists yet */
                if(!find_symbol(table, word))
                {
                    sym = create_symbol(word, EXTERNAL_Sym, 0);
                    add_symbol(table, sym);
                    return true;
                }
                else 
                {
                printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
                printf("ERROR:the label already exists\n"); /********************CHECK**************************************************/
                return false; 
                }
            }
            line_info.text += (strlen(word));
            SKIP_WHITESPACE(line_info.text);; 
            } 
            while (sscanf(line_info.text, "%s", word) != '\n');
        }
        
        return true;
    }
    if (current_inst == ENTRY_INST)
    {
   		 return true;
    }
     printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
     printf("ERROR:unreachable ERROR\n");
	return false;
}
/*
****************************************************************************************
  end entry and exter
****************************************************************************************
*/
/*check if the current word sent to the func is a data or a string */
bool checkDataAndStr(info_of_line line_info,instruction current_inst ,table* dataTable)
{
    int temp;
    /*if the symbol is data */
    printf("checkDataAndStr\n");
    if (current_inst == DATA_INST){
        if((temp = readData(line_info,dataTable)) > 0){
            DC += temp;
            print_table(dataTable, DC);
            return true; 
        }
    }
    /*if the symbol is string*/
    else if (current_inst == STRING_INST){
        if((temp = readStr(line_info,dataTable)) > 0){  
            DC += temp; 
            print_table(dataTable, DC);
            return true; 
        }
    }
    printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
    printf("ERROR: Unreachable ERROR");
    return false; 
}
/*coding the string to the data table, and checking for any faulties */
int readStr(info_of_line line_info,table* dataTable)
{	
	int tempDC = 0;
	int ascii;
    if (*(line_info.text) =='"')
  	{
        line_info.text++;
  		while(*(line_info.text) != '"' && *(line_info.text) != '\n' )
  		{	
  			if (isprint(*(line_info.text)))
  			{
                ascii = *(line_info.text);
                dataTable[DC+tempDC].code = ascii;
                dataTable[DC+tempDC].address = DC+tempDC;
                tempDC++; 
                line_info.text++;
  			}
  			else
            {
             	printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
             	printf("ERROR: The input String on line  is not on the ascii table\n");         
  				return 0;
            }
  		}
  			
        printf("The last character is: %c\n", *(line_info.text));
  		if (*(line_info.text) == '"')
  		{	
  		dataTable[DC+tempDC].code = 0; 
  		dataTable[DC+tempDC].address = DC+tempDC;
  		tempDC++;
  		return tempDC;
  		} 
  		else
  		{
  		printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
  		printf("ERROR: The input String is Wrong because it doesnt end with a %c!\n",'"');
  			return 0;
  		}	
  	}	
  	printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
  	printf("ERROR: The input String is Wrong because it doesnt start with a %c!\n",'"');
    return 0;
}
/*coding the data to the data table, and checking for any faulties */
int readData(info_of_line line_info,table* dataTable)
{
    int tempDC = 0;
    char* curNumStr;
    int curNum;
    int len = strlen(line_info.text);
    for (  ;isspace(*(line_info.text)+len) ;len--);
    if (',' == *(line_info.text)+len)
    {
      		printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
      		printf("ERROR: The input String is Wrong because it have an unnecessary");
      		printf(" %c in the end of line\n",',');
    		return 0;
    }
    curNumStr = strtok(line_info.text, ","); 
    while (curNumStr != NULL){
    if (!strcmp(curNumStr,"0"))
    {
    	dataTable[DC+tempDC].code = 0;
        dataTable[DC+tempDC].address = DC+tempDC;
        printf("DC = %d, tempDC = %d\n", DC, tempDC);
    	tempDC++;
   	}
    else
    {
    	curNum = atoi(curNumStr);
    	if (curNum == 0)
    	{
    			printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
    			printf("ERROR: The input String is Wrong because it");
    			printf("contains a nondigit operand\n");
    		return 0;
    	}
    	else
    	{
    		dataTable[DC+tempDC].code = curNum;
            dataTable[DC+tempDC].address = DC+tempDC;
            printf("DC = %d, tempDC = %d\n", DC, tempDC);
    		tempDC++;
   	 	}
    }
    curNumStr = strtok(NULL, ",");
    }
    printf("end checkData\n");
    return tempDC;
}
/*
****************************************************************************************
  end data and str 
****************************************************************************************
*/ 


/*
****************************************************************************************
  symbol 
****************************************************************************************
*/
/*
****************************************************************************************
  data and str 
****************************************************************************************
*/
/* coding the instructions ( such as mov .. jmp .. stop .. etc ) to the instruction table */
bool codePross(info_of_line line_info, char word[LINELEN], table *code_table)
{   
	int orgTokLen;
	int i,j;
	bool found = false;
    int commaCount = 0;
    char* commaCheck;
    char* tok;
    value addrs1;
    value addrs2;
    value addrs3;
    addrs_method address;
    addrs1.adr = NONE_ADDRS;
    addrs2.adr = NONE_ADDRS;
    addrs3.adr = NONE_ADDRS;
    /* check for 2 operands */
    for(i=0;i<(sizeof(opCodeVal2Op) / sizeof(struct opCode2));i++){
        if (!strcmp(word,opCodeVal2Op[i].commandName))
        {
       
            for( commaCheck = line_info.text ; *(commaCheck) != '\0' ; commaCheck++)
            {
                  if (*(commaCheck) == ',')
                  commaCount++;
            }
            printf("%d\n" , commaCount);
            if (commaCount != 1){
            	printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
               	printf("ERROR: The input String is Wrong because it ");
               	printf("have a comma count different then 1\n");
                return false;
            }
            else
            {
                tok = strtok(line_info.text, ",");
                while (tok != NULL)
                {
                    address = addressType(tok);                 
                    if (!found)
                    {
                    
                        for(j=0;j < opCodeVal2Op[i].opSourceLen ;j++)
                        {
                            if (address == opCodeVal2Op[i].opSource[j])
                            {
                            found = true; 
                            addrs1.adr = address;
                            addrs1.opName = tok;        
                            }

                        }
                    }
                   else
                    {
                        for(j=0;j < opCodeVal2Op[i].opDestinLen ;j++)
                        {
                            if (address == opCodeVal2Op[i].opDestination[j])
                            {                       
                            addrs2.adr = address;
                            addrs2.opName = tok;  
                                 
                            }
                        }                      
					}
					tok = strtok(NULL, ",");
                            
                }
                if (strchr(addrs2.opName,'\n'))
                	{
                		*strchr(addrs2.opName,'\n') = '\0';
                	}
                
         printf("address1 is %d\n", addrs1.adr);
         printf("address2 is %d\n", addrs2.adr);
                if (addrs1.adr == NONE_ADDRS || addrs2.adr == NONE_ADDRS)
                { 
                  printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
                  printf("ERROR: The input String is Wrong because of illegal operand types\n");
                    return false;
                 }
                else
                {
                printf("the addrs 1 is: %d\naddrs 1 name : %s\n the addrs 2 is: %d\n addrs 2 name : %s\n" 
                , addrs1.adr,addrs1.opName,addrs2.adr,addrs2.opName);
                kidud2(addrs1,addrs2,i, (code_table)); 
                print_table(code_table,IC);
                return true;
                }
                
            }
        }
    }
    /* check for 1 operand */
    for(i=0;i<(sizeof(opCodeVal1Op) / sizeof(struct opCode1));i++){
        if (!strcmp(word,opCodeVal1Op[i].commandName)){
        
        address = addressType(line_info.text); 
  
       
            for(j=0;j < opCodeVal1Op[i].opDestinLen ;j++)
            {
                if (address == opCodeVal1Op[i].opDestination[j])
                {
                    found = true; 
            	    addrs2.adr = address;
         		    addrs2.opName = line_info.text;
         		
         	    }
            }
            if ( found == false )
            {	
            	printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
                printf("ERROR: The input String is Wrong because of ");
                printf("illegal operand types\n");
                return false;
            }
            if (strchr(addrs2.opName,'\n'))
                	{
                		*strchr(addrs2.opName,'\n') = '\0';
                	}
          
            if (!strcmp(opCodeVal1Op[i].commandName,"jmp") || !strcmp(opCodeVal1Op[i].commandName,"jsr") || !strcmp(opCodeVal1Op[i].commandName,"bne"))
            {
           
        	    if(addrs2.adr == DIRECT)
        	    {  
        		for( ;(*(line_info.text) != '\0') && (*(line_info.text) != '(') ;line_info.text++);
                if (*(line_info.text) == '\0')
                {
                    kidud1(addrs2,i,(code_table));
                    print_table(code_table,IC);
                    return true;
                }
                else if (*(line_info.text) == '(') 
                {
                    found = false; 
                    addrs3.adr=JUMP;
                    tok = strtok((line_info.text+1), ",");
                    while (tok != NULL)
                    {

						
                        if (!found)
                        {      
                            address = addressType(tok);
                                if((address != JUMP) && (address != NONE_ADDRS))
                                {
                                    found = true; 
                                    addrs1.adr = address;
                                    addrs1.opName = tok; 
                                    printf("the addres 1 is : %d\n the opname 1 is: %s\n",addrs1.adr, addrs1.opName);     
                                }
                        }
                        else
                        {
                        	orgTokLen = strlen(tok);
             				if (strchr(tok,')'))
                			{
                				
                				*strchr(tok,')') = '\0';
                				
                			}    
                			if (orgTokLen - 1 != strlen(tok))
                			 {
            				printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
               			    printf("ERROR: redundant characters\n");
                			return false;
       	    				 }
                			
                            address = addressType(tok);
                   
                            if((address != JUMP) && (address != NONE_ADDRS)) 
                            {             
                                addrs2.adr = address;
                                addrs2.opName = tok;
          
                                    printf("the addres 2adr  is : %d\n the opname 2 is: %s\n",addrs2.adr, addrs2.opName);        
                             }
                        }                      
                        tok = strtok(NULL, ",");                
                    }
                        if (addrs1.adr == NONE_ADDRS || addrs2.adr == NONE_ADDRS)
                        {
                        printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
                        printf("ERROR: The input String is Wrong because of ");
                        printf("illegal operand types\n");
                         return false;
                        }
                           
                        else
                        {	
                            kidud_jump(addrs1,addrs2,addrs3,i,(code_table)); 
                            print_table(code_table,IC);
                            return true;
                        }
                    }   
        	    }
            }
         	if (found)
         	{
         	kidud1(addrs2,i,(code_table));
            print_table(code_table,IC); 
            return true; 
         	}
        }
    }  
    for(i=0;i<(sizeof(opCodeValZOp) / sizeof(struct opCode0));i++){
        if (!strcmp(word,opCodeValZOp[i].commandName)){
            code_table[IC].address = IC;
            code_table[IC].code |= (opCodeValZOp[i].commandNumber)<<6;
            IC++;
            SKIP_WHITESPACE(line_info.text);
            
            if (strlen(line_info.text)>1)
            {
            	printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
                printf("ERROR: redundant characters\n");
                return false;
       	    }
            print_table(code_table,IC); 
            return true;
            }
        }
        printf("File->%s:line->%ld:",line_info.file_name,line_info.line_num);
        printf("ERROR: The input String is Wrong because of a nonexisting opcode\n");
        return false;
    }
/* coding the operators of 1 operator instruction type to the instructions table */    
void kidud1(value addrs2,int index, table *code_table)
{
	int i;
    code_table[IC].address = IC;
    code_table[IC].code |= (opCodeVal1Op[index].commandNumber)<<6;
    code_table[IC].code |= (addrs2.adr<<2);
    IC++;
   	if (addrs2.adr == IMMEDIATE)
	{
    code_table[IC].address = IC;
	code_table[IC].code |= atoi(addrs2.opName+1)<<2; 
	IC++;
	}
	else if (addrs2.adr == DIRECT)
	{

    code_table[IC].address = IC;
	IC++;
	}
	else if (addrs2.adr == DIRECT_REG)
	{
	for (i = 0 ;i < REG_NUMBER; i++)
		{printf(" I SHOULD BE HERE" );
		if (!strcmp(addrs2.opName,registers[i]))
			{
                code_table[IC].address = IC;
				code_table[IC].code |= (i<<8);
			}
		}
	IC++;
	}
}	
/* coding the operators of jump operator instruction type to the instructions table */ 
void kidud_jump(value addrs1,value addrs2,value addrs3,int index,  table *code_table){
    int i =0;
    bool regfound = false; /* check if work without it , if does , delete*/
    printf("the IC :%d\n",IC);
    code_table[IC].address = IC;    
	code_table[IC].code |= ((opCodeVal1Op[index].commandNumber)<<6);
    code_table[IC].code |= (addrs3.adr<<2);
	code_table[IC].code |= (addrs1.adr<<12);
	code_table[IC].code |= (addrs2.adr<<10);
    IC++;
    code_table[IC].address = IC; 
    IC++;
    	if ( addrs1.adr == DIRECT_REG && addrs2.adr == DIRECT_REG )
	{ 
        code_table[IC].address = IC;  
		
		code_table[IC].code |= ((opCodeVal2Op[index].commandNumber)<<6);
		
		
		for ( i = 0 ;i < REG_NUMBER && (!regfound); i++)
		{
		if (!strcmp(addrs1.opName,registers[i]))
			{ 
				code_table[IC].code |= (i<<8);
				regfound=true;
			}
		}
		regfound = false;
		for ( i = 0 ;i < REG_NUMBER && (!regfound); i++)
		{
			if (!strcmp(addrs2.opName,registers[i]))
			{ 
				code_table[IC].code |= (i<<2);
				regfound=true;
				IC++;
			}
		}
		return;
	}
    if (addrs1.adr == IMMEDIATE)
	{
    code_table[IC].address = IC;
	code_table[IC].code |= atoi(addrs1.opName+1)<<2; /* twos complement */ 
	IC++;
	}
	else if (addrs1.adr == DIRECT)
	{
	code_table[IC].address = IC;
	IC++;
	}
	else if (addrs1.adr == DIRECT_REG)
	{
	for ( i = 0 ;i < REG_NUMBER && (!regfound); i++)
		{
            printf("%s\n" ,registers[i]);
		if (!strcmp(addrs1.opName,registers[i]))
			{
                code_table[IC].address = IC;
				code_table[IC].code |= (i<<8);
				regfound=true;
                IC++;
			}
		}
	}
	if (addrs2.adr == IMMEDIATE)
	{
    code_table[IC].address = IC;
	code_table[IC].code |= atoi(addrs2.opName+1)<<2; /* twos complement */ 
	IC++;
	}
	else if (addrs2.adr == DIRECT)
	{
    code_table[IC].address = IC;
	IC++;
	}
	else if (addrs2.adr == DIRECT_REG)
	{
	for ( i = 0 ;i < REG_NUMBER ; i++)
		{
		if (!strcmp(addrs2.opName, registers[i]))
			{
                code_table[IC].address = IC; 
				code_table[IC].code |= (i<<2);
				
			}
		}
	IC++;
	}	
			
}
/* coding the operators of the 2 operator instruction type to the instructions table */ 
void kidud2 (value addrs1,value addrs2,int index,  table *code_table)
{
	int i;
	
	if ( addrs1.adr == DIRECT_REG && addrs2.adr == DIRECT_REG )
	{ 
        code_table[IC].address = IC;  
		code_table[IC].code |= (15<<2);
		code_table[IC].code |= ((opCodeVal2Op[index].commandNumber)<<6);
		IC++;
		for ( i = 0 ;i < REG_NUMBER ; i++)
		{
		if (!strcmp(addrs1.opName,registers[i]))
			{ 
				code_table[IC].code |= (i<<8);
				
			}
		}
		
		code_table[IC].address = IC;
		
		
		for ( i = 0 ;i < REG_NUMBER ; i++)
		{	
			if (!strncmp(addrs2.opName,registers[i],2))
			{ 			
				code_table[IC].code |= (i<<2);
				
				IC++;
			}
		}
		
	}
	else 
	{
    code_table[IC].address = IC;    
	code_table[IC].code |= ((opCodeVal2Op[index].commandNumber)<<6);
	code_table[IC].code |= (addrs1.adr<<4);
	code_table[IC].code |= (addrs2.adr<<2);
	IC++;
	if (addrs1.adr == IMMEDIATE)
	{
    code_table[IC].address = IC;
	code_table[IC].code |= atoi(addrs1.opName+1)<<2; /* twos complement */ 
	IC++;
	}
	else if (addrs1.adr == DIRECT)
	{
	code_table[IC].address = IC;
	IC++;
	}
	else if (addrs1.adr == DIRECT_REG)
	{
	for ( i = 0 ;i < REG_NUMBER ; i++)
		{
            printf("%s\n" ,registers[i]);
		if (!strcmp(addrs1.opName,registers[i]))
			{
                code_table[IC].address = IC;
				code_table[IC].code |= (i<<8);
                IC++;
			}
		}
	}
	if (addrs2.adr == IMMEDIATE)
	{
    code_table[IC].address = IC;
	code_table[IC].code |= atoi(addrs2.opName+1)<<2; /* twos complement */ 
	IC++;
	}
	else if (addrs2.adr == DIRECT)
	{
    code_table[IC].address = IC;
	IC++;
	}
	else if (addrs2.adr == DIRECT_REG)
	{
	
	for ( i = 0 ;i < REG_NUMBER ; i++)
		{
		if (!strcmp(addrs2.opName, registers[i]))
			{
                code_table[IC].address = IC; 
				code_table[IC].code |= (i<<8);
				
			}
		}
	IC++;
	}	
			
}}
