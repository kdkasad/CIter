#include <stdio.h>

#include <citer.h>

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		fprintf(stderr, "Usage: %s <args...>\n", argv[0]);
		return 1;
	}

	iterator_t *it = citer_over_array((void **) (argv + 1), argc - 1);

	char *item;
	unsigned long count = 0;
	while ((item = (char *) citer_next(it))) {
		printf("Got: %s\n", item);
		count++;
	}
	printf("Count: %lu\n", count);

	citer_free(it);

	return 0;
}
