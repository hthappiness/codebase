namespace
{
	std::string make_daytime_string()
	{
		using namespace std; // For time_t, time and ctime;
		time_t now = time(0);
		return ctime(&now);
	}
}
 
void test_asio_synserver()
{
	typedef boost::asio::io_service IoService;
	typedef boost::asio::ip::tcp TCP;
 
	try
	{
		IoService ios;
		/*
			*以下构造函数相当于以下步骤
			* basic_socket_acceptor<tcp> acceptor(io_service);
			* acceptor.open(tcp::v4());
			* acceptor.bind(tcp::endpoint(13));
			* acceptor.listen(0);//default 
	    */
		/*
			tcp::endpoint(端点)由以下三个部分组成
			1. IP地址(address, address_v4, address_v6)
			2. 端口号
			3. 协议版本
	    */
		TCP::acceptor acceptor(ios, TCP::endpoint(TCP::v4(), 13));
		for (;;)
		{
			TCP::socket socket(ios);
			acceptor.accept(socket);
			std::string message = make_daytime_string();
 
			boost::system::error_code ignored_error;
			boost::asio::write(socket,
                               boost::asio::buffer(message), 
                               boost::asio::transfer_all(), 
                               ignored_error);
			PRINT_DEBUG(message);
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
