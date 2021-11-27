#include "web-server.hpp"
#include "web-server-commander.hpp"

int main() {
    try {    
        auto was = std::make_shared<WebServer>(); // => 0.0.0.0:8080
        WebServerCommander commander(was);

        was->view("/", "html/index.html", {"GET", "POST"});

        was->run();

        std::string cmd;
        while(true) {
            std::cout << "CMD> ";
            std::getline(std::cin, cmd);
            commander.run(cmd);
        }
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    } catch(...) {
        std::cerr << "Runtime Error " << std::endl;
        return -1;
    }

    return 0;
}