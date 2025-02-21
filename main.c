#include <stdio.h>
#include <stdlib.h>

int isspace_custom(char c) {
    return (c == ' '||  c == '\t'||  c == '\n'||  c == '\r'||  c == '\f'||  c == '\v');
}

int isdigit_custom(char c) {
    return (c >= '0' && c <= '9');
}

int evaluate(char *expression, int *index);

int factor(char *expression, int *index) {
    int result = 0;
    while (isspace_custom(expression[*index])) (*index)++;

    if (expression[*index] == '(') {
        (*index)++;
        result = evaluate(expression, index);
        while (isspace_custom(expression[*index])) (*index)++;
        if (expression[*index] == ')') (*index)++;
        else {
            exit(1);
        }

    } else if (isdigit_custom(expression[*index])) {
        while (isdigit_custom(expression[*index])) {
            result = result * 10 + (expression[*index] - '0');
           (*index)++;
        }
    } else {
        exit(1);
    }
    return result;
}


int term(char *expression, int *index) {
    int result = factor(expression, index);
    while (isspace_custom(expression[*index])) (*index)++;


    while (expression[*index] == '*' ||  expression[*index] == '/') {
        char operator = expression[*index];
        (*index)++;
        while (isspace_custom(expression[*index])) (*index)++;

        int next_factor = factor(expression, index);

        if (operator == '*') {
            result *= next_factor;
        } else {
            if (next_factor == 0) {
                exit(1);
            }
            result /= next_factor;
        }
         while (isspace_custom(expression[*index])) (*index)++;
    }

    return result;
}


int evaluate(char *expression, int *index) {

    int result = term(expression, index);
    while (isspace_custom(expression[*index])) (*index)++;


    while (expression[*index] == '+' || expression[*index] == '-') {
        char operator = expression[*index];
        (*index)++;
         while (isspace_custom(expression[*index])) (*index)++;

        int next_term = term(expression, index);

        if (operator == '+') {
            result += next_term;
        } else {
            result -= next_term;
        }
         while (isspace_custom(expression[*index])) (*index)++;
    }

    return result;
}



int main() {
    char expression[1024];
    if (fgets(expression, sizeof(expression), stdin) == NULL) {
        return 1;
    }

    int index = 0;
    int result = evaluate(expression, &index);
    printf("%d\n", result);

    return 0;
}
