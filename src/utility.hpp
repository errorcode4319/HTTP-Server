#ifndef _UTILITY_HPP
#define _UTILITY_HPP

#include <tuple>
#include <functional>


namespace util {

    template<typename... Types>
    using pkg = std::tuple<Types...>;

    template<typename... Types>
    constexpr auto pack(Types&&... Args) { return std::make_tuple(Args...);}

}

#endif 