#include <string.h>
#include "parser.h"

/*
typedef enum {
    BATTERY_INFO,
    BATTERY_INFO_JSON,
    MONITOR,
    HELP
} command
*/

command parse(int argc, char *argv[])
{
	if (argc == 1) {
		return BATTERY_INFO;
	}
	for (int i = 1; i < argc; i++) {
		if (strcmp("-s", argv[i]) == 0
		    || strcmp("--status", argv[i]) == 0) {
			return BATTERY_INFO;
		}
		if (strcmp("-j", argv[i]) == 0
		    || strcmp("--json", argv[i]) == 0) {
			return BATTERY_INFO_JSON;
		}
		if (strcmp("-m", argv[i]) == 0
		    || strcmp("--monitor", argv[i]) == 0) {
			return MONITOR;
		}
		if (strcmp("-h", argv[i]) == 0
		    || strcmp("--help", argv[i]) == 0) {
			return HELP;
		}
	}
	return HELP;
}
