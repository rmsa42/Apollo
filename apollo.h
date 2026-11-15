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
#ifndef L
#define L IList, int
#endif
//

#define LIST_NAME EXPAND(GET_ARG1, L)
#define LIST_TYPE EXPAND(GET_ARG2, L)

#define LIST_FUNC_DECL(name) CAT(list_, CAT(LIST_NAME, name))

typedef struct LIST_NAME {
	LIST_TYPE data;
	struct LIST_NAME *next;
} LIST_NAME;

static void LIST_FUNC_DECL(_push_back)(LIST_NAME **list, LIST_TYPE v) {
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

static void LIST_FUNC_DECL(_push_front)(LIST_NAME **list, LIST_TYPE v) {
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

static void LIST_FUNC_DECL(_pop_front)(LIST_NAME **list) {
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

static void LIST_FUNC_DECL(_pop_back)(LIST_NAME **list) {
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

static size_t LIST_FUNC_DECL(_size)(LIST_NAME *list) {
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

static LIST_NAME *LIST_FUNC_DECL(_front)(LIST_NAME *list) {
	return (list->next);
}

static LIST_NAME *LIST_FUNC_DECL(_back)(LIST_NAME *list) {
	return (list);
}

static bool LIST_FUNC_DECL(_empty)(LIST_NAME *list) {
	return (list == NULL ? true : false);
}

static void LIST_FUNC_DECL(_free)(LIST_NAME **list) {
	LIST_NAME *last = *list;

	while ((*list)->next != last) {
		LIST_NAME *tmp = *list;
		*list = (*list)->next;
		free(tmp);
	}
	free(*list);
	*list = NULL;
}

static void LIST_FUNC_DECL(_free_cb)(LIST_NAME **list, void (*cb)(LIST_NAME *)) {
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

#define APOLLO_HASHMAP_INIT_CAPACITY 10

#define HASHMAP_NAME EXPAND(GET_ARG1, HM)
#define HASHMAP_KEY_TYPE EXPAND(GET_ARG2, HM)
#define HASHMAP_VALUE_TYPE EXPAND(GET_ARG3, HM)

#define HASHMAP_FUNC_DECL(name) CAT(hmap_, CAT(HASHMAP_NAME, name))

typedef struct HASHMAP_NAME {
	HASHMAP_KEY_TYPE key;
	HASHMAP_VALUE_TYPE data;
} HASHMAP_NAME;

#ifndef HASHMAP_HASH_FUNC
#define HASHMAP_HASH_FUNC
static int hash_4B(uint32_t k) {
	int index = k * 5;

	index <<= 2;
	index ^= k;
	return (index);
}

static int hash_8B(uint64_t k) {
	int index = k * 5;

	index ^= k;
	index <<= 2;
	index ^= k >> 2;
	return (index);
}

static int hash(void *key, size_t type_size) {
	if (type_size < 8) {
		uint32_t k = 0;
		memcpy(&k, key, type_size);
		return (hash_4B(k));
	}
	uint64_t k = 0;
	memcpy(&k, key, sizeof(uintptr_t));\
	return (hash_8B(k));\
}
#endif

static void HASHMAP_FUNC_DECL(_push)(HASHMAP_NAME **hmap, HASHMAP_KEY_TYPE key, HASHMAP_VALUE_TYPE data) {
	if (*hmap == NULL) {
		apollo_arr_init(*hmap, APOLLO_HASHMAP_INIT_CAPACITY, sizeof(HASHMAP_NAME));
	}
	Apollo_arr_header *header = GET_APOLLO_ARR_HEADER(*hmap);
	int index = hash((void *)&key, sizeof(HASHMAP_KEY_TYPE)) % header->capacity;
	(*hmap)[index].key = key;
	(*hmap)[index].data = data;
	header->count++;
}

static HASHMAP_VALUE_TYPE HASHMAP_FUNC_DECL(_get)(HASHMAP_NAME *hmap, HASHMAP_KEY_TYPE key) {
	Apollo_arr_header *header = GET_APOLLO_ARR_HEADER(hmap);

	int index = hash((void *)&key, sizeof(HASHMAP_KEY_TYPE)) % header->capacity;
	return (hmap[index].data);
}

static size_t HASHMAP_FUNC_DECL(_size)(HASHMAP_NAME *hmap) {
	return (apollo_arr_size(hmap));
}

static bool HASHMAP_FUNC_DECL(_empty)(HASHMAP_NAME *hmap) {
	return (apollo_arr_empty(hmap));
}

static void HASHMAP_FUNC_DECL(_free)(HASHMAP_NAME *hmap) {
	apollo_arr_free(hmap);
}
// End Hashmap

