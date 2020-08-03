#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class CBase
{
public:
    virtual void print()
    {
        std::cout << "This is base class." << std::endl;
          std::cout << "" << a << "\n";
    }

    int a {5};
};

class CDerivedA : public CBase
{
public:
    void proxy()
    {
        std:: cout << "It is A proxy" <<std::endl;
        //测试用父类的函数去调用虚函数
        this->print();
        print();
    }

    virtual void print()
    {
        std::cout << "This is A class." << std::endl;
        std::cout << "" << a << "\n";
    }

    int a {6};
};

class CDerivedB : public CDerivedA
{
public:
    virtual void print()
    {
        std::cout << " This is B class." << std::endl;
          std::cout << " a = " << a << "\n";
    }

    void test()
    {
        proxy();
    }

     int a {7};  
     
     //同名变量和同名函数
};

int main()
{
    string src("Avatar 123 5.2 Titanic K");
    istringstream istrStream(src); //建立src到istrStream的联系
    string s1, s2;
    int n;  double d;  char c;
    
    cout << istrStream.str() << endl;
    istrStream >> s1 >> n >> d >> s2 >> c; //把src的内容当做输入流进行读取,从输入流中读取数据
    
    ostringstream ostrStream;
    ostrStream << s1 << endl << s2 << endl << n << endl << d << endl << c <<endl;
    cout << ostrStream.str();

    std::cout << "-----------------" << std::endl;
    CDerivedB* pobj = new CDerivedB();
    
    CBase* pBase = static_cast<CBase*>(pobj);
    //obj.test();

    pBase->print();

    std::cout << "-----------------a = "  << pBase->a << std::endl;   // 5

    return 0;
}


#if 0 
// static_pointer_cast example
#include <iostream>
#include <memory>

struct A {
  static const char* static_type;
  const char* dynamic_type;
  A() { dynamic_type = static_type; }
};
struct B: A {
  static const char* static_type;
  B() { dynamic_type = static_type; }
};

const char* A::static_type = "class A";
const char* B::static_type = "class B";

int main () {
  std::shared_ptr<A> foo;
  std::shared_ptr<B> bar;

  bar = std::make_shared<B>();

  foo = std::dynamic_pointer_cast<A>(bar);

  std::cout << "foo's static  type: " << foo->static_type << '\n';
  std::cout << "foo's dynamic type: " << foo->dynamic_type << '\n';
  std::cout << "bar's static  type: " << bar->static_type << '\n';
  std::cout << "bar's dynamic type: " << bar->dynamic_type << '\n';

  return 0;
}

Output:
foo's static  type: class A
foo's dynamic type: class B
bar's static  type: class B
bar's dynamic type: class B

#endif