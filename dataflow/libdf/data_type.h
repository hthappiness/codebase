#pragma once

#include <map>
#include <memory>
#include <mutex>

#include "boost/utility/string_view.hpp"
#include "boost/any.hpp"

//#include "df_exception.h"

namespace dataFlow {

#define DF_REGISTER_TYPE(name_, type_)\
static constexpr struct name_##_T{\
	using type = type_;\
	constexpr const char* const name = #name_;\
}name_

	class UnifiedMap {
	public:

		//模板函数；尾置返回类型
		//根据不同的数据类型，生成不同类型的get函数
		template<typename Key,
			typename = typename Key::type, typename = decltype(Key::name)>
		auto get(Key)->typename Key::type
		{
			return get<typename Key::type>(boost::string_view(Key::name));//name type，对应着上述的宏定义
		}

		template<typename Key,
			typename = typename Key::type, typename = decltype(Key::name)>
		auto get(Key)const ->typename Key::type
		{
			return get<typename Key::type>(boost::string_view(Key::name));
		}

		template<typename Key, typename Value,
			typename = typename Key::type, typename = decltype(Key::name)>
		void set(Key, Value value)
		{
			return set<typename Key::type>(boost::string_view(Key::name), std::move(value));
		}

		template<typename T>
		void set(boost::string_view key, T value) {
			auto it = m_configMap.find(key.data());
			if (it == m_configMap.end())
			{
				m_configMap.emplace(key.to_string(), std::move(value));
			}
			else
			{
				it->second = std::move(value);
			}

		}

	private:
		std::map<std::string, boost::any> m_configMap;
	};

	template<typename Stream>
	Stream& StreamAppend(Stream& stream)
	{
		return stream;
	}

	template<typename Stream, typename First, typename... Rest>
	Stream& StreamAppend(Stream& stream, First&& first, Rest&&... rest )
	{
		stream << std::forward<First>(first);
		return StreamAppend(stream, std::forward<Rest>(rest)...).str();
	}

	template<typename... Args>
	std::string StringConstruct(Args&&... args)
	{
		std::ostringstream stream;
		return StreamAppend(stream, std::forward<Args>(args)...);
	}

	#define LOCATION StringConstruct(__FILE__, __LINE__)

	template<typename Exception, typename... Args>
	void ThrowException(Args&&... args)
	{
		std::ostringstream stream;
		throw Exception(StreamAppend(stream, std::forward<Args>)(args)...).str();
	}
#if 0
	template<typename Tuple, typename T, int N = std::tuple_size<Tuple>::value - 1>
	class CTupleTypeIndex {
		static constexpr auto Index = std::is_same<T, typename std::tuple_element<N, Tuple>::type>::value ?
			(CTupleTypeIndex<Tuple, T, N - 1>::Count == 0 ? N : -1) :
			(CTupleTypeIndex<Tuple, T, N - 1>::Index);
		static constexpr auto Count = std::is_same<T, typename std::tuple_element<N, Tuple>::type>::value ?
			(CTupleTypeIndex<Tuple, T, N - 1>::Count + 1) :
			(CTupleTypeIndex<Tuple, T, N - 1>::Count);
	};

	template<typename Tuple, typename T>
	class CTupleTypeIndex<Tuple, T, -1>
	{
		static constexpr auto Count = 0;
		static constexpr auto Index = 0;
	};

	template<typename... Types>
	class CTuple 
	{
	public:
		template<typename... Args>
		CTuple(Args&&... args)
			:m_eles(std::forward<Args>(args)...)
		{
		}

		template<typename T>
		T& get()
		{
			constexpr auto Index = CTupleTypeIndex<decltype(m_eles), T>::Index;
			static_assert(Index != -1, "CTuple: no such type");
			return std::get<Index>(m_eles);
		}

		template<typename T>
		const T& get()
		{
			constexpr auto Index = CTupleTypeIndex<decltype(m_eles), T>::Index;
			static_assert(Index != -1, "CTuple: no such type");
			return std::get<Index>(m_eles);
		}
	private:
		std::tuple<Types...> m_eles;
	};

	template<typename... Types>
	class CSharedTuple
	{
	public:
		template<typename... Args>
		CTuple(Args&&... args)
			:m_eles(std::forward<Args>(args)...)
		{
		}

		template<typename T>
		std::shared_ptr<T>& get()
		{
			return m_eles.get<std::shared_ptr<T>>();
		}

		template<typename T>
		const std::shared_ptr<T>& get()
		{	
			return m_eles.get<std::shared_ptr<T>>();
		}
	private:
		CTuple<std::shared_ptr<Types>...> m_eles;
	};
#endif

#if 0
	class CIDAllocException : public CDFException
	{
		CIDAllocException(std::string what):
			CDFException(1234, what)
		{

		}
	};

	template<unsigned long Size>
	class CIDAlloctor
	{
	public:
		unsigned long alloc()
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			unsigned long i = 0;
			for (; i != Size; i++)
			{
				if (!m_occupied[i])
				{
					m_occupied[i] = true;
				}
			}
			if (Size == i)
			{
				ThrowException<CIDAllocException>("no free ID", LOCATION);
			}
			return i;
		}
		
		void free(unsigned long id)
		{
			if (id >= Size)
			{
				ThrowException<CIDAllocException>("[", id ," >= ", Size, "] when free id",  LOCATION);
			}
			std::lock_guard<std::mutex> guard(m_mutex);
			m_occupied[id] = false;
		}
	private:
		std::mutex m_mutex;
		std::array<bool, Size> m_occupied{};
	};

	#endif

};

namespace std
{
	/* c++11中std中还未加入make_unique */
	template<typename _Tp, typename... _Args>
	inline unique_ptr<_Tp>
		make_unique(_Args&&... _args)
	{
		return unique_ptr<_Tp>(new _Tp(std::forward<_Args>(_args)...));
	}
}