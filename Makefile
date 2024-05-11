MODULES = iterator repeat take over_array chain
NOINCLUDES = iterator
INCLUDE = include/citer.h
STATICLIB = citer.a

EXAMPLES = repeat_take over_array chain
EXAMPLES_BIN = $(addprefix examples/,$(EXAMPLES))

OBJS = $(patsubst %,build/%.o,$(MODULES))

CFLAGS = -Wall -Werror -std=c99
CPPFLAGS = -I./include -iquote ./src

ifneq ($(DBG),)
# Debugging flags
	CFLAGS += -g
else
# Optimisation flags
	CFLAGS += -O3
endif

.PHONY: all
all: $(STATICLIB) examples

.PHONY: examples
examples: $(EXAMPLES_BIN)

.PHONY: clean
clean: | clean-examples
	rm -f $(OBJS) $(STATICLIB)
	rm -fd build

.PHONY: clean-examples
clean-examples:
	rm -f $(EXAMPLES_BIN)

$(STATICLIB): $(OBJS)
	ar crs $@ $^

build:
	mkdir -p build

$(OBJS): build/%.o: src/%.c $(INCLUDE) | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(EXAMPLES_BIN): examples/%: examples/%.c $(STATICLIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(STATICLIB) $(LDFLAGS) $(LDLIBS)
