#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "battery_monitor.h"
#include "notification.h"

void print_battery_info();
void print_battery_info_json();
void loop_battery_monitor();

int main(int argc, char *argv[])
{
	if (argc == 1) {
		print_battery_info();
		return EXIT_SUCCESS;
	}
	for (int i = 1; i < argc; i++) {
		if (strcmp("-s", argv[i]) == 0
		    || strcmp("--status", argv[i]) == 0) {
			print_battery_info();
			return EXIT_SUCCESS;
		}
		if (strcmp("-j", argv[i]) == 0
		    || strcmp("--json", argv[i]) == 0) {
			print_battery_info_json();
			return EXIT_SUCCESS;
		}
		if (strcmp("-m", argv[i]) == 0
		    || strcmp("--monitor", argv[i]) == 0) {
			loop_battery_monitor();
		}
		puts("Print help: to implement");
	}
	return EXIT_SUCCESS;
}

void print_battery_info()
{
	int count = 0;
	int *percentage = all_battery_percentage(&count);
	for (int i = 0; i < count; i++) {
		printf("Battery %d: %d%%\n", i, percentage[i]);
	}
}

void print_battery_info_json()
{
	int count = 0;
	int *percentage = all_battery_percentage(&count);
	for (int i = 0; i < count; i++) {
		/*
		   {
		   "batteries": [
		   {
		   "battery_0": "100%"
		   },
		   {
		   "battery_1": "90%"
		   }
		   ]
		   }
		 */
		puts("To implement");
	}
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
