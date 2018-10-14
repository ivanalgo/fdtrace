#include "hook.hpp"
#include "fd_mgmt.hpp"

/*
 * fcntl is a special function which has a variable-length argurments,
 * PRELOAD_LIBC_FUNC macro doesn't handle well with a ... args in PROTO.
 * So don't include fcntl.h avoid prototype conflicting. 
 */

#define	F_DUPFD	  	0	/* Duplicate file descriptor.  */

#if 0
PRELOAD_LIBC_FUNC(
        fcntl,
        PROTO(3, int, fcntl, int, fd, int, cmd, long, arg),
        FAILURE(_return == -1),
        WRAPPER(ACTION_NULL, ACTION_COMP(ACTION_ACCESS(fd),
                                         ACTION_IF(cmd == F_DUPFD, ACTION_CREATE(_return))))
)
#endif
