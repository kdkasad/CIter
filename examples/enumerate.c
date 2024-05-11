#include <stdio.h>

#include <citer.h>

static void *deref_char_ptr_ptr(void *item) {
    return (void *) (*((char **) item));
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
		fprintf(stderr, "Usage: %s <args...>\n", argv[0]);
		return 1;
	}

	iterator_t *it = citer_over_array((void **) argv + 1, sizeof(*argv), argc - 1);
    it = citer_map(it, deref_char_ptr_ptr);
    it = citer_enumerate(it);

	citer_enumerate_item_t *item;
    while ((item = citer_next(it))) {
        printf("Got: { .index = %lu, .item = %s }\n", item->index, (char *) item->item);
    }

	citer_free(it);

	return 0;
}
