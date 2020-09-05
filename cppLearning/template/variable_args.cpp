
#include <iostream>
#include <memory>

//整型序列的定义
template<int...>
struct IndexSeq {};

//继承方式，开始展开参数包;递归，不断的展开，不断推导，继承，寻找定义
template<int N, int... Indexes>
struct MakeIndexes : MakeIndexes<N - 1, N - 1, Indexes...> {};

// 模板特化，终止展开参数包的条件
template<int... Indexes>
struct MakeIndexes<0, Indexes...> {
	typedef IndexSeq<Indexes...> type;
};

/*展开过程
MakeIndexes<3> : MakeIndexes<2, 2>{}

MakeIndexes<2, 2> : MakeIndexes<1, 1, 2>{}

MakeIndexes<1, 1, 2> : MakeIndexes<0, 0, 1, 2>

MakeIndexes<0, 0, 1, 2>{
	typedef IndexSeq<0, 1, 2> type;
}
*/

class A
{
public:
    A(int i)
    {
        std::cout << " construct A, i = " << i << std::endl;
    }
};

class B
{
public:
    B(int i, int j)
    {
        std::cout << " construct B, j = " << j << std::endl;
    }
};

template<typename T, typename... Args>
std::shared_ptr<T> Instance(Args&&... args)
{
    std::cout << sizeof...(args) << std::endl; //打印变参的个数
    return std::make_shared<T>(std::forward<Args>(args)...);
}

int main()
{
    //variable_args.cpp: In instantiation of ‘T* Instance(Args&& ...) [with T = A; Args = {int}]’:
    auto pa = Instance<A>(1);

    //variable_args.cpp: In instantiation of ‘T* Instance(Args&& ...) [with T = B; Args = {int, int}]’:
    auto pb = Instance<B>(1,2);

    MakeIndexes<3> b;
    MakeIndexes<6>::type c;
    std::cout << typeid(b).name() << std::endl;//struct IndexSeq<0,1,2>
    std::cout << typeid(c).name() << std::endl;//struct IndexSeq<0, 1, 2, 3, 4, 5>

}

//https://blog.csdn.net/tony__lin/article/details/84677316
