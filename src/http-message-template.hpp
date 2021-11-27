#ifndef _HTTP_MESSAGE_TEMPLATE_H
#define _HTTP_MESSAGE_TEMPLATE_H

#include "http-message.hpp"
#include <sstream>

namespace http {

/*
    https://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
*/
    namespace res_template {
        HttpResponse getResponse(int statusCode);
        
        HttpResponse OK();
        HttpResponse badRequest();
        HttpResponse notFound();
        HttpResponse notAllowed();
    }

}

#endif //_HTTP_MESSAGE_TEMPLATE_H