#pragma once

#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define APOLLO_INIT_CAPACITY 1

typedef struct Apollo_header {
	int count;
	int capacity;
} Apollo_header;

#define apollo_arr_push(arr, v)																				\
	{																										\
		if (arr == NULL) {																					\
			Apollo_header *header = malloc(sizeof(Apollo_header) + APOLLO_INIT_CAPACITY * sizeof(*(arr)));	\
			header->count = 0;																				\
			header->capacity = APOLLO_INIT_CAPACITY;														\
			(arr) = (void *)(header + 1);																	\
		}																									\
		Apollo_header *header = (Apollo_header *)(arr) - 1;													\
																											\
		if (header->capacity <= header->count) {															\
			header->capacity *= 2;																			\
			header = realloc(header, sizeof(Apollo_header) + header->capacity * sizeof(*(arr)));			\
			(arr) = (void *)(header + 1);																	\
		}																									\
		(arr)[header->count++] = (v);																		\
	}

#define apollo_arr_pop(arr)									\
	{														\
		Apollo_header *header = (Apollo_header *)(arr) - 1;	\
															\
		header->count--;									\
	}														\

#define apollo_arr_at(arr, i) (arr)[i]

#define apollo_arr_front(arr) apollo_arr_at(arr, 0)

#define apollo_arr_back(arr) apollo_arr_at(arr, ((Apollo_header *)(arr) - 1)->count - 1)

#define apollo_arr_size(arr) ((Apollo_header *)arr - 1)->count

#define apollo_arr_empty(arr) arr == NULL ? TRUE : FALSE

#define apollo_arr_free(arr) free((Apollo_header *)arr - 1)

