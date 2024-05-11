#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <citer.h>

static void *deref_intptr(void *item) {
    return (void *) (long) *((int *) item);
}

static bool is_divisible_by(void *item, void *arg) {
    int num = (int) (long) item;
    int divisor = (int) (long) arg;
    return num % divisor == 0;
}

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		fprintf(stderr, "Usage: %s <divisor> <nums...>\n", argv[0]);
		return 1;
	}

    /* Turn divisor into int */
    int divisor;
    if (sscanf(argv[1], "%d", &divisor) != 1) {
        fprintf(stderr, "Invalid divisor: %s\n", argv[1]);
        return 1;
    }

    /* Turn args into int array */
    int len = argc - 2;
    int *int_arr = calloc(len, sizeof(int));
    for (int i = 0; i < len; i++) {
        int num;
        if (sscanf(argv[i + 2], "%d", &num) != 1) {
            fprintf(stderr, "Invalid number: %s\n", argv[i]);
            return 1;
        }
        int_arr[i] = num;
    }

	iterator_t *it = citer_over_array(int_arr, sizeof(*int_arr), len);
    it = citer_map(it, deref_intptr);
    it = citer_filter(it, is_divisible_by, (void *) (long) divisor);

	int item;
	unsigned long count = 0;
	while ((item = (int) (long) citer_next(it))) {
		printf("Got: %d\n", item);
		count++;
	}
	printf("Count: %lu\n", count);

	citer_free(it);

	return 0;
}
