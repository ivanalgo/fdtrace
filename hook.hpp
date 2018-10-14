#ifndef _HOOK_H
#define _HOOK_H

#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <dirent.h>

#include "preloader.hpp"

extern int loglevel;
extern FILE *debugfp;

#define PROTO(...)		__VA_ARGS__
#define WRAPPER(...)		__VA_ARGS__

#define ACTION_NULL
#define ACTION_CREATE(fd)	mgmt_create_fd(fd)	
#define ACTION_ACCESS(fd)	mgmt_access_fd(fd)
#define ACTION_CLOSE(fd)	mgmt_close_fd(fd)
#define ACTION_COMP(act1, act2)	act1;act2
#define ACTION_IF(cond, act)	if (cond) act
#define ACTION_IF_ELSE(cond, act1, act2)	if (cond) act1 else act2

#define BEFORE(prev, next) prev
#define AFTER(prev, next) next

#define ARG_TYPES_1(type1, value1)		type1
#define ARG_TYPES_2(type2, value2, ...)		type2, ARG_TYPES_1(__VA_ARGS__)
#define ARG_TYPES_3(type3, value3, ...)		type3, ARG_TYPES_2(__VA_ARGS__)
#define ARG_TYPES_4(type4, value4, ...)		type4, ARG_TYPES_3(__VA_ARGS__)

#define template_return_arg_types(num, rtype, name, ...)			\
	rtype, ARG_TYPES_##num(__VA_ARGS__)

#define PAAS_ARGS_1(type1, value1)		value1
#define PAAS_ARGS_2(type2, value2, ...)		value2, PAAS_ARGS_1(__VA_ARGS__)
#define PAAS_ARGS_3(type3, value3, ...)		value3, PAAS_ARGS_2(__VA_ARGS__)
#define PAAS_ARGS_4(type4, value4, ...)		value4, PAAS_ARGS_3(__VA_ARGS__)

#define template_pass_args(num, rtype, name, ...)				\
	PAAS_ARGS_##num(__VA_ARGS__)

#define arg_type_values_1(type1, value1)	type1 value1
#define arg_type_values_2(type2, value2, ...)	type2 value2, arg_type_values_1(__VA_ARGS__)
#define arg_type_values_3(type3, value3, ...)	type3 value3, arg_type_values_2(__VA_ARGS__)
#define arg_type_values_4(type4, value4, ...)	type4 value4, arg_type_values_3(__VA_ARGS__)

#define arg_type_values(num, ...)						\
	arg_type_values_##num(__VA_ARGS__)

#define FUNCTION_DEFINE(num, rtype, name, ...)					\
	rtype name(arg_type_values(num, __VA_ARGS__))

#define RTYPE(num, rtype, name, ...)	rtype

#define _return reval
#define FAILURE(exp)	exp

#define PRELOAD_LIBC_FUNC(name, proto, _failure, _wrapper)		\
	static preloader<template_return_arg_types(proto)> preloader_##name(#name);     \
										\
	FUNCTION_DEFINE(proto)							\
	{									\
		typeof(RTYPE(proto)) retval;						\
		BEFORE(_wrapper);						\
		retval = preloader_##name.call(template_pass_args(proto));	\
		preloader_##name.log(retval, template_pass_args(proto));		\
		if (!(_failure)) {						\
			AFTER(_wrapper);					\
		}								\
		return retval;							\
	}

#endif /* _HOOKS_H */
