#
# CIter - C library for lazily-evaluated iterators.
# Copyright (C) 2024  Kian Kasad <kian@kasad.com>
#
# This file is part of CIter.
#
# CIter is free software: you can redistribute it and/or modify it under the terms
# of the GNU Lesser General Public License as published by the Free Software
# Foundation, version 3 of the License.
#
# CIter is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License along
# with CIter. If not, see <https://www.gnu.org/licenses/>.
#

PREFIX = /usr/local

VERSION = 0.0.0
STATICLIB = libciter.a
DYLIB = libciter.so
HEADER = citer.h
LICENSE_HEADER_LENGTH = 18

# Order matters here. The "iterator" module must come first, so that when
# citer.h is constructed, the iterator_t type is defined for all subsequent
# modules which use it.
MODULES = \
	iterator \
	repeat \
	take \
	over_array \
	chain \
	map \
	filters \
	enumerate \
	chunked \
	collect

EXAMPLES = \
	repeat_take \
	over_array \
	chain \
	map \
	filter \
	enumerate \
	minmax \
	chunked \
	collect \
	count_using_fold \
	sum
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
all: $(STATICLIB) $(DYLIB).$(VERSION) $(HEADER) examples

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
	sed '$(LICENSE_HEADER_LENGTH)q' $< > $@
	sed -s '1,$(LICENSE_HEADER_LENGTH)d; /^#include "[^"][^"]*"$$/d' $^ >> $@

$(DYLIB).$(VERSION): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -shared $(LDLIBS)

# Set SONAME on Linux
ifeq ($(shell uname -s),Linux)
$(DYLIB).$(VERSION): LDFLAGS += -Wl,-soname,$(SONAME)
endif

.PHONY: install
install: all
	install -D -m 644 $(HEADER) $(DESTDIR)$(PREFIX)/include/$(HEADER)
	install -D -m 644 $(STATICLIB) $(DESTDIR)$(PREFIX)/lib/$(STATICLIB)
	install -D -m 755 $(DYLIB).$(VERSION) $(DESTDIR)$(PREFIX)/lib/$(DYLIB).$(VERSION)
	ln -sf $(DYLIB).$(VERSION) $(DESTDIR)$(PREFIX)/lib/$(SONAME)
	ln -sf $(DYLIB).$(VERSION) $(DESTDIR)$(PREFIX)/lib/$(DYLIB)
	if command -v ldconfig >/dev/null 2>&1; then ldconfig; fi

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/include/$(HEADER) \
		$(DESTDIR)$(PREFIX)/lib/$(STATICLIB) \
		$(DESTDIR)$(PREFIX)/lib/$(DYLIB).$(VERSION) \
		$(DESTDIR)$(PREFIX)/lib/$(SONAME) \
		$(DESTDIR)$(PREFIX)/lib/$(DYLIB)
	if command -v ldconfig >/dev/null 2>&1; then ldconfig; fi
