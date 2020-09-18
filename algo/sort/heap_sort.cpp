

#include <iostream>
#include <array>

/* 
 * 不同性质的数据结构，对应不同的场景
 * 二叉堆
 * 最大堆：除了根节点之外，A[parent(i)] >= A[i]，子树都是最大堆；最大节点存在根节点中
 * 最小堆：除了根节点之外，A[parent(i)] <= A[i]，子树都是最小堆; 最小节点存在根节点中
 */
template<typename Key>
class Heap
{
public:
    using index_pos = int; //alias of the type
    int getHeapSize()
    {
        return m_heapSize;
    }
private:
 
    index_pos parent(index_pos i)
    {
        return (i - 1)/2;
    }

    index_pos left(index_pos i)
    {
        return 2*i + 1;
    }

    index_pos right(index_pos i)
    {
        return 2*i + 2;
    }

private:
    static constexpr auto MAX_SIZE = 100;
    std::array<Key, MAX_SIZE> m_items;

    int m_heapSize;
};

template<typename T>
class MaxHeap : public Heap<T>
{
public:
    /* 或以这种方式建堆 */
    void insertItem(T k)
    {
        int tmp = m_heapSize;
        while(int index = parent(tmp) >= 0 && m_item[index] < m_item[tmp] )
        {
            std::swap(m_item[index], m_item[tmp]);
            tmp = index;
        }
        m_heapSize++;
    }

    void buildMaxHeap(T& a[], std::size_t size)
    {
        m_heapSize = size;

        for(int i=0;i < size;i++)
        {
            m_items[i] = a[i];
        }

        for(int i = size/2; i > 0 ;i-- )
        {
            maxHeapify(i);
        }
    }

    /* 在建堆之后调用 */
    void sort()
    {
        while(m_heapSize > 0)
        {
            /* 输出最大值 */
            std::cout << m_item[0] << std::endl;

            /* 重新调整,把尾部数值换到头部，然后开始调整，没有破坏maxHeapify调用的前提条件 */
            std::swap(m_item[0], m_item[m_heapSize]);

            m_heapSize--;

            maxHeapify(0);

        }
    }
private:
    /* 调整位置,该操作需要保证其子树都是最大堆;分解这样的过程对于解决问题，描述过程，非常的有效 */
    /* 类似adjustHeap */
    void maxHeapify(index_pos i)
    {
        index_pos left = left(i);
        index_pos right = right(i);
        index_pos largest;

        if( left <= m_heapSize && m_item[left] > m_item[i])
        {
            largest = left;
        }
        else
        {
            largest = i;
        }

        if( right <= m_heapSize && m_item[right] > m_item[largest])
        {
            largest = right;
        }

        if( largest != i )
        {
            std::swap(m_item[i], m_item[largest]);
            maxHeapify(largest); /* 这里消除尾递归 */
        }
    }
};

template<typename T>
class MinHeap : public Heap<T>
{
private:
    void minHeapify(index_pos i)
    {
        index_pos left = left(i);
        index_pos right = right(i);
        index_pos smallest;

        if( left <= m_heapSize && m_item[left] < m_item[i])
        {
            smallest = left;
        }
        else
        {
            smallest = i;
        }

        if( right <= m_heapSize && m_item[right] < m_item[smallest])
        {
            smallest = right;
        }

        if( smallest != i )
        {
            std::swap(m_item[i], m_item[smallest]);
            minHeapify(largest); /* 这里消除尾递归 */
        }
    }
};