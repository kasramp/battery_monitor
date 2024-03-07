CC=gcc
CFLAGS=-Isrc/inc -Wall -Wextra
DEPS = $(wildcar src/inc/*.h)
LDLIBS = -lacpi
CLEANUP = rm -f

all: pre-build battery_monitor_make

pre-build:
	@echo "Compiling battery monitor program"

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

battery_monitor_make: src/main.o src/battery_monitor.o src/notification.o src/parser.o src/printer.o
	$(CC) -o battery_monitor $^ $(LDLIBS)

format:
	indent -linux src/*.c src/*.h
	rm -f *~
	@echo "The code is formatted!"

clean:
	rm -f src/*.o battery_monitor
	@echo "The project is cleaned!"
