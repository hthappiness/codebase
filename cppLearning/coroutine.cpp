// cppNewDemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//#include "coroutine.h"
//
//sync<int> answer() {
//	std::cout << "Thinking deep thoughts..." << std::endl;
//	co_return 42;
//}
#include <iostream>
#include <iomanip>
#include <vector>

#include <memory>

#include <coroutine>

//https://owent.net/2019/1904.html
//https://www.cnblogs.com/wpcockroach/p/7403460.html
//https://blog.csdn.net/github_18974657/article/details/108526591

struct wait_some_times {
	int left_times;
	std::coroutine_handle<> handle;
	wait_some_times(int t) : left_times(t), handle(nullptr) {}
};

//此处提供co_await operator的operand，操作数需要满足如下定义，包含几个函数，也就是constraint
struct suspend_some_times {
	wait_some_times& d;
	suspend_some_times(wait_some_times& _d) : d(_d) {}

	/* 判断是否需要wait，切出该协程；提供用户的控制接口 */
	bool await_ready() noexcept {
		std::cout << " co control : call await_ready: " << d.left_times << std::endl;
		return d.left_times <= 0;
	}

	/* suspend挂起函数 */
	void await_suspend(std::coroutine_handle <> h) noexcept {
		// 记下来handle以便后面resume用,;suspend的才需要resume，所以handle在suspend里赋值，在resume里清空
		d.handle = h;

		std::cout << " co control : call await_suspend: " << (--d.left_times) << std::endl;
	}

	/* resume之前会调用该函数 */
	void await_resume() noexcept {
		std::cout << " co control : call await_resume: " << d.left_times << std::endl;
		d.handle = nullptr;
	}
};

struct coroutine_task {
	struct promise_type {
		coroutine_task get_return_object() {
			return coroutine_task{};
		}
		auto initial_suspend() {
			return std::suspend_never{};
		}
		auto final_suspend() {
			return std::suspend_never{};
		}

		void unhandled_exception() {}
		void return_void() {}
	};
};

//重载该运算符，wait_some_times不满足要求co_await对operand的要求
auto operator co_await(wait_some_times& x) noexcept {
	return suspend_some_times{ x };
}

/* 这是协程函数，表示要执行的操作，下面展示的是如何写一个协程函数 */
coroutine_task f(wait_some_times& waiter) {
	std::cout << "begin to co_await operation " << std::endl;

	std::cout << "***begin to co_await 1" << std::endl;
	co_await waiter; // 只有前三次会协程切出,因为前三次await_ready返回false
	std::cout << "***begin to co_await 2" << std::endl;
	co_await waiter;
	std::cout << "***begin to co_await 3" << std::endl;
	co_await waiter;
	std::cout << "***begin to co_await 4" << std::endl;
	co_await waiter; // 这之后await_ready返回true了，不会再切出
	std::cout << "***begin to co_await 5" << std::endl;
	co_await waiter;
	std::cout << "end of coroutine" << std::endl;
}

//表示这些函数的运行流程
//begin to co_await operation
//* **begin to co_await 1
//co control : call await_ready : 3
//co control : call await_suspend : 2
//thread control pass here.
//about to resume : 2
//co control : call await_resume : 2
//* **begin to co_await 2
//co control : call await_ready : 2
//co control : call await_suspend : 1
//about to resume : 1
//co control : call await_resume : 1
//* **begin to co_await 3
//co control : call await_ready : 1
//co control : call await_suspend : 0
//about to resume : 0
//co control : call await_resume : 0
//* **begin to co_await 4
//co control : call await_ready : 0
//co control : call await_resume : 0
//* **begin to co_await 5
//co control : call await_ready : 0
//co control : call await_resume : 0
//end of coroutine

int main(int argc, char* argv[]) {
#ifdef __cpp_coroutines
	std::cout << "__cpp_coroutines: " << __cpp_coroutines << std::endl;
#endif
	wait_some_times waiter{ 3 };
	f(waiter);

	std::cout << "thread control pass here." << std::endl;

	while (waiter.handle && !waiter.handle.done()) {
		std::cout << "about to resume: " << waiter.left_times << std::endl;
		// 这里用传出的handle来恢复切入协程；该线程resume，恢复协程函数的运行
		waiter.handle.resume();
	}

	return 0;
}
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
