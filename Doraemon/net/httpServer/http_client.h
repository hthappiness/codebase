/****************************************************************************
copyright (c) 2020 
-----------------------------------------------------------------------------
           http_client.h

Module Name: 
Date Created: 2020/09/10
Author: hthappiness
Description: xxxxxxxxxxxxxxxxxxxxx
****************************************************************************/

#pragma once

#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"
#include "boost/asio/ip/connect.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/io_context.hpp"

namespace Doraemon {
    namespace http {

        //forward declaration
        class HttpRequest;
        
        using HttpResponse = boost::beast::http::response<boost::beast::http::string_body>;
        //class HttpResponse;
        enum class HttpMethod;

        class HttpClient {
        public:
            HttpClient();
        public:
            const HttpResponse sendRequest(
                const std::string& hostName, 
                const std::string& port,
                HttpRequest& req);

			const HttpResponse sendRequest(
                const std::string& hostName,
                const std::string& port,
				HttpMethod method,
                const std::string& target_url,
                const std::string& reqBody);

        public:
            const HttpResponse get(
				const std::string& hostName,
				const std::string& port,
                const std::string& targetUrl);

			const HttpResponse post(
				const std::string& hostName,
				const std::string& port,
				const std::string& targetUrl,
                const std::string& reqBody);
            
            const HttpResponse put(
				const std::string& hostName,
				const std::string& port,
				const std::string& targetUrl,
                const std::string& reqBody);

             const HttpResponse delete(
				const std::string& hostName,
				const std::string& port,
                const std::string& targetUrl);
        private:
            boost::asio::io_context m_ioContext;
            boost::asio::ip::tcp::resolver m_resolver;
            boost::asio::ip::tcp::socket m_socket;
        };
    }
}