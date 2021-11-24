#ifndef _HTTP_ROUTER_HPP
#define _HTTP_ROUTER_HPP

#include <iostream>
#include <string>
#include <unordered_map>

namespace http {

    class HttpMessage {

    };

    class HttpRequest : public HttpMessage {

    }

    class HttpResponse : public HttpMessage {

    }

    using REQ = HttpRequest;
    using RES = HttpResponse;

}

#endif 