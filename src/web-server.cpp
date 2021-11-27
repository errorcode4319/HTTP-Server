#include "web-server.hpp"

void WebServer::view(std::string_view uri, std::string_view htmlFilePath, std::vector<std::string> methods) {
    auto static_view_func = [htmlFilePath](const REQ& req) -> RES {
        auto res = res_template::OK();
        std::string contents;
        //utility.hpp
        if(file::readFile(htmlFilePath, contents) == 0) {
            res.setBody(contents);
        }
        return res;
    };
    route(uri, static_view_func, methods);
}