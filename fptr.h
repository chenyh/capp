#ifndef _FPTR_H_
#define _FPTR_H_

//------------------------------------------------------------------------------
/**
@class ptr

智能指针
*/

#define UNUSED_ARGUMENT(x) (void)x


namespace fs
{
	template<typename T>
	struct fptr_trait
	{
		enum{ auto_rawpointer=1 };
	};

	template<class T>
	class fptr
	{
		T* _prep;
	public:
		fptr();		
		fptr(const fptr<T>& p);
		fptr(T* p);

		~fptr();

		fptr<T>& operator=(const fptr<T>& r);
		fptr<T>& operator=(T* r);

		inline T& operator*() const;
		inline T* operator->() const;
		inline bool operator!() const;

		// 转成指针使用
		inline operator T*() const;
		// 检查指针是否有效
		inline bool isvalid() const;
		// 获取指针(安全)
		inline T* get() const;
		// 获取指针(非安全)
		inline T* get_unsafe() const ;

		//template <class OTHERTYPE> const ptr<OTHERTYPE>& cast() const { return *reinterpret_cast<const ptr<OTHERTYPE>*>(this);}
	};

	//------------------------------------------------------------------------------
	/**
	*/
	template <class T>
	fptr<T>::fptr():_prep(NULL)
	{
	}

	//------------------------------------------------------------------------------
	/**
	*/
	template <class T>
	fptr<T>::fptr(const fptr<T>& p)
	{
		_prep = p._prep;
		if(_prep){
			_prep->add_ref();
		}
	}

	//------------------------------------------------------------------------------
	/**
	*/
	template <class T>
	fptr<T>::fptr(T* p)
	{
		_prep = p;
		if(_prep) {
			_prep->add_ref();
		}
	}
	//------------------------------------------------------------------------------
	/**
	*/
	template <class T>
	fptr<T>::~fptr() 
	{	
		if(_prep){
			_prep->release(); 
			_prep = NULL;
		}
	}

	//------------------------------------------------------------------------------
	/**
	*/
	template <class T>
	inline T& 
		fptr<T>::operator*() const 
	{ 
		assert(_prep); 
		return *_prep; 
	}

	//------------------------------------------------------------------------------
	/**
	*/
	template <class T>
	inline T* 
		fptr<T>::operator->() const 
	{ 
		assert(_prep); 
		return _prep; 
	}

	//------------------------------------------------------------------------------
	/**
	*/
	template <class T>
	inline bool 
		fptr<T>::operator!() const
	{ 
		return _prep==NULL;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	template <class T>
	inline 
		fptr<T>::operator T*() const
	{
		int enauble_auto_rawpointer[fptr_trait<T>::auto_rawpointer];
		UNUSED_ARGUMENT(enauble_auto_rawpointer);
		return this->_prep;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	template <class T>
	inline bool 
		fptr<T>::isvalid() const 
	{ 
		return ( 0!= this->_prep);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	template <class T>		
	inline T* 
		fptr<T>::get() const 
	{ 
		assert(this->_prep && "NULL pointer access in T *ptr<T>::get()!");
		return this->_prep; 
	}

	//------------------------------------------------------------------------------
	/**
	*/
	template <class T>	
	inline T* 
		fptr<T>::get_unsafe() const 
	{
		return this->_prep;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	template <class T>
	fptr<T>& 
		fptr<T>::operator=(const fptr<T>& r)
	{
		if (_prep != r._prep){
			if( this->_prep ){
				this->_prep->release();
			}
			_prep = r._prep;
			if(this->_prep){
				this->_prep->add_ref();
			}
		}
		return *this;
	}


	//------------------------------------------------------------------------------
	/**
	*/
	template <class T>
	fptr<T>& 
		fptr<T>::operator=(T* r)
	{
		if (_prep != r){
			if( this->_prep ){
				this->_prep->release();
			}
			_prep = r;
			if(this->_prep){
				this->_prep->add_ref();
			}
		}
		return *this;
	}


	template<class T, class U> inline bool operator==(fptr<T> const& a, fptr<U> const& b)
	{
		return a.get_unsafe() == b.get_unsafe();
	}

	template<class T, class U> inline bool operator!=(fptr<T> const& a, fptr<U> const& b)
	{
		return a.get_unsafe() != b.get_unsafe();
	}


}// namespace fs




#define SAFE_ADDREF(p)		{ if(p){ p->add_ref();} }
#define SAFE_RELEASE(p) 	{ if ( p ) { (p)->release(); (p) = NULL;	} } 
#define	SAFE_DELETE(p)		{ if ( p ) { delete (p);	(p) = NULL;		} }	
#define SAFE_DELGRP(p)		{ if ( p ) { delete [](p); (p) = NULL;	} }	

#endif /* _FPTR_H_ */
