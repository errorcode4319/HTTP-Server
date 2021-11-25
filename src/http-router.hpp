#ifndef _HTTP_ROUTER_HPP
#define _HTTP_ROUTER_HPP

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <csignal>

#include "utility.hpp"
#include "http-message.hpp"
#include "http-message-template.hpp"

#include <iostream>
#include <unordered_map>
#include <functional>
#include <initializer_list>
#include <thread>
#include <mutex>
#include <future>
#include <queue>
#include <atomic>
#include <exception>
#include <stdexcept>

using namespace util;

namespace http {

    /*
        package => request, handler func, socket descriptor 
    */
    using RouteFunc = std::function<RES(const REQ&)>;

    using ReqPkg = pkg<REQ, RouteFunc, int>;
    using RoutePkg = pkg<RouteFunc, std::vector<std::string>>;

    class HttpRouter {
    public:
        HttpRouter() = default;
        ~HttpRouter() {stop();};

        void    start(int maxWorker, std::string_view ip="0.0.0.0", uint16_t port=8080);
        void    stop();

        void    addRoute(std::string_view uri, RouteFunc func, std::vector<std::string> methods = {"get"});

        bool    isRunning() const {return mIsRunning;}

    private:
        void    listenerProc(std::string_view ip, uint16_t port, std::promise<int> state);
        void    workerProc();

        void    pushReq(const ReqPkg& reqPkg);
        inline void pushReq(REQ req, RouteFunc func, int sock) {pushReq(pack(req, func, sock));}
        std::optional<ReqPkg>  pullReq();

        RouteFunc findRoute(std::string_view uri, std::string_view method);
    private:

        std::thread             mListenerThread;
        std::vector<std::thread> mWorkerThread;
        std::condition_variable mWorkerCondvar;
        std::mutex mReqMutex;

        std::queue<ReqPkg> mReqQ;
        std::mutex mReqQMutex;

        std::unordered_map<std::string, RoutePkg> mRouteMap;
        std::mutex mRouteMutex;

        bool mIsRunning = false;
        int mServSock = -1;
        std::string mIP = "";
        uint16_t    mPort = 0;
    };

} 

#endif 