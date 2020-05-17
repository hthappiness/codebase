/*
 *¹¤³Ì¹æ·¶
 *@In the beginning of every file ,write the description of the file content. 
 *@And the api 
 *@1. How to know the c++ code resource
 *@2. learn how to use
 *@shared_ptr,unique_ptr,weak_ptr,
 *@lamda
 *@
 */

#pragma once

/* base include */
#include "base_type.h"
#include "config.h"
#include "error_code.h"
#include "data_type.h"
#include "comm.h"

/* component include */
#include "producer.h"
#include "consumer.h"
#include "transformer.h"

namespace dataFlow{
	
	template<typename TCProducer, typename TCConsumer>
	boost::signals2::connection BindOutput(
		const std::shared_ptr<TCProducer>& spProducer,
		const std::shared_ptr<TCConsumer>& spConsumer)
	{
		assert(spProducer->outputType() == spConsumer->inputType());
		auto pConsumer = spConsumer.get();

		//auto callback = []

		return spProducer->addOutput(typename TCProducer::SlotType(
			[pConsumer](const typename TCProducer::OutputType& output) {
			pConsumer->consumeInner(output);
		}).track_foreign(spConsumer));
	}
	
	//process plugin libdataflow 
	
	/* singleton */
	//DF_API dataFlow make_dataFlow_context();
	
	DF_API LONG Initialize(int argc, char** argv);
	DF_API LONG Uninitialize(int argc, char** argv);
	
	//for plugin side call.
	DF_API VOID RegisterProducerFactory(const std::shared_ptr<CProducerFactory>& spProducer);
	DF_API VOID RegisterConsumerFactory(const std::shared_ptr<CConsumerFactory>& spConsumer);
	DF_API VOID RegisterTransformerFactory(const std::shared_ptr<CTransformerFactory>& spTransformer);
	
	//for process side call.
	DF_API std::shared_ptr<CProducerFactory> FindProducerFactoryByName(boost::string_view FactoryName);
	DF_API std::shared_ptr<CConsumerFactory> FindConsumerFactoryByName(boost::string_view FactoryName);
	DF_API std::shared_ptr<CTransformerFactory> FindTransformerFactoryByName(boost::string_view FactoryName);
	
	//for process side call.
	DF_API bool AddPluginDir(boost::string_view pluginDir);	
	DF_API DFErrorCode LoadPlugin(boost::string_view pluginPath); //when loading plugin, you need to register the factory
	DF_API DFErrorCode RemovePlugin(boost::string_view pluginPath);
	
	DF_API LONG LoadALLPlugins();
	DF_API VOID RemoveAllPlugins();
	
	DF_API VOID AddExitFun();
	
}