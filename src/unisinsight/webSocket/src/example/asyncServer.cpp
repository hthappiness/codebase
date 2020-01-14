
namespace
{
	typedef boost::asio::io_service IoService;
	typedef boost::asio::ip::tcp TCP;
 
	std::string make_daytime_string()
	{
		using namespace std;
		time_t now = std::time(NULL);
		return ctime(&now);
	}
 
	class tcp_connection
		: public boost::enable_shared_from_this<tcp_connection>
	{
	public:
		typedef boost::shared_ptr<tcp_connection> pointer;
 
		static pointer create(IoService& io_service)
		{
			return pointer(new tcp_connection(io_service));
		}
 
		TCP::socket& socket()
		{
			return socket_;
		}
 
		void start()
		{
			message_ = make_daytime_string();
 
			boost::asio::async_write(
				socket_, 
				boost::asio::buffer(message_), 
				boost::bind(&tcp_connection::handle_write, 
							shared_from_this());
							//boost::asio::placeholders::error, 
							//boost::asio::placeholders::bytes_transferred));
		}
	private:
		tcp_connection(IoService& io_service)
			: socket_(io_service)
		{
		}
 
		void handle_write(const boost::system::error_code& /*error*/,
			size_t /*bytes_transferred*/)
		{
			PRINT_DEBUG("write data!!!");
		}
 
		TCP::socket socket_;
		std::string message_;
	};
 
	class tcp_server
	{
	public:
		tcp_server(IoService& io_service)
			: acceptor_(io_service, TCP::endpoint(TCP::v4(), 10000))
		{
			start_accept();
		}
	private:
		void start_accept()
		{
			tcp_connection::pointer new_connection =
				tcp_connection::create(acceptor_.get_io_service());
			
			acceptor_.async_accept(
				new_connection->socket(), 
				boost::bind(&tcp_server::handle_accept, 
							this, 
							new_connection ));
		}
 
		void handle_accept(tcp_connection::pointer new_connection,
			const boost::system::error_code& error)
		{
			if (!error)
			{
				new_connection->start();
				start_accept();
			}
		}
 
		TCP::acceptor acceptor_;
	};
}
 
// tcp_connection与tcp_server封装后
void test_asio_asynserver()
{
	try
	{
		IoService io_service;
		tcp_server server(io_service);
 
		// 只有io_service类的run()方法运行之后回调对象才会被调用
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
