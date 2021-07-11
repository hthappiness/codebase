

#include <iostream>

//一道题是输入一个数组，每行3个元素代表用户做出方案选择的代价，
//相邻用户的选择方案不能相同；输出为最小代价，如果有相同的，则输出最后一个

static constexpr auto CHOICE_NUM = 3;
static constexpr auto PERSON_NUM = 3;

#if 0
int choiceMap[PERSON_NUM][CHOICE_NUM] = {
	{4, 5, 8},
	{4, 15, 8},
	{4, 5, 18},
	{14, 5, 8},
	{4, 5, 8},
	{4, 25, 8},
	{4, 7, 8},
	{4, 15, 8},
	{4, 5, 28},
	{14, 5, 8}
};
//[root@node1 hting] # . / solution
//result : 5 8 4 5 8 4 7 8 4 5
#endif

int choiceMap2[PERSON_NUM][CHOICE_NUM] = {
	{15,8,17},
	{12, 20, 9},
	{11, 7, 5}
};

void copy(int result[], int path[], int row)
{
	for (int i = 0; i < row; i++)
	{
		result[i] = path[i];
	}
}

void output( int path[], int row)
{
	std::cout << "result :";
	for (int i = 0; i < row; i++)
	{
		std::cout << choiceMap2[i][path[i]] <<" ";
	}
	std::cout << std::endl;
}

int sum(int path[], int row)
{
	int result = 0;
	for (int i = 0; i < row; i++)
	{
		result += choiceMap2[i][path[i]] ;
	}
	return result;
}

//回溯算法，用递归的代码形式实现；指数级的组合
//分治思想，不同情况之间的递推关系
void setResult(int path[], int result[], int row, int& min)
{
	for (int j = 0; j < CHOICE_NUM; j++)
	{
		//这是相邻三个用户不能选择相同；（题干中是相邻两个不能相同）
		if (row >= 2 && (path[row - 1] == j || path[row - 2] == j))
		{
			continue;
		}
		if (row == 1 && (path[row - 1] == j))
		{
			continue;
		}

		path[row] = j;

		int tmpRow = row + 1;

		if (tmpRow == PERSON_NUM)
		{
			int ret = sum(path, tmpRow);
			if (ret <= min) /* 如果有相同的则选择最后一个 */
			{
				min = ret;
				copy(result, path, tmpRow);
			}
		}
		else
		{
			setResult(path, result, tmpRow, min);
		}
	}
}

int main()
{
	int path[PERSON_NUM];
	int result[PERSON_NUM];

	int min = 0x7FFFFFFF;

	setResult(path, result, 0, min);

	output(result, PERSON_NUM);
	
}