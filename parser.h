#ifndef PARSER_H
#define PARSER_H

typedef enum {
	BATTERY_INFO,
	BATTERY_INFO_JSON,
	MONITOR,
	HELP
} command;

command parse(int argc, char *argv[]);

#endif
