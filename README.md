# Author: Carlos Daniel Arciniegas Murillas  
**Systems Software. Spring 2023**

---

## Compiler Overview

This document describes a **Parser and Code Generator** for a tiny virtual machine, written in C. The input is code written in **PL/0**, a small programming language with a specific grammar provided by the professor. The instructions for the virtual machine (VM) are displayed in the console and written to `elf.txt`.

### Functionality

- **Lexical Analysis**: The compiler accepts input in PL/0, processes it through a lexical analyzer, and uses a symbol table to tokenize each item in the input.
- **Parsing**: The compiler parses the tokens, producing assembly instructions.
- **Code Generation**: The generated assembly instructions are processed by a small virtual machine.
- **Output**: The instructions are displayed in the console and written to an `elf.txt` file.

### Program Description

This program implements a **top-down parser** that includes procedure call parsing. It reads an input file from the terminal and follows the syntactic rules provided in the pseudocode and homework file. 

- **Error Handling**: The program outputs error messages for syntax errors.
- **Files**: The project includes a header file (`dataTypes.h`) included in `compiler.c` and two different input files (`input1.txt` and `input2.txt`). These inputs contain procedure calls to test the compiler and code generation.
- **Output**: If the input file has no errors, an `elf.txt` file is generated containing instructions without mnemonics. The terminal will display the source code, the instructions with mnemonics, and a message indicating that no errors were found.

### Compilation and Execution Instructions

To compile and run the program, follow these steps:

1. **Compile the Program**:  
   Run the following command to compile the source code from `hw4compiler.c`:

   ```bash  
   gcc -g hw4compiler.c
2. **Run the program**:

    Run the following command to run the program:

    ```bash:
    ./a.out input(#).txt
The program exits safely if an error is found while parsing according to the grammar rules.

Please verify that a new file elf.txt was created after running the program. If not, the compiler outputs to the terminal an error message indicating the error in the input file.
