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
	char item[MAX_STACK_SIZE][50];
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

	return returnVal;
}

/*
infix to postfix by shunting yard algorithm.
*/

char *operators = "-+*/^sincostancotseccosecarcsinarccosarctanarccotlnlogsqrtmaxmin";

char **infix_to_postfix(char *input)
{
	/*
	THE SHUNTING YARD ALGORITHM
	https://en.wikipedia.org/wiki/Shunting_yard_algorithm
	*/
	Stack *operator_stack = malloc(sizeof(Stack));
	initializeStack(operator_stack);
	char **output = (char **)malloc(50 * sizeof(char *)); // allocating memory for an array of strings.
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
		else if (isspace(token))
		{
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

		else if (token == 'x' || token == 'X' || token == 'e' || token == 'E' || token == 'p' || token == 'P')
		{
			for (j = 0; j < 10; j++)
			{
				number[j] = 0;
			}
			number[0] = token;
			output[output_index++] = strdup(number);
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
  
  /*for(i = 0; i < output_index; i++){
    printf("%s\n", output[i]);
  }*/
  
	return output;
}

char *eval_postfix(char **postfix, char *x_val)
{
	Stack *number_stack = malloc(sizeof(Stack));
	initializeStack(number_stack);
	char *Token, *to_push, *eptr;
	to_push = malloc(sizeof(char) * 20);
	double val, params[2]; // params[0] will be the right and params[1] will be the left scalars.
	int i = 0, isarc = 0;
	while (postfix[i] != NULL)
	{
		if (strcmp(postfix[i], "x") == 0 || strcmp(postfix[i], "X") == 0)
		{
			push(number_stack, x_val);
		}
		else if (strcmp(postfix[i], "e") == 0 || strcmp(postfix[i], "E") == 0)
		{
			push(number_stack, "2.7182818284");
		}
		else if (strcmp(postfix[i], "p") == 0 || strcmp(postfix[i], "P") == 0)
		{
			push(number_stack, "3.1415926535");
		}
		else if (strstr(operators, postfix[i]) != NULL)
		{
			if (postfix[i][0] == '+')
			{
				params[0] = strtod(pop(number_stack), &eptr);
				if (number_stack->top != -1)
				{
					params[1] = strtod(pop(number_stack), &eptr);
				}
				else
				{
					params[1] = 0.0;
				}
				val = params[0] + params[1];
				sprintf(to_push, "%f", val);
				push(number_stack, to_push);
			}
			else if (postfix[i][0] == '-')
			{
				params[0] = strtod(pop(number_stack), &eptr);
				if (number_stack->top != -1)
				{
					params[1] = strtod(pop(number_stack), &eptr);
				}
				else
				{
					params[1] = 0.0;
				}

				val = params[1] - params[0];
				sprintf(to_push, "%f", val);
				push(number_stack, to_push);
			}
			else if (postfix[i][0] == '/')
			{
				params[0] = strtod(pop(number_stack), &eptr);
				params[1] = strtod(pop(number_stack), &eptr);
				val = params[1] / params[0];
				sprintf(to_push, "%f", val);
				push(number_stack, to_push);
			}
			else if (postfix[i][0] == '*')
			{
				params[0] = strtod(pop(number_stack), &eptr);
				params[1] = strtod(pop(number_stack), &eptr);
				val = params[0] * params[1];
				sprintf(to_push, "%f", val);
				push(number_stack, to_push);
			}
			else if (postfix[i][0] == '^')
			{
				params[0] = strtod(pop(number_stack), &eptr);
				params[1] = strtod(pop(number_stack), &eptr);
				val = pow(params[1], params[0]);
				sprintf(to_push, "%f", val);
				push(number_stack, to_push);
			
      }
      else if (postfix[i+1] != NULL && strcmp(postfix[i+1], "arc") == 0){
        strcpy(to_push, "arc");
        strcat(to_push, postfix[i]); 
        strcpy(postfix[i], to_push);
        strcpy(to_push, "");
      }
      else if (postfix[i+1] != NULL && strcmp(postfix[i+1], "co") == 0 && strcmp(postfix[i], "cot") != 0 && strcmp(postfix[i], "cos") != 0){
        strcpy(to_push, "co");
        strcat(to_push, postfix[i]); 
        strcpy(postfix[i], to_push);
        strcpy(to_push, "");
      }
			if (strcmp(postfix[i], "sin") == 0)
			{
				params[0] = strtod(pop(number_stack), &eptr);
				val = sin(params[0]);
				sprintf(to_push, "%f", val);
				push(number_stack, to_push);
			}
			if (strcmp(postfix[i], "cos") == 0)
			{
				params[0] = strtod(pop(number_stack), &eptr);

				val = cos(params[0]);
				sprintf(to_push, "%f", val);
				push(number_stack, to_push);
			}
			if (strcmp(postfix[i], "tan") == 0)
			{
				params[0] = strtod(pop(number_stack), &eptr);
				val = tan(params[0]);
				sprintf(to_push, "%f", val);
				push(number_stack, to_push);
			}
			if (strcmp(postfix[i], "sec") == 0)
			{
				params[0] = strtod(pop(number_stack), &eptr);
				val = 1 / cos(params[0]);
				sprintf(to_push, "%f", val);
				push(number_stack, to_push);
			}
			if (strcmp(postfix[i], "sec") == 0)
			{
				params[0] = strtod(pop(number_stack), &eptr);
				val = 1 / cos(params[0]);
				sprintf(to_push, "%f", val);
				push(number_stack, to_push);
			}
			if (strcmp(postfix[i], "cosec") == 0)
			{
				params[0] = strtod(pop(number_stack), &eptr);
				val = 1 / sin(params[0]);
				sprintf(to_push, "%f", val);
				push(number_stack, to_push);
			}
			if (strcmp(postfix[i], "arcsin") == 0)
			{
				params[0] = strtod(pop(number_stack), &eptr);
				val = asin(params[0]);
				sprintf(to_push, "%f", val);
				push(number_stack, to_push);
			}
			if (strcmp(postfix[i], "arccos") == 0)
			{
				params[0] = strtod(pop(number_stack), &eptr);
				val = acos(params[0]);
				sprintf(to_push, "%f", val);
				push(number_stack, to_push);
			}
			if (strcmp(postfix[i], "arctan") == 0)
			{
				params[0] = strtod(pop(number_stack), &eptr);
				val = atan(params[0]);
				sprintf(to_push, "%f", val);
				push(number_stack, to_push);
			}
			else if (strcmp(postfix[i], "log") == 0)
			{
				params[0] = strtod(pop(number_stack), &eptr);
				params[1] = strtod(pop(number_stack), &eptr);
				val = log(params[0]) / log(params[1]);
				sprintf(to_push, "%f", val);
				push(number_stack, to_push);
			}
		}
		else
		{
			push(number_stack, postfix[i]);
		}
		i++;
	}
	return pop(number_stack);
}

double eval_func(char *expression, char *x)
{
	char **postfix;
	char *eptr, *input;
	double result;
	input = eval_postfix(infix_to_postfix(x), "");
	postfix = infix_to_postfix(expression);
	result = strtod(eval_postfix(postfix, input), &eptr);
	return result;
}

void show_menu()
{
	system("clear");
	printf("\n0-) EVALUATE THE FUNCTION+\n1-) BISECTION METHOD+\n2-) REGULA-FALSI METHOD+\n3-) NEWTON-RAPHSON METHOD+\n4-) INVERSE OF A MATRIX- \n5-) GAUSS ELIMINATION METHOD+ \n6-) GAUSS-SEIDEL+- \n7-) NUMERICAL DERIVATIVE+ \n8-) NUMERICAL INTEGRAL WITH SIMPSON METHOD+ \n9-) NUMERICAL INTEGRAL WITH TRAPEZOIDAL METHOD+ \n10-) GREGORY NEWTON INTERPOLATION-\n");
}

double bisection(char *expression, char *bottom, char *top, double max_error)
{
	double new;
	char *eptr, *new_s;
	new_s = (char *)malloc(sizeof(char) * 30);
	if (eval_func(expression, bottom) * eval_func(expression, top) > 0)
	{
		printf("WRONG TOP AND BOTTOM VALUES!");
		return -1;
	}
	else
	{
		new = (double)((strtod(bottom, &eptr) + strtod(top, &eptr)) / 2);
		sprintf(new_s, "%f", new);
		if (fabs(eval_func(expression, new_s)) < max_error)
		{
			return new;
		}
		else
		{
			if (eval_func(expression, new_s) * eval_func(expression, bottom) < 0)
			{
				return bisection(expression, bottom, new_s, max_error);
			}
			else
			{
				return bisection(expression, new_s, top, max_error);
			}
		}
	}
}

double regula_falsi(char *expression, char *bottom, char *top, double max_error)
{
	double new, bot_d, top_d, fbot, ftop;
	char *eptr, *new_s;
	new_s = (char *)malloc(sizeof(char) * 30);
	if (eval_func(expression, bottom) * eval_func(expression, top) > 0)
	{
		printf("WRONG TOP AND BOTTOM VALUES!");
		return -1;
	}
	else
	{
		bot_d = strtod(bottom, &eptr);
		top_d = strtod(top, &eptr);
		fbot = eval_func(expression, bottom);
		ftop = eval_func(expression, top);
		new = (fbot * top_d - ftop * bot_d) / (fbot - ftop);
		sprintf(new_s, "%f", new);
		printf("x = %f, f(x) = %f \n", new, eval_func(expression, new_s));
		if (fabs(eval_func(expression, new_s)) < max_error)
		{
			return new;
		}
		else
		{
			if (eval_func(expression, new_s) * eval_func(expression, bottom) < 0)
			{
				return bisection(expression, bottom, new_s, max_error);
			}
			else
			{
				return bisection(expression, new_s, top, max_error);
			}
		}
	}
}

double numerical_derivative(char *expression, char *x0, int opt_d)
{
	double x, x1, x2, result;
	double h = 0.00001;
	char *x_s, *x1_s, *x2_s, *eptr;
	x_s = (char *)malloc(sizeof(char) * 20);
	x1_s = (char *)malloc(sizeof(char) * 20);
	x2_s = (char *)malloc(sizeof(char) * 20);
	x = eval_func(x0, "");
	if (opt_d == 1)
	{
		x1 = x + h;
		sprintf(x_s, "%f", x);
		sprintf(x1_s, "%f", x1);
		result = (eval_func(expression, x1_s) - eval_func(expression, x_s)) / h;
	}
	else if (opt_d == 2)
	{
		x1 = x - h;
		sprintf(x_s, "%f", x);
		sprintf(x1_s, "%f", x1);
		result = (eval_func(expression, x_s) - eval_func(expression, x1_s)) / h;
	}
	else if (opt_d == 3)
	{
		x1 = x + h;
		x2 = x - h;
		sprintf(x1_s, "%f", x1);
		sprintf(x2_s, "%f", x2);
		result = (eval_func(expression, x1_s) - eval_func(expression, x2_s)) / (2 * h);
	}
	return result;
}

double newton_raphson(char *expression, char *x0, double max_error)
{
	// iterative method :::>>> a1 = a0 - (f(a0) / f'(a0)
	int num_iter = 0;
	double result;
	char *result_s;
	result_s = (char *)malloc(sizeof(char) * 20);
	result = eval_func(x0, "");
	sprintf(result_s, "%f", result);
	while (fabs(eval_func(expression, result_s)) > max_error && num_iter < 1000)
	{
		result = result - (eval_func(expression, result_s) / numerical_derivative(expression, result_s, 3));
		sprintf(result_s, "%f", result);
		num_iter++;
	}
	return result;
}

double numerical_integral(char *expression, char *bottom, char *top, int n, int opt) //gözden geçir burayı full
{
	double result, h, sum = 0, x0, xn, x;
	int i, j;
	char *eptr, *x_s;
	x_s = (char *)malloc(sizeof(char) * 40);
	x0 = strtod(bottom, &eptr);
	xn = strtod(top, &eptr);
	h = (xn - x0) / n;
	x = x0;
	if (opt == 1)
	{
		for (i = 1; i < n; i++)
		{
			x = x0 + i * h;
			if (i % 2 == 0)
			{
				sprintf(x_s, "%f", x);
				sum += 2 * eval_func(expression, x_s);
			}
			else
			{
				sprintf(x_s, "%f", x);
				sum += 4 * eval_func(expression, x_s);
			}
		}
		sum += eval_func(expression, bottom) + eval_func(expression, top);
		result = sum * h / 3;
	}
	else if (opt == 2)
	{
    for(i = 0; i < n-1; i++){
      x = x0 + i * h;
      sum = 0;
      for (j = 0; j < 4; j++){
        x += j * (h/3);
        sprintf(x_s, "%f", x);
        if(j == 0 || j == 3){
          sum += eval_func(expression, x_s);   
        }else{
          sum += 3*eval_func(expression, x_s);
        }
      }
      result += h * (sum / 8);
    }
	}else if(opt == 0){
    sprintf(x_s, "%f", x0);
    sum += eval_func(expression, x_s) / 2;
    sprintf(x_s, "%f", x0);
    sum += eval_func(expression, x_s) / 2;
    for(i = 1; i < n-1; i++){
      x = x0 + i * h; 
      sprintf(x_s, "%f", x);
      sum += eval_func(expression, x_s);
    }
    result = sum * h;
  }
	return result;
}

double **inverse_matrix(double **mat, int n)
{
	return mat;
}

void gauss_methods(double **a, double *result_vec, int n, int type){
  int i, j, k;
  double temp;
  for (i = 0; i < n;i++) {
    temp = a[i][i];
    for (j = 0; j < n; j++){
      a[i][j] = a[i][j] / temp; 
    }
    result_vec[i] = result_vec[i] / temp;
    if (type == 1){
      for (j = 1; j < n-i; j++){
        temp = a[i+j][i];
        for(k = 0; k < n; k++){
          a[i+j][k] -= temp*a[i][k];
        }
        result_vec[i+j] -= temp*result_vec[i];
      }
    }else if(type == 2){
       for (j = 0; j < n; j++){
          if (j!=i){
            temp = a[j][i];
            for(k = 0; k < n; k++){
              a[j][k] -= temp*a[i][k];
            }
            result_vec[j] -= temp*result_vec[i];
          }
      }
    }
  }   
  for (i = 0; i < n; i++){
      for (j = 0; j < n; j++)
        printf("%0.3lf  ", a[i][j]);
      printf("| %0.3lf", result_vec[i]);
      printf("\n");
 
  }
 
}
void gauss_seidel(double **a, double *result_vec, int n, double max_error){
  double *X, temp_sum = 0.00;
  int i, j;
  double max_error_among = -1.00;
  X = (double*)malloc(sizeof(double) * n);
  for(i = 0; i < n; i++)
      X[i] = 0.00;

  while(max_error_among > max_error || max_error_among == -1.00){
    max_error_among = -1;
    for(i = 0; i < n; i++){
      for(j = 0; j < n; j++){
        if (i!=j){
          temp_sum -= X[j]*a[i][j];
        }
      }
      temp_sum = (temp_sum + result_vec[i]) / a[i][i];
      if(fabs(X[i] - temp_sum) > max_error || max_error_among == -1){
        max_error_among = fabs(X[i] - temp_sum);
      }
      X[i] = temp_sum;
      temp_sum = 0.00;
    } 
  } 
  for (i = 0; i < n; i++){
      printf("X[%d] = %.3lf\n", i, X[i]);
  }
}
int main()
{

	/* char direct_input[100], x[30];
	fgets(direct_input, 100, stdin);
	fgets(x, 30, stdin);
	printf("%0.3f \n", eval_func(direct_input, x));
	fgets(x, 30, stdin);*/
	double **mat, *result_vec;
	int choice, opt, n, i, j, cont = 1;
	double max_error_d;
	char expression[100], bottom[10], top[10], max_error[10], x0[10], *eptr;
	while (cont)
	{
		show_menu();
		scanf("%d", &choice);
		getchar();
		switch (choice)
		{
		case 0:
			printf("express the function:");
			fgets(expression, 100, stdin);
			printf("x:");
			fgets(bottom, 10, stdin);
			printf("result: %f", eval_func(expression, bottom));
			break;
		case 1:
			printf("express the function:");
			fgets(expression, 100, stdin);
			printf("write down the bottom of interval:");
			fgets(bottom, 10, stdin);
			printf("write down the top of interval:");
			fgets(top, 10, stdin);
			printf("maximum error:");
			fgets(max_error, 10, stdin);
			max_error_d = strtod(max_error, &eptr);
			printf("----------\nresult x = %f \n----------", bisection(expression, bottom, top, max_error_d));
			break;
		case 2:
			printf("express the function:");
			fgets(expression, 100, stdin);
			printf("write down the bottom of interval:");
			fgets(bottom, 10, stdin);
			printf("write down the top of interval:");
			fgets(top, 10, stdin);
			printf("maximum error:");
			fgets(max_error, 10, stdin);
			max_error_d = strtod(max_error, &eptr);
			printf("----------\nresult x = %f \n----------", regula_falsi(expression, bottom, top, max_error_d));
			break;
		case 3:
			printf("express the function:");
			fgets(expression, 100, stdin);
			printf("write down the x0:");
			fgets(x0, 10, stdin);
			printf("maximum error:");
			fgets(max_error, 10, stdin);
			max_error_d = strtod(max_error, &eptr);
			printf("%f", newton_raphson(expression, x0, max_error_d));
			break;

		case 4:
			printf("write down the dimension of the matrix, n: ");
			scanf("%d", &n);
			mat = (double **)malloc(sizeof(double *) * n);
			for (i = 0; i < n; i++)
				mat[i] = (double *)malloc(n * sizeof(double));

			for (i = 0; i < n; i++)
			{
				for (j = 0; j < n; j++)
				{
					printf("value of matrix[%d][%d]: ", i + 1, j + 1);
					scanf("%lf", &mat[i][j]);
				}
			}

			// mat = inverse_matrix(mat, n);
			for (i = 0; i < n; i++)
			{
				for (j = 0; j < n; j++)
				{
					printf("%0.3lf  ", mat[i][j]);
				}
				printf("\n");
			}
			break;
		case 5:
      printf("1 for gauss method(to upper-triangle matrix), 2 for gauss-jordan(to identity matrix)");
      scanf("%d", &opt);
			printf("write down the dimension of the matrix, n: ");
			scanf("%d", &n);
			result_vec = (double *)malloc(sizeof(double) * n);
			mat = (double **)malloc(sizeof(double*) * n);

			for (i = 0; i < n; i++)
				mat[i] = (double *)malloc(n * sizeof(double));

			for (i = 0; i < n; i++)
			{
				for (j = 0; j < n; j++)
				{
					printf("value of matrix[%d][%d]: ", i + 1, j + 1);
					scanf("%lf", &mat[i][j]);
				}
			}
			for (i = 0; i < n; i++)
			{
				printf("result_vec[%d]", i + 1);
				scanf("%lf", &result_vec[i]);
			}
			gauss_methods(mat, result_vec, n, opt);
			break;
		case 6:
			printf("write down the dimension of the matrix, n: ");
			scanf("%d", &n);
			result_vec = (double *)malloc(sizeof(double) * n);
			mat = (double **)malloc(sizeof(double*) * n);

			for (i = 0; i < n; i++)
				mat[i] = (double *)malloc(n * sizeof(double));

			for (i = 0; i < n; i++)
			{
				for (j = 0; j < n; j++)
				{
					printf("value of matrix[%d][%d]: ", i + 1, j + 1);
					scanf("%lf", &mat[i][j]);
				}
			}
			for (i = 0; i < n; i++)
			{
				printf("result_vec[%d]", i + 1);
				scanf("%lf", &result_vec[i]);
			}
      printf("max error: ");
      scanf("%lf", &max_error_d);
			gauss_seidel(mat, result_vec, n, max_error_d);

			break;
		case 7:
			printf("express the function: ");
			fgets(expression, 100, stdin);
			printf("write down the x: ");
			fgets(x0, 10, stdin);
			printf("write down the differentiation choice(1 for forward, 2 for backward, 3 for central)");
			scanf("%d", &opt);
			printf("%f", numerical_derivative(expression, x0, opt));
			break;
		case 8:
			printf("express the function: ");
			fgets(expression, 100, stdin);
			printf("write down the lower bound: ");
			fgets(bottom, 10, stdin);
			printf("write down the upper bound: ");
			fgets(top, 10, stdin);
			printf("write down the number of sub-parts: ");
			scanf("%d", &n);
			printf("1 for Simpson's 1/3, 2 for Simpson's 3/8");
			scanf("%d", &opt);
			printf("Estimated area: %f", numerical_integral(expression, bottom, top, n, opt));
			break;
    case 9:
      printf("express the function: ");
			fgets(expression, 100, stdin);
			printf("write down the lower bound: ");
			fgets(bottom, 10, stdin);
			printf("write down the upper bound: ");
			fgets(top, 10, stdin);
			printf("write down the number of sub-parts: ");
			scanf("%d", &n);
			printf("Estimated area: %f", numerical_integral(expression, bottom, top, n, 0));
			break;
    case 10:

		}
		printf("\ndo you want to continue? 1 for yes, 0 for no.");
		scanf("%d", &cont);
	}
	printf("FAREWELLLL. :'(\n");
	return 0;
}
