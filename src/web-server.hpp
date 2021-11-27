#ifndef _WEB_SERVER_HPP
#define _WEB_SERVER_HPP

#include "http-router.hpp"
#include <memory>
#include <set>

using namespace http;

class WebServer {
public:
    WebServer(std::string_view ip = "0.0.0.0", uint16_t port = 8080) : WebServer(8, ip, port){
    }
    WebServer(int maxWorker, std::string_view ip, uint16_t port) {
        mRouterInst = std::make_unique<HttpRouter>(maxWorker, ip, port);
    }
    void route(std::string_view uri, RouteFunc func, std::vector<std::string> methods = {"GET"}) {
        if(mRouterInst == nullptr) 
            throw std::runtime_error("No Router Instance");
        try {
            mRouterInst->addRoute(uri, func, methods);
        } catch(...) {
            std::cerr << "Add Route Failed => " << uri << std::endl;
        }
    }
    void view(std::string_view uri, std::string_view htmlFilePath,  std::vector<std::string> methods={"GET"});

public: //Commands
    void run() { mRouterInst->start(); }
    void stop() {mRouterInst->stop();}
    void restart() {stop(); run();}
    void reload() { std::cout << "Not Supported" << std::endl; }
    std::string getState() {
        std::stringstream ss;
        ss << "State: " << ((isRunning())?"Running":"Idle") << std::endl;
        ss << "IP: " << mRouterInst->getIP() << std::endl;
        ss << "Port: " << mRouterInst->getPort() << std::endl;
        return ss.str();
    }
    bool isRunning() {return mRouterInst->isRunning();}
private:
    std::unique_ptr<HttpRouter> mRouterInst;

    // uri, filename 
    // reload -> directory scan -> add new html files
    std::unordered_map<std::string, std::string> mHTMLBindMap;
};

#endif 