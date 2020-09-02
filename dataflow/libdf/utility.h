#pragma once

#include <iostream>
#include <chrono>
#include <string>

namespace dataFlow{

template<typename F, typename RetParameter>
void CostTimeStatisticsWrapper(F f, std::string callbackTag) {

    std::chrono::steady_clock::time_point m_startTp = std::chrono::steady_clock::now();

    auto&& res = f();
    std::cout << res << std::endl;

    auto elapsedClk = std::chrono::stready_clock::now() - m_startTp;
    auto elapsedTm  = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedClk);
    std::cout<< callbackTag <<"elapsed Time:"<<elapsedTm<<std::endl;
}

/* 
 *通过模板去抽象不同的参数，通过bind去抽象不同的参数
 *返回值如何抽象？可以先做这样一个限制, 或者加上typename RetParameter，使用tuple
 *举例如下:
int f1(int n) {
    return 2 * n;
}

bool f2() {
    return 0.1;
}

struct Obj {
    int operator()(int a, int b) {
        return a + b;
    }
};

int main() {
    foo(std::bind(&f1, 10));  // 一元参数
    foo(std::bind(&f2));       // 无参数函数
    int x = 10;
    foo([&]() { return x; }); // lambda表达式
    Obj obj;
    foo(std::bind(obj, 1, 2)); // 函数对象
    return 0;
}
*/

}