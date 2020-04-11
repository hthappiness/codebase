
#include <iostream>

int main()
{
	const int& a = 1;
	int& b = const_cast<int&>(a);
	std::cout<< &a << "," << &b << std::endl;
	b = 2;
	std::cout<< *&a << "," << a << std::endl;
}