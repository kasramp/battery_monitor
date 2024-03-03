#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "battery_monitor.h"
#include "notification.h"
#include "parser.h"
#include "printer.h"

void print_battery_info();
void print_battery_info_json();
char *resize_string(int extended_size, char *str);
void loop_battery_monitor();

int main(int argc, char *argv[])
{
	switch (parse(argc, argv)) {
	case BATTERY_INFO:
		print_battery_info();
		break;
	case BATTERY_INFO_JSON:
		print_battery_info_json();
		break;
	case MONITOR:
		loop_battery_monitor();
		break;
	case HELP:
	default:
		puts("Print help: to implement");
	}
	return EXIT_SUCCESS;
}

void loop_battery_monitor()
{
	while (1) {
		int battery_count = 0;
		battery_t *batteries = list_batteries(&battery_count);
		for (int i = 0; i < battery_count; i++) {
			battery_t battery = batteries[i];
			int percentage = battery_percentage(i, battery);
			if (percentage < 0) {
				continue;
			}
			if (is_discharging(battery) != 0) {
				// printf("The battery (%s) is fully charged, ignoring the check!\n", battery.name);
				continue;
			}
			// recommended percentage to prolong the battery life span
			if (percentage < 21) {
				notify_low_battery(percentage);
			}
			// printf("The battery (%s) capacity is: %d%%\n", battery.name, battery_capacity(battery));
		}
		sleep(30);
	}
}
