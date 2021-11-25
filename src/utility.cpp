#include "utility.hpp"

namespace util {

    
    struct sockaddr_in getSockAddr(std::string_view ip, uint16_t port) {
        struct sockaddr_in sockAddr{};
        sockAddr.sin_family = AF_INET;
        inet_pton(sockAddr.sin_family, ip.data(), &sockAddr.sin_addr.s_addr);
        sockAddr.sin_port = htons(port);
        return sockAddr;
    }

}