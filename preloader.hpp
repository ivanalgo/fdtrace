#include <iostream>
using namespace std;

template <class T>
void print(T item)
{
	cout << item << endl;
}

template <class T, class... Args>
void print(T head, Args... rest)
{
	cout << head;
	cout << ",";
	print(rest);
}

template <class _rtype, class... _arg_type>
struct preloader {
	_rtype (*real_func)(_arg_type);

	preloader(const char *func_name) {
		real_func = dlsym(RTLD_NEXT, func_name);
	}

	_rtype call(_arg_type) {
		return real_func(_args_type);
	}

	void print_log(_rtype value, ...arg_type) {
		print(arg_type);
		print(value);
	}
};
