#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// ��������� �����
typedef struct {
	unsigned int size;
	unsigned int ptr;

	uint64_t* stack;
} Stack;

// ����������
Stack* init_stack(unsigned int stack_size);
void push(Stack* stack, unsigned int value);
uint64_t pop(Stack* stack);

Stack* init_stack(unsigned int stack_size) {
	// ������� ������
	Stack* new_stack = (Stack*)malloc(sizeof(Stack));
	if (!new_stack) {
		printf("Unable to allocate the memory for the stack object\n");
		return NULL;
	}

	// ���������� � ������ ������ �����. ����� ����� �������� ������ ��� �����
	new_stack->stack = (uint64_t*)malloc(stack_size * sizeof(uint64_t));
	if (!new_stack->stack) {
		printf("Unable to allocate the memory for the stack\n");
		free(new_stack); // �� ������� �������� ������, ����� ����� �� ������
		return NULL;
	}

	// ������ ��������
	new_stack->size = stack_size;
	new_stack->ptr = 0;

	return new_stack;
}

void push(Stack* stack, uint64_t value) {
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

	// ������� ��������� �������� �� ������� ����� (stack->stack[stack->ptr]), � ����� �������� stack->ptr
	stack->stack[stack->ptr++] = value;
}

uint64_t pop(Stack* stack) {
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

	// ������� �������� stack->ptr, � ����� ������� �� ����� ��������
	return stack->stack[--(stack->ptr)];
}

void fini_stack(Stack* stack) {
	if (stack)
		free(stack->stack);
	
	free(stack);
}

int main() {
	printf("Creating my own stack object...\n");
	Stack* my_stack = init_stack(256);
	if (!my_stack) {
		printf("Oh godness my stack was not initialised =(\n");
		return 1;
	}

	printf("Let's push to stack value 777!\n");
	push(my_stack, 777);

	uint64_t value = pop(my_stack);
	printf("Popped value is: %llu\n", value);

	printf("Let's delete the stack!\n");
	fini_stack(my_stack);
	my_stack = NULL;

	return 0;
}