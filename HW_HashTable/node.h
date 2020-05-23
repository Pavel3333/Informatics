#pragma once

#include <stdint.h>

#include "error.h"

typedef enum gender {
    G_Male,
    G_Female,

    G_SIZE
} Gender;

const char* GenderDesc[G_SIZE];

typedef struct student {
    char* first_name;
    char* last_name;
    Gender gender;
    float height;
    float weight;
    uint16_t group;
    float average_score;
} Student;

typedef struct node {
    char* key;
    Student student;
    struct node* next;
} Node;

Error getGenderByDesc(const char* src, Gender* dst);

Error newStudent(
    Student* dest,
    char* first_name,
    char* last_name,
    Gender gender,
    float height,
    float weight,
    uint16_t group,
    float average_score
);
Error loadStudent(FILE* file, Student* dest);
Error dumpStudent(FILE* file, Student* student);
void  delStudent(Student* student);
void  printStudent(Student* student, uint16_t indent);

void delNode(Node* node);
void printNode(Node* node, uint16_t indent);