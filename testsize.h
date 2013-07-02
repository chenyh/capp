#pragma once

#include<iostream>
#include<string>

using namespace std;

namespace testsize {


	template<typename T>
	void tsize(T& o)
	{
		cout<<"template size"<<sizeof(T)<<" "<<sizeof(o)<<endl;    //4
	}

	int test()
	{
		string str="abc";
		char st[]="abc";
		char a='a';
		char *q=&a;
		int array[5]={1,2,3,4,5};
		int *p=array;
		int size=sizeof(array);
		cout<<size<<endl;         //20
		cout<<sizeof(p)<<endl;    //4
		cout<<sizeof(str)<<endl;  //16
		cout<<sizeof(char)<<endl; //1
		cout<<sizeof(int)<<endl;  //4
		cout<<sizeof(st)<<endl;   //4
		cout<<sizeof(q)<<endl;    //4
		cout<<sizeof(&a)<<endl;   //4





		tsize(p);         //    template size8 8
		tsize(str);       //    template size8 8
		tsize(st);        //    template size4 4
		tsize(q);         //    template size8 8
		tsize(array);     //    template size20 20
		return 0;
	}
}
