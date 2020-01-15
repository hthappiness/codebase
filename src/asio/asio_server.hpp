
#include <vector>
#include <boost/asio.hpp>

//boost asio 再封装；基于boost asio底层接口的封装

//类比
//https://www.boost.org/doc/libs/1_72_0/doc/html/boost_asio/example/cpp11/http/server/request_handler.cpp

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

using netMsgCb = std::function<int (char*, int )>;

class CMsgDispatcher
{

};

class CIoObject
{
    using handler = std::function<void ()>;
private:
    int operator()();
private:
    handler m_handle;
};

#if 0
class CIoNetDataObject:public CIoObject
{
public:
    CIoNetObject(std::string addr, int iPort)
    {

    }
};
#endif

class CNetUdpObject:public CIoObject
{
    udp::socket m_socket;
    udp::endpoint m_senderEndpoint;
};

class CNetTcpObject:public CIoObject
{
public:
    CNetTcpObject(tcp::socket socket)
    : m_socket(std::move(socket))
    {}

    void do_read()
    {
        m_socket.async_read(boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
            if (!ec)
            {
                //m_dispatcher.deliver(read_msg_);//回调实现
                m_netCallback(read_msg_.body(), read_msg_.body_length());
                do_read();
            }
            else
            {
                //room_.leave(shared_from_this());
            }
            });
    }

    void do_write(char* data, std::size_t length)
    {
        m_socket.async_write(boost::asio::buffer(data, length),
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    do_read();
                }
            });
    }

private:
    tcp::socket m_socket;
    netMsgCb m_netCallback;

    std::array<char, 8192> buffer_;
};

class CFileObject:public CIoObject
{
public:
    CFileObject(){}
};

class CAsioServer
{
public:
 
    int plugFileObject();

    virtual int serverStart(int iCnt);

public:
    std::vector<std::thread> m_threadVec;
    boost::asio::io_context m_ioCtx;
};

class CSession
{
public:
    CIoObject* m_pIoObj;
}

class CAsioUdpServer : public CAsioServer
{
    int plugNetObject();
};

class CAsioTcpServer : public CAsioServer
{
 public:
    CAsioTcpServer(int iPort, netMsgCb msgCb):
        m_acceptor(m_ioCtx, tcp::endpoint(tcp::v4(), iPort),
        m_netCallback(msgCb))
    {
     
    }

    int serverStart(int iCnt) override
    {
        start();
        for(auto i=0;i<iCnt;i++)
        {
            m_threadVec.push_back([this](){
                m_ioCtx.run();
                });
        }

        m_ioCtx.run();//block until no task in the executor context
    }

    int start()
    {
        m_acceptor.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!ec)
                {
                    std::make_shared<CNetTcpObject>(std::move(socket), m_netCallback)->start();
                }

                start();
            });
    }
private:
    boost::asio::ip::tcp::acceptor m_acceptor;

    netMsgCb m_netCallback;
};