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

// List

// This is to stop the lsp going crazy
#ifndef T
#define T IList, int
#endif
//

#define GET_CONTAINER_NAME(name, type) name
#define GET_CONTAINER_TYPE(name, type) type
#define CONTAINER_EXPAND_NAME(...) GET_CONTAINER_NAME(__VA_ARGS__)
#define CONTAINER_EXPAND_TYPE(...) GET_CONTAINER_TYPE(__VA_ARGS__)
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
