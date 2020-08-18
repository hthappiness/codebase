#include<iostream>
using namespace std;

int main()
{
	int a=0, b=0, c=0;
 
	__asm{
        movl %eax, 100
		movl a, %eax
    }
	cout << "a := " << a << endl;
	b = 99;
	c = 11;
	__asm{
        movl %ebx, b
		cmpxchg c, %ebx
		movl a, %eax  
    }
	cout << "b := " << b << endl;
	cout << "c := " << c << endl;
	cout << "a := " << a << endl;
	return 0;
}