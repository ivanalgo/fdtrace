#ifndef _HOOK_H
#define _HOOK_H

#include <string.h>
#include <stdarg.h>
#include <assert.h>

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

#define PROTO(args...)	 args
#define WRAPPER(args...) args
#define FAILURE(args...) args

#define CALL_EXP(num, rtype, name, ...)						\
	__##name(ARGS(num, __VA_ARGS__))

#define DEFINE(num, rtype, name, ...)						\
	rtype name(PARAM(num, __VA_ARGS__))

#define CALL(retval, num, rtype, name, ...)					\
	retval = CALL_EXP(num, rtype, name, __VA_ARGS__);			\
	if (retval < 0)								\
		return retval;							

const char *type2fmt(const char *type)
{
	char buf[64] = "Error type ";

	if (strcmp(type, "int") == 0) return "%d";
	if (strcmp(type, "long") == 0) return "%l";
	if (strcmp(type, "char") == 0) return "%c";
	if (strcmp(type, "char *") == 0) return "\"%s\"";
	if (strcmp(type, "const char *") == 0) return "\"%s\"";
	if (strcmp(type, "mode_t") == 0) return "0%03o";
	if (strcmp(type, "void *") == 0) return "%p";
	/* FIXME size_t and ssize_t is different in 32 and 64 bit systems ??? */
	if (strcmp(type, "size_t") == 0) return "%u";
	if (strcmp(type, "ssize_t") == 0) return "%d"; 

	if (strcmp(type, "FILE *") == 0) return "%p";

	return strcat(buf, type);
}

const char *generate_print_fmt(char *buf, const char *func, int num, ...)
{
	va_list ap;
	const char *rtype = NULL;
	const char *type = NULL;

	assert(num >= 1);

	strcat(buf, func);
	strcat(buf, " (");

	va_start(ap, num);			
	rtype = va_arg(ap, const char *);
	num--;
	
	while (num > 0) {
		type = va_arg(ap, const char *);
		strcat(buf, type2fmt(type));
		if (num > 1)
			strcat(buf, ", ");

		num--;
	}

	strcat(buf, ") = ");
	strcat(buf, type2fmt(rtype));
	strcat(buf, "\n");

	return buf;
}

#define TYPES_STR_1(type1, val1)	#type1
#define TYPES_STR_2(type2, val2, ...)	#type2, TYPES_STR_1(__VA_ARGS__)
#define TYPES_STR_3(type3, val3, ...)	#type3, TYPES_STR_2(__VA_ARGS__)
#define TYPES_STR_4(type4, val4, ...)	#type4, TYPES_STR_3(__VA_ARGS__)

#define TYPES_STR(num, ...)	TYPES_STR_##num(__VA_ARGS__)

#define PRINT_FMT(buf, retval, num, rtype, name, ...)				\
	generate_print_fmt(buf, #name, num + 1, TYPES_STR(1, rtype, 0), TYPES_STR(num, __VA_ARGS__))

#define PRINT_ARGS_1(type1, val1)	val1
#define PRINT_ARGS_2(type2, val2, ...)	val2, PRINT_ARGS_1(__VA_ARGS__)
#define PRINT_ARGS_3(type3, val3, ...)	val3, PRINT_ARGS_2(__VA_ARGS__)
#define PRINT_ARGS_4(type4, val4, ...)	val4, PRINT_ARGS_3(__VA_ARGS__)
#define PRINT_ARGS(retval, num, rtype, name, ...)				\
	PRINT_ARGS_##num(__VA_ARGS__), retval	

#define TRACE_LOG(retval, num, rtype, name, ...)				\
	{									\
		char buf[256] = "";						\
		printf(PRINT_FMT(buf, retval, num, rtype, name, __VA_ARGS__),	\
	   	       PRINT_ARGS(retval, num, rtype, name, __VA_ARGS__));	\
	}


#define TYPE(num, rtype, name, ...)	rtype (*)(PARAM(num, __VA_ARGS__))
#define RTYPE(num, rtype, name, ...)	rtype

#define ACTION_NULL
#define ACTION_CREATE(fd)	mgmt_create_fd(fd)	
#define ACTION_ACCESS(fd)	mgmt_access_fd(fd)
#define ACTION_CLOSE(fd)	mgmt_close_fd(fd)
#define ACTION_COMP(act1, act2)	act1;act2

#define BEFOR(prev, next) prev
#define AFTER(prev, next) next

#define PRELOAD_LIBC_FUNC(name, proto, failure, wrapper)			\
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
		RTYPE(proto) _return;						\
										\
		BEFOR(wrapper);							\
		CALL(_return, proto);						\
		TRACE_LOG(_return, proto);					\
		if (failure)							\
			return _return;						\
		AFTER(wrapper);							\
		return _return;							\
	}

#endif /* _HOOKS_H */
