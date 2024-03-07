#ifndef PARSER_H
#define PARSER_H

typedef enum {
	BATTERY_INFO,
	BATTERY_INFO_JSON,
	MONITOR,
	HELP,
	UNKNOWN
} command;

command parse(int argc, char *argv[], int *unknown_index);

#endif
