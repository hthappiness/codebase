
#include <iostream>

/* 插入排序，思想就是遍历原数组，然后之前的都是一个排序好的数组 */

int insertSort(int arr[])
{
	for (int i = 1; i < sizeof(arr)/sizeof(int); i++)
	{
		int key = arr[i];
		
		int j = i - 1;
		while ( j >= 0 && arr[j] > key)
		{
			arr[j + 1] = a[j];/* 降低复杂度，不需要更换位置 */
			j--;
		}
		arr[i + 1] = key;
	}
}