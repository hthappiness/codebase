#pragma once

#include <memory>
#include <string>

#include "producer.h"
#include "consumer.h"
#include "transformer.h"

namespace dataFlow {

	template <typename T>
	class CBaseFactory {
		virtual std::shared_ptr<T> create() = 0;	//工厂返回的必然是各种workNode类型的变量
		virtual std::string getFactoryName() = 0;
	};

	class CProducerFactory : public CBaseFactory<CAbstractAnyDataProducer> {

	};

	class CConsumerFactory : public CBaseFactory<CAbstractAnyDataConsumer> {

	};

	class CTransformerFactory : public CBaseFactory<CAbstractAnyDataTransformer> {

	};

	//using CProducerFactory = CBaseFactory<CAbstractAnyDataProducer>;

}