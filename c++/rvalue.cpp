#include <iostream>
using namespace std;
 
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
    A(A&& a) :m_ptr(a.m_ptr)
    {
        a.m_ptr = nullptr;
        cout << "move construct" << endl;
    }
    ~A(){ delete m_ptr;}
private:
    int* m_ptr;
};

A GetA()
{
    return A();
}

int main(){
    A a = GetA(); 
}