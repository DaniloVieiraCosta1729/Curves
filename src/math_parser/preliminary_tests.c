/*************************************************************
 * This file is just a test for my ideas about the shunting yard algorithm. I'll do it in C first to validate everything I'm thinking, and if it works here, I'll make it in assembly x86-64. Otherwise, I could lose too many hours in assembly making something that doesn't even work.  
**************************************************************/

#include <stdio.h>
#include <stdlib.h>

/*************************************************************
 * Constants
**************************************************************/

#define TOKENS_OUTPUT "0123456789x."
#define TOKENS_OUTPUT_SIZE 12

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
        send2output(' ', output);
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

// my slow memset (I will going to search about vector registers for x86-64 and then make a nicer version in assembly)
void my_memset(void * target, char filler, size_t size)
{
    size_t toRepeat = 0;

    if (size >= 8)
    {
        size_t times = size / sizeof(size_t);
        char * trick1 = (char *)&toRepeat;
        for (size_t i = 0; i < sizeof(size_t); i++)
        {
            trick1[i] = filler;
        }

        size_t * trick2 = (size_t *)target;
        for (size_t i = 0; i < times; i++)
        {
            trick2[i] = toRepeat;
        }

        int sobreposition = sizeof(size_t);
        trick2 = (size_t *)((char *)target + size - sobreposition);
        *trick2 = toRepeat;        
        
        return;
    } // I suspect it's awful for performance and readability, but, since it's only a hobby, I'll choose the implementation that makes me more motivated. In a professional environment, what moves me more is making something reliable with a good performance for the specific case and reaching the deadline (without sacrificing the safety for the deadline, though).

    char * result = (char *)target;

    for (size_t i = 0; i < size; i++)
    {
        result[i] = filler;
    }   
    
}

// expects the buffer to have a size bigger than the length passed as parameter.
void copyString(char * destine, const char * origin, size_t length) // I want to avoid the string.h in this parser, because It's the first time I make this type of parser.
{
    my_memset(destine, 0, length);

    for (size_t i = 0; i < length; i++)
    {
        destine[i] = origin[i];
        if (origin[i] == '\0')
        {
            return;
        }        
    }

    destine[length] = 0;
}

/*************************************************************
 * Main
**************************************************************/

int main(int argc, char const *argv[])
{
    if (argc > 2)
    {
        printf("Usage: %s <expression without spaces>", argv[0]);
        return -1;
    }
    
    char * expression = "156*7+(3+7*1)";
    char text[256] = {0}; 

    copyString(text, expression, 255);

    if (argc == 2)
    {
        copyString(text, argv[1], 255);
    }
    

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


    parser(text, 13, stack, output);

    printf("Input: %s\nOutput: %s\n", text, output->elements);

    return 0;
}
