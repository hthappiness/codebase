#include <iostream>
#include <vector>
#include <memory>

using namespace std;
 
//为何这样设计，为何这样定义？
//https://blog.csdn.net/veghlreywg/article/details/88556681
 
//c++对象模型
//1. 各种构造函数的调用
//2. 聚合的对象初始化顺序 reorder
//3. 

static int index = 0;

//新的对象模型
class A
{
public:
    A() :m_ptr(new int(0))
	{
		cout << "construct index = " << index << endl;
		m_index = index;
		index++;
	}
	
	//深拷贝的拷贝构造函数
    A(const A& a):m_ptr(new int(*a.m_ptr)) 
    {
		m_index = a.m_index;
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
		m_index = a.m_index;
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
    ~A()
	{
		cout << "deconstruct called, index = " << m_index << endl;
		// 有的对象是通过移动构造函数生成的
		if( nullptr != m_ptr )
		{ 
			delete m_ptr;
			m_ptr = nullptr;
			m_test = -1;
			cout << "deconstruct, index = " << m_index << endl;
			//cout << "deconstruct ： " << *m_ptr << endl; //这里重复析构，导致段错误
		}
	}

    int* m_ptr;
	int m_test{5};
	int m_index;
};

A GetA()
{
	auto a = A();
    //return A();
	return a;
}

A&& GetALValue()
{
	 return A();
}

const A& GetTmpValue()
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
//void funcTest(unique_ptr<A>& uniA),这个调用就会报错，编译还不会出错，
//因为没有触发编译时检查，unique_ptr禁用了拷贝构造函数和赋值运算
void funcTest2(std::unique_ptr<A> uniA)
{
	cout << "funcTest2 0. Temp is :" << uniA->m_ptr  << std::endl;
	uniA->m_test = 10;
}

void funcTest(std::unique_ptr<A>& uniA)
{
	cout << "funcTest 0. Temp is :" << uniA->m_ptr  << std::endl;
	uniA->m_test = 10;
}

#if 1
void fRValue(A& tmpA)
{
	cout << "fRValue 1. Temp is :" << tmpA.m_ptr  << std::endl;
	tmpA.m_test = 11;
	
	//A test = tmpA;  //区分构造行为和赋值行为
	/*这里呢?这里，tmpA就是一个左值，触发拷贝构造函数*/
	
	A test = std::move(tmpA);  
	//触发移动构造函数；在这个语境下，tmpA是右值引用，则可以触发移动构造函数，这是其它形式无法完成的
}
#else

void fRValue(A&& tmpA)
{
	cout << "fRValue 0. Temp is :" << tmpA.m_ptr  << std::endl;
	tmpA.m_test = 10;
	
	//A test = tmpA;  //区分构造行为和赋值行为
	/*这里呢?这里，tmpA就是一个左值，触发拷贝构造函数*/
	
	A test = std::move(tmpA);  
	//触发移动构造函数；在这个语境下，tmpA是右值引用，则可以触发移动构造函数，这是其它形式无法完成的
}
#endif
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
	/* 在不开启返回值优化的时候，可见，两次移动构造函数，两次析构 
	---------------------1--------------------------
	construct index = 0
	move construct
	deconstruct, index = 0
	move construct
	deconstruct, index = 0
 	*/
	
	std::cout << "---------------------2--------------------------" << std::endl;
	//A b = GetALValue(); //调用构造函数 和 移动构造函数
	A&& c = GetALValue();
	c.m_test = 224;
	std::cout << "This is :" << c.m_test << std::endl;

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
	//fRValue(A()); //临时对象可绑定到右值引用上，而不能绑定到左值引用上，可以绑定到const 左值引用上，c++这样的设计是为了什么？
	//fRValue(std::move(p));
	A tempA;
	fRValue(tempA);
	std::cout << "---------1. This is :" << tempA.m_ptr << std::endl;
	
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

	std::cout << "--------------------11--------------------------" << std::endl;
	{
		const A& d = GetTmpValue();
		std::cout << "This is :" << d.m_test << std::endl;
	}
	
	//test git log
	
	return 0;
}

//g++ -std=c++11 rvalue.cpp -o rvalue

#if 0
[root@iaemb01 c++]# ./rvalue
---------------------1--------------------------
construct index = 0
---------------------2--------------------------
construct index = 1
deconstruct, index = 1
This is :224
---------------------3--------------------------
construct index = 2
var = 5
---------------------4--------------------------
move construct
move . This is :0x2024030
---------------------5--------------------------
construct index = 3
0. Temp is :0x2024050
copy construct
deconstruct, index = 32766
deconstruct, index = 3
1. This is :0
var = 0
--------------------11--------------------------
construct index = 4
deconstruct, index = 4
This is :-1
deconstruct, index = 0
deconstruct, index = 2
deconstruct, index = 0
[root@iaemb01 c++]#
#endif