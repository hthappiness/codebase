#include <iostream>

using namespace std;

/* 
1. 关注构造和析构顺序
2. 关注virtual 虚构函数
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
  int a{1};
};

class B
{ 
public:
  B()
  {
    std::cout << "B is constructed." << std::endl;
  }
  ~B()
  {
    std::cout << "B is destructed." << std::endl;
  }
};

class C{
  public:
  C()
  {
    std::cout << "C is constructed" << std::endl;
  }
  C(int d)
  {
    (void)declval;
    std::cout << "C is constructed" << std::endl;
  }
   ~C()
  {
    std::cout << "C is destructed." << std::endl;
  }
};

//关注构造顺序
class D : public A{
 public:
  D():A(1)
  {
    std::cout << "D is constructed" << std::endl;
  } 
  // 这里不加virtual不妨碍
  ~D()
  {
    std::cout << "D is destructed." << std::endl;
  }
  int a{2};

  B b;
  C c;
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
    A* a = new D();
    delete a;
  #endif
}

//g++ --std=c++11 con-destruct.cpp -o what