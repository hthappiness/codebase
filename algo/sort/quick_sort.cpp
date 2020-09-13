
#include <iostream>

/* ��������
 * ѡ���׼�����Դ˱���������û�׼���ݵ�����������Ҳ��������λ��
 * һ������ת��Ϊ2�������һ��partionֵ���������������е�ֵ��partion�ֽ�
 */

/* ��ȡupperλ�����ֵ�����λ�ã�getIndex */
void partion(int a[], int low, int high)
{
	/*
	 *1.�����׼����pivot
	 *2.��������[j, high -1],��һֱ��С��pivot��swapʵ�����ǿղ���
	 *3.���д���pivot�������֣���i+1<j
	 */

	/* ��׼���� */
	int pivot = a[high];

	int i = low - 1;/* i��¼���Ǳ�pivot���ֵ��λ���������Ա��ڽ���λ�� */

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

/* �ֱ��low��high��������ȥ���� https://blog.csdn.net/nrsc272420199/article/details/82587933 */
void partionV2(int arr[], int low, int high)
{
	// ��low�����׼���ݣ����high��ʼ�������Ա��ڸ�ֵ
	int tmp = arr[low];
	while (low < high) {
		// ����β��Ԫ�ش��ڵ��ڻ�׼����ʱ,��ǰŲ��highָ��
		while (low < high && arr[high] >= tmp) {
			high--;
		}
		// �����βԪ��С��tmp��,��Ҫ���丳ֵ��low��֮��arr[high]��ֵ�Ϳ��Ա�����
		arr[low] = arr[high];
		
		// ������Ԫ��С�ڵ���tmpʱ,��ǰŲ��lowָ��
		while (low < high && arr[low] <= tmp) {
			low++;
		}
		// ������Ԫ�ش���tmpʱ,��Ҫ���丳ֵ��high
		arr[high] = arr[low];

	}
	// ����ѭ��ʱlow��high���,��ʱ��low��high����tmp����ȷ����λ��
	// ��ԭ���ֿ��Ժ������֪��lowλ�õ�ֵ������tmp,������Ҫ��tmp��ֵ��arr[low]
	arr[low] = tmp;
	return low; // ����tmp����ȷλ��
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