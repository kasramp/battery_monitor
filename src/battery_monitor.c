#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utmpx.h>
#include <fcntl.h>
#include "inc/battery_monitor.h"
#include "inc/notification.h"

static void battery_status(battery_t battery, char *status);

battery_t *list_batteries(int *battery_count)
{
	int result = check_acpi_support();
	if (result != SUCCESS) {
		perror("The device doesn't support ACPI");
		return NULL;
	}
	global_t device_info;
	result = init_acpi_batt(&device_info);
	if (result != SUCCESS) {
		perror("Cannot initialize the ACPI");
		return NULL;
	}
	if (device_info.batt_count < 1) {
		puts("The device doesn't have any battery");
		return NULL;
	}
	*battery_count = device_info.batt_count;
	return batteries;
}

int battery_percentage(int battery_index, battery_t battery)
{
	int result = read_acpi_batt(battery_index);
	if (result != SUCCESS) {
		perror("Failed to read the battery status");
		return -1;
	}
	char buffer[10];
	char *battery_file = battery.info_file;
	strcat(battery_file, "/capacity");
	FILE *file = fopen(battery_file, "r");
	if (file == NULL) {
		perror("Error opening the file");
		return -1;
	}
	if (fgets(buffer, sizeof(buffer), file) == NULL) {
		perror("Error reading the file");
		fclose(file);
		return -1;
	}
	fclose(file);
	return atoi(buffer);
}

int *all_battery_percentage(int *battery_count)
{
	battery_t *batteries = list_batteries(battery_count);
	int *percentage = (int *)malloc(*battery_count * sizeof(int));
	for (int i = 0; i < *battery_count; i++) {
		battery_t battery = batteries[i];
		percentage[i] = battery_percentage(i, battery);
	}
	return percentage;
}

int is_discharging(battery_t battery)
{
	char status[20];
	battery_status(battery, status);
	return strcmp("Discharging", status);
}

void monitor()
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

static void battery_status(battery_t battery, char *status)
{
	char buffer[32];
	char *battery_file = battery.info_file;
	strcat(battery_file, "/status");
	FILE *file = fopen(battery_file, "r");
	if (file == NULL) {
		perror("Error opening the file");
		status = NULL;
		return;
	}
	if (fgets(buffer, sizeof(buffer), file) == NULL) {
		perror("Error reading the file");
		fclose(file);
		status = NULL;
		return;
	}
	fclose(file);
	strncpy(status, buffer, strlen(buffer) - 1);
}
