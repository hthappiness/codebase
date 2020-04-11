#include <iostream>
using namespace std;
 
//为何这样设计，为何这样定义？
//https://blog.csdn.net/veghlreywg/article/details/88556681
 
//新的对象模型
class A
{
public:
    A() :m_ptr(new int(0)){
		cout << "construct" << endl;
	}
	
	//深拷贝的拷贝构造函数
    A(const A& a):m_ptr(new int(*a.m_ptr)) 
    {
        cout << "copy construct" << endl;
    }
	
	//浅拷贝的移动构造函数
    A(A&& a) 
		:m_ptr(a.m_ptr)
    {
        a.m_ptr = nullptr;
        cout << "move construct" << endl;
    }
	
	// 赋值运算符重载
	A&& operator=(A&& a)
	{
		m_ptr = a.m_ptr;
		
		a.m_ptr = nullptr;
		cout << "set value operator" << endl;
	}
	
    ~A(){ delete m_ptr;}

    int* m_ptr;
};

A GetA()
{
    return A();
}

int main(){
    //A a = GetA(); 
	
	A p;
	
	A& p2  = p;
	
    A&& p1 = std::move(p); //这个做的什么操作,不涉及构造函数和运算符重载？
	
	//A&& p3 = p; // error: cannot bind ‘A’ lvalue to ‘A&&’
	
	std::cout << "1. This is :" << p.m_ptr << std::endl;
	std::cout << "2. This is :" << p1.m_ptr << std::endl;
	
	return 0;
}

//g++ -std=c++11 rvalue.cpp -o rvalue