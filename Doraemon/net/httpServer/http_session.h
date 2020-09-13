/****************************************************************************
copyright (c) 2020 
-----------------------------------------------------------------------------
           http_session.h

Module Name: 
Date Created: 2020/09/11
Author: hthappiness
Description: xxxxxxxxxxxxxxxxxxxxx
****************************************************************************/

#pragma once

#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"

#include "boost/asio/bind_executor.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/strand.hpp"

#include "boost/thread/mutex.hpp"
#include "boost/config.hpp"

namespace Doraemon
{
    namespace http
    {
        class HttpSession
        {
            using sessionMessageCb = std::function<void(const std::shared_ptr<HttpSession>&)>;
        public:
            HttpSession();
        
            void readRequest();
        public:

        private:
            boost::asio::ip::tcp::socket m_socket;
            boost::asio::strand<boost::asio::io_context::executor_type> m_strand;
            boost::beast::flat_buffer m_buffer;
            boost::beast::http::request<boost::beast::http::string_body> m_request;
            boost::asio::steady_timer m_timer;

            //static sessionMessageCb m_httpMessageHandler;
        };
    };
};