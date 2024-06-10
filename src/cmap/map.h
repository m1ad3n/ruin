#ifndef MAP_H
#define MAP_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pair pair_t;
struct pair {
	void* first;
	void* second;
};

typedef struct map map_t;
struct map {
	pair_t** items;
	size_t size;
};

pair_t* pair_new();
pair_t* pair_from(void* __first, void* __second);
int pair_compare(pair_t* __first_pair, pair_t* __second_pair);
void pair_set(pair_t* __pair, void* __first, void* __second);
void pair_print(pair_t* __pair, const char* __format);
void pair_free(pair_t* __pair);
void pair_swap(pair_t* __first_pair, pair_t* __second_pair);

map_t* map_new();
pair_t* map_append_from(map_t* __map, void* __key, void* __value);
void map_append(map_t* __map, pair_t* __pair);
pair_t* map_at(map_t* __map, void* __key);
pair_t* map_insert_from(map_t* __map, void* __key, void* __value);
void map_insert(map_t* __map, pair_t* __pair);
pair_t* map_begin(map_t* __map);
pair_t* map_end(map_t* __map);
pair_t* map_extract(map_t* __map, void* __key);
size_t map_size(map_t* __map);
int map_contains(map_t* __map, void* __key);
int map_assign(map_t* __map, void* __key, void* __value);
int map_remove(map_t* __map, void* __key);
int map_remove_pair(map_t* __map, pair_t* __pair);
int map_empty(map_t* __map);
void map_clear(map_t* __map);
void map_erase(map_t* __map);
void map_print(map_t* __map, const char* __format);
void map_free(map_t* __map);

#ifdef __cplusplus
}
#endif

#endif
