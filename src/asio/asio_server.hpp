
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

class CIoNetDataObject:public CIoObject
{
public:
    CIoNetObject(std::string addr, int iPort)
    {

    }
};
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
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
            boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
            if (!ec)
            {
                m_dispatcher.deliver(read_msg_);//回调实现
                do_read();
            }
            else
            {
                //room_.leave(shared_from_this());
            }
            });
    }

    void do_write(std::size_t length)
    {
        boost::asio::async_write(m_socket, boost::asio::buffer(data_, length),
            make_custom_alloc_handler(handler_memory_,
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    do_read();
                }
            }));
    }

private:
    tcp::socket m_socket;
    netMsgCb m_netCallback;
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

    int serverStart(int iCnt)
    {
        start();
        for(auto i=0;i<iCnt;i+=)
        {
            m_threadVec.push_back([this](){
                m_ioCtx.run();
                });
        }
    }

    int start()
    {
        m_acceptor.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!ec)
                {
                    std::make_shared<CIoNetDataObject>(std::move(socket), m_netCallback)->start();
                }

                start();
            });
    }
private:
    boost::asio::ip::tcp::acceptor m_acceptor;

    netMsgCb m_netCallback;
};