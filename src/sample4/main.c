#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <memory.h>
#include <stdint.h>

typedef struct {
    size_t size;
    size_t capacity;
    size_t element_size;
    char memory[0];
} vector_t;

vector_t* make_vector(size_t element_size, size_t capacity) {
    vector_t* v = malloc(sizeof(vector_t) + element_size * capacity);
    v->element_size = element_size;
    v->capacity = capacity;
    v->size = 0;
    return v;
}

int reserve(vector_t** vector, size_t capacity) {
    if (capacity < 1) {
        capacity = 1;
    }
    if ((*vector)->capacity >= capacity) {
        return 0;
    }
    vector_t* new_vector = realloc(
        vector,
        capacity * (*vector)->element_size + sizeof(*vector)
    );
    if (new_vector != NULL) {
        new_vector->capacity = capacity;
        *vector = new_vector;
        return 0;
    }
    return -1;
}

void* get_ith(vector_t* vector, size_t i) {
    return vector->memory + vector->element_size * i;
}

void* push_back(vector_t** vector, void* element) {
    if ((*vector)->size == (*vector)->capacity) {
        reserve(vector, (*vector)->capacity << 1);
    }
    void* ptr = get_ith((*vector), (*vector)->size);
    memcpy(ptr, element, (*vector)->element_size);
    (*vector)->size++;
    return ptr;
}

void clear_vector(vector_t* vector) {
    free(vector);
}

int main() {
    char s[] = "abc";
    char m[1024];
    vector_t* v = (vector_t*)m;
    v->capacity = 16;
    v->element_size = sizeof(char*);
    v->size = 0;

    push_back(&v, s);
    push_back(&v, s);
    push_back(&v, s);

    printf("%ld\n", (size_t)v->memory);

    printf("%s\n", (char*)get_ith(v, 2));
    
    return 0;
}
