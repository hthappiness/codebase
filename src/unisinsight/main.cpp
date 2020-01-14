

#include <iostream>

#include "libTest.h"

int test(int a, int b)
{
	std::cout<<"hello!"<<std::endl;
	return a+b;
}

int main()
{
	regTestFunc(test);
	
	CallBack stObj;

	return 0;
}

//g++ -std=c++11 libTest.cpp -fPIC -shared -o libtest.so
//g++ -std=c++11 main.cpp -L . -ltest -0 main
//
//