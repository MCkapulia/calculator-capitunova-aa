#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX_INPUT_SIZE 1024
#define MAX_VALUE 2000000000
#define MIN_VALUE -2000000000
extern int mode;

float eval_expression_float(const char* expr, int* pos);
float eval_term_float(const char* expr, int* pos);
float eval_factor_float(const char* expr, int* pos);

int eval_expression_int(const char* expr, int* pos);
int eval_term_int(const char* expr, int* pos);
int eval_factor_int(const char* expr, int* pos);

int is_valid_expression(const char* expr);
int is_valid_char(char c);
int is_whitespace(char c);
int is_within_range(long long value);
