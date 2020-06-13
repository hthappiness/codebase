

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

#if 0
#include <iostream>

using namespace std;

class A
{

};

class B
{

};

class C{

};


class D : public A{

};

int main()
{
    cout << "group{A, B, A} = " << is_one_of<A, B, A>::value << endl;  //1
    cout << "group{A, B, D} = " << is_one_of<A, B, D>::value << endl;  // 0
}
#endif