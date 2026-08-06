/*************************************************************
 * After reading 2 pages from "Compiler Design in C" I finally got a better understanding about lexer analyzers and parsers (actually, I didn't even know they are different). Due to this rewiring of my brain, I'll try to make a cleaner implementation of the shunting yard algorithm.
**************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*************************************************************
 * Constants
**************************************************************/

#define DATA_SIZE 64
#define MAX_TOKENS_ALLOWED 1024

/*************************************************************
 * Types
**************************************************************/
typedef enum {PLUS, MINUS, TIMES, DIVIDE, TERMINAL, RPARENTHESES, LPARENTHESES} TOKEN;

typedef struct
{
    TOKEN token;
    char * lexeme;
} Data;

// Ok, my new knowledge ends here. I hope it's enough.

// Instead of making a stack of stacks, like I did in the first implementations, I'll use a finite automaton to deal with parentheses and with everything else related to scanning.

typedef struct
{
    int start_position;
    int current_position;
    int isFinished;
    int total_tokens_created
} State;

typedef struct
{
    size_t size;
    size_t capacity;
    Data * data;
} Expression;


/*************************************************************
 * Lexer Analyzer
**************************************************************/

void scan(const char * expression, State * situation)
{

}

Expression * createExpression(size_t capacity)
{
    Expression * expression = malloc(sizeof(*expression));
    if (expression == NULL)
    {
        perror("Fail on create expression.");
        return NULL;
    }
    

    expression->capacity = capacity;
    expression->size = 0;
    expression->data = NULL; // Since each lexeme will have a different size (operators consist of a few characters, but numbers can be arbitrarily long), I'll make an separate function to add Data to the Expression. 

    return expression;
}

int addData(const char * lexeme, TOKEN token, Expression * expression)
{
    Data * data = malloc(sizeof(Data));
    
}