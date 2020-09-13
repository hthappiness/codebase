/****************************************************************************
copyright (c) 2020 
-----------------------------------------------------------------------------
           http_response.h

Module Name: 
Date Created: 2020/09/11
Author: hthappiness
Description: xxxxxxxxxxxxxxxxxxxxx
****************************************************************************/

#pragma once

#include "boost/beast/http.hpp"

namespace Doraemon
{

    namespace http
    {
        using httpStringBodyResponse = boost::beast::http::response<boost::beast::http::string_body>;
        using httpEmptyBodyResponse  = boost::beast::http::response<boost::beast::http::empty_body>;
        using httpFileBodyResponse  = boost::beast::http::response<boost::beast::http::file_body>;

        class HttpBodyResponseBase : public httpStringBodyResponse
        {

        };

        class HttpBadRequestResponse : public HttpBodyResponseBase
        {

        };

        class HttpNotFoundResponse : public HttpBodyResponseBase
        {

        };
    }
}