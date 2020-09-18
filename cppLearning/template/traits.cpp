
#include <type_traits>

template<typename T1, typename T2>
typename std::common_type<T1, T2>::type max(T1 a, T2 b)
{
    return b < a ? a : b;
}

template<typename T>
struct Accumulationtraits;

template<>
struct Accumulationtraits<char> {
	/* 类型和变量 */
	using AccT = int;
	static const AccT zero = 0;
};

template<typename T>
auto accum(const T* beg, const T* end)
{
	using AccT = typename Accumulationtraits<T>::AccT;
	AccT total = Accumulationtraits<T>::zero;
	while (beg != end)
	{
		total += *beg;
		++beg;
	}
	return total;
}