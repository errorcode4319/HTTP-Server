#include "http-router.hpp"

namespace http {
    
    void HttpRouter::start() {
        
        if(isRunning()) {
            std::cerr << "Already Running" << std::endl; return;
        }
        
        std::cout << "Service Start" << std::endl;

        mIsRunning = true;
        std::promise<int> promListenerState;
        auto futState = promListenerState.get_future();
        mListenerThread = std::thread{&HttpRouter::listenerProc, this, mIP, mPort, std::move(promListenerState)};
        int state = futState.get();
        if (state != 0) { // Service Start Failed 
            mIsRunning = false;
            if(mListenerThread.joinable()) mListenerThread.join();
            std::cerr << "Service Start Failed" << std::endl;
            return;
        }
        for(int i=0;i<mMaxWorker;i++) {
            mWorkerThread.emplace_back(&HttpRouter::workerProc, this);
        }
    }

    void HttpRouter::stop() {
        std::cout << "Service Shutdown" << std::endl;
        if(isRunning()) {
            mIsRunning = false;
            std::unique_lock lock(mReqMutex);
            mWorkerCondvar.notify_all();
        }
        if(mListenerThread.joinable())
            mListenerThread.join();
        for(auto& t: mWorkerThread) {
            if(t.joinable()) t.join();
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
        static auto func404 = [](const REQ& req) -> RES {
            return res_template::notFound();
        };
        static auto func405 = [](const REQ& req) -> RES {
            return res_template::notAllowed();
        };

        std::scoped_lock lock(mRouteMutex);
        if(auto iter = mRouteMap.find(uri.data()); iter != std::end(mRouteMap)) {
            auto&[func, methods] = iter->second;
            for(auto& m:methods) {
                if(m == method) return func; 
            }
            return func405;
        }
        return func404;
    }

    void HttpRouter::pushReq(const ReqPkg& reqPkg) {
        {
            std::scoped_lock lock(mReqQMutex);
            mReqQ.emplace(reqPkg);
        }
        std::unique_lock lock(mReqMutex);
        mWorkerCondvar.notify_one();
    }
    std::optional<ReqPkg> HttpRouter::pullReq() {
        std::scoped_lock lock(mReqQMutex);
        if(mReqQ.empty()) return std::nullopt;
        auto data = mReqQ.front();
        mReqQ.pop();
        return data;
    }

    void HttpRouter::listenerProc(std::string_view ip, uint16_t port, std::promise<int> state) {
        
        static auto init_failed = [&](int stateCode, std::string_view msg) {
            std::cerr << msg << std::endl;
            state.set_value(stateCode);
        };

        mServSock = socket(PF_INET, SOCK_STREAM, 0); //TCP Socket 
        if(mServSock < 0) {
            init_failed(-1, "Create TCP Socket Failed"); return;
        }

        int sockopt_flag = 1;
        if(setsockopt(mServSock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&sockopt_flag), sizeof(sockopt_flag)) < 0) {
            init_failed(-2, "Set SocketOption Failed"); return;
        }

        auto sockAddr = network::getSockAddr(ip, port);   //utility.hpp
        
        socklen_t sockAddrLen = sizeof(sockAddr);
        if(bind(mServSock, (struct sockaddr*)&sockAddr, sockAddrLen) < 0) {
            init_failed(-3, "Bind Address Failed"); return;
        }

        if(listen(mServSock, 256) < 0) {
            init_failed(-4, "Listen Socket Failed"); return;
        }

        state.set_value(0); //init success 

        fd_set fd;
        FD_ZERO(&fd);
        

        while(isRunning()) {
            struct timeval tv;
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            FD_SET(mServSock, &fd);
            if(select(mServSock + 1, &fd, nullptr, nullptr, &tv) > 0) {
                int clntSock = accept(mServSock, (struct sockaddr*)&sockAddr, &sockAddrLen);
                if (clntSock < 0) {
                    std::cerr << "Client Socket Accept Failed" << std::endl; break;
                }
                std::string buf(4096, 0);
                if(read(clntSock, &buf[0], buf.size()) < 0) {
                    std::cerr << "Read Request Data Failed" << std::endl; break;
                }
                REQ req(buf);
                try {
                    auto func = findRoute(req.getURI(), req.getMethod());
                    pushReq(req, func, clntSock);
                } catch(const std::runtime_error& e) {
                    std::cerr << e.what() << std::endl;
                }
            }
        }
        

        close(mServSock);
        return;
    }

    void HttpRouter::workerProc() {
        while(isRunning()) {    
            {
                std::unique_lock lock(mReqMutex);
                mWorkerCondvar.wait(lock);
            }
            if(!isRunning()) return;

            auto optReq = pullReq();
            if(optReq.has_value() == false) 
                continue;

            auto[req, func, sock] = optReq.value();
            
            try {
                auto res = func(req);
                auto resMsg = res.to_serialized();
                write(sock, resMsg.c_str(), resMsg.size());
            } catch(...) {
                
            }

            close(sock);
        }
    }


}