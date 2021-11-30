#ifndef _UTILITY_HPP
#define _UTILITY_HPP

#include <tuple>
#include <functional>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <any>
#include <map>
#include <unordered_map>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

namespace util {

    template<typename... Types>
    using pkg = std::tuple<Types...>;

    template<typename... Types>
    constexpr auto pack(Types&&... Args) { return std::make_tuple(Args...);}

    namespace network {
        struct sockaddr_in getSockAddr(std::string_view ip, uint16_t port);
    }

    namespace file {
        int readFile(std::string_view filePath, std::string& buf);
    }

}

#endif 