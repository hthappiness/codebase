//
//计程车计费，逢4跳过计费
//
//输入是版本费用，输出打表器费用

#include <iostream>
#include <math.h>

int test()
{
	int sum = 0;

	unsigned int cnt;
	std::cout << "Please input the counter :";
	std::cin >> cnt;

	/* 计算该数有多少位 */
	int num = (int)log10(cnt) + 1;
	//std::cout << "total bit num = " << num << std::endl;
	
	unsigned int pos   = 0;
	unsigned int extra = 0;
	unsigned int lastPosVar = 0;
	unsigned int tmpCnt = cnt;

	while (num > 0)
	{
		//std::cout << "------process the round = " << num << std::endl;

		unsigned int powNum = pow(10, num - 1);
		unsigned int pos = tmpCnt / pow(10, num - 1);

		//std::cout << "process the num = " << pos << std::endl;

		extra += ((lastPosVar + (pos >= 4 ? 1 : 0)) * powNum);
		//std::cout << "last lastPosVar = " << lastPosVar << std::endl;
		//std::cout << "extra = " << extra << std::endl;

		lastPosVar = pos;
		num--;
		tmpCnt -= (pos * powNum);
	};

	std::cout << "extra = " << extra << std::endl;
	std::cout << "total = " << (extra + cnt) << std::endl;
}

int g_num;

int taxi(unsigned int n)
{
	if (n < 4)
	{
		return n;
	}

	unsigned int m = taxi(n-1) + 1;
	unsigned int tmp = m;
	int bitNum = (int)log10(m) + 1;

	int tmpBitNum = bitNum;
	while (tmpBitNum > 0)
	{
		if (tmp % 10 == 4)
		{
			m += pow(10, bitNum - tmpBitNum);
			break;
		}

		tmp /= 10;
		if (tmp == 0)
		{
			break;
		}

		tmpBitNum--;
	}
	return m;
}

/* 先要判断入参的有效性 */
int taxiReverse(unsigned int n)
{
	if (n < 4)
	{
		return n;
	}

	unsigned int last = n - 1;
	unsigned int cnt = 0;

	/* 修正每一个位数 */
	unsigned int tmp = last;
	int bitNum = (int)log10(tmp) + 1;

	int tmpBitNum = bitNum;
	while (tmpBitNum > 0)
	{
		if (tmp % 10 == 4)
		{
			//std::cout << "--------to fix value:-----" << last << std::endl;
			last -= pow(10, bitNum - tmpBitNum);
			//std::cout << "--------to fix value end:-----" << last << std::endl;
			break;
		}

		tmp /= 10;

		tmpBitNum--;
	}

	return taxiReverse(last) + 1;
}

int main()
{
	unsigned int result = 0;
	char more{ 'Y' };
	while (std::toupper(more) == 'Y')
	{
		unsigned int cnt;
		std::cout << "Please input the counter :";
		std::cin >> cnt;

		//std::cout << "result = " << taxi(cnt) << std::endl;
		std::cout << " result = " << taxiReverse(cnt) << std::endl;

		std::cout << "Do you want to enter another(Y or N)? ";
		std::cin >> more;
	}
}

