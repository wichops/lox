ARGS ?=

.PHONY: all setup build run clean

all: build

setup:
	@if [ ! -d builddir ]; then meson setup builddir; fi

build: setup
	@meson compile -C builddir

run: build
	@./builddir/lox $(ARGS)

clean:
	@rm -rf builddir
