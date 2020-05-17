
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

#include <cassert>
#include <string>
#include <leveldb/db.h>

#include <glog/logging.h>

#include "libdf/libdataflow.h"

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/http.hpp>

#define TEST_PLUGIN_PATH "./plugins/"

//c++ - Error: Expected template-name before '<' token, you need to add namespace "namespace dataflow" before the CDataProducer
class CDemoProducer : public dataFlow::CDataProducer<int>
{
public:
	int start(int j)
	{
		/* initial list.c++ 11 function . lamda funtion capture this .*/
		m_testTrd = std::thread{[this, j]()
			{
				run(j);
			}};
	}
	int wait_stop()
	{
		m_testTrd.join();
	}
private:
	void run(int j)
	{
		while(true)
		{
			produce(j++); //	this->produce(j++);
			if( j > 16 )
			{
				break;
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
private:
	std::thread m_testTrd;
};

class CDemoConsumer : public dataFlow::CDataConsumer<CDemoConsumer, int>
{
/* in c++, default member function and member variable accessed property is private. */
public:
	void consume(int i)
	{
		//std::cout<<"consume:i="<< i << std::endl;
		//LOG(ERROR)<<"consume:i="<< i;
		LOG_IF_EVERY_N(ERROR, i > 10, 3)<<"consume:i="<< i;
	}
};

int testDataFlow()
{
	//add a thread session
	const auto spDemoProducer = std::make_shared<CDemoProducer>();
	const auto spDemoConsumer = std::make_shared<CDemoConsumer>();

	dataFlow::BindOutput(spDemoProducer, spDemoConsumer);

	spDemoProducer->start(15);
	spDemoProducer->wait_stop();
}

int testLevelDb()
{	
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, "/tmp/password", &db);
	assert(status.ok());
	std::string key = "hting";
	std::string value = "ht329295821";
	std::string get;
	leveldb::Status s = db->Put(leveldb::WriteOptions(), key, value);
	if (s.ok())
	{
		s = db->Get(leveldb::ReadOptions(), key, &get);
	}

	if (s.ok())
	{
		std::cout << "read(key=" << key << ") (value=" << get << ")" << std::endl;
	}
	else
	{
		std::cout << "read failed!" << std::endl;
	}
	delete db;
	return 0;
}

#if 0
using namespace std;
using namespace boost;

class connection : public std::enable_shared_from_this<connection>
{
public:
	void operator()(asio::yield_context yield)
	{
		auto self = shared_from_this();
		try {
			cout << "--\n[" << self->counter << "] Socket ready" << endl;
			// >>> OUR CODE GOES HERE <<<
		}
		catch (system::system_error &e) {
			if (e.code() != system::error_code{ asio::error::eof }) {
				cerr << '[' << self->counter << "] Aborting on exception: "
					<< e.what() << endl;
				std::exit(1);
			}

			cout << '[' << self->counter << "] Error: "
				<< e.what() << endl;
		}
		catch (std::exception &e) {
			cerr << '[' << self->counter << "] Aborting on exception: "
				<< e.what() << endl;
			std::exit(1);
		}
	}

	asio::ip::tcp::socket &tcp_layer()
	{
		return socket.next_layer();
	}

	static std::shared_ptr<connection> make_connection(asio::io_context &ios,
		int counter)
	{
		return std::shared_ptr<connection>{new connection{ ios, counter }};
	}

private:
	connection(asio::io_context &ios, int counter)
		: socket(ios)
		, counter(counter)
	{}

	beast::http::buffered_socket socket;
	int counter;
};
#endif

using boost::asio::ip::tcp;

class CServer
{
public:
	CServer(boost::asio::io_context& io_context, short port)
		: acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
	{
		do_accept();
	}

private:
	void do_accept()
	{
		acceptor_.async_accept(
			[this](boost::system::error_code ec, tcp::socket socket)
		{
			if (!ec)
			{
				//std::make_shared<session>(std::move(socket))->start();
			}

			do_accept();
		});
	}

	tcp::acceptor acceptor_;
};

int main(int argc, char** argv)
{
    //dataFlow::Initialize(argc, argv);

    //dataFlow::AddPluginDir(TEST_PLUGIN_PATH);
    //dataFlow::LoadALLPlugins();
	google::InitGoogleLogging(argv[0]);
	//FLAGS_log_dir = "./";
	FLAGS_logtostderr = 1;

	//testDataFlow();
    // add pipeLine interface
	//testLevelDb();

    return 0;
}

/*to do:
 *1、+ gflags
 *2、+ glog
 *3、+ mysql levelDb mongoDb
 *4、+ kafka + redis
 *5、 + boost asio
 */