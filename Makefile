MODULES = iterator repeat take over_array chain map filters enumerate
STATICLIB = citer.a
HEADER = citer.h

EXAMPLES = repeat_take over_array chain map filter enumerate
EXAMPLES_BIN = $(addprefix examples/,$(EXAMPLES))

OBJS = $(patsubst %,build/%.o,$(MODULES))

CFLAGS = -Wall -Werror -std=c99
CPPFLAGS = -I.

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
	rm -f $(OBJS) $(STATICLIB) $(HEADER)
	rm -fd build

.PHONY: clean-examples
clean-examples:
	rm -f $(EXAMPLES_BIN)

$(STATICLIB): $(OBJS)
	ar crs $@ $^

build:
	mkdir -p build

$(OBJS): build/%.o: src/%.c $(HEADER) | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(EXAMPLES_BIN): examples/%: examples/%.c $(STATICLIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(STATICLIB) $(LDFLAGS) $(LDLIBS)

$(HEADER): $(patsubst %,src/%.h,$(MODULES))
	sed '/^#include "[^"][^"]*"$$/d' $^ > $@
