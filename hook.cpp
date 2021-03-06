#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/uio.h>
#include <errno.h>
#include <sys/socket.h>


#include "hook.hpp"
#include "fd_mgmt.hpp"
	
/* file operations */
PRELOAD_LIBC_FUNC(
	open,
	PROTO(3, int, open, const char *, pathname, int, flags, mode_t, mode),
	FAILURE(_return < 0),
	WRAPPER(ACTION_NULL, ACTION_CREATE(_return))
)

PRELOAD_LIBC_FUNC(
	open64,
	PROTO(3, int, open64, const char *, pathname, int, flags, mode_t, mode),
	FAILURE(_return < 0),
	WRAPPER(ACTION_NULL, ACTION_CREATE(_return))
)

PRELOAD_LIBC_FUNC(
	creat,
	PROTO(2, int, creat, const char *, pathname, mode_t, mode),
	FAILURE(_return < 0),
	WRAPPER(ACTION_NULL, ACTION_CREATE(_return))
)

PRELOAD_LIBC_FUNC(
	close,
	PROTO(1, int, close, int, fd),
	FAILURE(_return < 0),
	WRAPPER(ACTION_NULL, ACTION_CLOSE(fd))
)

PRELOAD_LIBC_FUNC(
	read,
	PROTO(3, ssize_t, read, int, fd, char *, buf, size_t, count),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	write,
	PROTO(3, ssize_t, write, int, fd, const char *, buf, size_t, count),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	readv,	
	PROTO(3, ssize_t, readv, int, fd, const struct iovec *, iov, int, iovcnt),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	writev,
	PROTO(3, ssize_t, writev, int, fd, const struct iovec *, iov, int, iovcnt),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	pread,
	PROTO(4, ssize_t, pread, int, fd, char *, buf, size_t, count, off_t, offset),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	pwrite,
	PROTO(4, ssize_t, pwrite, int, fd, const char *, buf, size_t, count, off_t, offset),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	lseek,
	PROTO(3, off_t, lseek, int, fd, off_t, offset, int, whence),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	fstat,
	PROTO(2, int, fstat, int, fd, struct stat *, buf),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	flock,
	PROTO(2, int, flock, int, fd, int, operation),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	fsync,
	PROTO(1, int, fsync, int, fd),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	fdatasync,
	PROTO(1, int, fdatasync, int, fd),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	ftruncate,
	PROTO(2, int, ftruncate, int, fd, off_t, length),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	fchdir,
	PROTO(1, int, fchdir, int, fd),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	fchmod,
	PROTO(2, int, fchmod, int, fd, mode_t, mode),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	fchown,
	PROTO(3, int, fchown, int, fd, uid_t, owner, gid_t, group),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	fstatfs,
	PROTO(2, int, fstatfs, int, fd, struct statfs *, buf),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	readahead,
	PROTO(3, ssize_t, readahead, int, fd, off64_t, offset, size_t, count),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	posix_fadvise,
	PROTO(4, int, posix_fadvise, int, fd, off_t, offset, off_t, len, int, advice),
	FAILURE(_return != 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

/* temp file operations */

PRELOAD_LIBC_FUNC(
	mkstemp,
	PROTO(1, int, mkstemp, char *, temp),
	FAILURE(_return < 0),
	WRAPPER(ACTION_NULL, ACTION_CREATE(_return))
)

PRELOAD_LIBC_FUNC(
	mkostemp,
	PROTO(2, int, mkostemp, char *, temp, int, flags),
	FAILURE(_return < 0),
	WRAPPER(ACTION_NULL, ACTION_CREATE(_return))
)
	
PRELOAD_LIBC_FUNC(
	mkstemps,
	PROTO(2, int, mkstemps, char *, temp, int, suffixlen),
	FAILURE(_return < 0),
	WRAPPER(ACTION_NULL, ACTION_CREATE(_return))
)

PRELOAD_LIBC_FUNC(
	mkostemps,
	PROTO(3, int, mkostemps, char *, temp, int, suffixlen, int, flags),
	FAILURE(_return < 0),
	WRAPPER(ACTION_NULL, ACTION_CREATE(_return))
)

/* duplication file operations */
PRELOAD_LIBC_FUNC(
	dup,
	PROTO(1, int, dup, int, oldfd),
	FAILURE(_return < 0),
	WRAPPER(ACTION_NULL, ACTION_COMP(ACTION_ACCESS(oldfd), ACTION_CREATE(_return)))
)

PRELOAD_LIBC_FUNC(
	dup2,
	PROTO(2, int, dup2, int, oldfd, int, newfd),
	FAILURE(_return < 0),
	WRAPPER(ACTION_NULL, ACTION_COMP(ACTION_ACCESS(oldfd), ACTION_CREATE(newfd)))
)

PRELOAD_LIBC_FUNC(
	dup3,
	PROTO(3, int, dup3, int, oldfd, int, newfd, int, flags),
	FAILURE(_return < 0),
	WRAPPER(ACTION_NULL, ACTION_COMP(ACTION_ACCESS(oldfd), ACTION_CREATE(newfd)))
)

/* C standard file operations */

PRELOAD_LIBC_FUNC(
	fopen,
	PROTO(2, FILE *, fopen, const char *, path, const char *, mode),
	FAILURE(_return == NULL),
	WRAPPER(ACTION_NULL, ACTION_CREATE(fileno(_return)))
)

PRELOAD_LIBC_FUNC(
	fopen64,
	PROTO(2, FILE *, fopen64, const char *, path, const char *, mode),
	FAILURE(_return == NULL),
	WRAPPER(ACTION_NULL, ACTION_CREATE(fileno(_return)))
)

/* pipe operations */
PRELOAD_LIBC_FUNC(
	pipe,
	PROTO(1, int, pipe, int *, pipefd),
	FAILURE(_return < 0),
	WRAPPER(ACTION_NULL, ACTION_COMP(ACTION_CREATE(pipefd[0]), ACTION_CREATE(pipefd[1])))
)

PRELOAD_LIBC_FUNC(
	pipe2,
	PROTO(2, int, pipe2, int *, pipefd, int, flags),
	FAILURE(_return < 0),
	WRAPPER(ACTION_NULL, ACTION_COMP(ACTION_CREATE(pipefd[0]), ACTION_CREATE(pipefd[1])))
)

/* socket operations */

PRELOAD_LIBC_FUNC(
	socket,
	PROTO(3, int, socket, int, domain, int, type, int, protocol),
	FAILURE(_return < 0),
	WRAPPER(ACTION_NULL, ACTION_CREATE(_return))
)

PRELOAD_LIBC_FUNC(
	socketpair,
	PROTO(4, int, socketpair, int, domain, int, type, int, protocol, int *, sv),
	FAILURE(_return < 0),
	WRAPPER(ACTION_NULL, ACTION_COMP(ACTION_CREATE(sv[0]), ACTION_CREATE(sv[1])))
)

/* specisal case: vfork, must be replace by fork 
 * child process spwaned by vfork can't modified the global data
 * because the parent and child shared the same data.
 * In generate child can close some fd before execve to a new image,
 * but in our approche, the close function will modify the fd_entry table.
 * So we need to replace vfork with fork, to avoid child modifying data 
 * which can be seen by parent.
 */
pid_t (*__fork) () = NULL;
static void probe_fork_real_func() __attribute__((constructor));
static void probe_fork_real_func()
{
	__fork = reinterpret_cast<pid_t (*)()>(dlsym(RTLD_NEXT, "fork"));
}

pid_t vfork()
{
	return __fork();
}

int loglevel = 0;
FILE *debugfp;

static void probe_debug(void) __attribute__((constructor(150)));
static void probe_debug(void)
{
	char *val = getenv("FDTRACE_DEBUG");
	char *debugfile = getenv("FDTRACE_FILE");
	FILE *fp = stderr;

	if (val)
		loglevel = atoi(val);

	if (debugfile && (fp = preloader_fopen.call(debugfile, "a")) == NULL) {
		fprintf(stderr, "Open file %s error, errno = %d %m\n",
				debugfile, errno);
		exit(1);
	}

	debugfp = fp;
}
