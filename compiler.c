// Student: Carlos Daniel Arciniegas Murillas
// UCF NID: ca273903
// UCF ID: 4525868
// COP 3402: Systems Software
// Spring 2023
// Parser and code generator for a tiny VM

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "dataTypes.h"

// Parsing functions
void parsingError(int error);
void emit(int instruction, int level, int m);
int symboltableCheck(char string[], int flag);
void popuSymbolTable(int k, char n[], int v, int l, int m, int mark);
void program();
void block();
void procedure(); // ADDED 
void mark(); // ADDED
void constDeclaration();
int varDeclaration();
void statement();
void condition();
void expression();
void term();
void factor();
void printSymbolTable();
void printCode();
void printSourceCode(int i); // ADDED

// Lexical analyzer functions and variables
static token_type type;
token_type const ERROR = -99;
token_type getTypeWord(char *temp);
void programSymbol();
void addToLexTable(token_type type, char *temp, int v, int err);
void printError(int typeError);

int main(int argc, char** argv)
{

    if (argc < 2) 
    {
        printf("Wrong directives. Exiting savely\n");
        exit(0);
    }

    FILE *ifp = fopen(argv[1], "r");

    char ch;
    int i = 0;
    while (!feof(ifp)) 
    {
        ch = fgetc(ifp);
        if (ch != EOF) 
        {
            myStr[i] = ch;
            i++;
        }
    }

    myStr[i] = '\0';
    myStrIdx = 0;

    fclose(ifp);

    // Main procedure
    popuSymbolTable(3, "main", 0, 0, 3, 1);

    while (myStr[myStrIdx] != '\0') {
        
        // Handles reserved words or variables. Rule: identifier ::= letter{letter|digit}
        if (isalpha(myStr[myStrIdx])) {
            bufferIdx = 0;

            while (isalpha(myStr[myStrIdx]) || isdigit(myStr[myStrIdx])) {
                buffer[bufferIdx] = myStr[myStrIdx];
                myStrIdx++;
                bufferIdx++;

                // detects an error if there is more than 11 characters
                if (bufferIdx >= CMAX) {
                    printError(3);
                    break; // breaks out of local while loop.
                }
            }

            // Getting rid of error by exhaustion 
            if (isalpha(myStr[myStrIdx] || isdigit(myStr[myStrIdx]))) {
                while (isalpha(myStr[myStrIdx] || isdigit(myStr[myStrIdx]))) {
                    myStrIdx++;
                }
            }

            else {
                buffer[bufferIdx] = '\0';
                type = getTypeWord(buffer);
                if (type == ERROR) {
                    continue;
                }

                addToLexTable(type, buffer, 0, 0);
            }
        }

        // Handles numbers. Rule: number::= digit{digit}
        else if (isdigit(myStr[myStrIdx])) {
            bufferIdx = 0;

            while (isdigit(myStr[myStrIdx])) {
                buffer[bufferIdx] = myStr[myStrIdx];
                myStrIdx++;
                bufferIdx++;

                // protecting buffer from number error
                if (bufferIdx >= MAX_NUM_DIGITS) {
                    printError(2);
                    break;
                }
            }

            if (isdigit(myStr[myStrIdx])) {
                // Getting rid of error by exhaustion
                while (isdigit(myStr[myStrIdx]) || isalpha(myStr[myStrIdx])) {
                    myStrIdx++;
                }
            }
            else if (isalpha(myStr[myStrIdx])) {
                // Getting rid of strange letters after number, or missing special symbol
                while (isdigit(myStr[myStrIdx] || isalpha(myStr[myStrIdx]))) {
                    myStrIdx++;
                }
                printError(1);
            }
            else { // if we get here, the number is valid and it's going to be tokenized
                buffer[bufferIdx] = '\0';
                addToLexTable(numsym, buffer, atoi(buffer), 0);
            }
        }

        // Handles Comments or spaces
        else if (isspace(myStr[myStrIdx]) || iscntrl(myStr[myStrIdx])) {
            myStrIdx++;
        }

        else if (myStr[myStrIdx] == '/' && myStr[myStrIdx+1] == '*') {
            while (myStr[myStrIdx] != '*' && myStr[myStrIdx+1] != '/') {
                // If the comment is super long
                if (myStr[myStrIdx] == '\0') {
                    break;
                }
                myStrIdx++;
            }
        }

        // Handles special symbols
        else {
            programSymbol();
        }        
    }   

    printSourceCode(i);
    program();

    FILE *output = fopen("elf.txt", "w");
    for (int j = 0; j < codeIndex; j++)
    {
        fprintf(output, "%d %d %d\n", code[j].instr, code[j].L, code[j].M);
    }

    fclose(output);

    // printf("\n");
    printf("\nNo errors, the program is syntactically correct\n");
    printf("\n");
    return 0;
}

token_type getTypeWord(char *temp)
{
    int foundFlag = 0;
    char *reservedWords[] = { "null", "begin", "call", "const", "do", "else", "end", "if",
                              "odd", "procedure", "read", "then", "var", "while", "write" };

    int wordLength = strlen(temp);

    // Checking error
    if (wordLength >= CMAX - 1) {
        printError(3);
        return ERROR;
    }

    // Returns a token if word in buffer is a reserved word.
    int i;
    for (i = 0; i < NORW; i++) {
        if (strcmp(temp, reservedWords[i]) == 0) {
            foundFlag = 1;
            break;
        }
    }

    if (foundFlag) {
        switch (i) {
            case 0:
                return nulsym;
            
            case 1:
                return beginsym;
            
            case 2:
                return callsym;
            
            case 3:
                return constsym;

            case 4:
                return dosym;
            
            case 5: 
                return elsesym;
            
            case 6:
                return endsym;
            
            case 7:
                return ifsym;
            
            case 8:
                return oddsym;
            
            case 9:
                return procsym;
            
            case 10:
                return readsym;

            case 11:
                return thensym;
            
            case 12:
                return varsym;

            case 13:
                return whilesym;
            
            case 14:
                return writesym;

            default:
                return ERROR;
        }
    }
    
    return idtsym;
}

void programSymbol()
{
    switch (myStr[myStrIdx]) {

        case '+':
            addToLexTable(plussym, "+", 0, 0);
            myStrIdx++;
            break;
        
        case '-':
            addToLexTable(minussym, "-", 0, 0);
            myStrIdx++;
            break;

        case '*':
            addToLexTable(multsym, "*", 0, 0);
            myStrIdx++;
            break;

        case '/':
            addToLexTable(slashsym, "/", 0, 0);
            myStrIdx++;
            break;           
        
        case '=':
            addToLexTable(eqsym, "=", 0, 0);
            myStrIdx++;
            break;

        case '(':
            addToLexTable(lparentsym, "(", 0, 0);
            myStrIdx++;
            break;
        
        case ')':
            addToLexTable(rparentsym, ")", 0, 0);
            myStrIdx++;
            break;
        
        case ',':
            addToLexTable(commasym, ",", 0, 0);
            myStrIdx++;
            break;
        
        case ';':
            addToLexTable(semicolonsym, ";", 0, 0);
            myStrIdx++;
            break;
        
        case '.':
            addToLexTable(periodsym, ".", 0, 0);
            myStrIdx++;
            break;
        
        case '>':
            // the >= case
            myStrIdx++;
            if (myStr[myStrIdx] == '=') {
                addToLexTable(geqsym, ">=", 0, 0);
                myStrIdx++;
            }

            else {
                addToLexTable(gtrsym, ">", 0, 0);                
            }

            break;

        case ':':
            myStrIdx++;
            // the := case
            if (myStr[myStrIdx] != '=') {
                printError(4);
            }

            else {
                addToLexTable(becomessym, ":=", 0, 0);
                myStrIdx++;
                // break;
            }

            break;
        
        case '<':
            myStrIdx++;

            switch (myStr[myStrIdx]) {

                // the <> case
                case '>':
                    addToLexTable(neqsym, "<>", 0, 0);
                    myStrIdx++;
                    break;

                // the <= case
                case '=':
                    addToLexTable(leqsym, "<=", 0, 0);
                    myStrIdx++;
                    break;

                default:
                    if (!isalpha(myStr[myStrIdx]) || !isdigit(myStr[myStrIdx]) || !isspace(myStr[myStrIdx]) || !iscntrl(myStr[myStrIdx])) {
                        addToLexTable(lessym, "<", 0, 0);
                    }
                    else {
                        printError(4);
                    }
            }
            break;
        
        default:
            printError(4); // Invalid symbol
            myStrIdx++;
    }
}

void addToLexTable(token_type type, char *temp, int v, int err)
{
    strcpy(lexTable[lexIdx].name, temp);
    lexTable[lexIdx].tokenType = type;
    lexTable[lexIdx].value = v;
    lexTable[lexIdx].errorCode = err;
    lexIdx++;
}

void popuSymbolTable(int k, char n[], int v, int l, int m, int mark)
{
    symbol_table[symCounter].kind = k;
    strcpy(symbol_table[symCounter].name, n);
    symbol_table[symCounter].val = v;
    symbol_table[symCounter].lexLevel = l;
    symbol_table[symCounter].addr = m;
    symbol_table[symCounter].mark = mark;
    symCounter++;
}

int symboltableCheck(char string[], int flag)
{
    // make sure we are in the closest lex level we are current searching on.
    for (int level = lexicographicalLevel; level >= 0; level--) 
    {
        for (int i = 0; i < symCounter; i++)
         {
            if ( (level == symbol_table[i].lexLevel) && (strcmp(symbol_table[i].name, string) == 0) )
                {
                    return i;
                }
        }

        if (flag)
        {
            break;
        }
    }

    // Returning -1 if no name is found
    // printf("Not Found\n");
    return -1;
}

// compares current token to identifier token. If there is a match get next token
// if token is varsym, check if such var exists in ST. If so, emit.
void factor()
{
    int symIdx = 0;
    if (lexTable[currentToken].tokenType == idtsym)
    {   
        char temp[20];
        strcpy(temp, lexTable[currentToken].name);

        symIdx = symboltableCheck(temp, 0); 

        if (symIdx == -1)
        {
            parsingError(6); // undecleared variable
        }

        // match found
        if (symbol_table[symIdx].kind == 1) // a constant 
        {
            // LIT (M = symol_table[symIdx].value)
            emit(1, lexicographicalLevel, symbol_table[symIdx].val); 
        }

        else // a variable 
        {
            // LOD (M = symbol_table[symIdx].address)
            emit(3, lexicographicalLevel - symbol_table[symIdx].lexLevel, symbol_table[symIdx].addr); 
        }

        currentToken++;
    }

    else if (lexTable[currentToken].tokenType == numsym)
    {
        // emit LIT
        emit(1, lexicographicalLevel, lexTable[currentToken].value);
        currentToken++;
    }

    else if (lexTable[currentToken].tokenType == lparentsym)
    {
        currentToken++;
        expression();

        if (lexTable[currentToken].tokenType != rparentsym) 
        {
            parsingError(13); // right parenthesis must follow left parenthesis
        }

        currentToken++;
    }

    else 
    {
        parsingError(14); // arithmetic equations must contain operands, parentheses, numbers, or symbols
    }
}

void term()
{
    // ignore modsym from pseudocode
    factor(); 
    while (lexTable[currentToken].tokenType == multsym || lexTable[currentToken].tokenType == slashsym)
    {
        if (lexTable[currentToken].tokenType == multsym)
        {
            currentToken++;
            factor();
            // emit(OPR = 2, level =  0, M = 3)
            emit(2, 0, 3);
        }

        else // if (lexTable[currentToken].tokenType == slashsym)
        {
            currentToken++;
            factor();
            // DIV
            emit(2, 0, 4); 
        }
    }
}

void expression()
{
    if (lexTable[currentToken].tokenType == minussym) 
    {
        currentToken++;
        term();
        // emit NEG 

        while (lexTable[currentToken].tokenType == plussym || lexTable[currentToken].tokenType == minussym)
        {
            if (lexTable[currentToken].tokenType == plussym) 
            {
                currentToken++;
                term();
                // ADD
                emit(2, 0, 1);
            }

            else 
            {
                currentToken++;
                term();
                // SUB
                emit(2, 0, 2);
            }
        }
    }

    else 
    {
        if (lexTable[currentToken].tokenType == plussym)
        {
            currentToken++;
        }
        
        term();
        while (lexTable[currentToken].tokenType == plussym || lexTable[currentToken].tokenType == minussym)
        {
            if (lexTable[currentToken].tokenType == plussym) 
            {
                currentToken++;
                term();
                // ADD, m = 1 for addition
                emit(2, 0, 1);
            }

            else 
            {
                currentToken++;
                term();
                // SUB
                emit(2, 0, 2);
            }
        }
    }
}

void condition()
{
    if (lexTable[currentToken].tokenType == oddsym)
    {
        currentToken++;
        expression();
        emit(2, 0, 11);
    }

    else 
    {
        expression();
        if (lexTable[currentToken].tokenType == eqsym) 
        {
            currentToken++;
            expression();
            emit(2, 0, 5);
        }

        else if (lexTable[currentToken].tokenType == neqsym) 
        {
            currentToken++;
            expression();
            emit(2, 0, 6);
        }

        else if (lexTable[currentToken].tokenType == lessym)
        {
            currentToken++;
            expression();
            emit(2, 0, 7);
        }

        else if (lexTable[currentToken].tokenType == leqsym)
        {
            currentToken++;
            expression();
            emit(2, 0, 8);
        }

        else if (lexTable[currentToken].tokenType == gtrsym)
        {
            currentToken++;
            expression();
            emit(2, 0, 9);
        }

        else if (lexTable[currentToken].tokenType == geqsym)
        {
            currentToken++;
            expression();
            emit(2, 0, 10);
        }

        else 
            parsingError(12); // condition must contain comparison operator
    }
}

void statement()
{
    int symIdx = 0;
    char temp[20];

    if (lexTable[currentToken].tokenType == idtsym) 
    {
        strcpy(temp, lexTable[currentToken].name);
        symIdx = symboltableCheck(temp, 0); // checking if name exists in the Symbol table
        if (symIdx == -1) // var not found
        {
            parsingError(6); // undeclared identifier
        }

        if (symbol_table[symIdx].kind != 2) // not a var
        {
            parsingError(7); // Only variable values may be altered
        }

        currentToken++;
        if (lexTable[currentToken].tokenType != becomessym) 
        {
            parsingError(8); // Assignment statements must use :=
        }

        currentToken++;
        expression();
        // STO, L = level - symbol_table[symIdx].lexLevel, M = symbol_table[symIdx].address
        emit(4, lexicographicalLevel - symbol_table[symIdx].lexLevel, symbol_table[symIdx].addr); 
        return;
    }

    // ADD CALL according to grammar in appendix B. Finish this block with return
    if (lexTable[currentToken].tokenType == callsym)
    {
        currentToken++;
        if (lexTable[currentToken].tokenType != idtsym)
        {
            // ERROR: call must be followed by an identifier
            parsingError(16);
        }

        // check in symbol table
        else
        {
            strcpy(temp, lexTable[currentToken].name);
            symIdx = symboltableCheck(temp, 0);
            if (symIdx == -1) // means undeclared procedure
            {
                // ERROR. Undeclared identifier
                parsingError(6);
            }

            else 
            {
                if (symbol_table[symIdx].kind == 3) // procedure
                { 
                    // emit(cal, lev-table[symIdx].level, table[symIdx].adr) 
                    emit(5, lexicographicalLevel - symbol_table[symIdx].lexLevel, symbol_table[symIdx].addr);
                }

                else
                {
                    // ERROR call of a constant or variable is meaningless
                    parsingError(17);
                }

            currentToken++;
            }
        }
        return;
    }

    if (lexTable[currentToken].tokenType == beginsym) 
    {
        currentToken++;
        statement();
        while (lexTable[currentToken].tokenType == semicolonsym) // || lexTable[currentToken].tokenType == commasym or comma 
        {
            currentToken++;
            statement();
        }

        if (lexTable[currentToken].tokenType != endsym)
        {
            parsingError(9); //begin must be followed by end
        }

        currentToken++;
        return;
    }

    // Generating code for if-then statements
    if (lexTable[currentToken].tokenType == ifsym) 
    {
        currentToken++;
        condition();
        jpcIdx = codeIndex; //current code index

        emit(8, lexicographicalLevel, symbol_table[jpcIdx].addr); // dummy jump on condition instruction

        if (lexTable[currentToken].tokenType != thensym)
        {
            parsingError(10); // if must be follows by then
        }

        currentToken++;
        statement();
        code[jpcIdx].M = codeIndex;
        return;
    }

    if (lexTable[currentToken].tokenType == whilesym)
    {
        currentToken++;
        loopIdx = codeIndex; 
        condition();

        if (lexTable[currentToken].tokenType != dosym) 
        {
            parsingError(11); // while must be followed by do
        }

        currentToken++;
        jpcIdx = codeIndex;

        // JPC
        emit(8, lexicographicalLevel, symbol_table[jpcIdx].addr);
        statement();

        // JMP (M = loopIdx)
        emit(7, lexicographicalLevel, loopIdx); 
        code[jpcIdx].M = codeIndex;
        return;
    }

    if (lexTable[currentToken].tokenType == readsym)
    {
        currentToken++;
        if (lexTable[currentToken].tokenType != idtsym) 
        {
            parsingError(15); // read must be followed by identifier
        }

        char tempName[20];
        strcpy(tempName, lexTable[currentToken].name);
        symIdx = symboltableCheck(tempName, 0);

        if (symIdx == -1) 
        {
            parsingError(6); // undecleared identifier
        }

        if (symbol_table[symIdx].kind != 2) // not a var
        {
            parsingError(2); // symbol name has already been declared
        }

        currentToken++;
        // READ ============ Input
        //   SYS 0 2
        emit(9, 0, 2);

        // STO (M = symbol_table[symIdx].addr)
        emit(4, lexicographicalLevel - symbol_table[symIdx].lexLevel, symbol_table[symIdx].addr); 
        return;
    }

    if (lexTable[currentToken].tokenType == writesym)
    {
        currentToken++;
        expression();
        //   SYS 0 1 ===== Output
        emit(9, 0, 1);
        return;
    }
}

int varDeclaration()
{
    char temp[20];
    int numVars = 0;
    if (lexTable[currentToken].tokenType == varsym) 
    {
        do 
        {
            numVars++;
            currentToken++;
            if (lexTable[currentToken].tokenType != idtsym) 
            {
                parsingError(6); // undeclared identifier
            }

            strcpy(temp, lexTable[currentToken].name); // if the identifier is found, error
            if (symboltableCheck(temp, 1) != -1) // var name found 
            {
                parsingError(6); // undeclared identifier
            }
 
            popuSymbolTable(2, temp, 0, lexicographicalLevel, numVars+2, 0); // add mark = 0 after numVars
            currentToken++;

        } while (lexTable[currentToken].tokenType == commasym);

        if (lexTable[currentToken].tokenType != semicolonsym) 
        {
            parsingError(5); // const and var's declarations must be followed by a semicolon
        }

        currentToken++;
    }
    return numVars;
}

void constDeclaration()
{
    char temp[20];
    if (lexTable[currentToken].tokenType == constsym) 
    {
        do 
        {
            currentToken++;
            if (lexTable[currentToken].tokenType != idtsym) 
            {
                parsingError(1); // consts must be followed by an identifier
            }

            strcpy(temp, lexTable[currentToken].name);

            if (symboltableCheck(temp, 0) != -1)
            {
                parsingError(2); // symbol name has already been declared
            }

            currentToken++;
            if (lexTable[currentToken].tokenType != eqsym) 
            {
                parsingError(3); // constants must be assigned with =
            }

            currentToken++;
            if (lexTable[currentToken].tokenType != numsym) 
            {
                parsingError(4); // constants must be assigned an integer value
            }

            popuSymbolTable(1, temp, lexTable[lexIdx].value, 0, 0, 0);
            currentToken++;

        } while (lexTable[currentToken].tokenType == commasym); 

        if (lexTable[currentToken].tokenType != semicolonsym) 
        {
            parsingError(5); // const and var's declarations must be followed by a semicolon
        }

        currentToken++;
    }
}

// Handles procedure token types
void procedure()
{
    while (lexTable[currentToken].tokenType == procsym)
    {
        char temp[20];
        currentToken++;
        if (lexTable[currentToken].tokenType == idtsym)
        {
            strcpy(temp, lexTable[currentToken].name);
            popuSymbolTable(3, temp, 0, lexicographicalLevel, 0, 0);
            currentToken++;
        }
        else 
        {
            // ERROR: procedure must be followed by an identifier
            parsingError(1);
        }

        if (lexTable[currentToken].tokenType == semicolonsym)
            currentToken++;
        else //ERROR: missing a semmicolon
            parsingError(5);

        block();

        if (lexTable[currentToken].tokenType == semicolonsym)
            currentToken++;

        else 
        {
            // ERROR: missing a semmicolon
            parsingError(5);
        }
    }
    // printf("\nBefore statement()\n");
    statement(); 
} 

void block()
{
    // increasing level everytime we call block()
    lexicographicalLevel++;
    // dx = 3. // Data allocation index. We have 3 elements for each AR
    // DL, SL, RA + num of variables.
    // dataSpace is unique because each function has its own AR and var's
    int dataSpace, tempCodeIndex, tempSymTableIndex;
    dataSpace = 3;

    tempSymTableIndex = symCounter - 1;
    tempCodeIndex = codeIndex;
    emit(7, 0, 0); //JMP random value for jump address.
    
    if (lexTable[currentToken].tokenType == constsym)
    {
        constDeclaration();
    }

    if (lexTable[currentToken].tokenType == varsym)
    {
        numVars = varDeclaration();
    }

    if (lexTable[currentToken].tokenType == procsym)
    {
        procedure();
    }

    // fixing the address
    code[tempCodeIndex].M = codeIndex*3; // codeIndex
    symbol_table[tempSymTableIndex].addr = codeIndex*3; //codeIndex

    // emit() INC
    emit(6, 0, dataSpace + numVars);
    statement();
    mark();
    lexicographicalLevel--; 

    // emit() RTN
    // emit(2, 0, 0); test after
}

void program()
{
    block();

    if (lexTable[currentToken].tokenType != periodsym) 
    {
        parsingError(0); // "Error: <Program must end with period>"
    }

    // HALT
    emit(9, 0, 3);
    printCode();
    // printSymbolTable();

}

void emit(int instruction, int level, int m)
{
    code[codeIndex].instr = instruction;
    code[codeIndex].L = level;
    code[codeIndex].M = m;   
    codeIndex++;       
}

void mark()
{
    for (int i = symCounter - 1; i >= 0; i--)
    {
        if (symbol_table[i].mark == 1)
            continue;
        if (symbol_table[i].lexLevel < lexicographicalLevel)
            return;

        symbol_table[i].mark = 1;
    }
}

void printSourceCode(int i)
{
    printf("\nInput source code:\n");
    for (int j = 0; j < i; j++) 
    {
        printf("%c", myStr[j]);
    }
}

void printCode()
{
    printf("\n\nInstructions:\n\n");
    printf("  OP\tL\tM\n");

    for (int i = 0; i < codeIndex; i++) 
    {
        printf("%d ", i);
        switch (code[i].instr)
        {
            case 1:
                printf("LIT"); // prints opCode 
                break;

            case 2:
                printf("OPR"); // prints opCode 
                break;

            case 3:
                printf("LOD"); // prints opCode 
                break;

            case 4:
                printf("STO"); // prints opCode 
                break;

            case 5:
                printf("CAL"); // prints opCode 
                break;
                
            case 6:
                printf("INC"); // prints opCode 
                break;

            case 7:
                printf("JMP"); // prints opCode 
                break;
            case 8:
                printf("JPC"); // prints opCode 
                break;
            case 9:
                printf("SYS"); // prints opCode 
                break;
            default:
                printf("DEFAULT ISA");
        }

        printf("\t%d\t%d\n", code[i].L, code[i].M);

    }

    printf("\n");
}

void printSymbolTable()
{
    printf("Symbol Table:\n\n");
    printf("Kind | Name      | Value | Level | Address |  Mark\n");
    printf("--------------------------------------------------\n");

    for(int i = 0; i < symCounter; i++)
    {
        printf("%4d | %9s | %5d | %4d  | %5d   | %5d\n", symbol_table[i].kind, symbol_table[i].name, symbol_table[i].val, symbol_table[i].lexLevel, symbol_table[i].addr, symbol_table[i].mark);
    }
}

void printError(int typeError)
{
    if (typeError == 1) {
        printf("Variable does not start with a letter.\n");
        return;
    }

    if (typeError == 2) {
        printf("Number too long.\n");
        return;
    }

    if (typeError == 3) {
        printf("Name too long.\n");
        return;
    }

    if (typeError == 4) {
        // printf("Here. Type error is 4\n");
        printf("Invalid symbols.\n");
        return;
    }    
}

void parsingError(int error)
{
    printf("\n");
    if (error == 0) 
    {
        printf("\nError number %d: program must end with period.\n", error);
    }

    else if (error == 1) 
    {
        printf("\nError number %d: const, var, and procedure keywords must be followed by identifier.\n", error);
    }

    else if (error == 2) 
    {
        printf("\nError number %d: symbol name has already been declared.\n", error);
    }

    else if (error == 3) 
    {
        printf("\nError number %d: constants must be assigned with =.\n", error);
    }

    else if (error == 4) 
    {
        printf("\nError number %d: constants must be assigned an integer value.\n", error);
    }

    else if (error == 5) 
    {
        printf("\nError number %d: semicolon or comma missing.\n", error);
    }

    else if (error == 6) 
    {
        printf("\nError number %d: undeclared identifier %s.\n", error, lexTable[currentToken].name);
    }

    else if (error == 7) 
    {
        printf("\nError number %d: only variable values may be altered.\n", error);
    }

    else if (error == 8) 
    {
        printf("\nError number %d: assignment statements must use :=.\n", error);
    }

    else if (error == 9) 
    {
        printf("\nError number %d: begin must be followed by end.\n", error);
    }

    else if (error == 10) 
    {
        printf("\nError number %d: if must be followed by then.\n", error);
    }

    else if (error == 11) 
    {
        printf("\nError number %d: while must be followed by do.\n", error);
    }

    else if (error == 12) 
    {
        printf("\nError number %d: condition must contain commparison operator.\n", error);

    }

    else if (error == 13) 
    {
        printf("\nError number %d: right parenthesis must follow left parenthesis.\n", error);
    }

    else if (error == 14)
    {
        printf("\nError number %d: arithmetic equations must contain operands, parenthesis, numbers, or symbols.\n", error);
    }

    else if (error == 15)
    {
        printf("\nError number %d: read must be followed by identifier.\n", error);
    }

    else if (error == 16)
    {
        printf("\nError number %d: call must be followed by identifier.\n", error);
    }

    else if (error == 17)
    {
        printf("\nError number %d: call of a constant or variable is meaningless.\n", error);
    }

    else 
    {
        printf("\nDefault error\n");
    }

    exit(0);
}
