#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libacpi.h>
#include <unistd.h>
#include <utmpx.h>
#include <fcntl.h>

char* get_user();
void notify_low_battery(int percentage);
int battery_capacity(battery_t battery);
void battery_status(battery_t battery, char *status);
int is_discharging(battery_t battery);

int main(int argc, char* argv[])
{
    int result = check_acpi_support();
    if (result != SUCCESS) {
        perror("The device doesn't support ACPI");
        return EXIT_FAILURE;
    }
    global_t device_info;
    result = init_acpi_batt(&device_info);
    if (result != SUCCESS) {
        perror("Cannot initialize the ACPI");
        return EXIT_FAILURE;
    }
    if (device_info.batt_count < 1) {
        puts("The device doesn't have any battery");
        return EXIT_SUCCESS;
    }
    for (int i=0; i<device_info.batt_count; i++) {
        result = read_acpi_batt(i);
        if (result != SUCCESS) {
            perror("Failed to read the battery status");
            continue;
        }
        battery_t battery = batteries[i];
        if (is_discharging(battery) != 0) {
            printf("The battery (%s) is fully charged, ignoring the check!\n", battery.name);
            continue;
        }

        int percentage = battery_capacity(battery);
        // recommended percentage to prolong the battery life span
        if (percentage < 21) {
           notify_low_battery(percentage); 
        }

        //printf("The battery (%s) capacity is: %d%%\n", battery.name, battery_capacity(battery));
    }
    return EXIT_SUCCESS;
}

int battery_capacity(battery_t battery)
{
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

int is_discharging(battery_t battery)
{
    char status[20];
    battery_status(battery, status);
    return strcmp("Discharging", status);
}

void battery_status(battery_t battery, char* status)
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
    strncpy(status, buffer, strlen(buffer)-1);
}

char* get_user()
{
    char *username = getlogin();
    return username;
}

void notify_low_battery(int percentage)
{
    // Open the utmpx database
    setutxent();
    // Iterate over the entries in the utmpx database
    struct utmpx *entry;
    while ((entry = getutxent()) != NULL) {
        // Check if the entry corresponds to the current user and is a terminal
        if (entry->ut_type == USER_PROCESS && strcmp(entry->ut_user, get_user()) == 0) {
            // Open the terminal device associated with the session
            char terminal[1024];
            snprintf(terminal, sizeof(terminal), "/dev/%s", entry->ut_line);
            int fd = open(terminal, O_WRONLY);
            if (fd != -1) {
                // Write the message to the terminal
                dprintf(fd, "Low battery, only %d%% remains\n", percentage);
                close(fd);
            }
        }
    }

    // Close the utmpx database
    endutxent();
}
