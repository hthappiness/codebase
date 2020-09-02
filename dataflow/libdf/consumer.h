#pragma once
#include "workNode.h"
#include "boost/any.hpp"

namespace dataFlow
{
	//输入的数据类型、输出的数据类型
	template <typename valueType>
	class CBaseConsumer :virtual public CWorkNode
	{
	public:
		//构造和析构函数
		CBaseConsumer() = default;
		~CBaseConsumer() = default;

		//拷贝构造
		CBaseConsumer(const CBaseConsumer&) = delete;

		//赋值运算
		// result : cannot allocate an object of abstract type ‘dataFlow::CBaseConsumer<boost::any>
		//CBaseConsumer operator=(CBaseConsumer&) = delete;

		//移动构造函数 std::move();在右值引用的情况下，调用对应的构造函数
		//c++11 丰富了对象模型
		//CBaseConsumer(CBaseConsumer&&) = delete;

		virtual std::string inputType() const = 0;
		virtual void consumeInner(valueType const&) = 0;
	};
	
	using CAbstractAnyDataConsumer = CBaseConsumer<boost::any>;
	//其实该类设计，可不用模板类，直接使用boost::any即可，毕竟该类模板只实例化了一个类别

	template <typename impl, typename Input>
	class CDataConsumer : public CAbstractAnyDataConsumer
	{
	public:
		//该函数由子类实现，但是produce直接调用此函数consume1
		//这里提供了父类调用子类函数的一个例子
		virtual void consumeInner(boost::any const& input) override final //signal之后，调用的都是这个函数，无法被重写，boost::any保证了其通用性
		{
			//这里的impl模板参数，就是为了让父类感知到子类，这也是种耦合，也许有更地道的用法来完成同样的功能
			//子类重写consume函数
			static_cast<impl*>(this)->consume(boost::any_cast<Input>(input));
		}
	public:
		virtual std::string inputType() const override
		{
			return typeid(Input).name();
		}
	};

}