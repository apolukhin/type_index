//
// Copyright (c) Antony Polukhin, 2012-2013.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_TYPE_INDEX_IMPL_HPP
#define BOOST_TYPE_INDEX_TYPE_INDEX_IMPL_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <cstring>
#include <string>
#include <boost/config.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/current_function.hpp>
#include <boost/functional/hash_fwd.hpp>

#ifndef BOOST_NO_RTTI
#include <typeinfo>
#endif

#if !defined(BOOST_NO_IOSTREAM)
#if !defined(BOOST_NO_IOSFWD)
#include <iosfwd>               // for std::basic_ostream
#else
#include <ostream>
#endif
#endif

#ifdef __GNUC__
#include <cxxabi.h>
#endif

namespace boost {

#ifndef BOOST_NO_RTTI

#ifndef BOOST_TYPE_INDEX_DOXYGEN_INVOKED

// for this compiler at least, cross-shared-library type_info
// comparisons don't work, so use typeid(x).name() instead. It's not
// yet clear what the best default strategy is.
# if (defined(__GNUC__) && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 5))) \
    || defined(_AIX) \
    || (defined(__sgi) && defined(__host_mips)) \
    || (defined(__hpux) && defined(__HP_aCC)) \
    || (defined(linux) && defined(__INTEL_COMPILER) && defined(__ICC))
#  define BOOST_CLASSINFO_COMPARE_BY_NAMES
# endif

#endif // BOOST_TYPE_INDEX_DOXYGEN_INVOKED

/// Copyable type_info class that requires RTTI.
class type_index {
public:

#ifdef BOOST_NO_STD_TYPEINFO
    typedef type_info   stl_type_info;
#else
    typedef std::type_info   stl_type_info;
#endif

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

    /// Factory method for constructing type_index instance for type T.
    /// Strips const, volatile and & modifiers from T.
    template <class T>
    static type_index construct() BOOST_NOEXCEPT {
        typedef BOOST_DEDUCED_TYPENAME boost::remove_reference<T>::type no_ref_t;
        typedef BOOST_DEDUCED_TYPENAME boost::remove_cv<no_ref_t>::type no_cvr_t;

        #  if (defined(__EDG_VERSION__) && __EDG_VERSION__ < 245) \
            || (defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 744)
                BOOST_STATIC_ASSERT_MSG( !boost::is_arithmetic<no_cvr_t>::type::value
                    , "Your EDG-based compiler seems to mistakenly distinguish 'int' from 'signed int', in typeid() expressions.");
        #endif

        return type_index(typeid(no_cvr_t));
    }

    /// Factory function, that works exactly like C++ typeid(rtti_val) call, but returns boost::type_index.
    /// This method available only with RTTI enabled.
    template <class T>
    static type_index construct_rtti_only(T& rtti_val) {
        return type_index(typeid(rtti_val));
    }

    /// Factory function, that works exactly like C++ typeid(rtti_val) call, but returns boost::type_index.
    /// This method available only with RTTI enabled.
    template <class T>
    static type_index construct_rtti_only(T* rtti_val) {
        return type_index(typeid(rtti_val));
    }

    /// Returns true if the type precedes the type of rhs in the collation order.
    /// The collation order is just an internal order.
    bool before(type_index const& rhs) const BOOST_NOEXCEPT {
        return !!pinfo_->before(*rhs.pinfo_);
    }

    /// Returns raw name
    const char* name() const BOOST_NOEXCEPT {
    #ifdef _MSC_VER
        return pinfo_->raw_name();
    #else
        return pinfo_->name();
    #endif
    }

    /// Returns user-friendly name
    std::string name_demangled() const {
        #if defined(__GNUC__)
            int status = 0 ;
            char* demang = abi::__cxa_demangle(pinfo_->name(), NULL, 0, &status);
            BOOST_ASSERT(!status);
            std::string ret(demang);
            free(demang);
            return ret;
        #else
            return pinfo_->name();
        #endif
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

/// Function, to get type_index for a type T. Strips const, volatile and & modifiers from T.
template <class T>
type_index type_id() BOOST_NOEXCEPT {
    return type_index::construct<T>();
}

/// Function, that works exactly like C++ typeid(rtti_val) call, but returns boost::type_index.
/// This method available only with RTTI enabled.
template <class T>
type_index type_id_rtti_only(T& rtti_val) BOOST_NOEXCEPT {
    return type_index::construct_rtti_only(rtti_val);
}

/// Function, that works exactly like C++ typeid(rtti_val) call, but returns boost::type_index.
/// This method available only with RTTI enabled.
template <class T>
type_index type_id_rtti_only(T* rtti_val) {
    return type_index::construct_rtti_only(rtti_val);
}

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

#endif // BOOST_NO_RTTI

} // namespace boost

#endif // BOOST_TYPE_INDEX_TYPE_INDEX_IMPL_HPP

