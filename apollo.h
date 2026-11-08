#pragma once

#include <stdlib.h>

#define APOLLO_INIT_CAPACITY 1

typedef struct Apollo_header {
	int count;
	int capacity;
} Apollo_header;

#define apollo_arr_init()																			\
	{																								\
		Apollo_header *header = malloc(sizeof(Apollo_header) + APOLLO_INIT_CAPACITY * sizeof(int));	\
																									\
		header->count = 0;																			\
		header->capacity = APOLLO_INIT_CAPACITY;													\
		return (header + 1);																		\
	}																								\

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

#define apollo_arr_len(arr) ((Apollo_header *)arr - 1)->count

#define apollo_arr_free(arr) free((Apollo_header *)arr - 1)

