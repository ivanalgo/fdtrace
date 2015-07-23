#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>

#include "fd_mgmt.h"

extern int open(const char *pathname, int flags, mode_t mode);
extern int close(int fd);
extern ssize_t read(int fd, void *buf, size_t count);

int (*__open)(const char *pathname, int flags, mode_t mode) = NULL;
int (*__close)(int fd);
ssize_t (*__read)(int fd, void *buf, size_t count);

int open(const char *pathname, int flags, mode_t mode)
{
	int fd;

	fd = __open(pathname, flags, mode);

	return mgmt_renew_empty_fd(fd);
}

int close(int fd)
{
	mgmt_close_fd(fd);
	return __close(fd);
}

ssize_t read(int fd, void *buf, size_t count)
{
	mgmt_access_fd(fd);
	return __read(fd, buf, count);
}

void fdcheck_init(void) __attribute__((constructor));

void fdcheck_init(void)
{
	__open = dlsym(RTLD_NEXT, "open");
	__close = dlsym(RTLD_NEXT, "close");
	__read  = dlsym(RTLD_NEXT, "read");
}
