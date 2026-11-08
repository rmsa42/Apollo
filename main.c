#include "apollo.h"
#include <stdio.h>

typedef struct List {
	void *ptr;
	struct List *next;
} List;

void list() {
	List *list = NULL;
	int a = 10;
	int b = 20;
	List nnext = { &b, NULL };
	List next = { &a, &nnext };

	apollo_arr_push(list, next);
	apollo_arr_push(list, nnext);

	printf("List: %d\n", *(int *)(apollo_arr_at(list, 0).next->ptr));
}

int main() {
	list();

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
