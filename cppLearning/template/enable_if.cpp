
#include <iostream>

#include <vector>
#include <assert.h>

#if 0
template <typename T, typename Enable=void>
struct check;
 
template <typename T>
struct check<T, typename std::enable_if<T::value>::type> {
  static constexpr bool value = T::value;

 static_assert(T::value == false, "test static assert.");
};

class CTest
{
public:
    static bool const value{true};
};
#endif

//stack用vector实现，或者用deque实现...

template<typename T>
class Stack {
public:
    void push(T*);
    T* pop();
    T* top() const;
    bool empty() const;
private:
    std::vector<T*> v;
};

template<typename T>
void Stack<T>::push(T* x)
{
    v.emplace_back(x);
}

template<typename T>
T* Stack<T>::pop()
{
    assert(!v.empty());
    T* p = v.back();
    v.pop_back();
    return p;
}

template<typename T>
T* Stack<T>::top() const
{
    assert(!v.empty());
    return v.back();
}

template<typename T>
bool Stack<T>::empty() const
{
    return v.empty();
}

int main()
{
    //check<CTest> test;

    Stack<int> s;
    s.push(new int{42});
    std::cout << *s.top(); 
    delete s.pop();

    std::cout << "output" << std::endl;
}