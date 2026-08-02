/*************************************************************
 * This file is just a test for my ideas about the shunting yard algorithm. I'll do it in C first to validate everything I'm thinking, and if it works here, I'll make it in assembly x86-64. Otherwise, I could lose too many hours in assembly making something that doesn't even work.  
**************************************************************/

#include <stdio.h>
#include <stdlib.h>

/*************************************************************
 * Constants
**************************************************************/

#define TOKENS_OUTPUT "0123456789x"
#define TOKENS_OUTPUT_SIZE 11

#define TOKENS_STACK "-+/*"
#define TOKENS_STACK_SIZE 4

#define MAX_SIZE_STACK4STACKS 16
#define MAX_SIZE_STACK 256

/*************************************************************
 * Types
**************************************************************/

typedef struct
{
    size_t max_size;
    size_t size;
    char * elements;
} Stack;

typedef struct 
{
    size_t max_size;
    size_t size;
    Stack * stacks;
} Stack4Stacks;

typedef Stack Output;


/*************************************************************
 * Methods
**************************************************************/

int precedence(char operation)
{
    switch (operation)
    {
    case '-':
        return 1;
    
    case '+':
        return 1;

    case '/':
        return 2;
    
    case '*':
        return 2;
    
    default:
        return -1;
    }
}

int isToken(char character, const char * TOKENS, const int sizeArray)
{
    for (size_t i = 0; i < sizeArray; i++)
    {
        if (character == TOKENS[i])
        {
            return 1;
        }        
    }

    return 0;    
}

void push_element(char character, Stack4Stacks * stack)
{
    Stack * storage = &stack->stacks[stack->size - 1];
    if (storage->size >= storage->max_size)
    {
        printf("Impossible to push. The stack is full. [push_element]");
        return;
    }

    storage->elements[storage->size] = character;
    storage->size++;
}

void send2output(char character, Output * output)
{
    if (output->size >= output->max_size)
    {
        printf("Error: the output reach the maximum size and cannot accept this element for this reason. [send2output]");
        return;
    }

    output->elements[output->size] = character;
    output->size++; 
}

void pop_element(Stack4Stacks * stack, Output * output)
{
    Stack * storage = &stack->stacks[stack->size -1];
    if (output->max_size <= output->size)
    {
        printf("Impossible to pop from the stack. The output reached its maximum. [pop_element]");
        return;
    }

    if (storage->size <= 0)
    {
        printf("Impossible to pop from stack because it is empty. [pop_element]");
        return;
    }    

    send2output(storage->elements[storage->size - 1], output);
    storage->size--;    
}

void pop_all(Stack4Stacks * stack, Output * output)
{
    Stack * storage = &stack->stacks[stack->size - 1];
    while (storage->size > 0)
    {
        pop_element(stack, output);
    }    
}

void pop_until_less_precedence(const char character, Stack4Stacks * stack, Output * output)
{
    Stack * storage = &stack->stacks[stack->size - 1];
    while (storage->size > 0)
    {
        if (precedence(character) >= precedence(storage->elements[storage->size - 1]))
        {
            return;
        }

        pop_element(stack, output);        
    }
}

void destroyStack(Stack * frame)
{
    free(frame->elements);
    free(frame);
}

void destroyStackElements(Stack * frame)
{
    free(frame->elements);
}

Stack4Stacks * createStack4Stacks(size_t max_size)
{
    Stack * elements = malloc(max_size * sizeof(Stack));
    if (elements == NULL)
    {
        perror("Error on create the stack of frames. (malloc) [createStack4Stacks]");
        return NULL;
    }

    Stack4Stacks * stack = malloc(sizeof(Stack4Stacks));
    if (stack == NULL)
    {
        perror("Error on create the stack of frames. (malloc) [createStack4Stacks]");
        return NULL;
    }    

    stack->max_size = max_size;
    stack->size = 0;
    stack->stacks = elements;

    return stack;
}

void destroyStack4Stacks(Stack4Stacks * stack)
{
    for (size_t i = 0; i < stack->size; i++) // in case we want to destroy a stack before finishing the algorithm.
    {
        destroyStackElements(stack->stacks + i); 
    }
    
    free(stack->stacks);
    free(stack);    
}

void pushStack(Stack4Stacks * stack)
{
    if (stack->size >= stack->max_size)
    {
        printf("Impossible to add a new stack frame because the stack is already full. [pushStack]");
        return;
    }

    char * elements = malloc(MAX_SIZE_STACK * sizeof(char));
    if (elements == NULL)
    {
        perror("Error on create the stack. (malloc) [pushStack]");
        return;
    }

    Stack frame = {0};
    frame.elements = elements;
    frame.max_size = MAX_SIZE_STACK;
    frame.size = 0;

    stack->stacks[stack->size] = frame;
    
    stack->size++;
}

void popStack(Stack4Stacks * stack)
{
    if (stack->size <= 0)
    {
        printf("The stack frame is empty. [popStack]");
        return;
    }

    stack->size--;
    destroyStackElements(stack->stacks + stack->size);    
}

Stack4Stacks * initializeStack()
{
    Stack4Stacks * stack = createStack4Stacks(MAX_SIZE_STACK4STACKS);
    if (stack == NULL)
    {
        printf("Error in creating the stack.");
        return NULL;
    }

    char * elements = malloc(MAX_SIZE_STACK * sizeof(char));
    if (elements == NULL)
    {
        perror("Error on create the stack. (malloc) [pushStack]");
        return NULL;
    }

    Stack frame = {0};
    frame.elements = elements;
    frame.max_size = MAX_SIZE_STACK;
    frame.size = 0;

    stack->stacks[stack->size] = frame;    
    stack->size++;

    return stack;
}

Output * initializeOutput()
{
    char * elements = malloc(MAX_SIZE_STACK * sizeof(char));
    if (elements == NULL)
    {
        perror("Error on creating the output (malloc) [initializeOutput]");
        return NULL;
    }

    Output * output = malloc(sizeof(Output));
    
    output->size = 0;
    output->elements = elements;
    output->max_size = MAX_SIZE_STACK;

    return output;
}

void handleChar(char character, Stack4Stacks * stack, Output * output)
{
    if (isToken(character, TOKENS_OUTPUT, TOKENS_OUTPUT_SIZE))
    {
        send2output(character, output);
        return;
    }

    if (isToken(character, TOKENS_STACK, TOKENS_STACK_SIZE))
    {
        send2output(',', output);
        pop_until_less_precedence(character, stack, output);        
        push_element(character, stack);
        return;
    }

    switch (character)
    {
    case '(':
        pushStack(stack);
        return;

    case ')':
        pop_all(stack, output);
        popStack(stack);
        return;
    
    default:
        pop_all(stack, output);
        return;
    }   
}

void parser(const char * expression, size_t length, Stack4Stacks * stack, Output * output)
{
    for (size_t i = 0; i < length; i++)
    {
        handleChar(expression[i], stack, output);
    }   
    
    pop_all(stack, output);
}

/*************************************************************
 * Main
**************************************************************/

int main(int argc, char const *argv[])
{
    Output * output = initializeOutput();
    if (output == NULL)
    {
        return -1;
    }
    

    Stack4Stacks * stack = initializeStack();
    if (stack == NULL)
    {
        return -1;
    }

    const char * expression = "156*7+(3+7*1)";

    parser(expression, 13, stack, output);

    printf("Input: %s\nOutput: %s\n", expression, output->elements);

    return 0;
}
