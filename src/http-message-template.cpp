#include "http-message-template.hpp"

namespace http {

    namespace res_template {

        namespace {
            std::unordered_map<int, std::string> StatusCodeDef = {
                /* 2XX Successful */
                {200, "OK"},
                {201, "Created"},
                {202, "Accepted"},
                {203, "None-Authoritative Information"},
                {204, "No Content"},
                {205, "Reset Content"},
                /* 3XX Redirection */
                {300, "Multiple Choices"},
                {301, "Moved Permanently"},
                {302, "Found"},
                {303, "See Other"},
                {304, "Modified"},
                {305, "Use Proxy"}, /* 306 deprecated */                
                {307, "Temporary Redirect"},
                /* 4XX Client Error */
                {400, "Bad Request"},
                {401, "Unauthorized"},
                {402, "Payment Required"},
                {403, "Forbidden"},
                {404, "Not Found"},
                {405, "Method Not Allowd"},
                {406, "Not Acceptable"},
                {407, "Proxy Authentication Required"},
                {408, "Request Timeout"},
                {409, "Conflict"},
                {410, "Gone"},
                {411, "Length Required"},
                {412, "Precondition Failed"},
                {413, "Request Entity Too Large"},
                {414, "Request-URI Too Long"},
                {415, "Unsupported Media Type"},
                {416, "Requested Range Not Satisfiable"},
                {417, "Expectation Failed"},
                /* 5XX Server Error */
                {500, "Internal Server Error"},
                {501, "Not Implemented"},
                {502, "Bad Gateway"},
                {503, "Service Unavailable"},
                {504, "Gateway Timeout"},
                {505, "HTTP Version Not Supported"}
            };
        }
        
        HttpResponse getResponse(int statusCode) {
            auto iter = StatusCodeDef.find(statusCode);
            if(iter == std::end(StatusCodeDef)) {
                return HttpResponse();
            }
            std::stringstream ss;
            ss << "HTTP/1.1 " << statusCode << ' ' << iter->second;
            return HttpResponse(ss.str());
        }
        HttpResponse OK() {
            return getResponse(200);
        }
        HttpResponse badRequest() {
            return getResponse(400);
        }
        HttpResponse notFound() {
            return getResponse(404);
        }
        HttpResponse notAllowed() {
            return getResponse(405);
        }
    }
}