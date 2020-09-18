# 概述

​	编译期间计算，包括模板推理deduction、代码生成、编译期”函数“

​	编译期可以确定的信息，运行期才能确定的信息



  模板是一种编程范式，一种新的组织类和函数等实体形式的方法

# 基本概念

## 内嵌依赖类型

nested dependent type name

```c++
template <class _Iterator>
struct iterator_traits 
{ 
    typedef typename _Iterator::iterator_category iterator_category; 
    typedef typename _Iterator::value_type value_type; 
    typedef typename _Iterator::difference_type difference_type; 
    typedef typename _Iterator::pointer pointer; 
    typedef typename _Iterator::reference reference;
    
    //T::const_iterator * x;编译器并不知道const_iterator是一个类型还是一个静态变量
};
```

内嵌是指定义在类名的定义中的。以上difference_type和value_type都是定义在iterator_traits中的。

依赖是指依赖于一个模板参数。 typename iterator_traits::difference_type中difference_type依赖于模板参数_InputIter。

类型名是指这里最终要指出的是个类型名，而不是变量。例如iterator_traits::difference_type完全有可能是类iterator_traits 类里的一个static对象。而且当我们这样写的时候，C＋＋默认就是解释为一个变量的。所以，为了和变量区分，必须使用typename告诉编译器。

那么是不是所有的T::type_or_variable, 或者tmpl:type_or_variable都需要使用typename呢？不是，有以下两个例外。



在类的继承列表和初始化列表中不需要添加typename

```c++
template<typename T>
class Derived: public Base<T>::Nested //in base class list, no typename
{
　public:
　　explicit Derived(int x)
　　: Base<T>::Nested(x) //in member init list, no typename
　　{
　　　typename Base<T>::Nested temp; //nested dependent type, need typename
　　}
};
```



另外一个例子

```c++
T::SubType* ptr;

//如果没有加typename前缀，上式会被解析为一个乘法，而不是声明一个指针;不知道SubType是类型还是静态变量，依赖于编译器的策略
(T::SubType) * ptr;
```

## 特化

specialization

全特化 full specialization

偏特化 part specialization

## 成员模板

> 类的成员也可以是模板，嵌套类和成员函数都可以是模板
>
> 正常情况下不能用不同类型的类互相赋值

# 模板基础

模板提供了另一种形式的抽象，在编译期确定类型之间的聚合关系

匹配

> 模板与具体调用之间的匹配关系，在这个匹配过程中，设定了一系列的规则
>
> 同样还有，函数“模型”与调用之间的匹配，比如形参和实参之间的匹配关系
>
> 匹配错误：编译期匹配，编译不过；匹配到了不符合程序员意志的模板



编译期可以完成的任务

编程语言的基本内容是 **具体** 和 **抽象** 之间相互关联的定义，而模板提供了一种组合的中间形式，模板参数，与变量、类和函数的结合，便于在编译期生成对应实体，拥有确定行为的类

模板的抽象表达能力，理解了就是一片蓝天



## 函数模板

[函数模板](https://en.cppreference.com/w/cpp/language/function_template)自身不是类型或函数或任何实体。从仅包含模板定义的源文件不生成代码。为使得代码出现，必须实例化模板，除了隐式实例化，还可以显式实例化

## 类模板



## 变量模板

variable-template

## 非类型模板参数



## 可变模板参数

sizeof操作符，语法如下：

```c++
template<typename T, typename... Types>
void print(T firstArg, Types... args)
{
    std::cout << sizeof...(Types) << '\n'; // print number of remaining types
    std::cout << sizeof...(args) << '\n'; // print number of remaining args
}
```

匹配规则

```c++
void print() {} // 没有参数时将调用此函数

template<typename T, typename... Types>
void print(T firstArg, Types... args)
{
    std::cout << firstArg << ' ';  // 打印第一个实参，无参数时将调用此函数
    print(args...); // 调用print()打印其余实参
}

int main()
{
    std::string s("world");
    print(3.14, "hello", s); // 3.14 hello world
}
```

如果两个函数模板只有尾置参数包不同，会优先匹配没有尾置参数包的版本

```c++
template<typename T>
void print(T x)
{
    std::cout << x << ' ';
}

template<typename T, typename... Types>
void print(T firstArg, Types... args)
{
    print(firstArg);
    print(args...);
}
```

## 模板的模板参数（Template Template Parameter）

定义：指一个模板，它本身的参数类型就是模板，在英语原文中是“template template parameter”。

例如：template<typename T1,  template<typename T2> class CONT>，就是将template参数类型声明中的一个（原本应该是typename T2）替换成template<typename T2> class CONT。



这个特性的用途就在于，不仅仅以类型为模板，还以模板本身为模板，模板的泛化；举例来说，我们需要定义一个类，但类的元素类型未知，存储该类型的容器也不知道是什么类型，所以需要定义两个模板参数，但问题是，容器本身就是模板，然后再把它重新定义为模板（有种递归，嵌套的感觉），



模板参数之间的关系

```c++
Stack<int, std::vector<int>> s; //int 的重复
// 通过模板的模板参数可以写为
Stack<int, std::vector> s; //这样的stack如何定义？
//我们可以让Stack的第二个参数直接是一个模板，而不再是一个具体类型

//为此必须把第二个模板参数指定为模板的模板参数
template<typename T,
    template<typename Elem> class Cont = std::deque>
class Stack {
public:
    void push(const T&);
    void pop();
    const T& top() const;
    bool empty() const { return v.empty(); }
private:
    Cont<T> v;
};


template<typename T,
    template<typename Elem> class Cont = std::deque>
class Stack {
public:
    void push(const T&);
    void pop();
    const T& top() const;
    bool empty() const { return v.empty(); }
private:
    Cont<T> v;
};
```

以模板作为模板参数，那么传入的参数就有了T<T>的行为，而不只是类型行为，比如定义对象，T obj，或者T::func T::staticMember



https://www.jianshu.com/p/c94184e295d7



## 其他用法

### 使用.template

有时调用一个成员模板，显式限定模板实参是有必要的，此时必须使用template关键字来确保`<`是模板实参列表的开始。下面这个例子中，如果没有template，编译器就不知道`<`是小于号还是模板实参列表的开始

```
template<unsigned long N>
void f(const std::bitset<N>& b)
{
    std::cout << b.template to_string<char, std::char_traits<char>, std::allocator<char>>();
    // .template只需要用于依赖于模板参数的名称之后，比如这里的b依赖于模板参数N
}
```



# 高级



# 模板与设计



# 参考

https://downdemo.gitbook.io/cpp-templates-2ed