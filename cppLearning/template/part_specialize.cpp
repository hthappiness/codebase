
#include <iostream>
#include <type_traits>

template <typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
is_odd(T t) {
  return bool(t%2);
}
 
template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
bool is_even(T t) {
  return !is_odd(t); 
}

template<typename T>
typename std::enable_if<(sizeof(T) > 4)>::type
f() {
     std::cout << "here we are. bool=" << (sizeof(T) > 4) << std::endl;
}


int main()
{
    int a = 10;
    double b = 11;
    std::cout << "ret = " << is_even(a) << std::endl;

    //如果(sizeof(T) > 4)为false，模板的定义将被忽略
    //std::enable_if是一个type traits，
    //编译期表达式作为首个模板实参传递。若表达式为false，
    //则enable_if::type未定义，由于SFINAE，这个带有enable_if的函数模板将被忽略。
    //若表达式为true，enable_if::type产生一个类型，若有第二个实参，
    //则类型为第二个实参类型，否则为void
    f<long>();
}

#if 0
template<typename T1, typename T2> 
class A
{};

// 偏特化：两个模板参数有相同类型
template<typename T> 
class A<T, T>
{};

// 偏特化：第二个模板参数类型为int
template<typename T> 
class A<T, int>
{};

// 偏特化：两个模板参数都是指针类型
template<typename T1, typename T2> 
class A<T1*, T2*>
{};

A<int, double> a; // A<T1, T2>
A<double, float> b; // A<T, T>
A<double, int> c; // A<T, int>
A<int*, double*> d; // A<T1*, T2*>
#endif