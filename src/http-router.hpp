#ifndef _HTTP_ROUTER_HPP
#define _HTTP_ROUTER_HPP

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <csignal>

#include <iostream>
#include <unordered_map>
#include <functional>
#include <thread>
#include <mutex>
#include <future>
#include <queue>

namespace http {

    class HttpRouter {
    public:
        HttpRouter() = default;
        ~HttpRouter() = default;

    private:
        std::thread             mListenerThread;
        std::condition_variable mWorkerCondvar;
        std::unordered_map<std::string, std::function<int(std::string, int)>> mRouteMap; 
    };

} 

#endif 