#include <vector>
using namespace std;

typedef	char				int8;
typedef	short				int16;
typedef	int					int32;
typedef	long long			int64;

typedef	unsigned char		uint8;
typedef	unsigned short		uint16;
typedef	unsigned int		uint32;
typedef	unsigned long long	uint64;
typedef float float32;
typedef double float64;

	enum point_type{
		PTR_NONE, // 非指针
		PTR_NORMAL, // 普通指针
		PTR_SMART, // 智能指针，自动管理对象的应用计数
	};

	struct ctype_info
	{
	public:
		enum basetype 
		{
			T_VOID,
			T_BOOL,
			T_INT8,
			T_UINT8,
			T_INT16,
			T_UINT16,
			T_INT32,
			T_UINT32,
			T_INT64,
			T_UINT64,
			T_F32,
			T_F64,			
			T_OBJECT,			//自定义类型
		};

	public:
		basetype	_type;
		point_type	_pointer;
		bool		_const;
		bool		_is_array;
		int16		_org_size;	//原始大小
		int16		_alg_size;	//对齐大小
		const char*	_name;

	public:
		ctype_info(){ memset(this,0,sizeof(*this));}
		ctype_info(const ctype_info& r){ memcpy(this,&r,sizeof(r));}
		ctype_info(basetype t,point_type p,bool c,uint32 s,const char* n):_type(t),_pointer(p),_const(c),_org_size(s),_alg_size((s+3)/4*4),_name(n){}
		const char* get_name() const{ return _name;}
		basetype get_basetype() const{ return _type;}
		uint32 get_org_size() const{ return _org_size;}
		uint32 get_alg_size() const{ return _alg_size;}
		bool is_builtin() const{ return _type<=T_F64;}
		bool is_pointer() const{ return _pointer != PTR_NONE;}
		bool is_const() const{ return _const;}
		bool is_normal_ptr() const { return this->_pointer == PTR_NORMAL; }
		bool is_smart_ptr() const { return this->_pointer == PTR_SMART; } 
		bool is_string() const { return _type==T_INT8&&is_pointer()&&is_const();}
		bool is_array() const { return _is_array; }
		
	};


	template <typename> struct ctype_name;

// to-do:只支持基本类型， 只返回数组的引用类型
#define DEF_ARRAY_TYPE(t,d)\
	template <>\
	struct ctype_name<vector<t>&>\
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::d,PTR_NONE,false,sizeof(t),0);\
			info._is_array = true;\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<vector<t*>&> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::d,PTR_NORMAL,false,sizeof(t*),0);\
			info._is_array = true;\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<vector<t&>&> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::d,PTR_NORMAL,false,sizeof(t*),0);\
			info._is_array = true;\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<vector<t>*>\
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::d,PTR_NONE,false,sizeof(t),0);\
			info._is_array = true;\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<vector<t*>*> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::d,PTR_NORMAL,false,sizeof(t*),0);\
			info._is_array = true;\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<vector<t&>*> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::d,PTR_NORMAL,false,sizeof(t*),0);\
			info._is_array = true;\
			return info;\
		}\
	};

#define DEF_BASE_TYPE(t,d)\
	template <>\
	struct ctype_name<t>\
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::d,PTR_NONE,false,sizeof(t),0);\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<t*> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::d,PTR_NORMAL,false,sizeof(t*),0);\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<t&> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::d,PTR_NORMAL,false,sizeof(t*),0);\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<const t> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::d,PTR_NONE,true,sizeof(t),0);\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<const t*> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::d,PTR_NORMAL,true,sizeof(t*),0);\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<const t&> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::d,PTR_NORMAL,true,sizeof(t*),0);\
			return info;\
		}\
	};\
	DEF_ARRAY_TYPE(t, d)


#define DEF_ENUM_TYPE(t)\
	template <>\
	struct ctype_name<t>\
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_UINT32,PTR_NONE,false,sizeof(t),0);\
			return info;\
		}\
	};\

#define DEF_VOID_TYPE(t)\
	template <>\
	struct ctype_name<t>\
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_VOID,PTR_NONE,false,0,0);\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<t*>\
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_VOID,PTR_NORMAL,false,sizeof(t*),0);\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<const t*>\
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_VOID,PTR_NORMAL,true,sizeof(t*),0);\
			return info;\
		}\
	};\

#define DEF_USER_TYPE(t,d)\
	template <>\
	struct ctype_name<t>\
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_OBJECT,PTR_NONE,false,sizeof(t),#d);\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<t*> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_OBJECT,PTR_NORMAL,false,sizeof(t*),#d);\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<t&> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_OBJECT,PTR_NORMAL,false,sizeof(t*),#d);\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<const t> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_OBJECT,PTR_NONE,true,sizeof(t),#d);\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<const t*> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_OBJECT,PTR_NORMAL,true,sizeof(t*),#d);\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<const t&> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_OBJECT,PTR_NORMAL,true,sizeof(t*),#d);\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name< fs::fptr<t> > \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_OBJECT,PTR_SMART,true,sizeof(t*),#d);\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<vector<t>&>\
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_OBJECT,PTR_NONE,false,sizeof(t),#d);\
			info._is_array = true;\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<vector<t*>&> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_OBJECT,PTR_NORMAL,false,sizeof(t*),#d);\
			info._is_array = true;\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<vector<t&> &> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_OBJECT,PTR_NORMAL,false,sizeof(t*),#d);\
			info._is_array = true;\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<vector<t>*>\
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_OBJECT,PTR_NONE,false,sizeof(t),#d);\
			info._is_array = true;\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<vector<t*>*> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_OBJECT,PTR_NORMAL,false,sizeof(t*),#d);\
			info._is_array = true;\
			return info;\
		}\
	};\
	template <>\
	struct ctype_name<vector<t&>*> \
	{\
		static ctype_info info()\
		{\
			static ctype_info info(ctype_info::T_OBJECT,PTR_NORMAL,false,sizeof(t*),#d);\
			info._is_array = true;\
			return info;\
		}\
	};

	

#define DEF_USER_TYPE_SIMPLE(t)\
	DEF_USER_TYPE(t,t)



//////////////////////////////////////////////////////////////////////////
	DEF_BASE_TYPE(bool,		T_BOOL);
	DEF_BASE_TYPE(int8,		T_INT8);
	DEF_BASE_TYPE(uint8,	T_UINT8);
	DEF_BASE_TYPE(int16,	T_INT16);
	DEF_BASE_TYPE(uint16,	T_UINT16);
	DEF_BASE_TYPE(int32,	T_INT32);
	DEF_BASE_TYPE(uint32,	T_UINT32);
	DEF_BASE_TYPE(int64,	T_INT64);
	DEF_BASE_TYPE(uint64,	T_UINT64);
	DEF_BASE_TYPE(float,	T_F32);
	DEF_BASE_TYPE(double,	T_F64);
	DEF_VOID_TYPE(void);
	
	DEF_USER_TYPE_SIMPLE(string);


//////////////////////////////////////////////////////////////////////////
//灰常高端的技术。。

	//提取基础类型
	template<typename T> struct BaseType { typedef T _Result; };
	template<typename T> struct BaseType<T*> { typedef T _Result; };
	template<typename T> struct BaseType<T&> { typedef T _Result; };
	template<typename T> struct BaseType<const T*> { typedef T _Result; };
	template<typename T> struct BaseType<const T&> { typedef T _Result; };
	template<typename T> struct BaseType<fs::fptr<T> > { typedef T _Result; };
	template<typename T> struct BaseType<fs::fptr<T>& > { typedef T _Result; };
	template<typename T> struct BaseType<const fs::fptr<T>& > { typedef T _Result; };


	//检测T是否能转成U (U是父类)
	template <class T , class U>
	class __conversion
	{
		static char test(U);
		static double test(...);
		static T maket();
	public:
		enum{exists = sizeof(test(maket())) == sizeof(char) };
		enum{exists2way = exists && __conversion<U,T>::exists};
		enum{sametype = false};
	};

	template <class T>
	class __conversion<T,T>
	{
	public:
		enum{exists = 1};
		enum{exists2way = 1};
		enum{sametype = 1};
	};

	//判断U是否是T的父类
#define SUPERSUBCLASS(T,U) (__conversion<const T*,const U*>::exists && !__conversion<const U*,void*>::sametype)

	//判断T是否是U的父类
#define SUPERSUBCLASS_STRICT(T,U) (SUPERSUBCLASS(T,U) && !__conversion<const T, const U>::sametype)


	template<class B, class D>	ptrdiff_t get_base_offset() { return ((char*)static_cast<B*>((D*)0x4000000)) - (char*)0x4000000; }

	//取两个类之间的偏移，如果是可以转换的，返回其偏移，如果是不能转换的，返回一个特殊值
	template<class B,class D,bool t=SUPERSUBCLASS(D,B)>
	struct base_offset
	{


	};

	template<class B,class D>
	struct base_offset<B,D,0>
	{
		static bool is_exist(){ return false;}
		static int get(){ return 0;} //无意义了

	};

	template<class B,class D>
	struct base_offset<B,D,1>
	{
		static bool is_exist(){ return true;}
		static int get(){ return static_cast<int>(get_base_offset< B, D >()); }
	};
