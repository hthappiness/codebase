#pragma once

#include "consumer.h"

namespace dataFlow {

	//带缓冲的队列，提供consumer方法，然后实例化的时候，绑定消费函数
	template< typename bufferType >
	class CBufferConsumer : public CAbstractAnyDataConsumer
	{
		//consume

		//run(std::function<void(std::shared_ptr<T>)>)
	};

}