#include "apollo.h"
#include <stdio.h>

int main() {
	int *arr = NULL;

	apollo_arr_push(arr, 10);
	apollo_arr_push(arr, 20);
	apollo_arr_push(arr, 40);

	for (int i = 0; i < apollo_arr_len(arr); i++) {
		printf("Arr: %d\n", arr[i]);
	}

	apollo_arr_free(arr);
}
