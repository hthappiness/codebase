#include <map>
#include <iostream>
int main( )
{
	using namespace std;
	typedef pair <const int, int> cInt2Int;
	map <int, int> m1;
	map <int, int> :: key_type key1;
	map <int, int> :: mapped_type mapped1;
	map <int, int> :: value_type value1;
	map <int, int> :: iterator pIter;
	// value_type can be used to pass the correct type
	// explicitly to avoid implicit type conversion
	m1.insert ( map <int, int> :: value_type ( 1, 10 ) );
	// Compare other ways to insert objects into a map
	m1.insert ( cInt2Int ( 2, 20 ) );
	m1[ 3 ] = 30;
	// Initializing key1 and mapped1
	key1 = ( m1.begin( ) -> first );
	mapped1 = ( m1.begin( ) -> second );
	cout << "The key of first element in the map is "<< key1 << "." << endl;
	cout << "The data value of first element in the map is "<< mapped1 << "." << endl;
	
	// The following line would cause an error because
	// the value_type is not assignable
	//value1 = cInt2Int ( 4, 40 );
	cout << "The keys of the mapped elements are:";
	for ( pIter = m1.begin( ) ; pIter != m1.end( ) ; pIter++ )
	{
		cout << " " << pIter -> first;
		cout << "." << endl;
		cout << "The values of the mapped elements are:";
	}
	for ( pIter = m1.begin( ) ; pIter != m1.end( ) ; pIter++ )
	{
		cout << " " << pIter -> second;
		cout << "." << endl;
	}
}
Output
The key of first element in the map is 1.
The data value of first element in the map is 10.
The keys of the mapped elements are: 1 2 3.
The values of the mapped elements are: 10 20 30.

class CException : public std::runtime_error{
public:
	CException(ULONG ulErrCode, CHAR const* errMsg);
	ULONG errorCode();
	
private:
	ULONG m_ulErrorCode;
}

class MyMap
{
public:
	template<typename T>
	T get(boost::string_view key) const{
		auto it = m_CapMap.find(key.to_string()); 
		if( it == m_CapMap.end() )
		{
			std::ostringstream stream;
			stream << "key [" << key << "] not found";
			throw CException(ERROR_KEY_NOT_FOUND, stream.str().c_str());
		}
		
		try
		{
			return boost::any_cast<T><it->second>;
		}catch(boost::bad_any_cast&) {
			std::ostringstream stream;
			stream << "key [" << key << "] not found";
			throw CException(ERROR_TYPE_MISMATCH, stream.str().c_str());
		}
	}
private:
	std::map<std::string, boost::any> m_CapMap;
}


template<typename FactoryMap>
typename FactoryMap::mapped_type getFactory(FactoryMap& fMap, boost::string_view factory_name) const
{
	auto it = m_CapMap.find(key.to_string()); 
	if( it == m_CapMap.end() )
	{
		return nullptr;
	}
	
	return typename FactoryMap::mapped_type{it->second.get(), []( typename FactoryMap::mapped_type::element_type*)
		{}
	};
}

class CDataBufNode: public CBaseTransformer
{
public:
	CDataBufNode(std::string& inputType, std::string& outputType,ULONG ulMaxTrdCnt, ULONG ulMaxQueueSize):
		m_inputType(inputType),m_outputType(outputType),ulTrdCnt(ulMaxTrdCnt),m_syncQueue(ulMaxQueueSize)
		{		
		}
		
	virtual void consume_bk(boost::any const& inputData)
	{
		try {
			if (m_syncQueue.full())
			{
				
			}
			m_syncQueue.push_back(inputData);
		}
		catch (boost::sync_queue_is_closed&)
		{
			
		}
	}
	
	virtual void start() override
	{
		for (ULONG i = 0;i < m_ulThreadCount; i++)
		{
			m_TrdList.emplace_back([this] {
				work();
			});
		}
	}
	
	void shutdown(){
		if( !m_syncQueue.closed())
		{
			m_syncQueue.close();
			std::foreach(m_TrdList,begin(), m_TrdList.end(), std::mem_fn(&std::thread::join));
		}
	}
	
private:
	void work(){
		while (true){
			try
			{
				auto item = m_syncQueue.pull_front();
				produce1(item);
			}
			catch (boost::sync_queue_is_closed&)
			{
				
			}
		}
	}
private:
	boost::sync_bounded_queue<boost::any> m_syncQueue;
	
	ULONG ulTrdCnt;
	std::list<std::thread> m_TrdList;

	std::string m_inputType;
	std::string m_outputType;
}

#define _WIN32
	#define DATAFLOW_SYMBOL_EXPORT __declspec(dllexport)
	//#define DATAFLOW_SYMBOL_EXPORT __declspec(dllimport)
#else
	#define DATAFLOW_SYMBOL_EXPORT __attribute__((visibility("default")))
#endif

#define DATAFLOW_PLUGIN_API DATAFLOW_SYMBOL_EXPORT
#define DATAFLOW_PLUGIN_API extern "C" DATAFLOW_SYMBOL_EXPORT

DATAFLOW_API void RegisterProducerFactory(const std::shared_ptr<CProducerFactory>& spFactory);

DATAFLOW_API std::shared_ptr<CProducerFactory> FindProducerFactory( const boost::string_view & FactoryName);


template< typename T>
class CBaseFactory{
public:
	virtual std::shared_ptr<T> create() = 0;
}
//其实可以不用template,用workNode就行，生成的节点都是这个类型

//主要是name it.不同层次的对象对应的语言，不断的具体化。
class CProducerFactory:public CBaseFactory<CProducer>{
	
};

class CConsumerFactory:public CBaseFactory<CConsumer>{
	
};

class CTransformerFactory:public CBaseFactory<CTransformer>{
	
};


/***********usage*****************/
/*********************************/

class CRtspProducerFactory:public CProducerFactory{
	
}

#if 0
RegisterProducerFactory(std::make_shared<CProducerFactory>());

auto spFactory = sys::FindProducerFactory("");
auto spNode    = spFactory->create();

#endif




