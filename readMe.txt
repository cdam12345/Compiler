Student: Carlos Daniel Arciniegas
UCF NID: ca273903
UCF ID: 4525868
COP 3402 Systems Software
Spring 2023

This program is an implemenetation of a top-down parser that includes procedures call parsing. This program reads in an input file from the terminal, and follows the syntactic rules given in the pseudocode and homeworkfile given. 

This program outputs error messages depending on syntax.

This file homework contains: A header file named dataTypes.h included in the hw4compiler.c, and 2 different inputs named: input1.txt and input2.txt. The inputs have procedure calls to test the compiler and code generation.

If the input file does not have any errors, an elf.txt is generated and it contains instructions without mnemonics. Also, the terminal will display the source code, the instructions using mnemonics, and a message indicating that no errors were found.

To compile and run this program:

Type the command below to generate a file from hw4compiler.c

gcc -g hw4compiler.c

Once the source is compiled, use the command below to feed the program with input1.txt (or input2.txt, or input3.txt, or input4.txt). The input source file will display in the command line along the message "No erros, the program is syntactically correct" if there aren't any errors.

./a.out input(#).txt

The program exits safely if an error is found while parsing.

Please verify that a new file elf.txt was created after running the program.
