//
// Copyright (c) Antony Polukhin, 2013-2014.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_TYPE_INDEX_FACADE_HPP
#define BOOST_TYPE_INDEX_TYPE_INDEX_FACADE_HPP

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

namespace boost { namespace typeind {

/// type_index_facade - use as a public base class for defining new
/// standard-conforming iterators.
///
template <class Derived, class TypeInfo>
class type_index_facade {
private:
    Derived& derived() BOOST_NOEXCEPT {
      return *static_cast<Derived*>(this);
    }

    const Derived & derived() const BOOST_NOEXCEPT {
      return *static_cast<Derived const*>(this);
    }

public:
    typedef TypeInfo                                type_info_t;
    typedef type_index_facade<Derived, TypeInfo>    this_type;

    const type_info_t& type_info() const BOOST_NOEXCEPT {
        return derived().type_info();
    }

    inline const char* raw_name() const BOOST_NOEXCEPT {
        return derived().raw_name();
    }

    inline const char* name() const BOOST_NOEXCEPT {
        return derived().name();
    }

    inline std::string pretty_name() const {
        return derived().pretty_name();
    }

    inline std::size_t hash_code() const BOOST_NOEXCEPT {
        return derived().hash_code();
    }

    inline bool equal(const this_type& rhs) const BOOST_NOEXCEPT {
        return derived().equal(rhs.derived());
    }

    inline bool before(const this_type& rhs) const BOOST_NOEXCEPT {
        return derived().before(rhs.derived());
    }
};


template <class Derived, class TypeInfo>
inline bool operator == (const type_index_facade<Derived, TypeInfo>& lhs, const type_index_facade<Derived, TypeInfo>& rhs) BOOST_NOEXCEPT {
    return lhs.equal(rhs);
}

template <class Derived, class TypeInfo>
inline bool operator < (const type_index_facade<Derived, TypeInfo>& lhs, const type_index_facade<Derived, TypeInfo>& rhs) BOOST_NOEXCEPT {
    return lhs.before(rhs);
}



template <class Derived, class TypeInfo>
inline bool operator > (const type_index_facade<Derived, TypeInfo>& lhs, const type_index_facade<Derived, TypeInfo>& rhs) BOOST_NOEXCEPT {
    return rhs < lhs;
}

template <class Derived, class TypeInfo>
inline bool operator <= (const type_index_facade<Derived, TypeInfo>& lhs, const type_index_facade<Derived, TypeInfo>& rhs) BOOST_NOEXCEPT {
    return !(lhs > rhs);
}

template <class Derived, class TypeInfo>
inline bool operator >= (const type_index_facade<Derived, TypeInfo>& lhs, const type_index_facade<Derived, TypeInfo>& rhs) BOOST_NOEXCEPT {
    return !(lhs < rhs);
}

template <class Derived, class TypeInfo>
inline bool operator != (const type_index_facade<Derived, TypeInfo>& lhs, const type_index_facade<Derived, TypeInfo>& rhs) BOOST_NOEXCEPT {
    return !(lhs == rhs);
}

// ######################### COMPARISONS with Derived ############################ //
template <class Derived, class TypeInfo>
inline bool operator == (const TypeInfo& lhs, const type_index_facade<Derived, TypeInfo>& rhs) BOOST_NOEXCEPT {
    return Derived(lhs) == rhs;
}

template <class Derived, class TypeInfo>
inline bool operator < (const TypeInfo& lhs, const type_index_facade<Derived, TypeInfo>& rhs) BOOST_NOEXCEPT {
    return Derived(lhs) < rhs;
}

template <class Derived, class TypeInfo>
inline bool operator > (const TypeInfo& lhs, const type_index_facade<Derived, TypeInfo>& rhs) BOOST_NOEXCEPT {
    return rhs < Derived(lhs);
}

template <class Derived, class TypeInfo>
inline bool operator <= (const TypeInfo& lhs, const type_index_facade<Derived, TypeInfo>& rhs) BOOST_NOEXCEPT {
    return !(Derived(lhs) > rhs);
}

template <class Derived, class TypeInfo>
inline bool operator >= (const TypeInfo& lhs, const type_index_facade<Derived, TypeInfo>& rhs) BOOST_NOEXCEPT {
    return !(Derived(lhs) < rhs);
}

template <class Derived, class TypeInfo>
inline bool operator != (const TypeInfo& lhs, const type_index_facade<Derived, TypeInfo>& rhs) BOOST_NOEXCEPT {
    return !(Derived(lhs) == rhs);
}


template <class Derived, class TypeInfo>
inline bool operator == (const type_index_facade<Derived, TypeInfo>& lhs, const TypeInfo& rhs) BOOST_NOEXCEPT {
    return lhs == Derived(rhs);
}

template <class Derived, class TypeInfo>
inline bool operator < (const type_index_facade<Derived, TypeInfo>& lhs, const TypeInfo& rhs) BOOST_NOEXCEPT {
    return lhs < Derived(rhs);
}

template <class Derived, class TypeInfo>
inline bool operator > (const type_index_facade<Derived, TypeInfo>& lhs, const TypeInfo& rhs) BOOST_NOEXCEPT {
    return Derived(rhs) < lhs;
}

template <class Derived, class TypeInfo>
inline bool operator <= (const type_index_facade<Derived, TypeInfo>& lhs, const TypeInfo& rhs) BOOST_NOEXCEPT {
    return !(lhs > Derived(rhs));
}

template <class Derived, class TypeInfo>
inline bool operator >= (const type_index_facade<Derived, TypeInfo>& lhs, const TypeInfo& rhs) BOOST_NOEXCEPT {
    return !(lhs < Derived(rhs));
}

template <class Derived, class TypeInfo>
inline bool operator != (const type_index_facade<Derived, TypeInfo>& lhs, const TypeInfo& rhs) BOOST_NOEXCEPT {
    return !(lhs == Derived(rhs));
}

// ######################### COMPARISONS with Derived END ############################ //


#ifndef BOOST_NO_IOSTREAM
#ifdef BOOST_NO_TEMPLATED_IOSTREAMS
/// Ostream operator that will output demangled name
template <class Derived, class TypeInfo>
inline std::ostream& operator<<(std::ostream& ostr, const type_index_facade<Derived, TypeInfo>& ind) {
    ostr << ind.pretty_name();
    return ostr;
}
#else
/// Ostream operator that will output demangled name
template <class CharT, class TriatT, class Derived, class TypeInfo>
inline std::basic_ostream<CharT, TriatT>& operator<<(
    std::basic_ostream<CharT, TriatT>& ostr, 
    const type_index_facade<Derived, TypeInfo>& ind) 
{
    ostr << ind.pretty_name();
    return ostr;
}
#endif // BOOST_NO_TEMPLATED_IOSTREAMS
#endif // BOOST_NO_IOSTREAM

template <class Derived, class TypeInfo>
inline std::size_t hash_value(const type_index_facade<Derived, TypeInfo>& lhs) BOOST_NOEXCEPT {
    return lhs.hash_code();
}

}} // namespace boost::typeind

#endif // BOOST_TYPE_INDEX_TYPE_INDEX_FACADE_HPP

