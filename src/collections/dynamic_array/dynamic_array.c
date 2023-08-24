// #include <stdbool.h>
// #include <stdint.h>
// #include <stdlib.h>
// #include <string.h>

#include "dynamic_array.h"

DynamicArray *create_dyn_array(size_t element_size, size_t array_size,
                               SearchCallback search_callback,
                               DropElement drop_element) {
    DynamicArray *dyn_array = malloc(sizeof(*dyn_array));

    if (dyn_array == NULL) {
        return NULL;
    }

    dyn_array->array = malloc(element_size * array_size);

    if (dyn_array->array == NULL) {
        free(dyn_array);
        return NULL;
    }

    dyn_array->current = 0;
    dyn_array->max = (size_t)array_size;
    dyn_array->drop_element = drop_element;
    dyn_array->search_callback = search_callback;

    return dyn_array;
}

void drop_all_elements(DynamicArray *dyn_array) {
    for (size_t i = 0; i < dyn_array->current; ++i) {
        dyn_array->drop_element(dyn_array->array[i]);
    }
}

void drop_dyn_array(DynamicArray *dyn_array) {
    if (dyn_array->drop_element) {
        drop_all_elements(dyn_array);
    }

    free(dyn_array->array);

    free(dyn_array);
}

bool insert_dyn_array(DynamicArray *dyn_array, void *element) {
    if ((dyn_array->current + 1) == dyn_array->max) {
        size_t new_size = dyn_array->max * MULTIPLIER;

        void *new_array = realloc(dyn_array->array, sizeof(void *) * new_size);

        if (new_array == NULL) {
            return false;
        }

        dyn_array->array = new_array;
        dyn_array->max = new_size;
    }

    dyn_array->array[dyn_array->current] = element;

    ++dyn_array->current;

    return true;
}

/** search an array for the first element
 *
 * @return 0 to size_t::max is the index to the element
 * @return -1 did not find the element
 * @return -2 missing the search function
 */
int search_dyn_array(DynamicArray *dyn_array, void *to_find) {
    if (dyn_array->search_callback == NULL) {
        return -2;
    }

    size_t index = -1;

    for (size_t i = 0; i < dyn_array->current; ++i) {
        if (dyn_array->search_callback(to_find, dyn_array->array[i])) {
            index = i;
            break;
        }
    }

    return index;
}

void *get_element_dyn_array(DynamicArray *dyn_array, size_t index) {
    if (index <= -1 || index >= dyn_array->current) {
        return NULL;
    }

    return dyn_array->array[index];
}
