#include "http-router.hpp"

int main() {
    std::cout << "MainTest" << std::endl;

    auto server = std::make_unique<http::HttpRouter>();
    server->addRoute("/", [](const http::REQ& req) -> http::RES {
        std::cout << "Index => " << req.getURI() << ' ' << req.getMethod() << std::endl;
        return http::RES();
    }, {"GET", "POST"});
    server->start(16, "0.0.0.0", 8080);
    
    getchar();
}