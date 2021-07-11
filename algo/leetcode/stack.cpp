//面试题：定义栈的数据结构，要求添加一个min函数，能够得到栈的最小元素，要求函数min，push及pop的时间复杂度都是O(1)

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <stack>
using namespace std;
class sTack
{
private:
	stack<int>datestack;
	stack<int>minstack;
public:
	void push(int k)
	{
		datestack.push(k);
		if(!minstack.size())
			minstack.push(k);
		else if(k<=minstack.top())
			minstack.push(k);
	}
	void pop()
	{
		assert(datestack.size()>0);
		assert(minstack.size()>0);
		if(datestack.top()==minstack.top())
			minstack.pop();
		datestack.pop();
	}
	int min()
	{
		return minstack.top();
	}
	int top()
	{
		return datestack.top();
	}
};
int main()
{
	sTack T;
	T.push(2);
	T.push(6);
	T.push(3);
	T.push(1);
	T.push(3);
	T.push(5);
	T.push(4);
	T.push(2);
	for(int i=0;i<8;i++)
	{
		cout<<T.min()<<endl;
		T.pop();
	}
	return 0;
}