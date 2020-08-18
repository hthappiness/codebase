#include <iostream>

using namespace std;

/* 
1. 关注virtual 虚构函数
2. 关注overload和override函数
*/

class A
{
public:
    A()
    {
        std::cout << "A is constructed." << std::endl;
    }
    A(int c)
    {
        (void)c;
        std::cout << "A is constructed" << std::endl;
    }
    //如果析构函数不置为virtual,那么delete 子类对象的父类指针，会出现调用不到子类的析构函数
    virtual ~A()
    {
        std::cout << "A is destructed." << std::endl;
    }

    virtual int operation_a()
    {
         std::cout << "A:operation_a called." << std::endl;
         std::cout << "a value=" << a << std::endl;
    }

    int operation_b()
    {
        std::cout << "A:operation_b called." << std::endl;
        std::cout << "a value=" << a << std::endl;
    }
    int a{1};
};

//关注构造顺序
class B : public A
{
public:
    B()
    {
        std::cout << "B is constructed" << std::endl;
    }
    // 这里不加virtual不妨碍
    ~B()
    {
        std::cout << "B is destructed." << std::endl;
    }

    virtual int operation_a()
    {
         std::cout << "B:operation_a called." << std::endl;
         std::cout << "a value=" << a << std::endl;
    }

    int operation_b()
    {
         std::cout << "B:operation_a called." << std::endl;
         std::cout << "a value=" << a << std::endl;
    }
    int a{2};
};

/* 
A is constructed
B is constructed.
C is constructed
D is constructed 
*/

int main()
{
#if 0
    D d;
#else
    A *a = new B();
    a->operation_a();  // B:operation_a called.
    a->operation_b();  // A:operation_b called.
    delete a;

    std::cout << "-----------------" << a << std::endl;

    B *c = new B();
    c->operation_a();  // B:operation_a called.
    c->operation_b();  // B:operation_b called.
    delete c;

    //变量类型决定其调用哪个函数
    //而virtual带来的多态影响其效果
#endif
}

//g++ --std=c++11 virtual.cpp -o what