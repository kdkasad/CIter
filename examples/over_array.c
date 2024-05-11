#include <stdio.h>

#include <citer.h>

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		fprintf(stderr, "Usage: %s <args...>\n", argv[0]);
		return 1;
	}

	iterator_t *it = citer_over_array((void **) argv + 1, sizeof(*argv), argc - 1);

	char **itemptr;
	unsigned long count = 0;
	while ((itemptr = (char **) citer_next(it))) {
		printf("Got: %s\n", *itemptr);
		count++;
	}
	printf("Count: %lu\n", count);

	citer_free(it);

	return 0;
}
