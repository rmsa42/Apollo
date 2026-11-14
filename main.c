#include <stdio.h>

#define T IList, int
#define HM Map, char, int
#include "apollo.h"

void map() {
	Map *map = NULL;

	// Char *
	//apollo_hashmap_push(&map, "fish", 10);
	//apollo_hashmap_push(&map, "sand", 20);
	//apollo_hashmap_push(&map, "coor", 30);
	//printf("Key: %s -> %d\n", "fish", apollo_hashmap_get(map, "fish"));
	//printf("Key: %s -> %d\n", "sand", apollo_hashmap_get(map, "sand"));
	//printf("Key: %s -> %d\n", "coor", apollo_hashmap_get(map, "coor"));

	// Char
	apollo_hashmap_push(&map, 'c', 10);
	apollo_hashmap_push(&map, 'd', 20);
	apollo_hashmap_push(&map, 'f', 30);
	printf("Key: %c -> %d\n", 'c', apollo_hashmap_get(map, 'c'));
	printf("Key: %c -> %d\n", 'd', apollo_hashmap_get(map, 'd'));
	printf("Key: %c -> %d\n", 'f', apollo_hashmap_get(map, 'f'));

	//printf("Index: %d\n", hash("a"));
	//printf("Index: %d\n", hash("b"));
	//printf("Index: %d\n", hash("fish"));
	//printf("Index: %d\n", hash("fih"));
	//printf("Index: %d\n", hash("polasd"));
	//printf("Index: %d\n", hash("bih"));
	//printf("Index: %d\n", hash("gih"));
	//printf("Index: %d\n", hash("load"));
	//printf("Index: %d\n", hash("bload"));

	apollo_hashmap_free(map);
	printf("\n");
}

void list() {
	IList *list = NULL;

	apollo_list_push_front(&list, 10);
	apollo_list_push_front(&list, 5);
	//apollo_list_push_front(&list, 0);
	//apollo_list_push_back(&list, 40);
	apollo_list_pop_front(&list);

	if (!apollo_list_empty(list)) {
		printf("List front: %d\n", apollo_list_front(list)->data);
		printf("List back: %d\n", apollo_list_back(list)->data);
		printf("List size: %zu\n", apollo_list_size(list));
	}
}

void stack() {
	int *stack = NULL;

	apollo_stack_push(stack, 120);
	apollo_stack_push(stack, 220);
	apollo_stack_push(stack, 320);
	apollo_stack_push(stack, 420);

	apollo_stack_pop(stack);

	printf("Top: %d\n", apollo_stack_top(stack));
	for (int i = 0; i < apollo_stack_size(stack); i++) {
		printf("Stack: %d\n", stack[i]);
	}
	
	apollo_stack_free(stack);
}

void arr() {
	float *arr = NULL;

	apollo_arr_push(arr, 10);
	apollo_arr_push(arr, 20);
	apollo_arr_push(arr, 40);

	for (int i = 0; i < apollo_arr_size(arr); i++) {
		printf("Arr: %f\n", arr[i]);
	}

	printf("At: %f\n", apollo_arr_at(arr, 1));
	printf("Front: %f\n", apollo_arr_front(arr));
	printf("Back: %f\n", apollo_arr_back(arr));
	printf("Empty: %d\n", apollo_arr_empty(arr));

	apollo_arr_free(arr);
}

int main() {
	map();
	//list();
	//stack();
	//arr();
	return 0;
}
