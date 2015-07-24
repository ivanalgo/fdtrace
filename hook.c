#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>

#include "hook.h"
#include "fd_mgmt.h"
	

PRELOAD_LIBC_FUNC(
	open,
	PROTO(3, int, open, const char *, pathname, int, flags, mode_t, mode),
	WRAPPER(ACTION_NULL, ACTION_CREATE(_return))
)

PRELOAD_LIBC_FUNC(
	close,
	PROTO(1, int, close, int, fd),
	WRAPPER(ACTION_ACCESS(fd), ACTION_CLOSE(fd))
)

PRELOAD_LIBC_FUNC(
	read,
	PROTO(3, ssize_t, read, int, fd, void *, buf, size_t, count),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	write,
	PROTO(3, ssize_t, write, int, fd, const void *, buf, size_t, count),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)
