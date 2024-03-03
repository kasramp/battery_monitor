CC=gcc
CFLAGS=-I.
DEPS = battery_monitor.h notification.h parser.h
LDLIBS = -lacpi
CLEANUP = rm -f

all: pre-build battery_monitor_make

pre-build:
	@echo "Compiling battery monitor program"

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

battery_monitor_make: main.o battery_monitor.o notification.o parser.o
	$(CC) -o battery_monitor main.o battery_monitor.o notification.o parser.o $(LDLIBS)

format:
	indent -linux *.c *.h
	rm -f *~
	@echo "The code is formatted!"

clean:
	rm -f *.o battery_monitor
	@echo "The project is cleaned!"
