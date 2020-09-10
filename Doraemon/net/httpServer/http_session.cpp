
#include "http_session.h"

namespace Doraemon
{
    namespace http
    {
        void HttpSession::onRequest(boost::system::error_code ec, std::size_t bytes_transferred)
        {
            //callback接口，为session类所共有，以处理http参数
        }

        void HttpSession::readRequest()
        {
            boost::beast::http::async_read(m_socket, m_buffer, m_req,
                                           boost::asio::bind_executor(m_strand,
                                                                      std::bind(
                                                                          &HttpSession::onRequest,
                                                                          shared_from_this(),
                                                                          std::placeholders::_1,
                                                                          std::placeholders::_2)));
        }
    } // namespace http
} // namespace Doraemon