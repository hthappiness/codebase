
#include <chrono>

template <typename function>
int function_cost(function f)
{
	auto beginTime = std::chrono::steady_clock::now();
	f();
	auto costTime = std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::steady_clock::now() - beginTime);
	
	LOG(INFO) << << costTime.count() << "] ms";
}

/*
function_cost(std::bind( [](){
	
}));

function_cost(std::bind(class::func, this, std::placeholder_1, ));
function_cost(std::bind(class::func, shared_from_this(), std::placeholder_1, ));
function_cost(std::bind(func, std::placeholder_1, ));

*/