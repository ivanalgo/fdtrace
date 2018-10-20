#include <sys/types.h>
#include <dirent.h>

#include "hook.hpp"
#include "fd_mgmt.hpp"

PRELOAD_LIBC_FUNC(
	opendir,
	PROTO(1, DIR *, opendir, const char *, name),
	FAILURE(_return == NULL),
	WRAPPER(ACTION_NULL, ACTION_CREATE(dirfd(_return)))
)

PRELOAD_LIBC_FUNC(
	fdopendir,
	PROTO(1, DIR *, fdopendir, int, fd),
	FAILURE(_return == NULL),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	closedir,
	PROTO(1, int, closedir, DIR *, dirp),
	FAILURE(_return < 0),
	WRAPPER(ACTION_COMP(ACTION_ACCESS(dirfd(dirp)),
			    ACTION_CLOSE(dirfd(dirp))),
		ACTION_NULL
	)
)
