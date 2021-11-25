#include "http-router.hpp"

int main() {
    std::cout << "MainTest" << std::endl;

    auto server = std::make_unique<http::HttpRouter>();
    server->addRoute("/", [](const http::REQ& req) -> http::RES {
        std::cout << "Index => " << req.getURI() << ' ' << req.getMethod() << std::endl;
        http::RES res;
        res.setStartLine("HTTP/1.1 200 OK");
        res.setBody("Request Received");
        return res;
    }, {"GET", "POST"});
    server->start(16, "0.0.0.0", 8080);

    getchar();
}