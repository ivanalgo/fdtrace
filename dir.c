#include "hook.h"
#include "fd_mgmt.h"

PRELOAD_LIBC_FUNC(
	opendir,
	PROTO(1, dir_p_t, opendir, const_string_t, name),
	FAILURE(_return == NULL),
	WRAPPER(ACTION_NULL, ACTION_CREATE(dirfd(_return)))
)

PRELOAD_LIBC_FUNC(
	fdopendir,
	PROTO(1, dir_p_t, fdopendir, int, fd),
	FAILURE(_return == NULL),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	closedir,
	PROTO(1, int, closedir, dir_p_t, dirp),
	FAILURE(_return < 0),
	WRAPPER(ACTION_COMP(ACTION_ACCESS(dirfd(dirp)),
			    ACTION_CLOSE(dirfd(dirp))),
		ACTION_NULL
	)
)
