#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utmpx.h>
#include <fcntl.h>
#include "notification.h"

static const int MAX_USER_LENGTH = 32;

static char *get_user();
static char *get_user_system();
static char *copy_user(char *user);

void notify_low_battery(int percentage)
{
	// Open the utmpx database
	setutxent();
	// Iterate over the entries in the utmpx database
	struct utmpx *entry;

	while ((entry = getutxent()) != NULL) {
		// Check if the entry corresponds to the current user and is a terminal
		char *user = get_user();
		if (entry->ut_type == USER_PROCESS
		    && strcmp(entry->ut_user, user) == 0) {
			// Open the terminal device associated with the session
			char terminal[1024];
			snprintf(terminal, sizeof(terminal), "/dev/%s",
				 entry->ut_line);
			int fd = open(terminal, O_WRONLY);
			if (fd != -1) {
				// Write the message to the terminal
				dprintf(fd, "Low battery, only %d%% remains\n",
					percentage);
				close(fd);
			}
		}
		free(user);
	}
	// Close the utmpx database
	endutxent();
}

static char *get_user()
{
	char *user = getlogin();
	return user == NULL ? get_user_system() : copy_user(user);
}

static char *get_user_system()
{
	char *user = malloc(MAX_USER_LENGTH * sizeof(char));
	FILE *pipe = popen("whoami", "r");
	fscanf(pipe, "%s", user);
	return user;
}

static char *copy_user(char *user)
{
	char *user_copy = malloc(MAX_USER_LENGTH * sizeof(char));
	strcpy(user_copy, user);
	return user_copy;
}
