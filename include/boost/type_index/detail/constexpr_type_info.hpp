//
// Copyright (c) Klemens D. Morgenstern, 2016
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_TYPE_INDEX_DETAIL_CONSTEXPR_TYPE_INFO_HPP_
#define BOOST_TYPE_INDEX_DETAIL_CONSTEXPR_TYPE_INFO_HPP_

#include <string>

namespace boost { namespace typeindex { namespace detail {


template<char...Cs>
struct constexpr_typename;

template<char ...Cs>
inline std::string to_string(constexpr_typename<Cs...> )
{
    return {Cs...};
}

template<char...Cs>
struct constexpr_typename
{
    std::string to_string() const
    {
        return
                ::boost::typeindex::detail::to_string(*this);
    }
};


template<char...C1s, char...C2s>
constexpr bool operator!=(constexpr_typename<C1s...>, constexpr_typename<C2s...>)
{
    return true;
}

template<char...Cs>
constexpr bool operator!=(constexpr_typename<Cs...>, constexpr_typename<Cs...>)
{
    return false;
}

template<char...C1s, char...C2s>
constexpr bool operator==(constexpr_typename<C1s...>, constexpr_typename<C2s...>)
{
    return false;
}

template<char...Cs>
constexpr bool operator==(constexpr_typename<Cs...>, constexpr_typename<Cs...>)
{
    return true;
}

template<char F1, char...C1s, char F2, char...C2s>
constexpr bool operator<(constexpr_typename<F1, C1s...>, constexpr_typename<F2, C2s...>)
{
    return (F1 < F2) ? true
           : (constexpr_typename<C1s...>() < constexpr_typename<C2s...>())
            ;
}

template<char F1, char F2>
constexpr bool operator<(constexpr_typename<F1>, constexpr_typename<F2>)
{
    return F1 < F2;
}

template<char F1, char F2, char...C2s>
constexpr bool operator<(constexpr_typename<F1>, constexpr_typename<F2, C2s...>)
{
    return (F1 < F2);
}

template<char C>
constexpr bool operator<(constexpr_typename<C>, constexpr_typename<C>)
{
    return false;
}

template<char...Cs>
constexpr bool operator<(constexpr_typename<Cs...>, constexpr_typename<>)
{
    return false;
}

template<char...Cs>
constexpr bool operator<(constexpr_typename<>, constexpr_typename<Cs...>)
{
    return true;
}

template<char...C1s, char...C2s>
constexpr bool operator>(constexpr_typename<C1s...> c1, constexpr_typename<C2s...> c2)
{
    return c2 < c1;
}

template<char...C1s, char...C2s>
constexpr bool operator<=(constexpr_typename<C1s...> c1, constexpr_typename<C2s...> c2)
{
    return c1 == c2 ?
            true :
            c1 < c2;
}

template<char...C1s, char...C2s>
constexpr bool operator>=(constexpr_typename<C1s...> c1, constexpr_typename<C2s...> c2)
{
    return c1 == c2 ?
            true :
            c1 > c2;
}




}

}}



#endif /* INCLUDE_BOOST_TYPE_INDEX_DETAIL_CONSTEXPR_TYPE_INFO_HPP_ */
