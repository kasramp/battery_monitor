#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utmpx.h>
#include <fcntl.h>
#include "notification.h"

static char *get_user();

void notify_low_battery(int percentage)
{
	// Open the utmpx database
	setutxent();
	// Iterate over the entries in the utmpx database
	struct utmpx *entry;

	while ((entry = getutxent()) != NULL) {
		// Check if the entry corresponds to the current user and is a terminal
		if (entry->ut_type == USER_PROCESS
		    && strcmp(entry->ut_user, get_user()) == 0) {
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
	}

	// Close the utmpx database
	endutxent();
}

static char *get_user()
{
	return getlogin();
}
