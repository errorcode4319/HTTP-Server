#ifndef _WEB_SERVER_CMD_HPP
#define _WEB_SERVER_CMD_HPP

#include "web-server.hpp"


class WebServerCommander {
public:
    WebServerCommander(std::shared_ptr<WebServer> was) : was(was) {    
        try {
            init();
        } catch(...) {
            std::cout << "Web Server Commander Init Failed" << std::endl;
            throw;
        }
    }
    
    void run(std::string_view cmdline);
    
private:
    void init();
    void addCommand(std::string_view cmd, std::function<void()> func);
    std::shared_ptr<WebServer> was;
    std::unordered_map<std::string, std::function<void()>> mCommandMap;
};

#endif // _WEB_SERVER_CMD_HPP