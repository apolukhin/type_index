//
// Copyright (c) Antony Polukhin, 2012-2013.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_TYPE_INDEX_HPP
#define BOOST_TYPE_INDEX_TYPE_INDEX_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/// \file type_index.hpp
/// \brief Contains implementation of boost::type_index class.
///
/// boost::type_index class is used in situations when RTTI is enabled.

#if  !defined(BOOST_NO_RTTI) && !defined(BOOST_TYPE_INDEX_FORCE_NORTTI_COMPATIBILITY)

#include <boost/type_index/type_info.hpp>

#if !defined(BOOST_NO_IOSTREAM)
#if !defined(BOOST_NO_IOSFWD)
#include <iosfwd>               // for std::basic_ostream
#else
#include <ostream>
#endif
#endif

namespace boost {

/// Copyable type_index class that requires RTTI.
class type_index {
public:
    typedef boost::type_info   stl_type_info;

private:
    const stl_type_info* pinfo_;

public:
    /// Default constructor.
    type_index() BOOST_NOEXCEPT
        : pinfo_(&typeid(void))
    {}

    /// Constructs type_index from an instance of std::type_info.
    explicit type_index(const stl_type_info& inf) BOOST_NOEXCEPT
        : pinfo_(&inf)
    {}

    /// Returns true if the type precedes the type of rhs in the collation order.
    /// The collation order is just an internal order.
    bool before(type_index const& rhs) const BOOST_NOEXCEPT {
        return pinfo_->before(rhs);
    }

    /// Returns raw name
    const char* name() const BOOST_NOEXCEPT {
        return pinfo_->name();
    }

    /// Returns user-friendly name
    std::string name_demangled() const {
        return pinfo_->name_demangled();
    }

#ifndef BOOST_TYPE_INDEX_DOXYGEN_INVOKED
    bool operator == (type_index const& rhs) const BOOST_NOEXCEPT {
        #ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
            return !std::strcmp(pinfo_->name(), rhs.pinfo_->name());
        #else
            return *pinfo_ == *rhs.pinfo_;
        #endif
    }

    bool operator != (type_index const& rhs) const BOOST_NOEXCEPT {
        return !(*this == rhs);
    }

    bool operator < (type_index const& rhs) const BOOST_NOEXCEPT {
        #ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
            return std::strcmp(pinfo_->name(), rhs.pinfo_->name()) < 0;
        #else
            return before(rhs);
        #endif
    }

    bool operator > (type_index const& rhs) const BOOST_NOEXCEPT {
        return (rhs < *this);
    }

    bool operator <= (type_index const& rhs) const BOOST_NOEXCEPT {
        return !(*this > rhs);
    }

    bool operator >= (type_index const& rhs) const BOOST_NOEXCEPT {
        return !(*this < rhs);
    }

    bool operator == (stl_type_info const& rhs) const BOOST_NOEXCEPT {
        #ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
            return !std::strcmp(pinfo_->name(), rhs.name());
        #else
            return *pinfo_ == rhs;
        #endif
    }

    bool operator != (stl_type_info const& rhs) const BOOST_NOEXCEPT {
        return !(*this == rhs);
    }

    bool operator < (stl_type_info const& rhs) const BOOST_NOEXCEPT {
        #ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
            return std::strcmp(pinfo_->name(), rhs.name()) < 0;
        #else
            return !!pinfo_->before(rhs);
        #endif
    }

    bool operator > (stl_type_info const& rhs) const BOOST_NOEXCEPT {
        #ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
            return std::strcmp(pinfo_->name(), rhs.name()) > 0;
        #else
            return !!rhs.before(*pinfo_);
        #endif
    }

    bool operator <= (stl_type_info const& rhs) const BOOST_NOEXCEPT {
        return !(*this > rhs);
    }

    bool operator >= (stl_type_info const& rhs) const BOOST_NOEXCEPT {
        return !(*this < rhs);
    }
#endif // BOOST_TYPE_INDEX_DOXYGEN_INVOKED

    /// Function for getting hash value
    std::size_t hash_code() const BOOST_NOEXCEPT {
#if _MSC_VER >= 1600 || (__GNUC__ == 4 && __GNUC_MINOR__ > 5 && defined(__GXX_EXPERIMENTAL_CXX0X__))
        return pinfo_->hash_code();
#else 
        return boost::hash_range(name(), name() + std::strlen(name()));
#endif 
    }
};

#ifndef BOOST_TYPE_INDEX_DOXYGEN_INVOKED

/* *************** type_index free functions ******************* */

inline bool operator == (type_index::stl_type_info const& lhs, type_index const& rhs) BOOST_NOEXCEPT {
    return rhs == lhs; // Operation is commutative
}

inline bool operator != (type_index::stl_type_info const& lhs, type_index const& rhs) BOOST_NOEXCEPT {
    return rhs != lhs; // Operation is commutative
}

inline bool operator < (type_index::stl_type_info const& lhs, type_index const& rhs) BOOST_NOEXCEPT {
    return rhs > lhs;
}

inline bool operator > (type_index::stl_type_info const& lhs, type_index const& rhs) BOOST_NOEXCEPT {
    return rhs < lhs;
}

inline bool operator <= (type_index::stl_type_info const& lhs, type_index const& rhs) BOOST_NOEXCEPT {
    return rhs >= lhs;
}

inline bool operator >= (type_index::stl_type_info const& lhs, type_index const& rhs) BOOST_NOEXCEPT {
    return rhs <= lhs;
}

#endif // BOOST_TYPE_INDEX_DOXYGEN_INVOKED

#ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
#undef BOOST_CLASSINFO_COMPARE_BY_NAMES
#endif

/// @endcond

/* *************** type_index free functions ******************* */

/// @cond

#ifndef BOOST_NO_IOSTREAM
#ifdef BOOST_NO_TEMPLATED_IOSTREAMS

/// Ostream operator that will output demangled name.
inline std::ostream& operator<<(std::ostream& ostr, type_index const& ind) {
    ostr << ind.name_demangled();
    return ostr;
}

#else

/// Ostream operator that will output demangled name.
template <class CharT, class TriatT>
inline std::basic_ostream<CharT, TriatT>& operator<<(std::basic_ostream<CharT, TriatT>& ostr, type_index const& ind) {
    ostr << ind.name_demangled();
    return ostr;
}

#endif // BOOST_NO_TEMPLATED_IOSTREAMS
#endif // BOOST_NO_IOSTREAM

/// hash_value function overload for type_index.
inline std::size_t hash_value(type_index const& v) BOOST_NOEXCEPT {
    return v.hash_code();
}

/// @endcond

} // namespace boost

#endif // !defined(BOOST_NO_RTTI) && !defined(BOOST_TYPE_INDEX_FORCE_NORTTI_COMPATIBILITY)

#endif // BOOST_TYPE_INDEX_TYPE_INDEX_HPP

