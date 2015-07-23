#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "fd_mgmt.h"

enum fstat {
	FSTAT_EMPTY,
	FSTAT_USED,
	FSTAT_CLOSED,

	FSTAT_NUM
};

struct fd_entry {
	time_t access_time;
	enum fstat fstat;
};

#define MAX_NR_OPEN_FILES 2048

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
struct fd_entry fd_entry[MAX_NR_OPEN_FILES];


static int find_empty_fd(int except);

int mgmt_renew_empty_fd(int old_fd)
{
	int new_fd = 0;

	pthread_mutex_lock(&lock);

	if (fd_entry[old_fd].fstat == FSTAT_EMPTY) {
		new_fd = old_fd;
	} else {
		new_fd = find_empty_fd(old_fd);	
		dup2(old_fd, new_fd);
	}

	fd_entry[new_fd].fstat = FSTAT_USED;
	fd_entry[new_fd].access_time = time(NULL);

	pthread_mutex_unlock(&lock);

	return new_fd;
}

void mgmt_close_fd(int fd)
{
	pthread_mutex_lock(&lock);

	if (fd_entry[fd].fstat != FSTAT_USED) {
		fprintf(stderr, "fd %d is in %d stated\n",
			fd, fd_entry[fd].fstat);
		abort();
	}

	fd_entry[fd].fstat = FSTAT_CLOSED;

	pthread_mutex_unlock(&lock);
}

void mgmt_access_fd(int fd)
{
	pthread_mutex_lock(&lock);

	if (fd_entry[fd].fstat != FSTAT_USED) {
		fprintf(stderr, "fd %d is not in used state\n",
			fd);
		abort();
	}
	
	fd_entry[fd].access_time = time(NULL);

	pthread_mutex_unlock(&lock);
}

static int find_empty_fd(int except)
{
	int fd;

	for (fd = 0; fd < MAX_NR_OPEN_FILES; ++fd) {
		if (fd == except)
			continue;

		if (fd_entry[fd].fstat == FSTAT_EMPTY)
			return fd;
	}

	return -1;
}
