#pragma once

#include "workNode.h"
#include <typeinfo>

#include <boost/any.hpp>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>

namespace dataFlow {

	template<typename ValueType>
	class CBaseProducer : virtual public CWorkNode
	{
	private:
		//在定义signal的时候，已经声明其slot类型，下面slotType是根据signalType决定的
		using SignalType = boost::signals2::signal<void(ValueType const&)>;
	public:

		using SlotType   = typename SignalType::slot_type;
		using OutputType = ValueType;

		CBaseProducer()  = default;
		~CBaseProducer() = default;

		virtual std::string outputType() const = 0;

		/* 生产者需要绑定对应的消费者 */
		boost::signals2::connection addOutput(const SlotType& slot) {
			return m_signal.connect(slot);
		}

	protected:
		void produce1(const ValueType& data) {
			m_signal(data);
		}

	private:
		SignalType m_signal;//重载了()操作符
	};

	using CAbstractAnyDataProducer = CBaseProducer<boost::any>;

	template< typename Output>
	class CDataProducer : public CAbstractAnyDataProducer
	{
	public:
		virtual std::string outputType() const override
		{
			return typeid(Output).name();
		}
	protected:

		void produce(Output const& output) {
			produce1(output);

			//CAbstractAnyDataProducer::produce(output);
		}
	};

}