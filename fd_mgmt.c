#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

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
	pthread_mutex_t lock;
};

#define MAX_NR_OPEN_FILES 2048
extern int loglevel;
static inline void debug_log(int fd)
{
	if (loglevel > 0 ) 
		printf("[pid %d] %s(%d)\n", getpid(), __FUNCTION__, fd);
}

struct fd_entry fd_entry[MAX_NR_OPEN_FILES];

void mgmt_create_fd(int fd)
{
	struct fd_entry *fde = &fd_entry[fd];

	pthread_mutex_lock(&fde->lock);

	debug_log(fd);
	fde->fstat = FSTAT_USED;
	fde->access_time = time(NULL);

	pthread_mutex_unlock(&fde->lock);
}

void mgmt_close_fd(int fd)
{
	struct fd_entry *fde = &fd_entry[fd];

	pthread_mutex_lock(&fde->lock);

	debug_log(fd);
	if (fde->fstat != FSTAT_USED) {
		fprintf(stderr, "[pid %d] fd %d is in %d stated\n",
			getpid(), fd, fde->fstat);
		abort();
	}

	fde->fstat = FSTAT_CLOSED;

	pthread_mutex_unlock(&fde->lock);
}

void mgmt_access_fd(int fd)
{
	struct fd_entry *fde = &fd_entry[fd];

	pthread_mutex_lock(&fde->lock);

	debug_log(fd);
	if (fde->fstat != FSTAT_USED) {
		fprintf(stderr, "[pid %d] fd %d is in %d state(not used)\n",
			getpid(), fd, fde->fstat);
		abort();
	}
	
	fde->access_time = time(NULL);

	pthread_mutex_unlock(&fde->lock);
}

int (*__read)(int fd, char *buf, size_t len);
static void init_all_fds() __attribute__((constructor));
static void init_all_fds()
{
	int i;
	struct fd_entry *fde;	

	for (i = 0, fde = &fd_entry[0]; i < MAX_NR_OPEN_FILES; ++i, ++fde) {
		/*
		 * test whether this @i fd is opend before.
		 * such as opend by ld, or aready opened before execve
		 */
		if (__read(i, NULL, 0) == -1 && errno == EBADF) {
			fde->fstat = FSTAT_EMPTY;
		} else {
			fde->fstat = FSTAT_USED;
		}

		pthread_mutex_init(&fde->lock, NULL);
		fde->access_time = 0;
	}
}
