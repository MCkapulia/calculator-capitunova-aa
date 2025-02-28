# Настройки компилятора
CC := gcc
CXX := g++
CFLAGS := -Wall -Wextra -Wpedantic -Werror -std=c11
CXXFLAGS := -Wall -Wextra -Wpedantic -Werror -std=c++17


BUILD_DIR := build

# Настройки Google Test
GTEST_DIR := googletest/googletest
GTEST_SRC_DIR := $(GTEST_DIR)/src
GTEST_HEADERS := $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_BUILD_DIR := $(BUILD_DIR)/gtest
GTEST_ALL_OBJ  := $(GTEST_BUILD_DIR)/gtest-all.o
GTEST_MAIN_OBJ := $(GTEST_BUILD_DIR)/gtest_main.o
GTEST_MAIN_A   := $(GTEST_BUILD_DIR)/gtest_main.a

# Настройки виртуального окружения Python
VENV_DIR := build/venv
PYTEST = $(VENV_DIR)/bin/pytest

# Исходные файлы приложения
APP_SRC := src/main.c src/calculator.c
APP_OBJ := $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(APP_SRC))

# Все тестовые .cpp
TEST_SRCS := tests/unit/testfunction.cpp

# Основные цели
.PHONY: all clean run-int run-float run-unit-test run-integration-tests venv clone-gtest

all: build/app.exe build/unit-tests.exe clang-format

clean:
	@echo "Очистка проекта..."
	rm -rf $(BUILD_DIR)/
	rm -rf tests/integration/__pycache__
	rm -rf .pytest_cache


# Сборка приложения
$(BUILD_DIR)/%.o: src/%.c
	  @echo "Компиляция $@"
	  @mkdir -p $(BUILD_DIR)
	  $(CC) $(CFLAGS) -c $< -o $@

build/app.exe: $(APP_OBJ)
	  @echo "Сборка app.exe"
	  $(CC) $(CFLAGS) -o $@ $^

# Сборка тестовой версии приложения
build/app-test.o: src/calculator.c
	  @echo "Сборка app-test.o с флагом -DUNIT_TEST"
	  @mkdir -p $(BUILD_DIR)
	  $(CC) $(CFLAGS) -DUNIT_TEST -DGTEST -c $< -o $@ -g

# Сборка Google Test
$(GTEST_ALL_OBJ): $(GTEST_SRC_DIR)/gtest-all.cc $(GTEST_HEADERS)
	  @echo "Компиляция gtest-all.o"
	  @mkdir -p $(GTEST_BUILD_DIR)
	  $(CXX) $(CXXFLAGS) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -c $< -o $@

$(GTEST_MAIN_OBJ): $(GTEST_SRC_DIR)/gtest_main.cc $(GTEST_HEADERS)
	  @echo "Компиляция gtest_main.o"
	  @mkdir -p $(GTEST_BUILD_DIR)
	  $(CXX) $(CXXFLAGS) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -c $< -o $@

$(GTEST_MAIN_A): $(GTEST_ALL_OBJ) $(GTEST_MAIN_OBJ)
	  @echo "Создание архива gtest_main.a"
	  ar rcs $@ $^

# Сборка unit-тестов
build/unit-tests.exe: $(GTEST_MAIN_A) build/app-test.o $(TEST_SRCS)
	  @echo "Сборка unit-tests.exe"
	  $(CXX) $(CXXFLAGS) -isystem $(GTEST_DIR)/include -pthread \
	  $(TEST_SRCS) build/app-test.o $(GTEST_MAIN_A) \
	  -o $@

# Запуск приложения в целочисленном режиме
run-int: build/app.exe
	  @echo "Запуск в целочисленном режиме"
	  @./build/app.exe

# Запуск приложения в режиме с плавающей точкой
run-float: build/app.exe
	  @echo "Запуск в режиме с плавающей точкой"
	  @./build/app.exe --float

# Запуск unit-тестов
run-unit-test: build/unit-tests.exe
	  @echo "Запуск unit-тестов..."
	  @./build/unit-tests.exe

# Создание виртуального окружения Python
venv:
	  @echo "Создание виртуального окружения $(VENV_DIR)"
	  @if [ ! -d "$(VENV_DIR)" ]; then \
	    python3 -m venv $(VENV_DIR); \
	    $(VENV_DIR)/bin/python3 -m pip install --upgrade pip; \
	    $(VENV_DIR)/bin/pip install pytest; \
	  fi

# Запуск интеграционных тестов
run-integration-tests: build/app.exe venv tests/integration/test_integration.py
	  @echo "Запуск интеграционных тестов с pytest..."
	  @. $(VENV_DIR)/bin/activate && $(VENV_DIR)/bin/pytest tests/integration/test_integration.py
	 
clang-format:
	@echo "Running clang-format..."
	find . -regex '.*\.\(h\|c\|cpp\)$$' -exec clang-format -i {} +
