/****************************************************************************
copyright (c) 2020
-----------------------------------------------------------------------------
           http_request.h

Module Name:
Date Created: 2020/09/10
Author: hthappiness
Description: xxxxxxxxxxxxxxxxxxxxx
****************************************************************************/

#pragma once

#include "boost/beast/http.hpp"

namespace Doraemon
{
    namespace http
    {
        enum class HttpMethod
        {
            HTTP_METHOD_UNKNOWN = -1,
            HTTP_METHOD_GET = 0,
            HTTP_METHOD_POST,
            HTTP_METHOD_PUT,
            HTTP_METHOD_DELETE,
            HTTP_METHOD_HEAD,
            HTTP_METHOD_OPTIONS
        };

        using HttpStringBodyRequest = boost::beast::http::request<boost::beast::http::string_body>;

        class HttpRequest : public HttpStringBodyRequest
        {
        public:
            HttpRequest(const HttpRequest& request);

            HttpRequest(boost::beast::string_view& url, unsigned int version);
        public:
        
        };
    }
} // namespace Doraemon