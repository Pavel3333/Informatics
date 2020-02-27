#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct File {
	char* path;
	size_t size;
};

struct File** files = NULL;
size_t count = 0;

void list_files() {
	if (!files || !count) {
		puts("There are no any files\n");
		return;
	}

	printf("There are %u files\n", count);
	for (size_t i = 0; i < count; i++) {
		puts("File:\n");
		printf("\tPath: %s\n", files[i]->path);
		printf("\tSize: %u\n", files[i]->size);
	}
}

void add_file() {
	char path[256] = { 0 };

	puts("Please enter the path to the file: ");
	int result = scanf("%256s", path);
	if (result != 1) {
		puts("Invalid input\n");
		rewind(stdin);
		return;
	}

	FILE* file = fopen(path, "rb");
	if (!file) {
		puts("Could not open the file\n");
		return;
	}

	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);

	struct File** files_new;
	files_new = realloc(files, sizeof(struct File*) * (count + 1));
	if (!files_new) {
		puts("Unable to reallocate the files array\n");
		return;
	}

	free(files);
	files = files_new;
	struct File* file_desc = malloc(sizeof(struct File));
	if (!file_desc) {
		puts("Unable to allocate the memory for the new file\n");
		return;
	}

	file_desc->size = size;
	file_desc->path = malloc(strlen(path) + 1);
	if (!file_desc->path) {
		puts("Unable to allocate the memory for the path\n");
		return;
	}
	strcpy(file_desc->path, path);

	files[count] = file_desc;
	count++;
}

int main() {
	while (1) {
		puts("Menu:\n");
		puts("\t1. List the files\n");
		puts("\t2. Add the file\n");
		puts("Please enter the command: ");
		uint16_t cmd = 0;
		int result = scanf("%hu", &cmd);
		if (result != 1) {
			puts("Invalid input\n");
			rewind(stdin);
			continue;
		}

		switch (cmd) {
		case 1:
			list_files();
			break;
		case 2:
			add_file();
			break;
		default:
			puts("Invalid command\n");
			break;
		}
	}
}