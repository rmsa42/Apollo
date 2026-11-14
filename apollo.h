#pragma once

#include <stdlib.h>
#include <stdbool.h>

// Core & Dynamic Array

#define APOLLO_INIT_CAPACITY 1

typedef struct Apollo_arr_header {
	int count;
	int capacity;
} Apollo_arr_header;

#define apollo_arr_push(arr, v)																						\
	{																												\
		if ((arr) == NULL) {																						\
			Apollo_arr_header *header = malloc(sizeof(Apollo_arr_header) + APOLLO_INIT_CAPACITY * sizeof(*(arr)));	\
			header->count = 0;																						\
			header->capacity = APOLLO_INIT_CAPACITY;																\
			(arr) = (void *)(header + 1);																			\
		}																											\
		Apollo_arr_header *header = (Apollo_arr_header *)(arr) - 1;													\
																													\
		if (header->capacity <= header->count) {																	\
			header->capacity *= 2;																					\
			header = realloc(header, sizeof(Apollo_arr_header) + header->capacity * sizeof(*(arr)));				\
			(arr) = (void *)(header + 1);																			\
		}																											\
		(arr)[header->count++] = (v);																				\
	}

#define apollo_arr_pop(arr)											\
	{																\
		Apollo_arr_header *header = (Apollo_arr_header *)(arr) - 1;	\
																	\
		header->count--;											\
	}

#define apollo_arr_at(arr, i) (arr)[i]

#define apollo_arr_front(arr) apollo_arr_at(arr, 0)

#define apollo_arr_back(arr) apollo_arr_at(arr, ((Apollo_arr_header *)(arr) - 1)->count - 1)

#define apollo_arr_size(arr) ((Apollo_arr_header *)arr - 1)->count

#define apollo_arr_empty(arr) arr == NULL ? true : false

#define apollo_arr_free(arr) free((Apollo_arr_header *)arr - 1)

//

// Stack (LIFO)

#define apollo_stack_push(stack, v) apollo_arr_push(stack, v)

#define apollo_stack_pop(stack) apollo_arr_pop(stack)

#define apollo_stack_top(stack) apollo_arr_back(stack)

#define apollo_stack_size(stack) apollo_arr_size(stack)

#define apollo_stack_empty(stack) apollo_arr_empty(stack)

#define apollo_stack_free(stack) apollo_arr_free(stack)

//

// Containers

#define GET_CONTAINER_NAME(name, type) name
#define GET_CONTAINER_TYPE(name, type) type
#define CONTAINER_EXPAND_NAME(...) GET_CONTAINER_NAME(__VA_ARGS__)
#define CONTAINER_EXPAND_TYPE(...) GET_CONTAINER_TYPE(__VA_ARGS__)

// List

// This is to stop the lsp going crazy
#ifndef T
#define T IList, int
#endif
//

#define CONTAINER_NAME CONTAINER_EXPAND_NAME(T)
#define CONTAINER_TYPE CONTAINER_EXPAND_TYPE(T)

typedef struct CONTAINER_NAME {
	CONTAINER_TYPE data;
	struct CONTAINER_NAME *next;
} CONTAINER_NAME;

static void apollo_list_push_back(CONTAINER_NAME **list, CONTAINER_TYPE v) {
	CONTAINER_NAME *ptr = (CONTAINER_NAME *)malloc(sizeof(CONTAINER_NAME));

	ptr->data = v;
	if (*list == NULL) {
		ptr->next = ptr;
	} else {
		ptr->next = (*list)->next;
		(*list)->next = ptr;
	}
	*list = ptr;
}

static void apollo_list_push_front(CONTAINER_NAME **list, CONTAINER_TYPE v) {
	CONTAINER_NAME *ptr = (CONTAINER_NAME *)malloc(sizeof(CONTAINER_NAME));
	
	ptr->data = v;
	if (*list == NULL) {
		ptr->next = ptr;
		*list = ptr;
		return;
	}
	ptr->next = (*list)->next;
	(*list)->next = ptr;
}

static void apollo_list_pop_front(CONTAINER_NAME **list) {
	if (*list == NULL) {
		return;
	}
	CONTAINER_NAME *tmp = (*list)->next;

	(*list)->next = tmp->next;
	if (*list == tmp) {
		*list = NULL;
	}
	free(tmp);
}

static void apollo_list_pop_back(CONTAINER_NAME **list) {
	if (*list == NULL) {
		return;
	}
	CONTAINER_NAME *tmp = *list;

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

static size_t apollo_list_size(CONTAINER_NAME *list) {
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

static CONTAINER_NAME *apollo_list_front(CONTAINER_NAME *list) {
	return (list->next);
}

static CONTAINER_NAME *apollo_list_back(CONTAINER_NAME *list) {
	return (list);
}

static bool apollo_list_empty(CONTAINER_NAME *list) {
	return (list == NULL ? true : false);
}

//

// Hashmaps

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifndef HM
#define HM Map, char *, int
#endif

#define APOLLO_HASHMAP_INIT_CAPACITY 100

#define GET_HASHMAP_NAME(name, key_type, value_type) name
#define GET_HASHMAP_KEY_TYPE(name, key_type, value_type) key_type
#define GET_HASHMAP_VALUE_TYPE(name, key_type, value_type) value_type
#define HASHMAP_EXPAND_NAME(...) GET_HASHMAP_NAME(__VA_ARGS__)
#define HASHMAP_EXPAND_KEY_TYPE(...) GET_HASHMAP_KEY_TYPE(__VA_ARGS__)
#define HASHMAP_EXPAND_VALUE_TYPE(...) GET_HASHMAP_VALUE_TYPE(__VA_ARGS__)

#define HASHMAP_NAME HASHMAP_EXPAND_NAME(HM)
#define HASHMAP_KEY_TYPE HASHMAP_EXPAND_KEY_TYPE(HM)
#define HASHMAP_VALUE_TYPE HASHMAP_EXPAND_VALUE_TYPE(HM)

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
	int index;

	if (*hmap == NULL) {
		Apollo_arr_header *header = malloc(sizeof(Apollo_arr_header) + APOLLO_HASHMAP_INIT_CAPACITY * sizeof(HASHMAP_NAME));
		header->capacity = APOLLO_INIT_CAPACITY;
		header->count = 0;
		*hmap = (HASHMAP_NAME *)(header + 1);
	}
	Apollo_arr_header *header = (Apollo_arr_header *)(*hmap) - 1;
	index = hash(key);
	//printf("Map: %c -> %d. Index %d\n", key, data, index);
	(*hmap)[index].key = key;
	(*hmap)[index].data = data;
	header->count++;
}

static HASHMAP_VALUE_TYPE apollo_hashmap_get(HASHMAP_NAME *hmap, HASHMAP_KEY_TYPE key) {
	Apollo_arr_header *header = (Apollo_arr_header *)hmap - 1;

	int index = hash(key);
	printf("Index %d\n", index);
	return (hmap[index].data);
}

static void apollo_hashmap_free(HASHMAP_NAME *hmap) {
	free((Apollo_arr_header *)hmap - 1);
}

//
//

