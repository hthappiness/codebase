

#include <iostream>
#include "libTest.h"

TEST pfTest;

int regTestFunc(TEST pfnReg)
{
	pfTest = pfnReg;
	
	return 0;
}

CallBack::CallBack()
{
	int a;
	if ( pfTest != nullptr)
	{
		a = pfTest(2, 3);
		std::cout<<"calc result:"<<a<<std::endl;
	}
}