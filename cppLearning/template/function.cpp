#include <iostream>
#include <vector>
#include <type_traits>

template<typename R, typename... Args>
class B { // 桥接口：负责函数对象的所有权和操作
public: // 实现为抽象基类，作为类模板A动态多态的基础
    virtual ~B() {}    
    virtual B* clone() const = 0;
    virtual R invoke(Args... args) const = 0;
};

template<typename F, typename R, typename... Args>
class X : public B<R, Args...> { // 抽象基类的实现
private:
    F f; // 参数化存储的函数对象类型，以实现类型擦除
public:
    template<typename T>
    X(T&& f) : f(std::forward<T>(f)) {}

    virtual X* clone() const override
    {
        return new X(f);
    }

    virtual R invoke(Args... args) const override
    {
        return f(std::forward<Args>(args)...);
    }
};

// 原始模板
template<typename Signature>
class A;

// 偏特化
template<typename R, typename... Args>
class A<R(Args...)> {
private:
    B<R, Args...>* bridge; // 该指针负责管理函数对象
public:
    A() : bridge(nullptr) {}

    A(const A& other) : bridge(nullptr)
    {
        if (other.bridge)
        {
            bridge = other.bridge->clone();
        }
    }

    A(A& other) : A(static_cast<const A&>(other)) {}

    A(A&& other) noexcept : bridge(other.bridge)
    {
        other.bridge = nullptr;
    }

    template<typename F>
    A(F&& f) : bridge(nullptr)
    {
		//g++ --std=c++11 function.cpp -o function 这里需要添加typename
        using Functor = typename std::decay<F>::type;
        using Bridge = X<Functor, R, Args...>;
        bridge = new Bridge(std::forward<F>(f));
    }

    A& operator=(const A& other)
    {
        A tmp(other);
        swap(*this, tmp);
        return *this;
    }

    A& operator=(A&& other) noexcept
    {
        delete bridge;
        bridge = other.bridge;
        other.bridge = nullptr;
        return *this;
    }

    template<typename F>
    A& operator=(F&& f)
    {
        A tmp(std::forward<F>(f));
        swap(*this, tmp);
        return *this;
    }

    ~A() { delete bridge; }

    friend void swap(A& fp1, A& fp2) noexcept
    {
        std::swap(fp1.bridge, fp2.bridge);
    }

    explicit operator bool() const
    {
        return bridge == nullptr;
    }

    R operator()(Args... args) const
    {
        return bridge->invoke(std::forward<Args>(args)...);
    }
};

void forUpTo(int n, A<void(int)> f)
{
    for (int i = 0; i < n; ++i) f(i);
}

void print(int i)
{
    std::cout << i << ' ' <<std::endl;
}

int main()
{
    std::vector<int> v;
    forUpTo(5, [&v] (int i) { v.push_back(i); });
    forUpTo(5, print);
}