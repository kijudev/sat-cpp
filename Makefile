CXX      := clang++
CXXOPT   := -O1 -g
CXXFLAGS := -std=c++23 -Wall -Wextra -Iinclude 

APPS      := $(wildcard apps/*/main.cpp)
APP_BINS  := $(patsubst apps/%/main.cpp,bin/%,$(APPS))

TESTS     := $(wildcard tests/*.cpp)
TEST_BINS := $(patsubst tests/%.cpp,bin/tests/%,$(TESTS))

.PHONY: all apps tests clean compile-commands run-tests

all: apps tests

apps: $(APP_BINS)

tests: $(TEST_BINS)

bin/%: apps/%/main.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(CXXOPT) $< -o $@

bin/tests/%: tests/%.cpp
	mkdir -p bin/tests
	$(CXX) $(CXXFLAGS) $(CXXOPT) $< -o $@

clean:
	rm -rf bin
	rm -f compile_commands.json

compile-commands: clean
	bear -- make

run-tests: tests
	@set -e; \
	for t in $(TEST_BINS); do \
	  printf "Running %s\n" "$$t"; \
	  if "$$t"; then \
	    printf "PASS$\n"; \
	  else \
	    printf "FAIL: %s\n" "$$t"; \
	    exit 1; \
	  fi \
	done
