

// is_one_of returns true it a class, Member, is present in a variadic list of
// classes, List.
template <class Member, class... List>
class is_one_of;

template <class Member, class... List>
class is_one_of<Member, Member, List...> {
 public:
  static constexpr const bool value = true;
};

template <class Member, class A, class... List>
class is_one_of<Member, A, List...> {
 public:
  static constexpr const bool value = is_one_of<Member, List...>::value;
};

template <class Member>
class is_one_of<Member> {
 public:
  static constexpr const bool value = false;
};

#include <iostream>

using namespace std;

class A
{ 
public:
  A()
  {
    std::cout << "A is constructed." << std::endl;
  }
  A(int a)
  {
    (void)a;
    std::cout << "A is constructed" << std::endl;
  }
  ~A()
  {
    std::cout << "A is destructed." << std::endl;
  }
};

class B
{ 
public:
  B()
  {
    std::cout << "B is constructed." << std::endl;
  }
  ~B()
  {
    std::cout << "B is destructed." << std::endl;
  }
};

class C{
  public:
  C()
  {
    std::cout << "C is constructed" << std::endl;
  }
  C(int a)
  {
    (void)a;
    std::cout << "C is constructed" << std::endl;
  }
   ~C()
  {
    std::cout << "C is destructed." << std::endl;
  }
};

//关注构造顺序
class D : public A{
 public:
  D():A(1)
  {
    std::cout << "D is constructed" << std::endl;
  }
  ~D()
  {
    std::cout << "D is destructed." << std::endl;
  }

  B b;
  C c;
};

/* 
A is constructed
B is constructed.
C is constructed
D is constructed 
*/

#if 0
class CInterceptor
{
public:
    int addMethod();

public:

private:
  std::vector<>;
};
#endif

template<int N>
class CallNoOp
{
public:
  void perform()
  {
    std::cout << "operation Num:" << N << std::endl;
  }
};

/* class CCallopInterface
{
protected:
  void perform();   
};

class Call
{
public:
  void performOps(CCallopInterface* callopSet)
  {
    callopSet->fillop(this);
  }
}; */

//偏特化Partial specialization
template <class Op1 = CallNoOp<1>, class Op2 = CallNoOp<2>,
          class Op3 = CallNoOp<3>, class Op4 = CallNoOp<4>,
          class Op5 = CallNoOp<5>, class Op6 = CallNoOp<6>>
class CallOpSet;

template<int N>
class CallOpMsgId
{
public:
  void perform()
  {
    std::cout << "default certain operation Num :" << N << std::endl;
  }
};

class CallOpInquire
{
public:
  int inquireAvailabel();
protected:
  void perform(); 
};

class CallOpStartTask
{
public:
  int startTask();
protected:
    void perform();
};

class CallOpCommit
{
public:
  int startCommit();
protected:
    void perform();
};

class Call
{

};

template <class Op1, class Op2, class Op3, class Op4, class Op5, class Op6>
class CallOpSet : public Op1,
                  public Op2,
                  public Op3,
                  public Op4,
                  public Op5,
                  public Op6 {
  public:
    void test()
    {
        this->Op1::perform();
        this->Op2::perform();
        this->Op3::perform();
        this->Op4::perform();
        this->Op5::perform();
        this->Op6::perform();
    }

    void fillop(Call* pstCall)
    {

    }

  private:
    //std::shared_ptr<CInterceptor> m_spInterceptor; //关于截获的逻辑
};

int main()
{
    cout << "group{A, B, A} = " << boolalpha << is_one_of<A, B, A>::value <<  endl;
    cout << "group{A, B, D} = " << boolalpha << is_one_of<A, B, D>::value << endl;

    CallOpSet<CallOpMsgId<1>, CallOpMsgId<2>> testCallOp;
    testCallOp.test();

    //D d;

    A* a = new D();
    delete a;
}

/*模板偏特化 
https://blog.csdn.net/k346k346/article/details/82179205
https://www.cnblogs.com/yyehl/p/7253254.html
*/

/*
[root@iaemb01 base]# ./traits
group{A, B, A} = true
group{A, B, D} = false
certain operation Num :1
certain operation Num :2
operation Num:3
operation Num:4
operation Num:5
operation Num:6
[root@iaemb01 base]#
*/

typedef void (*func)(void* args, int errCode);

class Closure
{

};

class CExecCtx
{
  static void run()
  {

  }
};