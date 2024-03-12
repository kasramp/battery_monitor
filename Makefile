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

install:
	@echo "Installing battery monitor"
	cp battery_monitor /usr/bin
	install -g 0 -o 0 -m 0644 man/battery_monitor.1 /usr/share/man/man1/
	gzip /usr/share/man/man1/battery_monitor.1

format:
	indent -linux src/*.c src/inc/*.h
	rm -f src/*~ src/inc/*~
	@echo "The code is formatted!"

clean:
	rm -f src/*.o battery_monitor
	@echo "The project is cleaned!"

uninstall:
	@echo "Uninstalling battery monitor"
	rm -rvf /usr/bin/battery_monitor
	rm -rvf /usr/share/man/man1/battery_monitor.1*
