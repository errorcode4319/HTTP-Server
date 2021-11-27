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
    void help();
    void init();
    void addCommand(std::string_view cmd, std::function<void()> func, std::string desc);
    std::shared_ptr<WebServer> was;
    std::unordered_map<std::string, pkg<std::function<void()>, std::string>> mCommandMap;
};

#endif // _WEB_SERVER_CMD_HPP