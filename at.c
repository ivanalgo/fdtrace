#include "hook.h"
#include "fd_mgmt.h"

#include <sys/types.h>

PRELOAD_LIBC_FUNC(
	openat,
	PROTO(4, int, openat, int, dirfd, const_string_t, pathname, int, flags, mode_t, mode),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(dirfd), ACTION_CREATE(_return))
)

PRELOAD_LIBC_FUNC(
	fmkdirat,
	PROTO(3, int, fmkdirat, int, dirfd, const_string_t, pathname, mode_t, mode),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(dirfd), ACTION_NULL)	
)

PRELOAD_LIBC_FUNC(
	mknodat,
	PROTO(4, int, mknodat, int, dirfd, const_string_t, pathname, mode_t, mode, dev_t, dev),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(dirfd), ACTION_NULL)
)
