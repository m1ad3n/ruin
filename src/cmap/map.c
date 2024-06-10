#include <stdlib.h>
#include <stdio.h>
#include "map.h"

pair_t* pair_new() {
	pair_t* temp = (pair_t*)malloc(sizeof(pair_t));
	temp->first = (void*)0;
	temp->second = (void*)0;
	return temp;
}

pair_t* pair_from(void* __first, void* __second) {
	pair_t* temp = (pair_t*)malloc(sizeof(pair_t));
	temp->first = __first;
	temp->second = __second;
	return temp;
}

int pair_compare(pair_t* __first_pair, pair_t* __second_pair) {
	if (!__first_pair || !__second_pair) return 0;
	if (__first_pair->first == __second_pair->first && __first_pair->second == __second_pair->second)
		return 1;
	else
		return 0;
}

void pair_set(pair_t* __pair, void* __first, void* __second) {
	if (!__pair) return;
	__pair->first = __first;
	__pair->second = __second;
}

void pair_print(pair_t* __pair, const char* __format) {
	if (!__pair || !__format) return;
	printf(__format, __pair->first, __pair->second);
}

void pair_swap(pair_t* __first_pair, pair_t* __second_pair) {
	if (!__first_pair || !__second_pair) return;
	pair_t temp = *__first_pair;
	*__first_pair = *__second_pair;
	*__second_pair = temp;
}

void pair_free(pair_t* __pair) {
	if (!__pair) return;
	free(__pair);
	__pair = NULL;
}

map_t* map_new() {
	map_t* temp = (map_t*)malloc(sizeof(map_t));
	temp->size = 0;
	temp->items = (pair_t**)malloc(0);
	return temp;
}

static int map_resize(map_t* __map, size_t capacity) {
	if (!__map) return 0;
	__map->items = (!__map->items) ?
			(pair_t**)malloc(capacity)
			: (pair_t**)realloc(__map->items, capacity);
	return 1;
}

pair_t* map_append_from(map_t* __map, void* __key, void* __value) {
	if (!__map) return NULL;
	pair_t* pair_temp = pair_from(__key, __value);
	map_resize(__map, (__map->size + 1) * sizeof(pair_t*));
	*(__map->items + __map->size++) = pair_temp;
	return pair_temp;
}

void map_append(map_t* __map, pair_t* __pair) {
	if (!__map) return;
	map_resize(__map, (__map->size + 1) * sizeof(pair_t*));
	__map->items[__map->size++] = __pair;
}

pair_t* map_insert_from(map_t* __map, void* __key, void* __value) {
	return map_append_from(__map, __key, __value);
}

void map_insert(map_t* __map, pair_t* __pair) {
	map_append(__map, __pair);
}

pair_t* map_extract(map_t* __map, void* __key) {
	if (!__map || !__key) return NULL;
	pair_t* temp = pair_new();
	for (size_t i = 0; i < __map->size; i++) {
		if (__map->items[i]->first == __key) {
			pair_set(temp, __map->items[i]->first, __map->items[i]->second);
			map_remove(__map, __key);
			return temp;
		}
	}
	return NULL;
}

pair_t* map_at(map_t* __map, void* __key) {
	if (!__map || !__map->items) return NULL;
	for (int i = 0; i < __map->size; i++)
		if (__map->items[i]->first == __key)
			return __map->items[i];

	return NULL;
}

pair_t* map_begin(map_t* __map) {
	if (!__map || !__map->size) return NULL;
	return __map->items[0];
}

pair_t* map_end(map_t* __map) {
	if (!__map || !__map->size) return NULL;
	return __map->items[__map->size];
}

int map_contains(map_t* __map, void* __key) {
	if (!__map || __key || !__map->size) return 0;
	for (size_t i = 0; i < __map->size; i++) {
		if (__map->items[i]->first == __key)
			return 1;
	}

	return 0;
}

size_t map_size(map_t* __map) {
	if (!__map) return 0;
	return __map->size;
}

void map_print(map_t* __map, const char* __format) {
	if (!__map || !__format) return;
	for (size_t i = 0; i < __map->size; i++) {
		pair_print(__map->items[i], __format);
	}
}

int map_remove(map_t* __map, void* __key) {
	if (!__map || !__key) return 0;
	for (size_t i = 0; i < __map->size; i++) {
		if (__map->items[i]->first == __key) {
			if (i == (__map->size - 1)) pair_free(__map->items[i]);
			else pair_swap(__map->items[i], __map->items[__map->size - 1]);

			map_resize(__map, --__map->size * sizeof(pair_t));
			return 1;
		}
	}
	return 0;
}

int map_remove_pair(map_t* __map, pair_t* __pair) {
	if (!__map || !__pair) return 0;

	for (size_t i = 0; i < __map->size; i++) {
		if (pair_compare(__pair, __map->items[i])) {
			if (i == (__map->size - 1)) pair_free(__map->items[i]);
			else pair_swap(__map->items[i], __map->items[__map->size - 1]);

			map_resize(__map, --__map->size * sizeof(pair_t));
			return 1;
		}
	}

	return 0;
}

int map_assign(map_t* __map, void* __key, void* __value) {
	if (!__map || !__key || !__value) return 0;
	for (size_t i = 0; i < __map->size; i++) {
		if (__map->items[i]->first == __key) {
			__map->items[i]->second = __value;
			return 1;
		}
	}
	return 0;
}

int map_empty(map_t* __map) {
	if (!__map || !__map->size) return 1;
	return 0;
}

void map_clear(map_t* __map) {
	if (!__map) return;
	for (size_t i = 0; i < __map->size; i++)
		pair_free(__map->items[i]);
	__map->size = 0;
	__map->items = (pair_t**)realloc(__map->items, 0);
}

void map_erase(map_t* __map) {
	map_clear(__map);
}

void map_free(map_t* __map) {
	if (!__map) return;

	for (size_t i = 0; i < __map->size; i++)
		pair_free(__map->items[i]);

	free(__map->items);
	free(__map);

	__map = NULL;
}
