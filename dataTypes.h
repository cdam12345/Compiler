// This file contains all the sizes, and data types for the scanner and the parser.

#define NORW 15 // num of reserved words.
#define CMAX 12 // Max num of chars for identifiers is 11 but + 1 for the '\0'.
#define MAX_INPUT_FILE 350 // Size of input stream.
#define MAX_TOKENS 300
#define MAX_NUM_DIGITS 5 // numbers must be at most 5 characters long + 1 for the null character.
#define BUFFER_SIZE 12
#define MAX_SYMBOL_TABLE_SIZE 100
#define MAX_NUM_CODE 200

// tokens
typedef enum {
    nulsym = 1, idtsym, numsym, plussym, minussym,
    multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
    gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
    periodsym, becomessym, beginsym, endsym, ifsym, thensym, 
    whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
    readsym, elsesym } token_type;

// Structure for Lexeme table
typedef struct lexeme { // struct that stores relevant data for each lexeme.

    char name[CMAX];        // store here the name of identifiers.
    int value;
    int errorCode;          // 0 is no error, else code refers to errorType.
    token_type tokenType;

} lexeme;

// Structure for Symbol table
typedef struct symbol 
{
    int kind;               // conts = 1, var = 2, procedure = 3.
    char name[CMAX];        // name up to 11 chars.
    int val;                // number.
    int lexLevel;           // Lexicographical level.
    int addr;               // M address.
    int mark;               // mark = 1 if symbol is retrieved.

} symbol;

// Structure for Instructions
typedef struct opCode
{
    int instr;              // instruction 
    int L;                  // L
    int M;                  // M

} opCode;

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
int symCounter = 0;

opCode code[MAX_NUM_CODE];
int codeIndex = 0;

lexeme lexTable[MAX_TOKENS];
int lexIdx = 0;

char myStr[MAX_INPUT_FILE];
int myStrIdx = 0;
 
char buffer[BUFFER_SIZE];
int bufferIdx = 0;

int lexicographicalLevel = -1; // starting at -1 for consistency
int jpcIdx = 0;
int loopIdx = 0;
int currentToken = 0;
int numVars;