/*
函数模板的隐式实例化和显式实例化

1. 隐式实例化：
  1 #include <iostream>
  2 #include <typeinfo>
  3 using namespace std;
  4 
  5 template <typename T>
  6 bool compare(T a,T b)
  7 {
  8     cout<<typeid(T).name()<<endl;
  9     cout<<typeid(a).name()<<endl;
 10     cout<<typeid(b).name()<<endl;
 11     return a > b;
 12 }
 13 
 14 int main()
 15 {   
        //隐式实例化，根据编译的时候
 16     compare(100,220);
 17     compare<int>(120,99);
 18     compare<double>(999,100.0);
 19     return 0;
 20 } 

以上 compare(100,220), compare<int>(120,99),compare<double>(999,100.0)等
都属于隐式的实例化,产生相应类型的模板函数.

2. 显式实例化:

我们知道,一般的我们都将模板相关的都放在一个头文件中供使用者包含使用,如STL等
在这里创建两个源文件,template.cc 和 main.cc ,内容如下:

//template.cc

//不进行编译,就不会产生任何类型的函数,自然就不会产生符号了,链接时一定会找不到符号
template <typename T>
bool compare(T a,T b)
 {
     cout<<typeid(T).name()<<endl;
     cout<<typeid(a).name()<<endl;
     cout<<typeid(b).name()<<endl;
     return a > b;
 }
1
2
3
4
5
6
7
8
9
10
11
//main.cc

//声明
template<typename T>
bool compare(T a,T b);

int main()
{
    compare(100,220);
    compare<int>(120,99);
    compare<double>(999,100.0);
    return 0;
} 

正常情况下,这样的代码是编译不通过的,因为函数模板不编译,所以就不会产生符号,
这样在main.cc中压根就找不到compare各个类型的函数符号.
产生一个链接时的错误,即找不到模板函数的符号.说白了就是找不到函数的定义.

所以,我们可以在template.cc中就把各个类型的函数产生出来,也就是在template.cc中显式的实例化出这些函数.

如何,显式实例化产生这些类型的函数代码呢?
*/
//template.cc

template <typename T>
bool compare(T a,T b)
 {
     cout<<typeid(T).name()<<endl;
     cout<<typeid(a).name()<<endl;
     cout<<typeid(b).name()<<endl;
     return a > b;
 }
 
 //显式实例化
 template bool compare<int>(int,int);
 template bool compare<float>(float,float);
 template bool compare<double>(double,double);
 
 //我们知道,程序编译的时候各个cpp文件是单独编译的
 //当编译到该文件的时候,编译器看到这三行代码后,会产生三种模板函数
 //类型分别为int  float  double
 //这样当我在其他源文件调用的时候,自然就不会产生链接的错误了,因为符号已经产生了.

/*
总结:

一般,模板的使用或者实例化的时候一定要看到模板的定义.建议将模板写头文件中.使用的时候直接将头文件包含即可引用;
模板的意义在于泛型编程,我们无法预知模板的使用者使用的时候究竟用什么样的类型进行实例化,所以,还是将模板放在头文件较为稳妥;
*/