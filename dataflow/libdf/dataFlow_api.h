#pragma once

#include <memory>
#include "boost/signals2/connection.hpp"

#include "consumer.h"
#include "producer.h"

namespace dataFlow {
	template <typename ProducerType, typename ConsumerType>
	boost::signals2::connection BindOutPut(const std::shared_ptr<ProducerType>& spProducer,
		const std::shared_ptr<ConsumerType>& spConsumer)
	{
		assert(spProducer->outputType() == spConsumer->inputType());
		auto pConsumer = spConsumer.get();

		//auto callback = []

		// typename ProducerType::SlotType 这是调用slot的构造函数
		return spProducer->addOutput(typename ProducerType::SlotType(
			[pConsumer](const typename ProducerType::OutputType& output) {
			pConsumer->consumer1(output);
		}).track_foreign(spConsumer));
	}
}

//lamda表达式，捕获列表的值传递
//匿名函数，注册函数回调

#if 0
typedef boost::signals2::signal<void()> sig_type;
sig_type sig;

std::shared_ptr<object_with_slot> ptr(new object_with_slot);
sig.connect(sig_type::slot_type(*ptr).track_foreign(ptr)); // ptr is tracked

sig();
#endif