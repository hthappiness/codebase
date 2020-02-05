
//
//https://www.cnblogs.com/fnlingnzb-learner/p/10402276.html
//https://cloud.tencent.com/developer/ask/41090

class AsioIOServicePool
{
public:
    using IOService = boost::asio::io_service;
    using Work = boost::asio::io_service::work;
    using WorkPtr = std::unique_ptr<Work>;

    AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency())
        : ioServices_(size),
          works_(size),
          nextIOService_(0)
    {
        for (std::size_t i = 0; i < size; ++i)
        {
            works_[i] = std::unique_ptr<Work>(new Work(ioServices_[i]));
        }
        for (std::size_t i = 0; i < ioServices_.size(); ++i)
        {
            threads_.emplace_back([this, i] ()
                                  {
                                      ioServices_[i].run();
                                  });
        }
    }
    AsioIOServicePool(const AsioIOServicePool &) = delete;
    AsioIOServicePool &operator=(const AsioIOServicePool &) = delete;
    // 使用 round-robin 的方式返回一个 io_service
    boost::asio::io_service &getIOService()
    {
        auto &service = ioServices_[nextIOService_++];
        if (nextIOService_ == ioServices_.size())
        {
            nextIOService_ = 0;
        }
        return service;
    }
    void stop()
    {
        for (auto &work: works_)
        {
            work.reset();
        }
        for (auto &t: threads_)
        {
            t.join();
        }
    }
private:
    std::vector<IOService>       ioServices_;
    std::vector<WorkPtr>         works_;
    std::vector<std::thread>     threads_;
    std::size_t                  nextIOService_;
};

class AsioThreadPool
{
public:
    AsioThreadPool(int threadNum = std::thread::hardware_concurrency())
        : work_(new boost::asio::io_service::work(service_))
    {
        for (int i = 0; i < threadNum; ++i)
        {
            threads_.emplace_back([this] () { service_.run(); });
        }
    }
    AsioThreadPool(const AsioThreadPool &) = delete;
    AsioThreadPool &operator=(const AsioThreadPool &) = delete;
    boost::asio::io_service &getIOService()
    {
        return service_;
    }
    void stop()
    {
        work_.reset();
        for (auto &t: threads_)
        {
            t.join();            
        }        
    }
private:
    boost::asio::io_service service_;
    std::unique_ptr<boost::asio::io_service::work> work_;
    std::vector<std::thread> threads_;
};

class TCPConnection : public std::enable_shared_from_this<TCPConnection> 
{
public:
    TCPConnection(boost::asio::io_service &io_service)
        : socket_(io_service),
          strand_(io_service)
    { }
    
    tcp::socket &socket() {  return socket_;  }
    void start() {  doRead();  }
    
private:
    void doRead()
    {
        auto self = shared_from_this();
        socket_.async_read_some(
            boost::asio::buffer(buffer_, buffer_.size()),
            strand_.wrap([this, self](boost::system::error_code ec, 
                                      std::size_t bytes_transferred)
                         {
                             if (!ec) {  doWrite(bytes_transferred);  }
                         }));
    }
    void doWrite(std::size_t length)
    {
        auto self = shared_from_this();
        boost::asio::async_write(
            socket_, boost::asio::buffer(buffer_, length),
            strand_.wrap([this, self](boost::system::error_code ec,
                                      std::size_t /* bytes_transferred */)
                         {
                             if (!ec) {  doRead();  }
                         }));
    }
private:
    tcp::socket socket_;
    boost::asio::io_service::strand strand_;
    std::array<char, 8192> buffer_;
};
class EchoServer
{
public:
    EchoServer(boost::asio::io_service &io_service, unsigned short port)
        : io_service_(io_service),
          acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
    {
        doAccept();
    }
    void doAccept()
    {
        auto conn = std::make_shared<TCPConnection>(io_service_);
        acceptor_.async_accept(conn->socket(),
                               [this, conn](boost::system::error_code ec)
                               {
                                   if (!ec) {  conn->start();  }
                                   this->doAccept();
                               });
    }
    
private: 
    boost::asio::io_service &io_service_;
    tcp::acceptor acceptor_;
};

int test1()
{
    std::mutex mtx;             // protect std::cout
    AsioIOServicePool pool;
 
    boost::asio::steady_timer timer{pool.getIOService(), std::chrono::seconds{2}};
    timer.async_wait([&mtx] (const boost::system::error_code &ec)
                  {
                      std::lock_guard<std::mutex> lock(mtx);
                      std::cout << "Hello, World! " << std::endl;
                  });
    pool.stop();
}

int test2()
{
    AsioThreadPool pool(4);    // 开启 4 个线程
    boost::asio::steady_timer timer1{pool.getIOService(), std::chrono::seconds{1}};
    boost::asio::steady_timer timer2{pool.getIOService(), std::chrono::seconds{1}};
    int value = 0;
    boost::asio::io_service::strand strand{pool.getIOService()};    
    
    timer1.async_wait(strand.wrap([&value] (const boost::system::error_code &ec)
                              {
                                  std::cout << "Hello, World! " << value++ << std::endl;
                              }));
    timer2.async_wait(strand.wrap([&value] (const boost::system::error_code &ec)
                              {
                                  std::cout << "Hello, World! " << value++ << std::endl;
                              }));
    pool.stop();
}

int main()
{
   test1();
   test2();

   return 0;
}

