#include <thread>
#include <memory>
#include <iostream>
#include <chrono>

//http://www.cplusplus.com/forum/general/272397/

class R
{
  std::shared_ptr<std::thread> th;
  public:
  R(void) { };
  virtual ~R(void) { };
  virtual void poll(void) = 0;
  void init_th(void) { th = std::make_shared<std::thread>(&R::poll, this); };
  void deinit_th(void) { th->join(); };
};

class C : public R
{
  int m;
  public:
  C(int n) : m{n}{init_th(); std::cout << "construct" << m << std::endl;};
  ~C(void) { deinit_th(); std::cout << "deconstruct" << m << std::endl;};
  void poll(void) { std::cout << "C" << m << std::endl; while (1) std::this_thread::sleep_for(std::chrono::seconds(1));; };
  void print(){ std::cout << "I am " << m << std::endl;};
};

int main()
{
  //anonymous object  new className(parameter); desconstruct in this line.
  C(1).print();       

  C(2);
  //C c1(1);                                                                                                                                                                                      
  //C c2(2);
  while (1) std::this_thread::sleep_for(std::chrono::seconds(1));;
  return 0;
}

//关于临时对象的构造
// func(A()); 这是构造的一种方式