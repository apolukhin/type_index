//
// Copyright (c) Antony Polukhin, 2013-2014.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_TYPE_INDEX_BASE_HPP
#define BOOST_TYPE_INDEX_TYPE_INDEX_BASE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/config.hpp>
#include <string>

#if !defined(BOOST_NO_IOSTREAM)
#if !defined(BOOST_NO_IOSFWD)
#include <iosfwd>               // for std::basic_ostream
#else
#include <ostream>
#endif
#endif

namespace boost { namespace typeind { namespace detail {

template <class T> class cvr_saver{};

template <class TypeInfo>
class type_index_base {
    const TypeInfo* data_;

public:
    typedef TypeInfo                    type_info_t;
    typedef type_index_base<TypeInfo>   this_type;

    type_index_base(const TypeInfo& info) BOOST_NOEXCEPT
        : data_(&info)
    {}

    const type_info_t& type_info() const BOOST_NOEXCEPT {
        return *data_;
    }

    // All of the following methods must be explicitly specialized for each type_index_base.
    const char* raw_name() const BOOST_NOEXCEPT;
    const char* name() const BOOST_NOEXCEPT;
    std::string pretty_name() const;
    std::size_t hash_code() const BOOST_NOEXCEPT;
    bool equal(const this_type& rhs) const BOOST_NOEXCEPT;
    bool before(const this_type& rhs) const BOOST_NOEXCEPT;

    template <class T>
    static this_type construct() BOOST_NOEXCEPT;
    template <class T>
    static this_type construct_with_cvr() BOOST_NOEXCEPT;
    template <class T>
    static this_type construct_runtime(T&) BOOST_NOEXCEPT;
    template <class T>
    static this_type construct_runtime(T* rtti_val);
};


template <class TypeInfo>
inline bool operator == (const type_index_base<TypeInfo>& lhs, const type_index_base<TypeInfo>& rhs) BOOST_NOEXCEPT {
    return lhs.equal(rhs);
}

template <class TypeInfo>
inline bool operator < (const type_index_base<TypeInfo>& lhs, const type_index_base<TypeInfo>& rhs) BOOST_NOEXCEPT {
    return lhs.before(rhs);
}



template <class TypeInfo>
inline bool operator > (const type_index_base<TypeInfo>& lhs, const type_index_base<TypeInfo>& rhs) BOOST_NOEXCEPT {
    return rhs < lhs;
}

template <class TypeInfo>
inline bool operator <= (const type_index_base<TypeInfo>& lhs, const type_index_base<TypeInfo>& rhs) BOOST_NOEXCEPT {
    return !(lhs > rhs);
}

template <class TypeInfo>
inline bool operator >= (const type_index_base<TypeInfo>& lhs, const type_index_base<TypeInfo>& rhs) BOOST_NOEXCEPT {
    return !(lhs < rhs);
}

template <class TypeInfo>
inline bool operator != (const type_index_base<TypeInfo>& lhs, const type_index_base<TypeInfo>& rhs) BOOST_NOEXCEPT {
    return !(lhs == rhs);
}


#ifndef BOOST_NO_IOSTREAM
#ifdef BOOST_NO_TEMPLATED_IOSTREAMS
/// Ostream operator that will output demangled name
template <class TypeInfo>
inline std::ostream& operator<<(std::ostream& ostr, const type_index_base<TypeInfo>& ind) {
    ostr << ind.pretty_name();
    return ostr;
}
#else
/// Ostream operator that will output demangled name
template <class CharT, class TriatT, class TypeInfo>
inline std::basic_ostream<CharT, TriatT>& operator<<(
    std::basic_ostream<CharT, TriatT>& ostr, 
    const type_index_base<TypeInfo>& ind) 
{
    ostr << ind.pretty_name();
    return ostr;
}
#endif // BOOST_NO_TEMPLATED_IOSTREAMS
#endif // BOOST_NO_IOSTREAM


}}} // namespace boost::typeind::detail

#endif // BOOST_TYPE_INDEX_TYPE_INDEX_BASE_HPP

