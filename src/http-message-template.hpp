#include "http-message.hpp"

namespace http {

/*
    https://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
*/
    namespace res_template {
        HttpResponse http_res_with_startline(std::string_view startLine);
        
        HttpResponse badRequest();
        HttpResponse notFound();
        HttpResponse notAllowed();
    }

}