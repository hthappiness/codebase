#pragma once

#include <thread>
#include <string>
#include <boost\lockfree\queue.hpp>

namespace dataFlow {
	
	template <typename input, size_t consumerLimit>
	class threadQueue
	{

		using Handler = std::function<void(const std::vector<input>&)>;

	public:
		threadQueue(size_t capacity)
			:m_queue(capacity)
		{
		
		}
		~threadQueue()
		{
		
		}

		threadQueue(const threadQueue&) = delete;
		threadQueue£¨threadQueue&£© = delete;
		threadQueue& operator=(const threadQueue&) = delete;
		threadQueue& operator=(threadQueue&) = delete;

		void run(const Handler& handler, const std::function<void()>& pre = nullptr)
		{
			//m_thread = new std::thread(&work, );
			m_pfnConsume = handler;
			m_pfnPreProcess = pre;
			m_thread = std::thread(&threadQueue::work, this);
		}

		int push(const input& value)
		{
			m_queue.push_front(value);
		}

		template< typename T>
		void work() {
		
		}

	private:
		void work()
		{
			if (m_pfnPreProcess)
			{
				m_pfnPreProcess();
			}

			while (true)
			{
				static_assert(consumerLimit > 0, "ConsumerLimit should larger than zero!");
				m_buffer.resize(consumerLimit);
				auto status = m_queue.wait_full_front(m_buffer[0]);

				if (boost::concurrent::queue_op_status::success == status)
				{
					size_t count = 1;
					while (count < consumerLimit && boost::concurrent::queue::)
					{
						++count;
					}
					m_buffer.resize(count);
					m_pfnConsume(m_buffer);
				}
			}
		}

	private:
		boost::concurrent::sync_bounded_queue<input> m_queue;
		std::thread m_thread;
		std::function<void()> m_pfnPreProcess;
		std::function<void(const std::vector<input>&)> m_pfnConsume;

		std::shared_ptr<map<std::string, int>> m_ptrMap;

		std::vector<input> m_buffer;
	};

}