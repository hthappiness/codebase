#pragma once
#include "consumer.h"
#include "producer.h"

namespace dataFlow {

	class CBaseTransformer :public CAbstractAnyDataProducer, public CAbstractAnyDataConsumer
	{

	};

	using CAbstractAnyDataTransformer = CBaseTransformer;

	template<typename Impl, typename InputType, typename OutputType>
	class CTransformer :public CAbstractAnyDataTransformer
	{
	protected:

		virtual void produce(OutputType const& output)
		{
			return produce1(output);
		}
	public:
		CTransformer()
		{

		}
		~CTransformer()
		{

		}

		virtual std::string inputType() const override
		{
			return typeid(InputType).name();
		}

		virtual std::string ouputType() const override
		{
			return typeid(OutputType).name();
		}
	};

}