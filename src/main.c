#include <stdio.h>
#include <stdlib.h>
#include "inc/battery_monitor.h"
#include "inc/parser.h"
#include "inc/printer.h"

int main(int argc, char *argv[])
{
	int unknown_index = -1;
	switch (parse(argc, argv, &unknown_index)) {
	case BATTERY_INFO:
		print_battery_info();
		break;
	case BATTERY_INFO_JSON:
		print_battery_info_json();
		break;
	case MONITOR:
		monitor();
		break;
	case HELP:
		print_help();
		break;
	case UNKNOWN:
		print_unknown_command(argv[unknown_index]);
	}
	return EXIT_SUCCESS;
}
