#include <stdio.h>
#include <stdlib.h>

#include <citer.h>

void *deref_int_ptr(void *item) {
    return (void *) (long) (*((int *) item));
}

int cmp(const void *a, const void *b, const void *extra_data) {
    (void) extra_data; /* Prevents unused argument warnings. */

    return ((int) (long) a) - ((int) (long) b);
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
		fprintf(stderr, "Usage: %s <nums...>\n", argv[0]);
		return 1;
	}

    int len = argc - 1;
    int *int_arr = calloc(len, sizeof(int));
    for (int i = 1; i < argc; i++) {
        int num;
        if (sscanf(argv[i], "%d", &num) != 1) {
            fprintf(stderr, "Invalid number: %s\n", argv[i]);
            return 1;
        }
        int_arr[i - 1] = num;
    }

	iterator_t *it1 = citer_map(citer_over_array(int_arr, sizeof(*int_arr), len), deref_int_ptr);
	iterator_t *it2 = citer_map(citer_over_array(int_arr, sizeof(*int_arr), len), deref_int_ptr);

    printf("Min: %d\n", (int) (long) citer_min(it1, (citer_cmp_fn_t) cmp, NULL));
    printf("Max: %d\n", (int) (long) citer_max(it2, (citer_cmp_fn_t) cmp, NULL));

	citer_free(it1);
	citer_free(it2);

	return 0;
}
