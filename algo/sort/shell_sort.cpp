#include <iostream>

void swapItem(int a[], int i, int j)
{
    int tmp;
    tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

int shellSort(int a[], int arraySize)
{
    //先分组，组内直接插入排序
    int iStep = arraySize / 2;

    // change step
    while(iStep)
    {
        std::cout << "-------start group, step size-------" << iStep <<"\r\n";
        
        // sort every group
        for( int i = 0 ; i < iStep ; i++)
        {
            // sort certain group
            std::cout << "-------start group -------" << i <<"\r\n";
            for( int j = i; j < arraySize; j += iStep)
            {
                while( j - iStep >= 0 && a[ j ] < a[ j - iStep ])
                {
                    //std::cout << "-------start compare -------" << j <<"\r\n";
                    swapItem(a, j, j - iStep);
                    std::cout << "swap index:" << a[j] << " and " << a[j - iStep]<<"\r\n";
                    j -= iStep;
                }
            }
        }
        iStep /= 2;
    }
}

int main()
{
    auto prinfArray = [](int a[], int len)
    {
        for(int i = 0 ; i < len ; i++)
        {
            std::cout << a[i] ;
            std::cout << " " ;
        }
        std::cout << "\r\n";
    };
    int a[] = { 5, 34, 8, 7, -1, 6, 85, 22} ;

    std::cout << "before sort:" << std::endl;
    prinfArray(a, sizeof(a)/sizeof(int));
    
    shellSort(a, sizeof(a)/sizeof(int));

    std::cout << "after sort:" << std::endl;
    prinfArray(a, sizeof(a)/sizeof(int));

}