//#include <iostream>

#include "Ioc.hpp"
#include "lexical_cast.hpp"

using namespace std;

class test{
public:
	test():m_var(0){
	}	
	int setConst(int var){
		//m_var = var;
	}
private:
	const int m_var;
};

int main(int argc, char* argv[])
{
	int a=10;
	int& b=a;
	int c=1111;
	b=c;
	cout<<"a="<<a<<endl;
	cout<<"b="<<b<<endl;
	
	test tmp;
	//tmp.setConst(1);
	return 0;
}
