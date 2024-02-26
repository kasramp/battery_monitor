#include <libacpi.h>

#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

battery_t *list_batteries();
int battery_percentage(int battery_index, battery_t battery);
int is_discharging(battery_t battery);

#endif
