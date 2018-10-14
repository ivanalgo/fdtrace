#include "hook.hpp"
#include "fd_mgmt.hpp"

PRELOAD_LIBC_FUNC(
	openat,
	PROTO(4, int, openat, int, dirfd, const_string_t, pathname, int, flags, mode_t, mode),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(dirfd), ACTION_CREATE(_return))
)
