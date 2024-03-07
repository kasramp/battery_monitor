CC=gcc
CFLAGS=-Isrc/inc -Wall -Wextra
DEPS = $(wildcard src/inc/*.h)
LDLIBS = -lacpi
CLEANUP = rm -f

all: pre-build battery_monitor_make

pre-build:
	@echo "Compiling battery monitor program"

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

battery_monitor_make: $(patsubst src/%.c, src/%.o, $(wildcard src/*.c))
	$(CC) -o battery_monitor $^ $(LDLIBS)

format:
	indent -linux src/*.c src/inc/*.h
	rm -f src/*~ src/inc/*~
	@echo "The code is formatted!"

clean:
	rm -f src/*.o battery_monitor
	@echo "The project is cleaned!"
