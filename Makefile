VERSION = 0.0.0
STATICLIB = libciter.a
DYLIB = libciter.so
HEADER = citer.h

MODULES = \
	iterator \
	repeat \
	take \
	over_array \
	chain \
	map \
	filters \
	enumerate

EXAMPLES = \
	repeat_take \
	over_array \
	chain \
	map \
	filter \
	enumerate \
	minmax
EXAMPLES_BIN = $(addprefix examples/,$(EXAMPLES))

OBJS = $(patsubst %,build/%.o,$(MODULES))
SONAME = $(DYLIB).$(firstword $(subst ., ,$(VERSION)))

CFLAGS = -Wall -Werror -std=c99 -fPIC
CPPFLAGS = -I.

ifneq ($(DBG),)
# Debugging flags
	CFLAGS += -g
else
# Optimisation flags
	CFLAGS += -O3
endif

.PHONY: all
all: $(STATICLIB) $(SONAME) $(HEADER) examples

.PHONY: examples
examples: $(EXAMPLES_BIN)

.PHONY: clean
clean: | clean-examples
	rm -f $(OBJS) $(STATICLIB) $(HEADER) $(DYLIB) $(DYLIB).$(VERSION) $(SONAME)
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

$(SONAME): $(DYLIB).$(VERSION)
	ln -sf $< $@

$(DYLIB).$(VERSION): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -shared $(LDLIBS)

# Set SONAME on Linux
ifeq ($(shell uname -s),Linux)
$(DYLIB).$(VERSION): LDFLAGS += -Wl,-soname,$(SONAME)
endif
