#include <iostream>
#include <typeinfo>
using namespace std;


template <class T>
void print(T t){
   cout << t << endl;
}

template <class ...Args>
void expands(Args... args){
   int arr[] = {(print(args), 0)...};
   cout<<arr[0]<<arr[1]<<arr[2]<<arr[3];
}

//iexpand(1,2,3,4);
//用到数组的初始化列表，这个数组的目的纯粹是为了在数组构造的过程展开参数包。
////{(printarg(args), 0)...}将会展开成((printarg(arg1),0), (printarg(arg2),0), (printarg(arg3),0),  etc... )，
////最终会创建一个元素值都为0的数组int arr[sizeof...(Args)]。printarg便会处理参数包中每一个参数。
//
//
//支持lambda表达式
template<class F, class... Args>void expand(const F& f, Args&&...args) {//这里用到了完美转发
  initializer_list<int>{(f(std::forward< Args>(args)),0)...};
  }
 //expand([](int i){cout<<i<<endl;}, 1,2,3);


//整型序列的定义
template<int...>
struct IndexSeq {};
//
////继承方式，开始展开参数包
template<int N, int... Indexes>
struct MakeIndexes : MakeIndexes<N - 1, N - 1, Indexes...> {};

// 模板特化，终止展开参数包的条件
template<int... Indexes>
struct MakeIndexes<0, Indexes...> {
	typedef IndexSeq<Indexes...> type;
	};


	//MakeIndexes<3> b;
	//MakeIndexes<6>::type c;
	//cout << typeid(b).name() << endl;//struct IndexSeq<0,1,2>
	//cout << typeid(c).name() << endl;//struct IndexSeq<0, 1, 2, 3, 4, 5>



int main()
{
	cout<<"lamda"<<endl;
	expand([](int i){cout<<i<<endl;}, 1,2,3);
	cout<<"---------------"<<endl;
	expands(1,2,3,4,5);
	
	cout<<endl<<"make index"<<endl;
	MakeIndexes<3> b;
	MakeIndexes<4> d;
        MakeIndexes<6>::type c;
        cout << typeid(b).name() << endl;//struct IndexSeq<0,1,2>
	cout << typeid(d).name() << endl;
        cout << typeid(c).name() << endl;//struct IndexSeq<0, 1, 2, 3, 4, 5>

	return 0;
}
