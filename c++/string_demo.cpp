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
    }
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
    }
};

class CDerivedB : public CDerivedA
{
public:
    virtual void print()
    {
        std::cout << "This is B class." << std::endl;
    }

    void test()
    {
        proxy();
    }
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
    CDerivedB obj;
    obj.test();

    return 0;
}