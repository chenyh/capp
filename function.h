#pragma once

#include "typeinfo.h"

/*
** 封神 typelist 实现
*/
namespace fs{

//----------------------------------------------------------------------------------
/*
** typelist
*/
class nulltype {};
 
template <typename HEAD, typename TAIL>
struct typelist {
   typedef HEAD head;
   typedef TAIL tail;
}; 

#define TYPELIST_1(T1)	typelist<T1, nulltype>
#define TYPELIST_2(T1, T2)	typelist<T1, TYPELIST_1(T2)>
#define TYPELIST_3(T1, T2, T3)	typelist<T1, TYPELIST_2(T2, T3)>
#define TYPELIST_4(T1, T2, T3, T4)	typelist<T1, TYPELIST_3(T2, T3, T4)>
#define TYPELIST_5(T1, T2, T3, T4, T5)   typelist<T1, TYPELIST_4(T2, T3, T4, T5)>
#define TYPELIST_6(T1, T2, T3, T4, T5, T6)   typelist<T1, TYPELIST_5(T2, T3, T4, T5, T6)>
#define TYPELIST_7(T1, T2, T3, T4, T5, T6, T7)   typelist<T1, TYPELIST_6(T2, T3, T4, T5, T6, T7)>
#define TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8)   typelist<T1, TYPELIST_7(T2, T3, T4, T5, T6, T7, T8)>
#define TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9)   typelist<T1, TYPELIST_8(T2, T3, T4, T5, T6, T7, T8, T9)>


//------------------------------------------------------------------------------------
/*
** typelist 信息
*/ 
template <typename> struct typelist_info;
 
template <typename HEAD, typename TAIL>
struct typelist_info< typelist<HEAD, TAIL> > {
	// 返回typelist长度
	static int len()
	{
		return 1 + typelist_info<TAIL>::len();
	}
	
	// 返回typelist类型列表
	static vector<ctype_info> types()
	{
		vector<ctype_info> arr = typelist_info<TAIL>::types();
		arr.insert( arr.begin(), ctype_name<HEAD>::info() );
		return arr;
	}
};

template <typename HEAD, typename TAIL>
struct typelist_info< typelist<const HEAD&, TAIL> > {
	// 返回typelist长度
	static int len()
	{
		return 1 + typelist_info<TAIL>::len();
	}
	// 返回typelist类型列表
	static vector<ctype_info> types()
	{
		vector<ctype_info> arr = typelist_info<TAIL>::types();
		arr.insert( arr.begin(), ctype_name<const HEAD&>::info() );
		return arr;
	}
};

template <typename HEAD, typename TAIL>
struct typelist_info< typelist<const HEAD*, TAIL> > {
	
	// 返回typelist长度
	static int len()
	{
		return 1 + typelist_info<TAIL>::len();
	}
	
	// 返回typelist类型列表
	static vector<ctype_info> types()
	{
		vector<ctype_info> arr = typelist_info<TAIL>::types();
		arr.insert( arr.begin(), ctype_name<const HEAD*>::info() );
		return arr;
	}
};

template <> 
struct typelist_info< nulltype > {
	// 返回typelist长度
	static int len()
	{
		return 0;
	}

	// 返回typelist类型列表
	static vector<ctype_info> types()
	{
		static vector<ctype_info> tarray;
		tarray.clear();
		return tarray;
	}
};


//////////////////////////////////////////////////////////////////////////
/*
** 函数识别
** to-do: 识别各种类型的参数列表( 2010-05-24 01:42 BlueQ )
*/

typedef void (*function_run_t)(void*, void*, vector<void *>, void* &);

//------------------------------------------------------------------------------
/** 应用类型转指针类型
*/

template <typename T> struct type_filter;

template <typename T> 
struct type_filter<T &>
{
	typedef T* type;
};

template <typename T>
struct type_filter<const T *>
{
	typedef T* type;
};


template <typename T>
struct type_filter<const T>
{
	typedef T type;
};

template <typename T> 
struct type_filter
{
	typedef T type;
};

#define TF(T) typename type_filter<T>::type
//----------------------------------------------------------------------------------
/*
** functor的实现
*/
template <typename F> struct function;

//----------------------------------------------------------------------------------
/*
** 静态函数
*/
//------------------------------------------------------------------------------
/** 0
*/
template <>
struct function<void (*)()>
{
	typedef nulltype object_type;
	typedef void result_type;
	typedef nulltype tlist;
	static const bool need_thispointer = false;
};

template <typename R>
struct function<R (*)()>
{
	typedef nulltype object_type;
	typedef R result_type;
	typedef nulltype tlist;
	static const bool need_thispointer = false;
};

//------------------------------------------------------------------------------
/** 1
*/
template <typename P1>
struct function<void (*)(P1)> 
{
	typedef nulltype object_type;
	typedef void result_type;
	typedef TYPELIST_1(P1) tlist;
	static const bool need_thispointer = false;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef void (*function_t)(TF(P1));
		function_t func = * (function_t *)&func_addr;
		func(*(TF(P1)*)&arg[0]);
		ret = (void *)0;
	}
};

template <typename R, typename P1>
struct function<R (*)(P1)> 
{
	typedef nulltype object_type;
	typedef R result_type;
	typedef TYPELIST_1(P1) tlist;
	static const bool need_thispointer = false;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef TF(R) (*function_t)(TF(P1));
		function_t func = * (function_t *)&func_addr;
		*(TF(R) *)&ret = func(*(TF(P1) *)&arg[0]);
	}
};
//------------------------------------------------------------------------------
/** 2
*/

template <typename P1, typename P2>
struct function<void (*)(P1, P2)> 
{
	typedef nulltype object_type;
	typedef void result_type;
	typedef TYPELIST_2(P1, P2) tlist;
	static const bool need_thispointer = false;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef void (*function_t)(TF(P1), TF(P2));
		function_t func = * (function_t *)&func_addr;
		func(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1]);
		ret = (void *)0;
	}
};
template <typename R, typename P1, typename P2>
struct function<R (*)(P1, P2)> 
{
	typedef nulltype object_type;
	typedef R result_type;
	typedef TYPELIST_2(P1, P2) tlist;
	static const bool need_thispointer = false;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef TF(R) (*function_t)(TF(P1), TF(P2));
		function_t func = * (function_t *)&func_addr;
		*(R*)&ret = func(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1]);
	}
};

//------------------------------------------------------------------------------
/** 3
*/

template <typename P1, typename P2, typename P3>
struct function<void (*)(P1, P2, P3)> 
{
	typedef nulltype object_type;
	typedef void result_type;
	typedef TYPELIST_3(P1, P2, P3) tlist;
	static const bool need_thispointer = false;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef void (*function_t)(TF(P1), TF(P2));
		function_t func = * (function_t *)&func_addr;
		func(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1]);
		ret = (void *)0;
	}
};
template <typename R, typename P1, typename P2, typename P3>
struct function<R (*)(P1, P2, P3)> 
{
	typedef nulltype object_type;
	typedef R result_type;
	typedef TYPELIST_3(P1, P2, P3) tlist;
	static const bool need_thispointer = false;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef TF(R) (*function_t)(TF(P1), TF(P2));
		function_t func = * (function_t *)&func_addr;
		*(R*)&ret = func(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1]);
	}
};


//------------------------------------------------------------------------------
/** 4
*/

template <typename P1, typename P2, typename P3, typename P4>
struct function<void (*)(P1, P2, P3, P4)> 
{
	typedef nulltype object_type;
	typedef void result_type;
	typedef TYPELIST_4(P1, P2, P3, P4) tlist;
	static const bool need_thispointer = false;

};
template <typename R, typename P1, typename P2, typename P3,typename P4>
struct function<R (*)(P1, P2, P3, P4)> 
{
	typedef nulltype object_type;
	typedef R result_type;
	typedef TYPELIST_4(P1, P2, P3, P4) tlist;
	static const bool need_thispointer = false;

};


//------------------------------------------------------------------------------
/** 5
*/
template <typename P1, typename P2, typename P3, typename P4, typename P5>
struct function<void (*)(P1, P2, P3, P4,P5)> 
{
	typedef nulltype object_type;
	typedef void result_type;
	typedef TYPELIST_5(P1, P2, P3, P4, P5) tlist;
	static const bool need_thispointer = false;

};
template <typename R, typename P1, typename P2, typename P3,typename P4, typename P5>
struct function<R (*)(P1, P2, P3, P4,P5)> 
{
	typedef nulltype object_type;
	typedef R result_type;
	typedef TYPELIST_5(P1, P2, P3, P4, P5) tlist;
	static const bool need_thispointer = false;

};


//----------------------------------------------------------------------------------
/*
** 类的成员函数
*/
//------------------------------------------------------------------------------
/** 0
*/
template <class O>
struct function<void (O::*)()>
{
	typedef O object_type;
	typedef void result_type;
	typedef nulltype tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef void (O::*function_t)();
		function_t func = * (function_t *)&func_addr;
		(((object_type *)this_object)->*func)();
		ret = (void *)0;
	}
};

template <class O, typename R>
struct function<R (O::*)()>
{
	typedef O object_type;
	typedef R result_type;
	typedef nulltype tlist;
	static const bool need_thispointer = true;
};

template <class O, typename R>
struct function<R (O::*)() const>
{
	typedef O object_type;
	typedef R result_type;
	typedef nulltype tlist;
	static const bool need_thispointer = true;
};

//------------------------------------------------------------------------------
/** 1
*/
template <class O, typename P1>
struct function<void (O::*)(P1)>
{
	typedef O object_type;
	typedef void result_type;
	typedef TYPELIST_1(P1) tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef void (O::* function_t)(TF(P1));
		function_t func = *(function_t *) & func_addr;
		(((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0]);
		ret = (void *)0;
	}
};

template <class O, typename R, typename P1>
struct function<R (O::*)(P1)>
{
	typedef O object_type;
	typedef R result_type;
	typedef TYPELIST_1(P1) tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef TF(R) (O::* function_t)(TF(P1));
		function_t func = *(function_t *) & func_addr;
		*(TF(R) *)&ret = std::mem_fun(func)((object_type *)this_object, *(TF(P1)*)&arg[0]);
	}
};

//------------------------------------------------------------------------------
/** 2
*/
template <class O, typename P1, typename P2>
struct function<void (O::*)(P1, P2)> 
{
	typedef O object_type;
	typedef void result_type;
	typedef TYPELIST_2(P1, P2) tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef void (O::*function_t)( TF(P1), TF(P2) );
		function_t func = *(function_t *) & func_addr;
		(((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1]);
		ret = (void *)0;
	}
};

template <class O, typename R, typename P1, typename P2>
struct function<R (O::*)(P1, P2)> 
{
	typedef O object_type;
	typedef R result_type;
	typedef TYPELIST_2(P1, P2) tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef TF(R) (O::*function_t)(TF(P1), TF(P2));
		function_t func = *(function_t *) & func_addr;
		*(TF(R)*)&ret = (((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1]);
	}
};
//------------------------------------------------------------------------------
/** 3
*/
template <class O, typename P1, typename P2, typename P3>
struct function<void (O::*)(P1, P2, P3)> 
{
	typedef O object_type;
	typedef void result_type;
	typedef TYPELIST_3(P1, P2, P3) tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef void (O::*function_t)(TF(P1), TF(P2), TF(P3));
		function_t func = *(function_t *) & func_addr;
		(((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1], *(TF(P3)*)&arg[2]);
		ret = (void *)0;
	}
};
template <class O, typename R, typename P1, typename P2, typename P3>
struct function<R (O::*)(P1, P2, P3)> 
{
	typedef O object_type;
	typedef R result_type;
	typedef TYPELIST_3(P1, P2, P3) tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef TF(R) (O::*function_t)(TF(P1), TF(P2), TF(P3));
		function_t func = *(function_t *) & func_addr;
		*(TF(R)*)&ret = (((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1], *(TF(P3)*)&arg[2]);
	}
};

//------------------------------------------------------------------------------
/** 4
*/
template <class O, typename P1, typename P2, typename P3, typename P4>
struct function<void (O::*)(P1, P2, P3, P4)> 
{
	typedef O object_type;
	typedef void result_type;
	typedef TYPELIST_4(P1, P2, P3, P4) tlist;
	static const bool need_thispointer = true;


	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef void (O::*function_t)(TF(P1), TF(P2), TF(P3), TF(P4));
		function_t func = *(function_t *) & func_addr;
		(((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1], *(TF(P3)*)&arg[2], *(TF(P4)*)&arg[3]);
		ret = (void *)0;
	}
};

template <class O, typename R, typename P1, typename P2, typename P3, typename P4>
struct function<R (O::*)(P1, P2, P3, P4)> 
{
	typedef O object_type;
	typedef R result_type;
	typedef TYPELIST_4(P1, P2, P3, P4) tlist;
	static const bool need_thispointer = true;


	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef TF(R) (O::*function_t)(TF(P1), TF(P2), TF(P3), TF(P4));
		function_t func = *(function_t *) & func_addr;
		*(TF(R)*)&ret = (((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1], *(TF(P3)*)&arg[2], *(TF(P4)*)&arg[3]);
	}
};

//------------------------------------------------------------------------------
/** 5
*/
template <class O, typename P1, typename P2, typename P3, typename P4, typename P5>
struct function<void (O::*)(P1, P2, P3, P4, P5)> 
{
	typedef O object_type;
	typedef void result_type;
	typedef TYPELIST_5(P1, P2, P3, P4, P5) tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef void (O::*function_t)(TF(P1), TF(P2), TF(P3), TF(P4),TF(P5));
		function_t func = *(function_t *) & func_addr;
		(((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1], *(TF(P3)*)&arg[2], *(TF(P4)*)&arg[3], *(TF(P5)*)&arg[4]);
		ret = (void *)0;
	}
};

template <class O, typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
struct function<R (O::*)(P1, P2, P3, P4, P5)> 
{
	typedef O object_type;
	typedef R result_type;
	typedef TYPELIST_5(P1, P2, P3, P4, P5) tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef TF(R) (O::*function_t)(TF(P1), TF(P2), TF(P3), TF(P4),TF(P5));
		function_t func = *(function_t *) & func_addr;
		*(TF(R)*)&ret = (((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1], *(TF(P3)*)&arg[2], *(TF(P4)*)&arg[3], *(TF(P5)*)&arg[4]);
	}
};
//------------------------------------------------------------------------------
/** 6
*/
template <class O, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
struct function<void (O::*)(P1, P2, P3, P4, P5, P6)> 
{
	typedef O object_type;
	typedef void result_type;
	typedef TYPELIST_6(P1, P2, P3, P4, P5, P6) tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef void (O::*function_t)(TF(P1), TF(P2), TF(P3), TF(P4), TF(P5), TF(P6));
		function_t func = *(function_t *) & func_addr;
		(((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1], *(TF(P3)*)&arg[2],  *(TF(P4)*)&arg[3],*(TF(P5)*)&arg[4], *(TF(P6)*)&arg[5]);
		ret = (void *)0;
	}
};

template <class O, typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
struct function<R (O::*)(P1, P2, P3, P4, P5, P6)> 
{
	typedef O object_type;
	typedef R result_type;
	typedef TYPELIST_6(P1, P2, P3, P4, P5, P6) tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef TF(R) (O::*function_t)(TF(P1), TF(P2), TF(P3), TF(P4), TF(P5), TF(P6));
		function_t func = *(function_t *) & func_addr;
		*(TF(R)*)&ret = (((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1], *(TF(P3)*)&arg[2],  *(TF(P4)*)&arg[3],*(TF(P5)*)&arg[4], *(TF(P6)*)&arg[5]);
	}
};
//------------------------------------------------------------------------------
/** 7
*/

template <class O, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
struct function<void (O::*)(P1, P2, P3, P4, P5, P6, P7)> 
{
	typedef O object_type;
	typedef void result_type;
	typedef TYPELIST_7(P1, P2, P3, P4, P5, P6, P7) tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef void (O::*function_t)(TF(P1), TF(P2), TF(P3), TF(P4), TF(P5), TF(P6), TF(P7));
		function_t func = *(function_t *) & func_addr;
		(((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1], *(TF(P3)*)&arg[2],  *(TF(P4)*)&arg[3],*(TF(P5)*)&arg[4], *(TF(P6)*)&arg[5], *(TF(P7)*)&arg[6]);
		ret = (void *)0;
	}
};

template <class O, typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
struct function<R (O::*)(P1, P2, P3, P4, P5, P6, P7)> 
{
	typedef O object_type;
	typedef R result_type;
	typedef TYPELIST_7(P1, P2, P3, P4, P5, P6, P7) tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef TF(R) (O::*function_t)(TF(P1), TF(P2), TF(P3), TF(P4), TF(P5), TF(P6), TF(P7));
		function_t func = *(function_t *) & func_addr;
		*(TF(R)*)&ret = (((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1], *(TF(P3)*)&arg[2],  *(TF(P4)*)&arg[3],*(TF(P5)*)&arg[4], *(TF(P6)*)&arg[5], *(TF(P7)*)&arg[6]);
	}
};
//------------------------------------------------------------------------------
/** 8
*/
template <class O, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
struct function<void (O::*)(P1, P2, P3, P4, P5, P6, P7, P8)> 
{
	typedef O object_type;
	typedef void result_type;
	typedef TYPELIST_8(P1, P2, P3, P4, P5, P6, P7, P8) tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef void (O::*function_t)(TF(P1), TF(P2), TF(P3), TF(P4), TF(P5), TF(P6), TF(P7), TF(P8));
		function_t func = *(function_t *) & func_addr;
		(((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1], *(TF(P3)*)&arg[2],  *(TF(P4)*)&arg[3],*(TF(P5)*)&arg[4], *(TF(P6)*)&arg[5], *(TF(P7)*)&arg[6], *(TF(P8)*)&arg[7]);
		ret = (void *)0;
	}
};

template <class O, typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
struct function<R (O::*)(P1, P2, P3, P4, P5, P6, P7, P8)> 
{
	typedef O object_type;
	typedef R result_type;
	typedef TYPELIST_8(P1, P2, P3, P4, P5, P6, P7, P8) tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef TF(R) (O::*function_t)(TF(P1), TF(P2), TF(P3), TF(P4), TF(P5), TF(P6), TF(P7), TF(P8));
		function_t func = *(function_t *) & func_addr;
		*(TF(R)*)&ret = (((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1], *(TF(P3)*)&arg[2],  *(TF(P4)*)&arg[3],*(TF(P5)*)&arg[4], *(TF(P6)*)&arg[5], *(TF(P7)*)&arg[6], *(TF(P8)*)&arg[7]);
	}
};
//------------------------------------------------------------------------------
/** 9
*/
template <class O,  typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
struct function<void (O::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9)> 
{
	typedef O object_type;
	typedef void result_type;
	typedef TYPELIST_9(P1, P2, P3, P4, P5, P6, P7, P8, P9) tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef void (O::*function_t)(TF(P1), TF(P2), TF(P3), TF(P4), TF(P5), TF(P6), TF(P7), TF(P8), TF(P9));
		function_t func = *(function_t *) & func_addr;
		(((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1], *(TF(P3)*)&arg[2],  *(TF(P4)*)&arg[3],*(TF(P5)*)&arg[4], *(TF(P6)*)&arg[5], *(TF(P7)*)&arg[6], *(TF(P8)*)&arg[7], *(TF(P9)*)arg[8]);
		ret = (void *)0;
	}
};


template <class O, typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
struct function<R (O::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9)> 
{
	typedef O object_type;
	typedef R result_type;
	typedef TYPELIST_9(P1, P2, P3, P4, P5, P6, P7, P8, P9) tlist;
	static const bool need_thispointer = true;

	static void run(void* this_object, void* func_addr, vector<void *> arg, void* &ret){
		assert(arg.size() == typelist_info<tlist>::len());
		typedef TF(R) (O::*function_t)(TF(P1), TF(P2), TF(P3), TF(P4), TF(P5), TF(P6), TF(P7), TF(P8), TF(P9));
		function_t func = *(function_t *) & func_addr;
		*(TF(R)*)&ret = (((object_type *)this_object)->*func)(*(TF(P1)*)&arg[0], *(TF(P2)*)&arg[1], *(TF(P3)*)&arg[2],  *(TF(P4)*)&arg[3],*(TF(P5)*)&arg[4], *(TF(P6)*)&arg[5], *(TF(P7)*)&arg[6], *(TF(P8)*)&arg[7], *(TF(P9)*)arg[8]);
	}
};
} // namespace fs



