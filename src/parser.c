#include <string.h>
#include "inc/parser.h"

command parse(int argc, char *argv[], int *unknown_index)
{
	if (argc == 1) {
		return BATTERY_INFO;
	}
	for (int i = 1; i < argc; i++) {
		if (strcmp("-s", argv[i]) == 0
		    || strcmp("--status", argv[i]) == 0) {
			return BATTERY_INFO;
		} else if (strcmp("-j", argv[i]) == 0
			   || strcmp("--json", argv[i]) == 0) {
			return BATTERY_INFO_JSON;
		} else if (strcmp("-m", argv[i]) == 0
			   || strcmp("--monitor", argv[i]) == 0) {
			return MONITOR;
		} else if (strcmp("-h", argv[i]) == 0
			   || strcmp("--help", argv[i]) == 0) {
			return HELP;
		} else {
			*unknown_index = i;
			return UNKNOWN;
		}
	}
	return HELP;
}
