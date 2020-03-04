#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Структура стэка
typedef struct {
	unsigned int size;
	unsigned int ptr;

	int64_t* stack;
} Stack;

// Объявление
Stack* init_stack(unsigned int stack_size);
void push(Stack* stack, int64_t value);
int64_t pop(Stack* stack);

Stack* init_stack(unsigned int stack_size) {
	// Выделим память
	Stack* new_stack = (Stack*)malloc(sizeof(Stack));
	if (!new_stack) {
		printf("Unable to allocate the memory for the stack object\n");
		return NULL;
	}

	// Разместили в памяти объект стэка. Самое время выделить память для стэка
	new_stack->stack = (int64_t*)malloc(stack_size * sizeof(int64_t));
	if (!new_stack->stack) {
		printf("Unable to allocate the memory for the stack\n");
		free(new_stack); // Не забудем очистить память, иначе будет ее утечка
		return NULL;
	}

	// Задаем значения
	new_stack->size = stack_size;
	new_stack->ptr = 0;

	return new_stack;
}

void push(Stack* stack, int64_t value) {
	if (!stack) {
		printf("Stack object cannot be a NULL pointer for push\n");
		return;
	}
	else if (!stack->stack) {
		printf("Stack cannot be a NULL pointer for push\n");
		return;
	}

	if (stack->ptr >= stack->size) {
		printf("Unable to push value: stack is full\n");
		return;
	}

	// Сначала разместим значение на вершине стека (stack->stack[stack->ptr]), а потом прибавим stack->ptr
	stack->stack[stack->ptr++] = value;
}

int64_t pop(Stack* stack) {
	if (!stack) {
		printf("Stack object cannot be a NULL pointer\n");
		return 0;
	}
	else if (!stack->stack) {
		printf("Stack cannot be a NULL pointer\n");
		return 0;
	}

	if (!stack->ptr) {
		printf("Unable to pop value: stack is empty\n");
		return 0;
	}

	// Сначала уменьшим stack->ptr, а потом возьмем из стека значение
	return stack->stack[--(stack->ptr)];
}

void fini_stack(Stack* stack) {
	if (stack)
		free(stack->stack);

	free(stack);
}

bool is_operation(char* input, unsigned int size) {
	if (size == 1)
		return input[0] == '+' || input[0] == '-' || input[0] == '*' || input[0] == '/' || input[0] == '^';
	else if (size == 2)
		return !strcmp(input, "**");
	else return 0;
}

bool is_operand(char* input, unsigned int size) {
	for (int i = 0; i < size; i++) {
		if (input[i] < '0' || input[i] > '9')
			return false;
	}
	return true;
}

int64_t operation_pow(int64_t number, unsigned short stepen) { //TODO: translate it!
	if (!number) return 0;

	for (int i = 0; i < stepen; i++) {
		number *= number;
	}

	return number;
}

int process(Stack* stack, char* input) {
	unsigned int size = strlen(input);
	if (!size) return 1;

	if (is_operation(input, size)) { // process the operation
		char* operation = input;

		int64_t operand_2 = pop(stack);
		int64_t operand_1 = pop(stack);

		int64_t result = 0;

		if (size == 1) {
			if (operation[0] == '+')            // add
				result = operand_1 + operand_2;
			else if (operation[0] == '-')       // substitute
				result = operand_1 - operand_2;
			else if (operation[0] == '*')       // multiply
				result = operand_1 * operand_2;
			else if (operation[0] == '/')       // divide
				result = operand_1 / operand_2;
			else if (operation[0] == '^')       // xor
				result = operand_1 ^ operand_2;
		}
		else if (size == 2) {
			if (!strcmp(operation, "**"))       // pow TODO
				result = operation_pow(operand_1, (unsigned short)operand_2);
		}

		printf("%lld %s %lld = %lld\n", operand_1, operation, operand_2, result);

		push(stack, result);
		return 0;
	}
	else if (is_operand(input, size)) { // add operand
		push(stack, (int64_t)(atoi(input)));
		return 0;
	}

	return 2;
}

int main() {
	char* input = "7 2 3 * - ";

	printf("Creating my own stack object...\n");
	Stack* my_stack = init_stack(256);
	if (!my_stack) {
		printf("Oh godness my stack was not initialised =(\n");
		return 1;
	}

	printf("Let's calculate '%s'\n", input);

	char buffer[256];

	char* found = input;
	while (1) {
		char* found_end = strstr(found, " "); // TODO: strchr
		if (!found_end) break;

		unsigned int size = found_end - found;

		memcpy(buffer, found, size + 1);
		buffer[size] = 0; // NULL-terminator

		process(my_stack, buffer);

		found = found_end + strlen(" ");
	}

	printf("Result is %lld\n", pop(my_stack));

	printf("Let's delete the stack!\n");
	fini_stack(my_stack);
	my_stack = NULL;

	return 0;
}
