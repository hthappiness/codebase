
#include <iostream>
#include <array>
#include <vector>
#include <utility>

#include <algorithm>

#include <map>
#include <unordered_map>
#include <tuple>
#include <string>
#include <functional>

#include <complex>

using namespace std;

//http://www.mamicode.com/info-detail-2536209.html

// piecewise_construct, 占位符，匹配构造函数
pair<string, complex<double>> scp(piecewise_construct, make_tuple("hello"), make_tuple(1, 2));

map<string, complex<double>> testMap;

//testMap.emplace(piecewise_construct,forward_as_tuple("hello"),forward_as_tuple(1, 2));

typedef struct tagTestCopy
{
    int a;
    int b;
    int c;
    int d;
}TEST_COPY_S;

int main()
{
    int a[4] = {1, 2, 3, 4};
    //int b[4];
    //std::array<int , 4> a = {1, 2, 3, 4};
    std::vector<int> b(4);
    std::copy(a, a+4, b.begin());

    for(int& tmp : b)
    {
        std::cout << tmp << std::endl;
    }

    TEST_COPY_S test = {2, 5, 9, 10};
    TEST_COPY_S dest ;
    std::copy(&test, &test + 1, &dest);
    std::cout << dest.d <<std::endl; // 10
}