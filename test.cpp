#include <iostream>
using namespace std;

#include "function.h"
#include "typeinfo.h"

void foo()
{
	cout << "in foo" << endl;
}

void bar()
{
	cout << "in bar" << endl;
}

/*
class nulltype {};

template <typename F> struct function;
template <>
struct function<void (*)()>
{
	typedef nulltype object_type;
	typedef void result_type;
	typedef nulltype tlist;
	static const bool need_thispointer = false;
};
*/

template<typename F>
inline void get_function_info(const char *str, F func)
{
	// func;
	ctype_info  result_type = ctype_name<typename fs::function<F>::result_type>::info();
}

void test_main()
{
	cout << "hello" << endl;
	get_function_info("", foo);
	get_function_info("", bar);
	return 0;
}
