#ifndef _HTTP_ROUTER_HPP
#define _HTTP_ROUTER_HPP

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <csignal>

#include "utility.hpp"
#include "http-message.hpp"

#include <iostream>
#include <unordered_map>
#include <functional>
#include <thread>
#include <mutex>
#include <future>
#include <queue>

using namespace util;

namespace http {


    /*
        package => request, handler func, socket descriptor 
    */
    using ReqPkg = pkg<REQ, std::function<RES(REQ)>, int>;

    class HttpRouter {
    public:
        HttpRouter() = default;
        ~HttpRouter() = default;

    private:

        void    pushReq(const ReqPkg& reqPkg);
        ReqPkg  pullReq();
    private:

        std::thread             mListenerThread;
        std::condition_variable mWorkerCondvar;

        std::queue<ReqPkg> mReqQ;
        std::mutex mReqMutex;

    };

} 

#endif 