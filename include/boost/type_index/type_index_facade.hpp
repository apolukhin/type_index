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
#include <boost/functional/hash_fwd.hpp>
#include <string>
#include <cstring>

#if !defined(BOOST_NO_IOSTREAM)
#if !defined(BOOST_NO_IOSFWD)
#include <iosfwd>               // for std::basic_ostream
#else
#include <ostream>
#endif
#endif

namespace boost { namespace typeind {

/// \class type_index_facade
///
/// This class takes care about the comparison operators, hash functions and 
/// ostream operators. Use this class as a public base class for defining new
/// type_info-conforming classes.
///
/// \b Example:
/// \code
/// class stl_type_index: public type_index_facade<stl_type_index, std::type_info> 
/// {
/// public:
///     typedef std::type_info type_info_t;
/// private:
///     const type_info_t* data_;
///
/// public:
///     stl_type_index(const type_info_t& data) noexcept
///         : data_(&data)
///     {}
/// // ...
/// };
/// \endcode
///
/// \tparam Derived Class derived from type_index_facade.
/// \tparam TypeInfo Class that will be used as a base type_info class.
template <class Derived, class TypeInfo>
class type_index_facade {
private:
    /// @cond
    const Derived & derived() const BOOST_NOEXCEPT {
      return *static_cast<Derived const*>(this);
    }
    /// @endcond
public:
    typedef TypeInfo                                type_info_t;

    /// \b Override: This function \b must be redefined in Derived class. Overrides \b must not throw.
    /// \return Const reference to underlying low level type_info_t.
    inline const type_info_t& type_info() const BOOST_NOEXCEPT {
        return derived().type_info();
    }

    /// \b Override: This function \b must be redefined in Derived class. Overrides \b must not throw.
    /// \return Pointer to unredable/raw type name.
    inline const char* raw_name() const BOOST_NOEXCEPT {
        return derived().raw_name();
    }

    /// \b Override: This function \b must be redefined in Derived class. Overrides may throw.
    /// \return Human redable type name.
    inline std::string pretty_name() const {
        return derived().pretty_name();
    }

    /// \b Override: This function \b may be redefined in Derived class. Overrides \b must not throw.
    /// \return Name of a type. By default retuns raw_name().
    inline const char* name() const BOOST_NOEXCEPT {
        return raw_name();
    }

    /// \b Override: This function \b may be redefined in Derived class. Overrides \b must not throw.
    /// \return True if two types are equal. By default compares types by raw_name().
    inline bool equal(const Derived& rhs) const BOOST_NOEXCEPT {
        return raw_name() == rhs.raw_name() || !std::strcmp(raw_name(), rhs.raw_name());
    }

    /// \b Override: This function \b may be redefined in Derived class. Overrides \b must not throw.
    /// \return True if rhs is greater than this. By default compares types by raw_name().
    inline bool before(const Derived& rhs) const BOOST_NOEXCEPT {
        return raw_name() != rhs.raw_name() && std::strcmp(raw_name(), rhs.raw_name()) < 0;
    }

    /// \b Override: This function \b may be redefined in Derived class. Overrides \b must not throw.
    /// \return Hash code of a type. By default hashes types by raw_name().
    inline std::size_t hash_code() const BOOST_NOEXCEPT {
        return boost::hash_range(raw_name(), raw_name() + std::strlen(raw_name()));
    }
};

/// @cond
template <class Derived, class TypeInfo>
inline bool operator == (const type_index_facade<Derived, TypeInfo>& lhs, const type_index_facade<Derived, TypeInfo>& rhs) BOOST_NOEXCEPT {
    return static_cast<Derived const&>(lhs).equal(static_cast<Derived const&>(rhs));
}

template <class Derived, class TypeInfo>
inline bool operator < (const type_index_facade<Derived, TypeInfo>& lhs, const type_index_facade<Derived, TypeInfo>& rhs) BOOST_NOEXCEPT {
    return static_cast<Derived const&>(lhs).before(static_cast<Derived const&>(rhs));;
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

/// @endcond

#if defined(BOOST_TYPE_INDEX_DOXYGEN_INVOKED)

/// noexcept comparison operators for type_index_facade classes.
bool operator ? (const type_index_facade& lhs, const type_index_facade& rhs) noexcept;

/// noexcept comparison operators for type_index_facade and it's TypeInfo classes.
bool operator ? (const type_index_facade& lhs, const TypeInfo& rhs) noexcept;

/// noexcept comparison operators for type_index_facade's TypeInfo and type_index_facade classes.
bool operator ? (const TypeInfo& lhs, const type_index_facade& rhs) noexcept;

#endif

#ifndef BOOST_NO_IOSTREAM
#ifdef BOOST_NO_TEMPLATED_IOSTREAMS
/// @cond
/// Ostream operator that will output demangled name
template <class Derived, class TypeInfo>
inline std::ostream& operator<<(std::ostream& ostr, const type_index_facade<Derived, TypeInfo>& ind) {
    ostr << ind.pretty_name();
    return ostr;
}
/// @endcond
#else
/// Ostream operator that will output demangled name.
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

/// This free function is used by Boost's unordered containers.
template <class Derived, class TypeInfo>
inline std::size_t hash_value(const type_index_facade<Derived, TypeInfo>& lhs) BOOST_NOEXCEPT {
    return static_cast<Derived const&>(lhs).hash_code();
}

}} // namespace boost::typeind

#endif // BOOST_TYPE_INDEX_TYPE_INDEX_FACADE_HPP

