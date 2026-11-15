#include <stdlib.h>
#include <stdbool.h>

// General Macros
#define GET_ARG1(a, ...) a
#define GET_ARG2(a, b, ...) b
#define GET_ARG3(a, b, c, ...) c
#define EXPAND(macro, ...) macro(__VA_ARGS__)
#define _CAT(A, B) A##B
#define CAT(A, B) _CAT(A, B)
// End General Macros

// Dynamic Arrays
#define APOLLO_ARR_INIT_CAPACITY 1

// Protect the header vs multiple includes
#ifndef APOLLO_ARR_HEADER
#define APOLLO_ARR_HEADER
typedef struct Apollo_arr_header {
	int count;
	int capacity;
} Apollo_arr_header;

#define GET_APOLLO_ARR_HEADER(arr) ((Apollo_arr_header *)(arr) - 1)
#endif

#define apollo_arr_init(arr, cap, type_size)												\
	{																						\
		Apollo_arr_header *header = malloc(sizeof(Apollo_arr_header) + cap * type_size);	\
																							\
		header->count = 0;																	\
		header->capacity = cap;																\
		(arr) = (void *)(header + 1);														\
	}																						\

#define apollo_arr_push(arr, v)																			\
	{																									\
		if ((arr) == NULL) {																			\
			apollo_arr_init(arr, APOLLO_ARR_INIT_CAPACITY, sizeof(*(arr)));								\
		}																								\
		Apollo_arr_header *header = GET_APOLLO_ARR_HEADER(arr);											\
																										\
		if (header->capacity <= header->count) {														\
			header->capacity *= 2;																		\
			header = realloc(header, sizeof(Apollo_arr_header) + header->capacity * sizeof(*(arr)));	\
			(arr) = (void *)(header + 1);																\
		}																								\
		(arr)[header->count++] = (v);																	\
	}

#define apollo_arr_pop(arr)											\
	{																\
		Apollo_arr_header *header = GET_APOLLO_ARR_HEADER(arr);		\
																	\
		header->count--;											\
	}

#define apollo_arr_at(arr, i) (arr)[i]

#define apollo_arr_front(arr) apollo_arr_at(arr, 0)

#define apollo_arr_back(arr) apollo_arr_at(arr, GET_APOLLO_ARR_HEADER(arr)->count - 1)

#define apollo_arr_size(arr) GET_APOLLO_ARR_HEADER(arr)->count

#define apollo_arr_empty(arr) arr == NULL ? true : false

#define apollo_arr_free(arr) free(GET_APOLLO_ARR_HEADER(arr))
// End Dynamic Arrays

// Stack (LIFO)
#define apollo_stack_push(stack, v) apollo_arr_push(stack, v)

#define apollo_stack_pop(stack) apollo_arr_pop(stack)

#define apollo_stack_top(stack) apollo_arr_back(stack)

#define apollo_stack_size(stack) apollo_arr_size(stack)

#define apollo_stack_empty(stack) apollo_arr_empty(stack)

#define apollo_stack_free(stack) apollo_arr_free(stack)
// End Stack (LIFO)


// List
// This is to stop the lsp going crazy
#ifndef T
#define T IList, int
#endif
//

#define LIST_NAME EXPAND(GET_ARG1, T)
#define LIST_TYPE EXPAND(GET_ARG2, T)

typedef struct LIST_NAME {
	LIST_TYPE data;
	struct LIST_NAME *next;
} LIST_NAME;

static void apollo_list_push_back(LIST_NAME **list, LIST_TYPE v) {
	LIST_NAME *ptr = (LIST_NAME *)malloc(sizeof(LIST_NAME));

	ptr->data = v;
	if (*list == NULL) {
		ptr->next = ptr;
	} else {
		ptr->next = (*list)->next;
		(*list)->next = ptr;
	}
	*list = ptr;
}

static void apollo_list_push_front(LIST_NAME **list, LIST_TYPE v) {
	LIST_NAME *ptr = (LIST_NAME *)malloc(sizeof(LIST_NAME));
	
	ptr->data = v;
	if (*list == NULL) {
		ptr->next = ptr;
		*list = ptr;
		return;
	}
	ptr->next = (*list)->next;
	(*list)->next = ptr;
}

static void apollo_list_pop_front(LIST_NAME **list) {
	if (*list == NULL) {
		return;
	}
	LIST_NAME *tmp = (*list)->next;

	(*list)->next = tmp->next;
	if (*list == tmp) {
		*list = NULL;
	}
	free(tmp);
}

static void apollo_list_pop_back(LIST_NAME **list) {
	if (*list == NULL) {
		return;
	}
	LIST_NAME *tmp = *list;

	while (tmp->next != *list) {
		tmp = tmp->next;
	}
	tmp->next = (*list)->next;
	if (*list == tmp) {
		tmp = NULL;
	}
	free(*list);
	(*list) = tmp;
}

static size_t apollo_list_size(LIST_NAME *list) {
	void *last = list;
	size_t size = 0;

	if (list == NULL) {
		return (size);
	}
	while (list->next != last) {
		size++;
		list = list->next;
	}
	return (size + 1);
}

static LIST_NAME *apollo_list_front(LIST_NAME *list) {
	return (list->next);
}

static LIST_NAME *apollo_list_back(LIST_NAME *list) {
	return (list);
}

static bool apollo_list_empty(LIST_NAME *list) {
	return (list == NULL ? true : false);
}

static void apollo_list_free(LIST_NAME **list) {
	LIST_NAME *last = *list;

	while ((*list)->next != last) {
		LIST_NAME *tmp = *list;
		*list = (*list)->next;
		free(tmp);
	}
	free(*list);
	*list = NULL;
}

static void apollo_list_free_cb(LIST_NAME **list, void (*cb)(LIST_NAME *)) {
	LIST_NAME *last = *list;

	while ((*list)->next != last) {
		LIST_NAME *tmp = *list;
		*list = (*list)->next;
		cb(tmp);
	}
	cb(*list);
	*list = NULL;
}
// End List

// Hashmap
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Same thing as lists
#ifndef HM
#define HM Map, char *, int
#endif
//

#define APOLLO_HASHMAP_INIT_CAPACITY 100

#define HASHMAP_NAME EXPAND(GET_ARG1, HM)
#define HASHMAP_KEY_TYPE EXPAND(GET_ARG2, HM)
#define HASHMAP_VALUE_TYPE EXPAND(GET_ARG3, HM)

//#define HASHMAP_FUNC(name) CAT(apollo_hashmap_, CAT(HASHMAP_NAME, name))

typedef struct HASHMAP_NAME {
	HASHMAP_KEY_TYPE key;
	HASHMAP_VALUE_TYPE data;
} HASHMAP_NAME;

static int hash(HASHMAP_KEY_TYPE key) {
	if (sizeof(HASHMAP_KEY_TYPE) < 8) {
		int index = key * 5;
		
		index <<= 2;
		index ^= key;
		return (index % 100);
	}
	uintptr_t nbr;
	uintptr_t index = 0;

	memcpy(&nbr, &key, sizeof(HASHMAP_KEY_TYPE));
	//printf("ptr: %ld\n", nbr);
	index = nbr * 5;
	//printf("Mult: %ld\n", index);
	index ^= nbr;
	//printf("XOR: %ld\n", index);
	index <<= 2;
	index ^= nbr >> 2;
	//printf("2XOR: %ld\n", index);
	return (index % 100);
}

static void apollo_hashmap_push(HASHMAP_NAME **hmap, HASHMAP_KEY_TYPE key, HASHMAP_VALUE_TYPE data) {
	if (*hmap == NULL) {
		apollo_arr_init(*hmap, APOLLO_HASHMAP_INIT_CAPACITY, sizeof(HASHMAP_NAME));
	}
	Apollo_arr_header *header = GET_APOLLO_ARR_HEADER(*hmap);
	int index = hash(key);
	//printf("Map: %c -> %d. Index %d\n", key, data, index);
	(*hmap)[index].key = key;
	(*hmap)[index].data = data;
	header->count++;
}

static HASHMAP_VALUE_TYPE apollo_hashmap_get(HASHMAP_NAME *hmap, HASHMAP_KEY_TYPE key) {
	Apollo_arr_header *header = GET_APOLLO_ARR_HEADER(hmap);

	int index = hash(key);
	printf("Index %d\n", index);
	return (hmap[index].data);
}

static size_t apollo_hashmap_size(HASHMAP_NAME *hmap) {
	return (apollo_arr_size(hmap));
}

static bool apollo_hashmap_empty(HASHMAP_NAME *hmap) {
	return (apollo_arr_empty(hmap));
}

static void apollo_hashmap_free(HASHMAP_NAME *hmap) {
	apollo_arr_free(hmap);
}
// End Hashmap

