
#include <iostream>

/* ��������˼����Ǳ���ԭ���飬Ȼ��֮ǰ�Ķ���һ������õ����� */

int insertSort(int arr[])
{
	for (int i = 1; i < sizeof(arr)/sizeof(int); i++)
	{
		int key = arr[i];
		
		int j = i - 1;
		while ( j >= 0 && arr[j] > key)
		{
			arr[j + 1] = a[j];/* ���͸��Ӷȣ�����Ҫ����λ�� */
			j--;
		}
		arr[i + 1] = key;
	}
}