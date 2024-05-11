#include <stdio.h>
#include <stdlib.h>

#include <citer.h>

static void *map_int_times2(void *_item) {
    int item = *((int *) _item);
    return (void *) (long) (item * 2);
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

	iterator_t *it = citer_over_array(int_arr, sizeof(*int_arr), len);
    it = citer_map(it, map_int_times2);

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
