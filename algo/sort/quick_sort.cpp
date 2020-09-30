
#include <iostream>

/* 快速排序：
 * 选择基准，并以此遍历，计算该基准数据的数组索引，也就是序列位置
 * 一个数组转化为2个数组和一个partion值，并且两个数组中的值以partion分界
 */

/* 获取upper位置数字的序列位置，getIndex */
void partion(int a[], int low, int high)
{
	/*
	 *1.定义基准数据pivot
	 *2.遍历数组[j, high -1],当一直是小于pivot，swap实际上是空操作
	 *3.当有大于pivot的数出现，则i+1<j
	 */

	/* 基准数据 */
	int pivot = a[high];

	int i = low - 1;/* i记录的是比pivot大的值的位置索引，以便于交换位置 */

	for (int j = low ; j < high; j++)
	{
		if (a[j] <= pivot)
		{
			i++;
			std::swap(a[i], a[j]);
		}
	}

	std::swap(a[i+1], a[high]);
}

/* 分别从low和high两个方向去遍历 https://blog.csdn.net/nrsc272420199/article/details/82587933 */
void partionV2(int arr[], int low, int high)
{
	// 从low定义基准数据，则从high开始遍历，以便于赋值
	int tmp = arr[low];
	while (low < high) {
		// 当队尾的元素大于等于基准数据时,向前挪动high指针
		while (low < high && arr[high] >= tmp) {
			high--;
		}
		// 如果队尾元素小于tmp了,需要将其赋值给low；之后，arr[high]的值就可以被覆盖
		arr[low] = arr[high];
		
		// 当队首元素小于等于tmp时,向前挪动low指针
		while (low < high && arr[low] <= tmp) {
			low++;
		}
		// 当队首元素大于tmp时,需要将其赋值给high
		arr[high] = arr[low];

	}
	// 跳出循环时low和high相等,此时的low或high就是tmp的正确索引位置
	// 由原理部分可以很清楚的知道low位置的值并不是tmp,所以需要将tmp赋值给arr[low]
	arr[low] = tmp;
	return low; // 返回tmp的正确位置
}

void quickSort(int a[], int low, int high)
{
	if (low < high)
	{
		int index = partion(a, low, high);
		quickSort(a, low, index - 1);
		quickSort(a, index + 1, high);
	}
}

int main()
{
	int[] arr = { 49, 38, 65, 97, 23, 22, 76, 1, 5, 8, 2, 0, -1, 22 };
	quickSort(arr, 0, arr.length - 1);
}