#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// Структура очереди
typedef struct {
	unsigned int size;
	unsigned int start;
	unsigned int end;

	uint64_t* queue;
} Queue;

// Объявление
Queue* init_queue(unsigned int queue_size);

void     push(Queue* queue, uint64_t value);
uint64_t pop(Queue* queue);


Queue* init_queue(unsigned int queue_size) {
	// Выделим память
	Queue* new_queue = (Queue*)malloc(sizeof(Queue));
	if (!new_queue) {
		printf("Unable to allocate the memory for the queue object\n");
		return NULL;
	}

	// Разместили в памяти объект стэка. Самое время выделить память для стэка
	new_queue->queue = (uint64_t*)malloc(queue_size * sizeof(uint64_t));
	if (!new_queue->queue) {
		printf("Unable to allocate the memory for the queue\n");
		free(new_queue); // Не забудем очистить память, иначе будет ее утечка
		return NULL;
	}

	// Задаем значения
	new_queue->size = queue_size;
	new_queue->start = 0;
	new_queue->end = 0;

	return new_queue;
}

void push(Queue* queue, uint64_t value) {
	if (!queue) {
		printf("Stack object cannot be a NULL pointer for push\n");
		return;
	}
	else if (!queue->queue) {
		printf("Stack cannot be a NULL pointer for push\n");
		return;
	}

	if (queue->end == queue->start && queue->start > 0)
		queue->start = queue->end = 0;

	if (queue->end >= queue->size) {
		if (queue->start == 0) {
			printf("Unable to push value: queue is full\n");
			return;
		}
		else { // Сдвиг значений в начало очереди
			memcpy(queue->queue, queue->queue + queue->start, queue->end - queue->start);

			queue->end -= queue->start;
			queue->start = 0;
		}
	}

	// Сначала разместим значение в конце очереди (queue->queue[queue->end]), а потом прибавим queue->end
	queue->queue[queue->end++] = value;
}

uint64_t pop(Queue* queue) {
	if (!queue) {
		printf("Stack object cannot be a NULL pointer\n");
		return 0;
	}
	else if (!queue->queue) {
		printf("Stack cannot be a NULL pointer\n");
		return 0;
	}

	if (queue->start == queue->end) {
		printf("Unable to pop value: queue is empty\n");
		return 0;
	}

	// Сначала возьмем из начала очереди значение, потом увеличим позицию начала очереди
	return queue->queue[queue->start++];
}

void fini_queue(Queue* queue) {
	if (queue)
		free(queue->queue);

	free(queue);
}

int main() {
	printf("Creating my own queue object...\n");
	Queue* my_queue = init_queue(256);
	if (!my_queue) {
		printf("Oh godness my queue was not initialised =(\n");
		return 1;
	}

	printf("Let's push to queue value 321!\n");
	push(my_queue, 321);

	uint64_t value = pop(my_queue);
	printf("Popped value is: %llu\n", value);

	printf("Let's delete the queue!\n");
	fini_queue(my_queue);
	my_queue = NULL;

	return 0;
}