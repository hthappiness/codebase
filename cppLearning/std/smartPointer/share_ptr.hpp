// enable_shared_from_this的实现
// 基于(/usr/include/c++/7.3.0/bits/shared_ptr.h)
// 此代码是对gcc实现的简化版本, 仅作为描述原理用.    
// https://www.cnblogs.com/yang-wen/p/8573269.html

template<typename T>
class enable_shared_from_this
{
public:
    shared_ptr<T> shared_from_this()
    {
        return shared_ptr<T>(this->weak_this);
    }
    shared_ptr<const T> shared_from_this() const
    {
        return shared_ptr<const T>(this->weak_this);
    }
private:
    template<typename>
    friend class shared_ptr;

    template<typename T1>
    void _M_weak_assign(T1* p, const shared_count<>& n)
    {
    　　weak_this._M_assign(p, n);
    }

    mutable weak_ptr<T> weak_this;
};

struct node
{
	node(int a1 = 1, int b1 = 2, int c1 =3, int d1 = 4)
	{
		a = a1;
		b = b1;
		c = c1;
		d = d1;
	}
	void printf()
	{
		
	}
	int a;
	int b;
	int c;
	int d;
};

int main()
{
	using namespace std;
	static int cnt = 8;
	
	//别名构造函数
	node* buffer = calloc(cnt, sizeof node);
	auto spBuffer = shared_ptr<node>(buffer);

	vector<shared_ptr<node>> nodeArr;
	nodeArr.resize(cnt);
	
	nodeArr[0].reset(&buffer[0], [](node* p ){free(p)});
	
	for(decltype(cnt) i =1 ; i < cnt ; i++)
	{
        //别名构造函数
		nodeArr[i] = shared_ptr<node>(nodeArr[0], &buffer[i]);
	}
	
}

#if 0
// shared_ptr constructor example
#include <iostream>
#include <memory>

struct C {int* data;};

int main () {
  std::shared_ptr<int> p1;
  std::shared_ptr<int> p2 (nullptr);
  std::shared_ptr<int> p3 (new int);
  std::shared_ptr<int> p4 (new int, std::default_delete<int>());
  std::shared_ptr<int> p5 (new int, [](int* p){delete p;}, std::allocator<int>());
  std::shared_ptr<int> p6 (p5);
  std::shared_ptr<int> p7 (std::move(p6));
  std::shared_ptr<int> p8 (std::unique_ptr<int>(new int));
  std::shared_ptr<C> obj (new C);
  std::shared_ptr<int> p9 (obj, obj->data);

  std::cout << "use_count:\n";
  std::cout << "p1: " << p1.use_count() << '\n';
  std::cout << "p2: " << p2.use_count() << '\n';
  std::cout << "p3: " << p3.use_count() << '\n';
  std::cout << "p4: " << p4.use_count() << '\n';
  std::cout << "p5: " << p5.use_count() << '\n';
  std::cout << "p6: " << p6.use_count() << '\n';
  std::cout << "p7: " << p7.use_count() << '\n';
  std::cout << "p8: " << p8.use_count() << '\n';
  std::cout << "p9: " << p9.use_count() << '\n';
  return 0;
}
 
Output:
use_count:
p1: 0
p2: 0
p3: 1
p4: 1
p5: 2
p6: 0
p7: 2
p8: 1
p9: 2
#endif
