#include <libacpi.h>

#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

battery_t *list_batteries(int *battery_count);
int battery_percentage(int battery_index, battery_t battery);
int *all_battery_percentage(int *battery_count);
int is_discharging(battery_t battery);
void monitor();

#endif
