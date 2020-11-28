
#include <iostream>
#include <string>
#include <memory>

using namespace std;

class Point3d
{
public:
    int m_x;
    int m_y;
    int m_z;
public:
    Point3d(int x,int y,int z):m_x(x),m_y(y),m_z(z)
    {
        cout << "constructor"<<endl;
    }
    ~Point3d()
    {
        cout << "deconstructor"<<endl;
    }
    Point3d(const Point3d &other)
    {
        m_x = other.m_x;
        m_y = other.m_y;
        m_z = other.m_z;
        cout << "copy constructor"<<endl;
    }
    Point3d &operator=(const Point3d &other)
    {
        if(this != &other)
        {
            m_x = other.m_x;
            m_y = other.m_y;
            m_z = other.m_z;
        }
        cout << "operator="<<endl;
        return *this;
    }

    Point3d(Point3d &&other)
    {
        m_x = other.m_x;
        m_y = other.m_y;
        m_z = other.m_z;
        cout << " move constructor "<<endl;
    }
};

Point3d factory()
{
    Point3d po(1,2,3);
    return po;
}

std::unique_ptr<Point3d> create()
{
    auto p = std::unique_ptr<Point3d>(new Point3d(3, 4, 5));

    string test = "hello";
    cout << " point: " << p->m_x << p->m_y << p->m_z << std::endl;
    return p;
}

Point3d createObj()
{
    //涉及三个对象，函数内部生成的局部对象，返回值的临时对象，以及函数外定义的对象
    return Point3d(14, 15, 16);
}

int main()
{
    char tmp[100] = "123453";
    std::string str1 = tmp;
    std::string str2 = str1;

    std::cout << "tmp=" << std::string(tmp) << std::endl;
    std::cout << "str1=" << std::hex << str1.data() << std::endl;
    std::cout << "str2=" << std::hex << str2.data() << std::endl;

    char* ch = const_cast<char*>(str2.c_str());
    *(ch+3)='f';

    str2="hello world";

    std::cout << "tmp=" << std::string(tmp) << std::endl;
    std::cout << "str1=" << str1.data() << std::endl;
    std::cout << "str2=" << str2.data() << std::endl;
/*
    在开始的两个语句后，str1和str2存放数据的地址是一样的，而在修改内容后，
    str1的地址发生了变化，而str2的地址还是原来的。从这个例子，我们可以看到string类的Copy-On-Write技术。
    */

    std::cout <<"-------------------" << std::endl;
    {
        auto p = create();
        std::cout << "result = " << p->m_x << std::endl;
    }

    std::cout <<"-------------------" << std::endl;
    {
        std::cout << "result = " << createObj().m_x << std::endl;
    }

    std::cout <<"-------------------" << std::endl;
    {
        auto obj = createObj();
        std::cout << "result = " << obj.m_x << std::endl;
    }
}

//g++ --std=c++11 string_test.cpp -o string_test -g -fno-elide-constructors

// tmp=123453
// str1=123f53
// str2=hello world
// -------------------
// constructor
//  point: 345
// result = 3
// deconstructor
// -------------------
// constructor
//  move constructor
// deconstructor
// result = 14
// deconstructor
// -------------------
// constructor          构造函数内部调用生成对象
//  move constructor    构造函数返回值的临时对象
// deconstructor
//  move constructor    构造调用方定义的对象
// deconstructor
// result = 14
// deconstructor

//g++ --std=c++11 string_test.cpp -o string_test -g

// [root@vcloud-single stl]# ./string_test
// tmp=123453
// str1=123f53
// str2=hello world   //copy on write
// -------------------
// constructor
//  point: 345
// result = 3
// deconstructor
// -------------------
// constructor
// result = 14
// deconstructor
// -------------------
// constructor
// result = 14
// deconstructor

//开了NVO的情况下，一视同仁
// RVO (return value optimization) 和NRVO (named return value optimization) 