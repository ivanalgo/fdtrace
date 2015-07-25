#ifndef _HOOK_H
#define _HOOK_H

#define PARAM_1(type1, arg1) 		type1 arg1
#define PARAM_2(type2, arg2, ...)	type2 arg2, PARAM_1(__VA_ARGS__)
#define PARAM_3(type3, arg3, ...)	type3 arg3, PARAM_2(__VA_ARGS__)
#define PARAM_4(type4, arg4, ...)	type4 arg4, PARAM_3(__VA_ARGS__)

#define PARAM(num, ...)			PARAM_##num(__VA_ARGS__)

#define ARGS_1(type1, arg1)		arg1
#define ARGS_2(type2, arg2, ...)	arg2, ARGS_1(__VA_ARGS__)
#define ARGS_3(type3, arg3, ...)	arg3, ARGS_2(__VA_ARGS__)
#define ARGS_4(type4, arg4, ...)	arg4, ARGS_3(__VA_ARGS__)

#define ARGS(num, ...)			ARGS_##num(__VA_ARGS__)

#define PROTO(args...)	args
#define WRAPPER(args...) args

#define CALL_EXP(num, rtype, name, ...)						\
	__##name(ARGS(num, __VA_ARGS__))

#define DEFINE(num, rtype, name, ...)						\
	rtype name(PARAM(num, __VA_ARGS__))

#define CALL(retval, num, rtype, name, ...)					\
	retval = CALL_EXP(num, rtype, name, __VA_ARGS__);			\
	if (retval < 0)								\
		return retval;							


#define TYPE(num, rtype, name, ...)	rtype (*)(PARAM(num, __VA_ARGS__))

#define ACTION_NULL
#define ACTION_CREATE(fd)	mgmt_create_fd(fd)	

#define ACTION_ACCESS(fd)	mgmt_access_fd(fd)

#define ACTION_CLOSE(fd)	mgmt_close_fd(fd)

#define BEFOR(prev, next) prev
#define AFTER(prev, next) next

#define PRELOAD_LIBC_FUNC(name, proto, wrapper)					\
	typeof(TYPE(proto)) __##name = NULL;					\
										\
	static void probe_##name##_real_func(void) __attribute__((constructor));\
	static void probe_##name##_real_func(void)				\
	{									\
		__##name = dlsym(RTLD_NEXT, #name);				\
	}									\
										\
	DEFINE(proto)								\
	{									\
		int _return;							\
										\
		BEFOR(wrapper);							\
		CALL(_return, proto);						\
		AFTER(wrapper);							\
		return _return;							\
	}

#endif /* _HOOKS_H */
