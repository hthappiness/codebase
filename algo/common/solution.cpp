

#include <iostream>

//һ����������һ�����飬ÿ��3��Ԫ�ش����û���������ѡ��Ĵ��ۣ�
//�����û���ѡ�񷽰�������ͬ�����Ϊ��С���ۣ��������ͬ�ģ���������һ��

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

//�����㷨���õݹ�Ĵ�����ʽʵ�֣�ָ���������
//����˼�룬��ͬ���֮��ĵ��ƹ�ϵ
void setResult(int path[], int result[], int row, int& min)
{
	for (int j = 0; j < CHOICE_NUM; j++)
	{
		//�������������û�����ѡ����ͬ�������������������������ͬ��
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
			if (ret <= min) /* �������ͬ����ѡ�����һ�� */
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