#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/uio.h>

#include "hook.h"
#include "fd_mgmt.h"
	
/* file operations */
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

PRELOAD_LIBC_FUNC(
	readv,	
	PROTO(3, ssize_t, readv, int, fd, const struct iovec *, iov, int, iovcnt),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	writev,
	PROTO(3, ssize_t, writev, int, fd, const struct iovec *, iov, int, iovcnt),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	pread,
	PROTO(4, ssize_t, pread, int, fd, void *, buf, size_t, count, off_t, offset),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	pwrite,
	PROTO(4, ssize_t, pwrite, int, fd, const void *, buf, size_t, count, off_t, offset),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	lseek,
	PROTO(3, off_t, lseek, int, fd, off_t, offset, int, whence),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)
