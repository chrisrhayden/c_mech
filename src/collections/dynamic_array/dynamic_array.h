#ifndef C_DYNAMIC_ARRAY
#define C_DYNAMIC_ARRAY

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MULTIPLIER (2)

typedef bool (*SearchCallback)(void *to_find, void *element);

typedef void (*DropElement)(void *element);

typedef struct {
    size_t max;
    size_t current;
    DropElement drop_element;
    SearchCallback search_callback;
    void **array;
} DynamicArray;

DynamicArray *create_dyn_array(size_t element_size, size_t array_size,
                               SearchCallback search_callback,
                               DropElement drop_element);

void drop_dyn_array(DynamicArray *dyn_array);

bool insert_dyn_array(DynamicArray *dyn_array, void *element);

int search_dyn_array(DynamicArray *dyn_array, void *to_find);

// void *remove_dyn_array(DynamicArray *dyn_array, size_t index);

void *get_element_dyn_array(DynamicArray *dyn_array, size_t index);

#endif
