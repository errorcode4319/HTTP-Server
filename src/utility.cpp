#include "utility.hpp"

namespace util {
    
    struct sockaddr_in network::getSockAddr(std::string_view ip, uint16_t port) {
        struct sockaddr_in sockAddr{};
        sockAddr.sin_family = AF_INET;
        inet_pton(sockAddr.sin_family, ip.data(), &sockAddr.sin_addr.s_addr);
        sockAddr.sin_port = htons(port);
        return sockAddr;
    }
    
    int file::readFile(std::string_view filePath, std::string& buf) {
        std::ifstream file(filePath.data());
        if(!file) 
            return -1;
        std::stringstream ss;
        ss << file.rdbuf();
        buf = ss.str();
        return 0;
    }
}