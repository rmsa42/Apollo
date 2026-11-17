#include <stdio.h>
#include "apollo.h"

LIST_DECL(IList, int)
HASHMAP_DECL(Map, char, int);
HASHMAP_DECL(HMap, char *, int);

void map() {
	Map *map = NULL;
	HMap *lol = NULL;

	// Map, char, int
	Map_push(&map, 'a', 1);
	Map_push(&map, 'd', 10);
	Map_push(&map, 'f', 20);
	printf("Key: %c -> %d\n", 'a', Map_get(map, 'a'));
	printf("Key: %c -> %d\n", 'd', Map_get(map, 'd'));
	printf("Key: %c -> %d\n", 'f', Map_get(map, 'f'));
	printf("HashMap size: %zu %d\n", Map_size(map), Map_empty(map));

	// HMap, char *, int
	HMap_push(&lol, "fish", 100);
	printf("Key: %s -> %d\n", "fish", HMap_get(lol, "fish"));

	Map_free(map);
	HMap_free(lol);
	printf("\n");
}

void list() {
	IList *list = NULL;

	IList_push_front(&list, 10);
	IList_push_front(&list, 5);
	//list_push_front(&list, 0);
	IList_push_back(&list, 40);
	IList_pop_front(&list);
	IList_pop_back(&list);

	if (!IList_empty(list)) {
		printf("List front: %d\n", IList_front(list)->data);
		printf("List back: %d\n", IList_back(list)->data);
		printf("List size: %zu\n", IList_size(list));
	}

	IList_free(&list);
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
	list();
	stack();
	arr();

	return 0;
}
