

#include <functional>

#include "boost/asio/io_context.hpp"
#include "boost/asio/thread_pool.hpp"

#include "boost/asio/ip/tcp.hpp"

namespace Doraemon
{
    class HttpRequest;

    namespace http
    {
        using httpRequestHandler = std::function<int(HttpRequest&, std::string&)>;

        class HttpListener
        {
        public:
            long run(boost::asio::ip::tcp::endpoint& endpoint)
            {
                boost::system::error_code ec;
                m_acceptor.open(endpoint.protocol(), ec);

                m_acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec);

                m_acceptor.bind(endpoint, ec);

                m_acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);

                if(m_acceptor.is_open())
                {

                }

                accept();//继续注册accept函数
            }
        private:
            boost::asio::ip::tcp::acceptor m_acceptor;
            boost::asio::ip::tcp::socket m_socket;
        };

        class HttpServer
        {

        public:

        private:
            HttpListener m_listener;
        };
    };
}