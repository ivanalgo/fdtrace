#include "hook.hpp"
#include "fd_mgmt.hpp"

#if 0
PRELOAD_LIBC_FUNC(
	ioctl,
	PROTO(3, int, ioctl, int, fd, int, request, long, arg),
	FAILURE(_return < 0),
	WRAPPER(ACTION_ACCESS(fd), ACTION_NULL)
)
#endif
