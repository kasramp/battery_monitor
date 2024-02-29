#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "battery_monitor.h"
#include "notification.h"

void print_battery_info();
void print_battery_info_json();
char *resize_string(int extended_size, char *str);
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

/*
 * Generates a JSON output with the following format
 *
 * {
 *    "batteries":[
 *       {
 *          "battery_0":"100%"
 *       },
 *       {
 *          "battery_1":"90%"
 *       }
 *    ]
 * }
 */
void print_battery_info_json()
{
	int count = 0;
	int *percentage = all_battery_percentage(&count);
	const char *main_template = "{\n   \"batteries\":[\n%s   ]\n}\n";
	const char *inner_template =
	    "      {\n         \"BATTERY_%d\":\"%d%%\"\n      }";

	int json_size = 64;
	char *json = malloc(json_size * sizeof(char));
	strcpy(json, "");

	for (int i = 0; i < count; i++) {
		if (strlen(json) + 64 >= json_size) {
			json_size *= 2;
			json = resize_string(json_size, json);
		}
		strcat(json, inner_template);
		sprintf(json, json, i, percentage[i]);
		strcat(json, i + 1 < count ? ",\n" : "\n");
	}
	if (strlen(json) + strlen(main_template) >= json_size) {
		json_size *= 2;
		json = resize_string(json_size, json);
	}
	printf(main_template, json);
	free(json);
}

char *resize_string(int extended_size, char *str)
{
	char *temp = malloc(extended_size * sizeof(char));
	strcpy(temp, str);
	free(str);
	return temp;
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
