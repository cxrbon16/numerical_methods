#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

#define MAX_STACK_SIZE 1000

/*
Defining stack and relevant functions.
*/

typedef struct
{
    char item[MAX_STACK_SIZE][10];
    int top;
} Stack;

void initializeStack(Stack *s)
{
    s->top = -1;
}

void push(Stack *s, const char *val)
{

    if (s->top != MAX_STACK_SIZE - 1)
    {
        s->top++;
        strcpy(s->item[s->top], val);
    }
    else
    {
        printf("Stack overflow!\n");
    }
}

char *pop(Stack *s)
{
    char *returnVal = NULL;
    if (s->top != -1)
    {
        returnVal = s->item[s->top];
        s->top--;
    }
    else
    {
        printf("stack boş.!\n");
    }
    return returnVal;
}


/*
infix to postfix by shunting yard algorithm.
*/

char *operators = "-+*/^sincostancotseccosecarcsinarccosarctanarccotlnlogsqrtmaxmin";

char** infix_to_postfix(char *input)
{
    /*
    THE SHUNTING YARD ALGORITHM
    https://en.wikipedia.org/wiki/Shunting_yard_algorithm
    */
    Stack *operator_stack = malloc(sizeof(Stack));
    initializeStack(operator_stack);
    char** output = (char**)malloc(50 * sizeof(char*)); // allocating memory for an array of strings.
    char *substring = (char *)malloc(sizeof(char) * 6);
    char *number = (char *)malloc(sizeof(char) * 10);
    char temp[10] = "\0";

    int i = 0, j = 0, output_index = 0, substring_index = 0;
    char token;
    for (i = 0; i < strlen(input); i++)
    {
        token = input[i];
        if (isdigit(token))
        {
            for (j = 0; j < 10; j++)
            {
                number[j] = 0;
            }
            j = 0;
            while (isdigit(input[i]) || input[i] == '.' || input[i] == ',')
            {
                number[j++] = input[i++];
            }
            i--;
            output[output_index++] = strdup(number);
        }
	else if(isspace(token)){
		continue;
	}
        else if (token == '(')
        {
            temp[0] = token;
            push(operator_stack, temp);
            temp[0] = '\0';
        }
        else if (token == ')')
        {

            while (operator_stack->top != 0 && operator_stack->item[operator_stack->top][0] != '(')
            {
                output[output_index++] = strdup(pop(operator_stack));
            }
            pop(operator_stack);
        }
        else if (isalpha(token))
        {
            substring[substring_index] = token;
            substring_index++;
            if (substring_index > 2)
            {

                if (strstr(operators, substring) != NULL)
                {
                    push(operator_stack, substring);
                    strcpy(substring, "");
                    substring_index = 0;
                }
            }
        }
        else
        {
	    temp[0] = token;
            while (operator_stack->top != -1 && strstr(operators, temp) < strstr(operators, operator_stack->item[operator_stack->top]))
            {
                // stack'in son elemanı token'ın öncelik sırasından yüksekse bu elemanı outputa atıyoruz.
                output[output_index++] = strdup(pop(operator_stack));
            }
                        push(operator_stack, temp);
            temp[0] = '\0';
        }

    }
    while (operator_stack->top != -1)
    {
        output[output_index++] = strdup(pop(operator_stack));
    }
    for (j = 0; j < output_index; j++){
        printf("%s ", output[j]);
    }
    return output;
}



char* eval_postfix(char** postfix){
    Stack* number_stack = malloc(sizeof(Stack));
    initializeStack(number_stack);
    char *Token, *to_push, *eptr;
    to_push = malloc(sizeof(char) * 20);
    double val, params[2]; //params[0] will be the right and params[1] will be the left scalars.
    int i = 0;
    while(postfix[i] != NULL){
	if(strstr(operators, postfix[i]) != NULL){
		if(postfix[i][0] == '+'){
			params[0] = strtod(pop(number_stack), &eptr);
			params[1] = strtod(pop(number_stack), &eptr);
			val = params[0] + params[1];
			sprintf(to_push, "%f", val);
			push(number_stack, to_push);
		}
		else if(postfix[i][0] == '-'){
			params[0] = strtod(pop(number_stack), &eptr);
			params[1] = strtod(pop(number_stack), &eptr);
			val = params[1] - params[0];
			sprintf(to_push, "%f", val);
			push(number_stack, to_push);
		}
		else if(postfix[i][0] == '/'){
			params[0] = strtod(pop(number_stack), &eptr);
			params[1] = strtod(pop(number_stack), &eptr);
			val = params[1] /  params[0];
			sprintf(to_push, "%f", val);
			push(number_stack, to_push);
		}
		else if(postfix[i][0] == '*'){
			params[0] = strtod(pop(number_stack), &eptr);
			params[1] = strtod(pop(number_stack), &eptr);
			val = params[0] * params[1];
			sprintf(to_push, "%f", val);
			push(number_stack, to_push);
		}
		else if(postfix[i][0] == '^'){
			params[0] = strtod(pop(number_stack), &eptr);
			params[1] = strtod(pop(number_stack), &eptr);
			val = pow(params[1], params[0]);
			sprintf(to_push, "%f", val);
			push(number_stack, to_push);
		}
		else if(strcmp(postfix[i], "sin") == 0) {
			params[0] = strtod(pop(number_stack), &eptr);
			printf("%f", params[0]);
			val = sin(params[0]);
			sprintf(to_push, "%f", val);
			push(number_stack, to_push);
		}
		else if(strcmp(postfix[i], "cos") == 0) {
			params[0] = strtod(pop(number_stack), &eptr);
			printf("%f", params[0]);
			val = cos(params[0]);
			sprintf(to_push, "%f", val);
			push(number_stack, to_push);
		}
		else if(strcmp(postfix[i], "tan") == 0) {
			params[0] = strtod(pop(number_stack), &eptr);
			printf("%f", params[0]);
			val = tan(params[0]);
			sprintf(to_push, "%f", val);
			push(number_stack, to_push);
		}
		else if(strcmp(postfix[i], "sec") == 0) {
			params[0] = strtod(pop(number_stack), &eptr);
			printf("%f", params[0]);
			val = 1/cos(params[0]);
			sprintf(to_push, "%f", val);
			push(number_stack, to_push);
		}
		else if(strcmp(postfix[i], "cosec") == 0) {
			params[0] = strtod(pop(number_stack), &eptr);
			printf("%f", params[0]);
			val = 1/sin(params[0]);
			sprintf(to_push, "%f", val);
			push(number_stack, to_push);
		}
		else if(strcmp(postfix[i], "cosec") == 0) {
			params[0] = strtod(pop(number_stack), &eptr);
			printf("%f", params[0]);
			val = 1/sin(params[0]);
			sprintf(to_push, "%f", val);
			push(number_stack, to_push);
		}
		else if (strcmp(postfix[i], "log") == 0){
			params[0] = strtod(pop(number_stack), &eptr);
			params[1] = strtod(pop(number_stack), &eptr);
			val = log(params[0]) / log(params[1]);
			sprintf(to_push, "%f", val);
			push(number_stack, to_push);
		}
		
			
	}
	else{
	     push(number_stack, postfix[i]);
	}
	i++;
    }
    
    
    return pop(number_stack);
	

	
}

int main()
{
    char** postfix;
    char direct_input[100];
    fgets(direct_input, 100, stdin);
    postfix = infix_to_postfix(direct_input); 
    printf("\n%s", eval_postfix(postfix));
    char* s;
    scanf("%s", s);
    return 0;


}
