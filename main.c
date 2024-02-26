#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "battery_monitor.h"
#include "notification.h"

int main(int argc, char *argv[])
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
				printf
				    ("The battery (%s) is fully charged, ignoring the check!\n",
				     battery.name);
				continue;
			}
			// recommended percentage to prolong the battery life span
			if (percentage < 21) {
				notify_low_battery(percentage);
			}
			//printf("The battery (%s) capacity is: %d%%\n", battery.name, battery_capacity(battery));
		}
		sleep(30);
	}
	return EXIT_SUCCESS;
}
