#include "http-router.hpp"

namespace http {
    
    void HttpRouter::start(int maxWorker, std::string_view ip, uint16_t port) {
        if(isRunning()) {
            std::cerr << "Already Running" << std::endl; return;
        }
        mIsRunning = true;
        mIP = ip;
        mPort = port;
        std::promise<int> promListenerState;
        auto futState = promListenerState.get_future();
        mListenerThread = std::thread{&HttpRouter::listenerProc, this, ip, port, std::move(promListenerState)};
        int state = futState.get();
        if (state != 0) { // Service Start Failed 
            mIsRunning = false;
            if(mListenerThread.joinable()) mListenerThread.join();
            std::cerr << "Service Start Failed" << std::endl;
            return;
        }
        for(int i=0;i<maxWorker;i++) {
            mWorkerThread.emplace_back(&HttpRouter::worker, this);
        }
    }

    void HttpRouter::stop() {
        if(isRunning()) {
            mIsRunning = false;
            if(mListenerThread.joinable()) mListenerThread.join();
            for(auto& t: mWorkerThread) {
                if(t.joinable()) t.join();
            }
        }
    }

    void HttpRouter::addRoute(std::string_view uri, std::function<RES(const REQ&)> func, std::vector<std::string> methods) {
        std::scoped_lock lock(mRouteMutex);
        if(auto iter = mRouteMap.find(uri.data()); iter == std::end(mRouteMap)) {
            mRouteMap.insert({uri.data(), pack(func, methods)});
        }
        else {
            throw std::runtime_error("Add Route Failed");
        }
    }

    RouteFunc HttpRouter::findRoute(std::string_view uri, std::string_view method) {
        std::scoped_lock lock(mRouteMutex);
        if(auto iter = mRouteMap.find(uri.data()); iter != std::end(mRouteMap)) {
            auto&[func, methods] = iter->second;
            for(auto& m:methods) {
                if(m == method) return func; 
            }
            throw std::runtime_error("Not Allowed Method");
        }
        throw std::runtime_error("404 Not Found");
    }

    void HttpRouter::listenerProc(std::string_view ip, uint16_t port, std::promise<int> state) {
        
        static auto init_failed = [&](int stateCode, std::string_view msg) {
            std::cerr << msg << std::endl;
            state.set(stateCode);
        };

        mServSock = socket(PF_INET, SOCK_STREAM, 0); //TCP Socket 
        if(mServSock < 0) {
            init_failed(-1, "Create TCP Socket Failed");
        }

        int sockopt_flag = 1;
        if(setsockopt(mServSock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&sockopt_flag), sizeof(sockopt_flag)) < 0) {
            init_failed(-2, "Set SocketOption Failed");
        }

        auto sockAddr = getSockAddr("0.0.0.0", 8080);   //utility.hpp
        socklen_t sockAddrLen = sizeof(sockAddr);
        if(bind(mServSock, (struct sockaddr*)&sockAddr, sockAddrLen) < 0) {
            init_failed(-3, "Bind Address Failed");
        }

        if(listen(mServSock, 256) < 0) {
            init_failed(-4, "Listen Socket Failed");
        }

        state.set(0); //init success 

        fd_set fd;
        FD_ZERO(&fd);
        struct timeval to;
        to.tv_set = 1;
        to_tv_usec = 0;
    
    }

    void HttpRouter::workerProc() {
        while(isRunning()) {    
            {
                std::unique_lock lock(mReqMutex);
                mWorkerCondvar.wait(lock);
            }

            auto optReq = pullReq();
            if(optReq.has_value() == false) 
                continue;

            auto[req, func, sock] = optReq.value();
            
            try {
                auto res = func(req);
                auto resMsg = res.to_serialized();
                std::cout << resMsg << std::endl;
            } catch(...) {
                
            }
        }
    }


}