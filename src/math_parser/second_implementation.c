/*************************************************************
 * After reading 2 pages from "Compiler Design in C" I finally got a better understanding about lexer analyzers and parsers (actually, I didn't even know they are different). Due to this rewiring of my brain, I'll try to make a cleaner implementation of the shunting yard algorithm.
**************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*************************************************************
 * Macros
**************************************************************/

#define ERROR_NULL(ptr, name) do{\
    if (ptr == NULL)\
    {\
        perror(name);\
    }\
} while (0) 
   

/*************************************************************
 * Constants and Types
**************************************************************/

#define DATA_SIZE 64
#define MAX_TOKENS_ALLOWED 1024

typedef enum {PLUS = 2, MINUS, TIMES, DIVIDE, NUMBER, VARIABLE, RPARENTHESES, LPARENTHESES} TOKEN;

const char is_alphabet[256] = {
    [0] = 1,
    [10] = 1,
    ['x'] = VARIABLE, ['y'] = VARIABLE, ['z'] = VARIABLE,
    ['('] = RPARENTHESES, [')'] = LPARENTHESES,
    ['+'] = PLUS, ['-'] = MINUS, ['*'] = TIMES, ['/'] = DIVIDE
}; // this is a nice way to see if a character is in our alphabet or not. I'll put all the letters in the final implementation.

typedef struct
{
    TOKEN token;
    char * lexeme;
} Data;

typedef enum {READY_LEXER, VARIABLE_LEXER, DIGIT_LEXER, NUMBER_LEXER, ERROR_LEXER, ACCEPT_LEXER, END_LEXER} Lexer_State;

typedef struct
{
    size_t size;
    size_t capacity;
    Data * data;
} Expression;


/*************************************************************
 * Lexer Analyzer
**************************************************************/


Expression * createExpression(size_t capacity)
{
    Expression * expression = malloc(sizeof(*expression));
    ERROR_NULL(expression, "Fail on create expression.");
    
    Data * data = malloc(capacity * sizeof(Data));
    ERROR_NULL(data, "Fail on create expression.");
    
    expression->capacity = capacity;
    expression->size = 0;
    expression->data = NULL; // Since each lexeme will have a different size (operators consist of a few characters, but numbers can be arbitrarily long), I'll make an separate function to add Data to the Expression. 
    
    return expression;
}

void DestroyExpression(Expression * expression) // I know it's a pretty bad to manage memory this way. I'll make an arena later to replace this.
{
    for (size_t i = 0; i < expression->size; i++)
    {
        free(expression->data[i].lexeme);
    }
    
    free(expression->data);
    free(expression);
}

int addData(const char * lexeme, TOKEN token, Expression * expression)
{
    if (expression->size >= expression->capacity)
    {
        printf("It is impossible to add the new data to the expression because the expression has reached the maximum size.");
        return -1;
    }
    
    int lexeme_size = strlen(lexeme) + 1;
    char * lxm = malloc(lexeme_size);
    
    strncpy(lxm, lexeme, lexeme_size);
    lxm[lexeme_size] = 0;
    
    expression->data[expression->size].lexeme = lxm;
    expression->data[expression->size].token = token;
    expression->size++;
    
    return 0;
}

// transition functions to the lexer fsm
void deltaReady();
void deltaVariable();
void deltaDigit();
void deltaNumber();
void deltaAccept();
void deltaError();

Expression * scan(const char * expression, size_t length)
{
}