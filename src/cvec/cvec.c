#include "cvec.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static void cvec_debug(cvec* vec, const char* format);

cvec* cvec_new() {
	cvec* temp = (cvec*)malloc(sizeof(cvec));
	temp->capacity = 0;
	temp->size = 0;
	temp->items = (void**)malloc(0);
	return temp;
}

cvec* cvec_from(int count, ...) {
    cvec* temp = cvec_new();
    va_list list;
    va_start(list, count);

    for (int i = 0; i < count; i++)
        cvec_push(temp, va_arg(list, void*));

    va_end(list);
    return temp;
}

void* cvec_front(cvec* vec) {
	if (!vec || vec->size < 1) return NULL;
	return vec->items[vec->size - 1];
}

void* cvec_back(cvec* vec) {
	if (!vec || vec->size < 1) return NULL;
	return vec->items[0];
}

int cvec_insert(cvec* vec, unsigned int index, void* item) {
	if (!vec || vec->size < index) return false;
	else if (vec->size == index) {
		cvec_push(vec, item);
		return true;
	}

	cvec_resize(vec, (vec->size + 1) + sizeof(item));
	for (int i = vec->size; i > index; i--)
		vec->items[i] = vec->items[i - 1];
	vec->items[index] = item;
	vec->size++;

	return true;
}

int cvec_size(cvec* vec) {
	if (vec != NULL) return vec->size;
	return 0;
}

int cvec_remove(cvec* vec, unsigned int index) {
    if (!vec || vec->size <= index || vec->size < 1) return false;
    for (int i = index; i < vec->size - 1; i++)
        vec->items[i] = vec->items[i + 1];

    vec->size--;
    cvec_resize(vec, vec->size * sizeof(void*));
    return true;
}

int cvec_clean(cvec* vec) {
    if (!vec || vec->size < 1) return false;
    cvec_resize(vec, 0);
    vec->size = 0;
    return true;
}

static void cvec_debug(cvec* vec, const char* format) {
    if (vec->size < 1) {
        printf("[]\n");
        return;
    }

    printf("[");
    for (int i = 0; i < vec->size - 1; i++) {
        printf(format, vec->items[i]);
        printf(" ");
    }

    printf(format, cvec_front(vec));
    printf("]\n");
}

int cvec_resize(cvec* vec, unsigned int capacity) {
	if (!vec) return false;
	if (vec->items == NULL) vec->items = (void**)malloc(capacity);
	else {
		vec->items = (void**)realloc(vec->items, capacity);
		vec->capacity = capacity;
	}
	return true;
}

int cvec_push(cvec* vec, void *item) {
	if (!vec || !item) return false;

	cvec_resize(vec, (vec->size + 1) * sizeof(item));
	vec->items[vec->size] = item;
	vec->size++;

	return true;
}

int cvec_free(cvec* vec) {
	if (!vec || !vec->items || vec->size < 1) return false;

	free(vec->items);
	free(vec);

	vec->items = NULL;
	vec = NULL;

	return true;
}

int cvec_pop(cvec* vec) {
	if (vec->size < 1) return false;
	vec->size--;
	cvec_resize(vec, (vec->size) * sizeof(void*));
	return true;
}

void* cvec_at(cvec* vec, int index) {
    if (!vec || vec->size <= index) return NULL;
	return vec->items[index];
}

void* cvec_iter(cvec* vec) {
    if (vec->size < 1) return NULL;
	static int count = 0;
	if (count >= vec->size)
		count = 0;

	return vec->items[count++];
}