#include <stdio.h>

#define T IList, int
#include "apollo.h"

void list() {
	IList *list = NULL;

	apollo_list_push_front(&list, 10);
	apollo_list_push_front(&list, 5);
	apollo_list_push_front(&list, 0);
	apollo_list_push_back(&list, 40);

	printf("List front: %d\n", apollo_list_front(list)->data);
	printf("List back: %d\n", apollo_list_back(list)->data);
	printf("List size: %zu\n", apollo_list_size(list));
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

int main() {
	list();
	stack();

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
	return 0;
}
