#include <stdio.h>
#define STACK_SIZE 1

typedef struct {
	int size;
	int offset;
	int st[STACK_SIZE];
} Stack;

void push(Stack* stack, int value) {
	if (stack == NULL) {
		printf("AaaaSSsfffsfskofmsijmfisjmfiosmfismfijsenm\n");
		return;
	}

	// CHECK FULL
	if (stack->offset >= stack->size) {
		printf("chto-to \n");
		return;
	}

	stack->st[stack->offset++] = value;

	printf("Pasha value is %d\n", value);
}

int pop(Stack* stack) {
	if (stack == NULL) {
		printf("AaaaSSsfffsfskofmsijmfisjmfiosmfismfijsenm\n");
		return 0;
	}

	// CHECK EMPTY
	if (stack->offset <= 0) {
		printf("chto-to 222 \n");
		return 0;
	}
	int value = stack->st[--(stack->offset)];

	printf("Poppa value is %d\n", value);

	return value;
}

void init_stack(Stack* stack) {
	stack->size = STACK_SIZE;
	stack->offset = 0;
}

int main() {
	init_stack()
	Stack my_stack;
	char* arr = "43+27/-";
	for (int i = 0; i < 7; i++) {
		if (arr[i] = '0' && arr[i] <= '9') {
			push(&my_stack, arr[i] - '0');
		}
		else if (arr[i] == '+' || arr[i] == '-' || arr[i] == '*' || arr[i] == '/')
			int val1 = pop(&my_stack);
		    int val2 = pop(&my_stack);
			if (arr[i] == '+') {
				push(&my_stack, val1 + val2)
			}


		}
	}
	

	push(&my_stack, 443256543);
	pop(&my_stack);

	return 0;
}