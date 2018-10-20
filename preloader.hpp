#include <dlfcn.h>
#include <iostream>
using namespace std;

template <class T>
void print(T arg)
{
	cout << arg << endl;
}

template <class T, class... Args>
void print(T head, Args... rest)
{
	cout << head;
	cout << ",";
	print(rest...);
}

template <class _rtype, class... _arg_type>
struct preloader {
	_rtype (*real_func)(_arg_type...);

	preloader(const char *func_name) {
		cout << "preloader_" << func_name << "init..." << endl;
		real_func = reinterpret_cast<_rtype (*)(_arg_type...)>(dlsym(RTLD_NEXT, func_name));
	}

	_rtype call(_arg_type... args) {
		return real_func(args...);
	}

	void trace(_rtype value, _arg_type... args) {
		print(args...);
		print(value);
	}
};
