#include "calculator.h"

int mode = 0;

float eval_expression_float(const char* expr, int* pos)
{
    float result = eval_term_float(expr, pos);

    while (expr[*pos] != '\0' && expr[*pos] != ')') {
        if (is_whitespace(expr[*pos])) {
            (*pos)++;
            continue;
        }

        char op = expr[*pos];
        if (op != '+' && op != '-') { break; }
        (*pos)++;

        float term = eval_term_float(expr, pos);
        if (op == '+') {
            result += term;
        } else if (op == '-') {
            result -= term;
        }
        /*if (!is_within_range(result)){
            fprintf(stderr, "Error: result is out of range\n");
            exit(1);
        }*/
    }

    return result;
}

float eval_term_float(const char* expr, int* pos)
{
    float result = eval_factor_float(expr, pos);

    while (expr[*pos] != '\0' && expr[*pos] != ')') {
        if (is_whitespace(expr[*pos])) {
            (*pos)++;
            continue;
        }

        char op = expr[*pos];
        if (op != '*' && op != '/') { break; }
        (*pos)++;

        float factor = eval_factor_float(expr, pos);
        if (op == '*') {
            /*if (result!= 0 && factor>MAX_VALUE/result){
                fprintf(stderr, "Error: result is out of range\n");
            exit(1);
        }*/
            result *= factor;
        } else if (op == '/') {
            result /= factor;
        }

        /*if (!is_within_range(result)){
            fprintf(stderr, "Error: result is out of range\n");
            exit(1);
        }*/
    }

    return result;
}

float eval_factor_float(const char* expr, int* pos)
{
    while (is_whitespace(expr[*pos])) (*pos)++;

    if (expr[*pos] == '(') {
        (*pos)++;
        float result = eval_expression_float(expr, pos);
        while (is_whitespace(expr[*pos])) (*pos)++;
        if (expr[*pos] != ')') {
            fprintf(stderr, "Expected closing parenthesis\n");
            return -1;
        }
        (*pos)++;
        return result;
    } else if (isdigit(expr[*pos]) || expr[*pos] == '.') {
        double num = 0.0;
        while (isdigit(expr[*pos])) {
            num = num * 10 + (expr[*pos] - '0');
            if (num > MAX_VALUE) {
                fprintf(stderr, "Error: number is out of range\n");
                exit(1);
            }
            (*pos)++;
        }
        if (expr[*pos] == '.') {
            (*pos)++;
            float fraction = 0.1;
            while (isdigit(expr[*pos])) {
                num += (expr[*pos] - '0') * fraction;
                fraction *= 0.1;
                (*pos)++;
            }
        }

        // Проверка наличия оператора перед следующей открывающейся скобкой
        if (expr[*pos] == '(') {
            fprintf(stderr, "Missing operator before opening parenthesis\n");
            exit(1);
        }

        return num;
    } else {
        fprintf(stderr, "Invalid character in expression: %c\n", expr[*pos]);
        exit(1);
    }
}

int eval_expression_int(const char* expr, int* pos)
{
    int result = eval_term_int(expr, pos);

    while (expr[*pos] != '\0' && expr[*pos] != ')') {
        if (is_whitespace(expr[*pos])) {
            (*pos)++;
            continue;
        }

        char op = expr[*pos];
        if (op != '+' && op != '-') { break; }
        (*pos)++;

        int term = eval_term_int(expr, pos);
        if (op == '+') {
            result += term;
        } else if (op == '-') {
            result -= term;
        }

        /*if (!is_within_range(result)){
            fprintf(stderr, "Error: result is out of range\n");
            exit(1);
        }*/
    }

    return result;
}

int eval_term_int(const char* expr, int* pos)
{
    int result = eval_factor_int(expr, pos);

    while (expr[*pos] != '\0' && expr[*pos] != ')') {
        if (is_whitespace(expr[*pos])) {
            (*pos)++;
            continue;
        }

        char op = expr[*pos];
        if (op != '*' && op != '/') { break; }
        (*pos)++;

        int factor = eval_factor_int(expr, pos);
        if (op == '*') {
            if (result != 0 && factor > MAX_VALUE / result) {
                fprintf(stderr, "Error: result is out of range\n");
                exit(1);
            }
            result *= factor;
        } else if (op == '/') {
            if (factor == 0) {
                fprintf(stderr, "Division by zero\n");
                return -1;
            }

            // Кастомная реализация округления
            float quotient = (float)result / factor;
            int truncated = (int)quotient;
            if ((quotient - truncated) >= 0.5) {
                result = truncated + 1;
            } else {
                result = truncated;
            }
        }
        if (!is_within_range(result)) {
            fprintf(stderr, "Error: result is out of range\n");
            exit(1);
        }
    }

    return result;
}

int eval_factor_int(const char* expr, int* pos)
{
    while (is_whitespace(expr[*pos])) (*pos)++;

    if (expr[*pos] == '-') {
        fprintf(stderr, "Invalid character in expression: %c\n", expr[*pos]);
        exit(1);
    }

    if (expr[*pos] == '(') {
        (*pos)++;
        int result = eval_expression_int(expr, pos);
        while (is_whitespace(expr[*pos])) (*pos)++;
        if (expr[*pos] != ')') {
            fprintf(stderr, "Expected closing parenthesis\n");
            return -1;
        }
        (*pos)++;
        return result;
    } else if (isdigit(expr[*pos])) {
        long long num = 0;
        while (isdigit(expr[*pos])) {
            num = num * 10 + (expr[*pos] - '0');

            if (num > MAX_VALUE) {
                fprintf(stderr, "Error: number is out of range\n");
                exit(1);
            }
            (*pos)++;
        }

        if (expr[*pos] == '(') {
            fprintf(stderr, "Missing operator before opening parenthesis\n");
            exit(1);
        }
        return num;
    } else {
        fprintf(stderr, "Invalid character in expression: %c\n", expr[*pos]);
        // return -1;
        exit(1);
    }
}

int is_valid_expression(const char* expr)
{
    int paren_count = 0;
    for (int i = 0; expr[i] != '\0'; i++) {
        if (!is_valid_char(expr[i])) {
            fprintf(stderr, "Invalid character in expression: %c\n", expr[i]);
            return -1;
        }

        if (isdigit(expr[i])
            && (expr[i + 1] == ' ' || expr[i + 1] == ' ' || expr[i + 1] == '\t' || expr[i + 1] == '\n'
                || expr[i + 1] == '\r' || expr[i + 1] == '\f' || expr[i + 1] == '\v')
            && expr[i + 2] == '(') {
            fprintf(stderr, "Error: Space between number and opening parenth eses\n");
            return -1;
        }
        if (is_whitespace(expr[i])) continue;
        if (expr[i] == '(')
            paren_count++;
        else if (expr[i] == ')')
            paren_count--;
    }
    if (paren_count != 0) {
        fprintf(stderr, "Error: missing parenthesis\n");
        return -1;
    }
    return paren_count == 0;
}

int is_valid_char(char c)
{
    return isdigit(c) || c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == 'V'
           || is_whitespace(c);
}

int is_whitespace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'; }

int is_within_range(long long value) { return value >= MIN_VALUE && value <= MAX_VALUE; }
