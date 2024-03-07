#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/battery_monitor.h"
#include "inc/printer.h"

static char *resize_string(int extended_size, char *str);

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
	    "      {\n         \"battery_%d\":\"%d%%\"\n      }";

	long unsigned int json_size = 64L;
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

void print_help()
{
	puts("Usage: battery_monitor [options]");
	puts("Options:");
	puts("  -s, --status                    Print battery status");
	puts("  -j, --json                      Print battery status in JSON format");
	puts("  -m, --monitor                   Monitor the battery and send notification to all user's open tty when battery is below 22%");
	puts("  -h, --help                      Print this message and exit");
}

void print_unknown_command(char *unknown_command)
{
	printf("battery_monitor: unrecognized option '%s'\n", unknown_command);
	print_help();
}

static char *resize_string(int extended_size, char *str)
{
	char *temp = malloc(extended_size * sizeof(char));
	strcpy(temp, str);
	free(str);
	return temp;
}
