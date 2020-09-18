#include <iostream>
#include <vector>

template<typename Iter, typename Callable>
void foreach(Iter current, Iter end, Callable op)
{
	while (current != end)
	{
		op(*current);
		++current;
	}
}

void func(int i)
{
	std::cout << i << '\n';
}

struct FuncObj {
	void operator()(int i) const
	{
		std::cout << i << '\n';
	}
};

int main()
{
	std::vector<int> primes = { 2, 3, 5, 7, 11, 13, 17, 19 };
	foreach(primes.begin(), primes.end(), func);
	foreach(primes.begin(), primes.end(), &func);
	foreach(primes.begin(), primes.end(), FuncObj());
	foreach(primes.begin(), primes.end(), [](int i) { std::cout << i << '\n'; });
}