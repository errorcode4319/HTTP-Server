#include "http-message-template.hpp"

namespace http {

    namespace res_template {
        
        HttpResponse http_res_with_startline(std::string_view startLine) {
            HttpResponse res;
            res.setStartLine(startLine);
            return res;
        }

        HttpResponse badRequest() {
            static HttpResponse res = http_res_with_startline("HTTP/1.1 404 Bad Request");
            return res;
        }
        HttpResponse notFound() {
            static HttpResponse res = http_res_with_startline("HTTP/1.1 404 Not Found");
            return res;
        }
        HttpResponse notAllowed() {
            static HttpResponse res = http_res_with_startline("HTTP/1.1 405 Method Not Allowed");
            return res;
        }
    }
}