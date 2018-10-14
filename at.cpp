#include "hook.hpp"
#include "fd_mgmt.hpp"

#include <sys/types.h>
#include <fcntl.h>

#if 0
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

PRELOAD_LIBC_FUNC(
	fchownat,
	PROTO(5, int, fchownat, int, dirfd, const_string_t, pathname, uid_t, owner, gid_t, group, int, flags),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(dirfd), ACTION_NULL)	
)

PRELOAD_LIBC_FUNC(
	futimesat,
	PROTO(3, int, futimesat, int, dirfd, const_string_t, pathname, const_timeval_pair_t, times),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(dirfd), ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	unlinkat,
	PROTO(3, int, unlinkat, int, dirfd, const_string_t, pathname, int, flags),
	FAILURE(_return < 0),
	WRAPPER(ACTION_IF(pathname[0] != '/' && dirfd != AT_FDCWD, ACTION_ACCESS(dirfd)), ACTION_NULL)	
)

PRELOAD_LIBC_FUNC(
	renameat,
	PROTO(4, int, renameat, int, olddirfd, const_string_t, oldpath, int, newdirfd, const_string_t, newpath),
	FAILURE(_return < 0),
	WRAPPER(ACTION_COMP(
			ACTION_IF(oldpath[0] != '/' && olddirfd != AT_FDCWD, ACTION_ACCESS(olddirfd)),
			ACTION_IF(newpath[0] != '/' && newdirfd != AT_FDCWD, ACTION_ACCESS(newdirfd))
		),
		ACTION_NULL)
)

PRELOAD_LIBC_FUNC(
	linkat,
	PROTO(5, int, linkat, int, olddirfd, const_string_t, oldpath, int, newdirfd, const_string_t, newpath, int, flags),
	FAILURE(_return < 0),
	WRAPPER(ACTION_COMP(
			ACTION_IF(oldpath[0] != '/' && olddirfd != AT_FDCWD, ACTION_ACCESS(olddirfd)),
			ACTION_IF(newpath[0] != '/' && newdirfd != AT_FDCWD, ACTION_ACCESS(newdirfd))
		),
		ACTION_NULL)
)
#endif
