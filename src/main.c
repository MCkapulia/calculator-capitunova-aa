
#include "calculator.h"

int main(int argc, char* argv[])
{
    if (argc > 1 && strcmp(argv[1], "--float") == 0) { mode = 1; }

    char input[MAX_INPUT_SIZE];
    if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Ошибка чтения ввода\n");
        return 1;
    }

    // Удаляем символ новой строки, если он есть
    input[strcspn(input, "\n")] = '\0';

    // Проверка корректности выражения
    if (is_valid_expression(input) == -1 || !is_valid_expression(input)) { return -1; }
    // printf("%d\n", is_valid_expression(input));
    int pos = 0;
    if (mode == 0) {
        int result = eval_expression_int(input, &pos);

        printf("%d\n", result);
    } else {
        float result = eval_expression_float(input, &pos);

        printf("%.4f\n", result);
    }

    return 0;
}
