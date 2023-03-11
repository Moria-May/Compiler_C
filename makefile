FLAGS = -Wall -pedantic -ansi 
all: main.o macro.o first_pass.o second_pass.o dataStructure.o output.o
	gcc $(FLAGS) main.o macro.o first_pass.o second_pass.o dataStructure.o output.o -o assembler
	
main.o: proto.h main.c macro.h first_pass.h second_pass.h dataStructure.h output.h
	gcc $(FLAGS) main.c -c -o main.o
	
macro.o: proto.h macro.c macro.h
	gcc $(FLAGS) macro.c -c -o macro.o
	
first_pass.o:proto.h first_pass.c first_pass.h  dataStructure.h
	gcc $(FLAGS) first_pass.c -c -o first_pass.o
	
second_pass.o:proto.h second_pass.c second_pass.h  dataStructure.h
	gcc $(FLAGS) second_pass.c -c -o second_pass.o
	
dataStructure.o:proto.h dataStructure.c dataStructure.h	
	gcc $(FLAGS) dataStructure.c -c -o dataStructure.o

output.o:proto.h output.c output.h dataStructure.h
	gcc $(FLAGS) output.c -c -o output.o

clean: 
	rm *.o assembler	
		
