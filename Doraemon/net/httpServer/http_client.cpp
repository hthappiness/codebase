

#include "http_client.h"

namespace Doraemon
{
    namespace http
    {
        const HttpResponse HttpClient::sendRequest(
            const std::string &hostName,
            const std::string &port,
            HttpRequest &req)
        {
            auto const results = m_resolver.resolve(hostName, port);
            boost::asio::connect(m_socket, results.begin(), results.end());

            req.set(boost::beast::http::fields::host, );
            req.set(boost::beast::http::fields::content_length, req.body().length());

            boost::beast::http::write(m_socket, req);

            boost::beast::flat_buffer flatBuffer;
            HttpResponse resp;
            boost::beast::http::read(m_socket, flatBuffer, resp);

            boost::system::error_code ec;
            m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

            return resp;
        }
    } // namespace http

} // namespace Doraemon