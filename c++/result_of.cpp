// result_of example
#include <iostream>
#include <type_traits>
#include <functional>

int fn(int) {return int();}                            // function
typedef int(&fn_ref)(int);                             // function reference
typedef int(*fn_ptr)(int);                             // function pointer
struct fn_class { int operator()(int i){return i;} };  // function-like class

class CTestResult
{
public:
    CTestResult(int test)
        :m_test(test)
    {

    }

    int testMemFunc(int a)
    {
        std::cout << "This is the member function. " << a << std::endl;
    }

    int m_test;
};

int main() {
  typedef std::result_of<decltype(fn)&(int)>::type A;  // int
  typedef std::result_of<fn_ref(int)>::type B;         // int
  typedef std::result_of<fn_ptr(int)>::type C;         // int
  typedef std::result_of<fn_class(int)>::type D;       // int

  std::cout << "bool alpha:" << std::boolalpha;
  std::cout << "typedefs of int:" << std::endl;
#if 0
  std::function<void (int)> func = [](int a)
  {
      std::cout << "This is the member function. var = " << a << std::endl;
  };

  std::result_of<func, int>::type E;
    function is a function wrapper , not function object. 
#endif

  typedef std::result_of<decltype(&CTestResult::testMemFunc)(int)>::type E;
  std::cout << "A: " << std::is_same<int,A>::value << std::endl;
  std::cout << "B: " << std::is_same<int,B>::value << std::endl;
  std::cout << "C: " << std::is_same<int,C>::value << std::endl;
  std::cout << "D: " << std::is_same<int,D>::value << std::endl;

  return 0;
}