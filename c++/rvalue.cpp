#include <iostream>
#include <vector>
#include <memory>

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
	
	void show()
	{
		std::cout << "var = " << m_test << std::endl;
	}
	
	//浅拷贝的移动构造函数
    A(A&& a) 
		:m_ptr(a.m_ptr)
    {
        a.m_ptr = nullptr;
		a.m_test = 0;
        cout << "move construct" << endl;
    }
	
	// 赋值运算符重载
	A&& operator=(A&& a)
	{
		m_ptr = a.m_ptr;
		
		a.m_ptr = nullptr;
		cout << "set value operator" << endl;
	}

public:	
    ~A(){ 
		delete m_ptr;
		}

    int* m_ptr;
	int m_test{5};
};

A GetA()
{
    return A();
}

A&& GetALValue()
{
	 return A();
}

class Data
{
	int m_test;
};

//接收一个右值引用
void func(Data && data)
{
}

void func(std::unique_ptr<A> && uniA)
{
	cout << "0. Temp is :" << uniA->m_ptr  << std::endl;
	uniA->m_test = 10;
}

//unique_ptr做函数形参，引用并未触发拷贝，所以不涉及unique_ptr的限制（只能被一个所管理）
//void funcTest(unique_ptr<A>& uniA),这个调用就会报错，编译还不会出错，因为没有触发编译时检查，unique_ptr禁用了拷贝构造函数和赋值运算
void funcTest2(std::unique_ptr<A> uniA)
{
	cout << "0. Temp is :" << uniA->m_ptr  << std::endl;
	uniA->m_test = 10;
}

void funcTest(std::unique_ptr<A>& uniA)
{
	cout << "0. Temp is :" << uniA->m_ptr  << std::endl;
	uniA->m_test = 10;
}

void fRValue(A&& tmpA)
{
	cout << "0. Temp is :" << tmpA.m_ptr  << std::endl;
	tmpA.m_test = 10;
	
	A test = tmpA;
	/*这里呢?*/
	
	//A test = std::move(tmpA);  
	//触发移动构造函数；在这个语境下，tmpA是右值引用，则可以触发移动构造函数，这是其它形式无法完成的
}

void test()
{
	//test rvalue
	#if 0
	Data p;
    Data && p1 = std::move(p);
    func(p1); // [Error] cannot bind 'Data' lvalue to 'Data&&'
	#endif
	
	Data p;
	Data && p1 = std::move(p);
	func(std::forward<Data>(p1)); // OK
}

int main(){
    
	std::cout << "---------------------1--------------------------" << std::endl;
	A a = GetA(); //调用构造函数
	
	std::cout << "---------------------2--------------------------" << std::endl;
	A b = GetALValue(); //调用构造函数 和 移动构造函数

	std::cout << "---------------------3--------------------------" << std::endl;
	A p;
	A& p2  = p; // 这不涉及任何构造函数和赋值函数，只是一个引用
	p.show() ;
	
	std::cout << "---------------------4--------------------------" << std::endl;
    A pn = std::move(p); //这个做的什么操作,不涉及构造函数和赋值运算符重载？涉及的是移动构造函数
	std::cout << "move . This is :" << pn.m_ptr << std::endl;

	//A&& p3 = p; // error: cannot bind ‘A’ lvalue to ‘A&&’
	
	//f(p);//error
	//f(std::forward<A>(p));// ok
	//f(A());
	std::cout << "---------------------5--------------------------" << std::endl;
	fRValue(A()); //临时对象可绑定到右值引用上，而不能绑定到左值引用上，可以绑定到const 左值引用上，c++这样的设计是为了什么？
	//fRValue(std::move(p));
	
	//std::vector<A> vs;
	//vs.push_back(std::move(p));
	
	std::cout << "1. This is :" << p.m_ptr << std::endl;
	//std::cout << "2. This is :" << p1.m_ptr << std::endl;
	p.show() ;


	/* test unique_ptr, 通过禁用拷贝构造等方式在编译器检查错误 */
	#if 0
	auto uniqueA = std::unique_ptr<A>(new A());
	uniqueA->show();
	//func(std::move(uniqueA));
	funcTest(uniqueA);
	//std::cout << "test:" << uniqueA->m_test << std::endl;
	
	uniqueA->show();
	#endif
	
	//test git log
	
	return 0;
}

//g++ -std=c++11 rvalue.cpp -o rvalue