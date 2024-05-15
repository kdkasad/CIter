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

NAME = citer
VERSION = 0.2.0
LICENSE_HEADER_LENGTH = 18

# Order matters here. The "iterator" module must come first, so that when
# citer.h is constructed, the iterator_t type is defined for all subsequent
# modules which use it.
MODULES = \
	size \
	iterator \
	filters \
	repeat \
	take \
	over_array \
	chain \
	map \
	enumerate \
	chunked \
	collect \
	inspect \
	zip \
	reverse
HEADERONLY = size

EXAMPLES = \
	repeat_take \
	over_array \
	chain \
	map \
	filter \
	enumerate \
	minmax \
	chunked \
	count_using_fold \
	sum \
	inspect \
	skip_take_while \
	zip \
	reverse \
	double_ended
EXAMPLES_BIN = $(addprefix examples/,$(EXAMPLES))

TESTS = \
	collect \
	transform_reverse
TESTS_BIN = $(addprefix tests/,$(TESTS))
TESTS_REPORTS = $(addsuffix .out,$(TESTS_BIN))

STATICLIB = lib$(NAME).a
DYLIB = lib$(NAME).so
HEADER = $(NAME).h
OBJS = $(patsubst %,build/%.o,$(filter-out $(HEADERONLY),$(MODULES)))
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
all: $(STATICLIB) $(DYLIB).$(VERSION) $(HEADER) examples tests

.PHONY: help
help: TGT = @printf '    %-24s %s\n'
help:
	@echo Available targets:
	$(TGT) help "Print this help message"
	$(TGT) all "Build header file and libraries"
	$(TGT) examples "Build examples"
	$(TGT) tests "Build tests"
	$(TGT) clean "Clean all build outputs and artifacts"
	$(TGT) clean-examples "Clean only examples"
	$(TGT) clean-tests "Clean only tests"
	$(TGT) check "Run tests"
	$(TGT) install "Install header file and libraries to system"
	$(TGT) uninstall "Reverse effects of 'install'"
	@echo
	@echo 'Set DBG=1 compile with debug symbols (make DBG=1 ...)'

.PHONY: examples
examples: $(EXAMPLES_BIN)

.PHONY: tests
tests: $(TESTS_BIN)

.PHONY: clean
clean: | clean-examples clean-tests
	rm -f $(OBJS) $(STATICLIB) $(HEADER) $(DYLIB) $(DYLIB).$(VERSION) $(SONAME)
	rm -fd build

.PHONY: clean-examples
clean-examples:
	rm -f $(EXAMPLES_BIN)

.PHONY: clean-tests
clean-tests:
	rm -f $(TESTS_BIN)
	rm -f $(TESTS_REPORTS)

$(STATICLIB): $(OBJS)
	ar crs $@ $^

build:
	mkdir -p build

$(OBJS): build/%.o: src/%.c $(HEADER) | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(EXAMPLES_BIN) $(TESTS_BIN): LDFLAGS += -L.
$(EXAMPLES_BIN) $(TESTS_BIN): LDLIBS += -l$(NAME)
$(EXAMPLES_BIN) $(TESTS_BIN): %: %.c $(STATICLIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LDLIBS)

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

.PHONY: check
check: ./tests/run.sh $(TESTS_BIN)
	@$< $(TESTS)
