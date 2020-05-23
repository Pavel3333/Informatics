#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"

#include "io.h"
#include "interface.h"

const char* GenderDesc[G_SIZE] = {
    "Man",
    "Woman"
};

// Internal functions. Trading safety for speed

Error _checkStudent(char* first_name, char* last_name, Gender gender) {
    if (!first_name || !last_name)
        return E_InvalidStudentFields;

    if (gender < 0 || gender >= G_SIZE)
        return W_GenderNotFound;

    return E_Success;
}

static Error _loadFileString(FILE* file, char** dest) {
    Error err;
    size_t readed, string_size;

    readed = fread(&string_size, sizeof(string_size), 1, file);
    if (readed != 1)
        return W_EOF;

    char* string = malloc(string_size + 1);
    if (!string)
        return E_MemoryAlloc;

    readed = fread(string, sizeof(char), string_size, file);
    if (readed != string_size) {
        err = W_EOF;
        goto _LFS_err_1;
    }

    string[string_size] = '\0'; // NULL-terminator

    *dest = string;
    return E_Success;

    // Error occured
_LFS_err_1:
    free(string);
    return err;
}

#define fread_check(buffer, size, count, io) \
    readed = fread(buffer, size, count, io); \
    if (readed != count)                     \
        return W_EOF;

static Error _loadStudentPODFields(FILE* file, Student* dest) {
    size_t readed;

    fread_check(&(dest->gender),        sizeof(dest->gender),        1, file);
    fread_check(&(dest->height),        sizeof(dest->height),        1, file);
    fread_check(&(dest->weight),        sizeof(dest->weight),        1, file);
    fread_check(&(dest->group),         sizeof(dest->group),         1, file);
    fread_check(&(dest->average_score), sizeof(dest->average_score), 1, file);

    return E_Success;
}

// Public functions

Error getGenderByDesc(const char* src, Gender* dst) {
    if (!src || !dst)
        return E_NullArg;

    for (Gender g = 0; g < G_SIZE; g++) {
        if (strcmp(src, GenderDesc[g]))
            continue;

        *dst = g;
        return E_Success;
    }

    return W_GenderNotFound;
}

Error newStudent(
    Student* dest,
    char* first_name,
    char* last_name,
    Gender gender,
    float height,
    float weight,
    uint16_t group,
    float average_score
) {
    if (!dest)
        return E_NullArg;

    Error err;

    err = _checkStudent(first_name, last_name, gender);
    if (!is_success(err))
        return err;

    Student new_student = { 0 };

    size_t first_name_size = strlen(first_name) + 1;
    size_t last_name_size  = strlen(last_name)  + 1;

    new_student.first_name = malloc(sizeof(char) * first_name_size);
    if (!new_student.first_name)
        return E_MemoryAlloc;

#ifdef _WIN32
    strcpy_s(new_student.first_name, first_name_size, first_name);
#else
    strncpy(new_student.first_name, first_name, first_name_size);
#endif

    new_student.last_name = malloc(sizeof(char) * last_name_size);
    if (!new_student.last_name) {
        err = E_MemoryAlloc;
        goto NS_err_1;
    }

#ifdef _WIN32
    strcpy_s(new_student.last_name, last_name_size, last_name);
#else
    strncpy(new_student.last_name, last_name, last_name_size);
#endif

    new_student.gender = gender;
    new_student.height = height;
    new_student.weight = weight;
    new_student.group  = group;
    new_student.average_score     = average_score;

    *dest = new_student;

    return E_Success;

    // Error occured
NS_err_1:
    free(new_student.first_name);
    return err;
}

Error loadStudent(FILE* file, Student* dest) {
    if (!file || !dest)
        return E_NullArg;

    Error   err;
    Student student;

    err = _loadFileString(file, &student.first_name);
    if (!is_success(err))
        return err;

    err = _loadFileString(file, &student.last_name);
    if (!is_success(err))
        goto LS_err_1;

    err = _loadStudentPODFields(file, &student);
    if (!is_success(err))
        goto LS_err_2;

    *dest = student;
    return E_Success;

    // Error occured
LS_err_2:
    free(student.last_name);
LS_err_1:
    free(student.first_name);
    return err;
}

Error dumpStudent(FILE* file, Student* student) {
    if (!file)
        return E_NullArg;

    Error err;

    err = _checkStudent(student->first_name, student->last_name, student->gender);
    if (!is_success(err))
        return err;

    size_t first_name_size = strlen(student->first_name);
    size_t last_name_size  = strlen(student->last_name);

    fwrite(&first_name_size,          sizeof(char), sizeof(first_name_size), file);
    fwrite(student->first_name,       sizeof(char), first_name_size,         file);

    fwrite(&last_name_size,           sizeof(char), sizeof(last_name_size), file);
    fwrite(student->last_name,        sizeof(char), last_name_size,         file);

    fwrite(&(student->gender),        sizeof(student->gender),        1, file);
    fwrite(&(student->height),        sizeof(student->height),        1, file);
    fwrite(&(student->weight),        sizeof(student->weight),        1, file);
    fwrite(&(student->group),         sizeof(student->group),         1, file);
    fwrite(&(student->average_score), sizeof(student->average_score), 1, file);

    return E_Success;
}

void delStudent(Student* student) {
    if (!student)
        return;

    free(student->first_name);
    free(student->last_name);
}

void printStudent(Student* student, uint16_t indent) {
    if (!student)
        return;

    printIndent(indent);
    puts("Student data: {");

    printIndent(indent + INDENT_SIZE);
    printf("First name: %s,\n", student->first_name);

    printIndent(indent + INDENT_SIZE);
    printf("Last name: %s\n", student->last_name);

    printIndent(indent + INDENT_SIZE);
    printf("Gender: %s\n", GenderDesc[student->gender]);

    printIndent(indent + INDENT_SIZE);
    printf("Height: %f\n", student->height);

    printIndent(indent + INDENT_SIZE);
    printf("Weight: %f\n", student->weight);

    printIndent(indent + INDENT_SIZE);
    printf("Group: %hu\n", student->group);

    printIndent(indent + INDENT_SIZE);
    printf("Average score: %f\n", student->average_score);

    printIndent(indent);
    puts("}");
}


void delNode(Node* node) {
    if (!node)
        return;

    free(node->key);
    delStudent(&(node->student));

    free(node);
}

void printNode(Node* node, uint16_t indent) {
    printIndent(indent);

    if (!node) {
        puts("Undefined node");
        return;
    }

    puts("Node: {");

    printIndent(indent + INDENT_SIZE);
    printf("Key: \"%s\"\n", node->key);

    printStudent(&(node->student), indent + INDENT_SIZE);

    printIndent(indent);
    puts("}");
}