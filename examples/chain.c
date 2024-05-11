#include <stdio.h>

#include <citer.h>

int main(int argc, char *argv[]) {
	if (argc <= 3) {
		fprintf(stderr, "Usage: %s <repeat_item1> <repeat_count1> <repeat_item2> <repeat_count2>\n", argv[0]);
		return 1;
	}

	char *repeat_item1 = argv[1];
	char *repeat_item2 = argv[3];
	unsigned long repeat_count1, repeat_count2;

	if (sscanf(argv[2], "%lu", &repeat_count1) != 1) {
		fprintf(stderr, "Error: repeat_count1 must be a non-negative integer.\n");
		return 1;
	}
	if (sscanf(argv[4], "%lu", &repeat_count2) != 1) {
		fprintf(stderr, "Error: repeat_count2 must be a non-negative integer.\n");
		return 1;
	}
	iterator_t *first = citer_take(citer_repeat(repeat_item1), repeat_count1);
	iterator_t *second = citer_take(citer_repeat(repeat_item2), repeat_count2);
	iterator_t *it = citer_chain(first, second);

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
