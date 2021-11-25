#ifndef _UTILITY_HPP
#define _UTILITY_HPP

#include <tuple>
#include <functional>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

namespace util {

    template<typename... Types>
    using pkg = std::tuple<Types...>;

    template<typename... Types>
    constexpr auto pack(Types&&... Args) { return std::make_tuple(Args...);}

    struct sockaddr_in getSockAddr(std::string_view ip, uint16_t port);

}

#endif 