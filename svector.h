#pragma once
#include <iostream>

using namespace std;

typedef unsigned int size_u;

#define SIZE_ONCE  100

template<typename T>
class svector {
	public:
		typedef T Vtype;
		typedef Vtype* itor;
		~svector() {
			cout << "in destructor" << endl;
			destroy(_pFirst);
		}
		svector() 
		{
			_size = SIZE_ONCE;
			_len = 0;
			_pFirst = new Vtype[_size + 1];
			memset(_pFirst, 0, sizeof(Vtype) * (_size + 1));
			_pLast = _pFirst;
		}
		svector(Vtype val, size_u n) 
		{
			if (n <= 0) {svector();	return;}
			_size = (n / SIZE_ONCE + 1) * SIZE_ONCE;
			_len = n;
			_pFirst = new Vtype[_size + 1];
			for (int i = 0; i < n; i++) {
				_pFirst[i] = val;
			}
			_pLast = _pFirst;
		}

	protected:
		Vtype *_pFirst;
		Vtype *_pLast;
		size_u _size;
		size_u _len;
};

