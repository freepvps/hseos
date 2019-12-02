#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <memory.h>
#include <stdint.h>

typedef struct vector {
    size_t size;
    size_t capacity;
    size_t element_size;
    void* memory;
} vector_t;

vector_t make_vector(size_t element_size) {
    vector_t v = {0, 0, element_size, NULL};
    return v;
}

void reserve(vector_t* vector, size_t capacity) {
    if (capacity < 1) {
        capacity = 1;
    }
    if (vector->capacity >= capacity) {
        return;
    }
    if (vector->memory == NULL) {
        vector->memory = malloc(capacity * vector->element_size);
    } else {
        vector->memory = realloc(vector->memory, capacity * vector->element_size);
    }
    vector->capacity = capacity;
}

void* get_ith(vector_t* vector, size_t i) {
    return vector->memory + vector->element_size * i;
}

void push_back(vector_t* vector, void* element) {
    if (vector->size == vector->capacity) {
        reserve(vector, vector->capacity << 1);
    }
    void* ptr = get_ith(vector, vector->size);
    memcpy(ptr, element, vector->element_size);
    vector->size++;
}

void clear_vector(vector_t* vector) {
    if (vector->memory) {
        free(vector->memory);
        vector->capacity = vector->size = 0;
    }
}

typedef struct {
    int* counter_destination;
    int value1;
    int value2;
} counter_t;


void strings() {
    const char
        * const s1 = "abc",
        c2 = 'x',
        c3 = 'z',
        c4 = '\0'
    ;
    printf("%s\n", s1);
    printf("%s\n", &c2);
}

void malloc_big() {
    size_t n = 1ULL << 30;
    void* mem = malloc(n);
    printf("memory: %p\n", mem);
    int x;
    scanf("%d", &x);
    memset(mem, x, n);
    while (1);
}


int main() {
//    strings();
    malloc_big();
    return 0;
    char s[] = "abc";
    vector_t v = make_vector(sizeof(s));
    push_back(&v, s);
    push_back(&v, s);
    push_back(&v, s);

    printf("%ld\n", (size_t)v.memory);

    printf("%s\n", (char*)get_ith(&v, 2));
    clear_vector(&v);
    
    return 0;
}
