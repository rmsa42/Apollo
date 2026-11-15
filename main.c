#include <stdio.h>

#define L IList, int
#define HM Map, char, int
#include "apollo.h"
#undef HM
#undef L
#define L DList, double
#define HM HMap, char *, int
#include "apollo.h"

void map() {
	Map *map = NULL;
	HMap *lol = NULL;

	// Map, char, int
	hmap_Map_push(&map, 'a', 1);
	hmap_Map_push(&map, 'd', 10);
	hmap_Map_push(&map, 'f', 20);
	printf("Key: %c -> %d\n", 'a', hmap_Map_get(map, 'a'));
	printf("Key: %c -> %d\n", 'd', hmap_Map_get(map, 'd'));
	printf("Key: %c -> %d\n", 'f', hmap_Map_get(map, 'f'));
	printf("HashMap size: %zu %d\n", hmap_Map_size(map), hmap_Map_empty(map));

	// HMap, char *, int
	hmap_HMap_push(&lol, "fish", 100);
	printf("Key: %s -> %d\n", "fish", hmap_HMap_get(lol, "fish"));

	hmap_Map_free(map);
	hmap_HMap_free(lol);
	printf("\n");
}

void list() {
	IList *list = NULL;

	list_IList_push_front(&list, 10);
	list_IList_push_front(&list, 5);
	//list_push_front(&list, 0);
	list_IList_push_back(&list, 40);
	list_IList_pop_front(&list);
	list_IList_pop_back(&list);

	if (!list_IList_empty(list)) {
		printf("List front: %d\n", list_IList_front(list)->data);
		printf("List back: %d\n", list_IList_back(list)->data);
		printf("List size: %zu\n", list_IList_size(list));
	}

	list_IList_free(&list);
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
