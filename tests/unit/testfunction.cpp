#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// Подключаем функции из вашего основного файла
extern "C" {
 #include "../../src/calculator.h"
 

}
// Тесты для eval_expression_float
TEST(CalculatorTest, EvalExpressionFloat) {
    const char* expr = "3.5 + 2.5 * 2";
    int pos = 0;
    EXPECT_FLOAT_EQ(eval_expression_float(expr, &pos), 8.5);
}

// Тесты для eval_term_float
TEST(CalculatorTest, EvalTermFloat) {
    const char* expr = "3.5 * 2.5";
    int pos = 0;
    EXPECT_FLOAT_EQ(eval_term_float(expr, &pos), 8.75);
}

// Тесты для eval_factor_float
TEST(CalculatorTest, EvalFactorFloat) {
    const char* expr = "3.5";
    int pos = 0;
    EXPECT_FLOAT_EQ(eval_factor_float(expr, &pos), 3.5);
}

// Тесты для eval_expression_int
TEST(CalculatorTest, EvalExpressionInt) {
    const char* expr = "3 + 2 * 2";
    int pos = 0;
    EXPECT_EQ(eval_expression_int(expr, &pos), 7);
}

// Тесты для eval_term_int
TEST(CalculatorTest, EvalTermInt) {
    const char* expr = "3 * 2";
    int pos = 0;
    EXPECT_EQ(eval_term_int(expr, &pos), 6);
}

// Тесты для eval_factor_int
TEST(CalculatorTest, EvalFactorInt) {
    const char* expr = "3";
    int pos = 0;
    EXPECT_EQ(eval_factor_int(expr, &pos), 3);
}

// Тесты для is_valid_expression
TEST(CalculatorTest, IsValidExpression) {
    const char* validExpr = "3 + 2 * (5 - 1)";
    const char* invalidExpr = "3 + 2 * (5 - 1";
    EXPECT_TRUE(is_valid_expression(validExpr));
    EXPECT_EQ(is_valid_expression(invalidExpr), -1);
}

// Тесты для is_valid_char
TEST(CalculatorTest, IsValidChar) {
    EXPECT_TRUE(is_valid_char('3'));
    EXPECT_TRUE(is_valid_char('+'));
    EXPECT_TRUE(is_valid_char('('));
    EXPECT_FALSE(is_valid_char('a'));
}

// Тесты для is_whitespace
TEST(CalculatorTest, IsWhitespace) {
    EXPECT_TRUE(is_whitespace(' '));
    EXPECT_TRUE(is_whitespace('\t'));
    EXPECT_FALSE(is_whitespace('a'));
}


