#include "http-router.hpp"

int main() {
    std::cout << "MainTest" << std::endl;

    http::HttpRouter server;
    server.start(8);

    std::string buf;
    std::cin >> buf;

    server.stop();
}