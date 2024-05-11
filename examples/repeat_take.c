#include <stdio.h>

#include <citer.h>

int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <string> <count>\n", argv[0]);
		return 1;
	}

	char *item = argv[1];

	unsigned long count;
	if (sscanf(argv[2], "%lu", &count) != 1) {
		fprintf(stderr, "Error: count must be a non-negative integer\n");
		return 1;
	}

	iterator_t *take = citer_take(citer_repeat(item), count);

	char *returned;
	unsigned long returned_count = 0;
	while ((returned = (char *) citer_next(take))) {
		printf("Got: %s\n", item);
		returned_count++;
	}
	printf("Count: %lu\n", returned_count);

	citer_free(take);

	return 0;
}
