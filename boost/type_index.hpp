//
// Copyright (c) Antony Polukhin, 2012.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_HPP
#define BOOST_TYPE_INDEX_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
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

namespace detail {
#ifdef _MSC_VER
    // sizeof("const char *__cdecl boost::detail::ctti<") - 1
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_begin = 40);

    // sizeof(">::n(void)") - 1
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_end = 10);

#elif defined __clang__
    // sizeof("static const char *boost::detail::ctti<") - 1
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_begin = 39);

    // == sizeof(">::n()") - 1
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_end = 6);
#elif defined __GNUC__
    // sizeof("static const char* boost::detail::ctti<T>::n() [with T = ") - 1
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_begin = 57);

    // == sizeof("]") - 1
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_end = 1);

#else
    // TODO: Code for other platforms
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_begin = 0);   // skip nothing
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_end = 0);     // skip nothing
#endif

    /// Noncopyable type_info that does not require RTTI
    /// CTTI == Compile Time Type Info
    /// This name must be as short as posible, to avoid code bloat
    template <class T>
    struct ctti {
        typedef T          template_type;
        typedef ctti       this_type;

        /// Returns raw name. Must be as short, as possible, to avoid code bloat
        static const char* n() BOOST_NOEXCEPT {
            return BOOST_CURRENT_FUNCTION + detail::ctti_skip_size_at_begin;
        }

        /// Returns raw name
        static const char* name() BOOST_NOEXCEPT {
            return this_type::n();
        }

        /// Returns true if the type precedes the type of rhs in the collation order.
        /// The collation order is just an internal order.
        template <class BefT>
        static bool before() BOOST_NOEXCEPT {
            return std::strcmp(this_type::n(), ctti<BefT>::n()) < 0;
        }

        /// Returns length of demangled name
        static std::size_t name_length() BOOST_NOEXCEPT {
            return std::strlen(this_type::n()) - detail::ctti_skip_size_at_end;
        }

        /// Returns user-friendly name
        static std::string name_demangled() BOOST_NOEXCEPT {
            return std::string(this_type::n(), this_type::name_length());
        }
    };
} // namespace detail

/// Copyable type_info that does not require RTTI and could store const,
/// volatile and references if constructed via construct_with_cvr()
class template_index {
private:
    const char* name_;

    /// @cond
    explicit template_index(const char* name) BOOST_NOEXCEPT
        : name_(name)
    {}
    /// @endcond

public:
    /// Default constructor.
    template_index() BOOST_NOEXCEPT
        : name_(detail::ctti<void>::name())
    {}

    /// Factory method for constructing template_index instance for type T.
    /// Strips const, volatile and & modifiers from T
    template <class T>
    static template_index construct(){
        typedef BOOST_DEDUCED_TYPENAME boost::remove_reference<T>::type no_ref_t;
        typedef BOOST_DEDUCED_TYPENAME boost::remove_cv<no_ref_t>::type no_cvr_t;

        #  if (defined(__EDG_VERSION__) && __EDG_VERSION__ < 245) \
            || (defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 744)
                BOOST_STATIC_ASSERT_MSG( !boost::is_arithmetic<no_cvr_t>::type::value
                    , "Your EDG-based compiler seems to mistakenly distinguish 'int' from 'signed int', in typeid() expressions.");
        #endif

        return template_index(detail::ctti<no_cvr_t>::name());
    }

    /// Factory method for constructing template_index instance for type T.
    /// Does not strips const, volatile and & modifiers from T
    template <class T>
    static template_index construct_with_cvr() {
        #  if (defined(__EDG_VERSION__) && __EDG_VERSION__ < 245) \
            || (defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 744)
                BOOST_STATIC_ASSERT_MSG( !boost::is_arithmetic<T>::type::value
                    , "Your EDG-based compiler seems to mistakenly distinguish 'int' from 'signed int', in typeid() expressions.");
        #endif

        return template_index(detail::ctti<T>::name());
    }


    /// Returns true if the type precedes the type of rhs in the collation order.
    /// The collation order is just an internal order.
    bool before(const template_index& rhs) const BOOST_NOEXCEPT {
        return std::strcmp(name(), rhs.name()) < 0;
    }

    /// Returns raw name
    const char* name() const BOOST_NOEXCEPT {
        return name_;
    }

    /// Returns user-friendly name
    std::string name_demangled() const {
        return std::string(name_, std::strlen(name_) - detail::ctti_skip_size_at_end);
    }

    /// @cond
    bool operator == (const template_index& rhs) const BOOST_NOEXCEPT {
        return !std::strcmp(name_, rhs.name());
    }

    bool operator != (const template_index& rhs) const BOOST_NOEXCEPT {
        return !!std::strcmp(name_, rhs.name());
    }

    bool operator < (const template_index& rhs) const BOOST_NOEXCEPT {
        return std::strcmp(name_, rhs.name()) < 0;
    }

    bool operator > (const template_index& rhs) const BOOST_NOEXCEPT {
        return std::strcmp(name_, rhs.name()) > 0;
    }

    bool operator <= (const template_index& rhs) const BOOST_NOEXCEPT {
        return std::strcmp(name_, rhs.name()) <= 0;
    }

    bool operator >= (const template_index& rhs) const BOOST_NOEXCEPT {
        return std::strcmp(name_, rhs.name()) >= 0;
    }
    /// @endcond

    /// Function for getting hash value
    std::size_t hash_code() const BOOST_NOEXCEPT {
        return boost::hash_range(name_, name_ + std::strlen(name_));
    }
};

/// Method for constructing template_index instance for type T.
/// Strips const, volatile and & modifiers from T.
template <class T>
template_index template_id() BOOST_NOEXCEPT {
    return template_index::construct<T>();
}

/// Factory method for constructing template_index instance for type T.
/// Does not strips const, volatile and & modifiers from T.
template <class T>
template_index template_id_with_cvr() BOOST_NOEXCEPT {
    return template_index::construct_with_cvr<T>();
}

#ifndef BOOST_NO_RTTI


#ifndef BOOST_TYPE_INDEX_DOXYGEN_INVOKED

// for this compiler at least, cross-shared-library type_info
// comparisons don't work, so use typeid(x).name() instead. It's not
// yet clear what the best default strategy is.
# if (defined(__GNUC__) && __GNUC__ >= 3 && __GNUC_MINOR__ < 5) \
    || defined(_AIX) \
    || (defined(__sgi) && defined(__host_mips)) \
    || (defined(__hpux) && defined(__HP_aCC)) \
    || (defined(linux) && defined(__INTEL_COMPILER) && defined(__ICC))
#  define BOOST_CLASSINFO_COMPARE_BY_NAMES
#  define BOOST_CLASSINFO_COMPARISON_NOEXCEPT
#else
#  define BOOST_CLASSINFO_COMPARISON_NOEXCEPT BOOST_NOEXCEPT
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

    /// @cond
    explicit type_index(const stl_type_info& inf) BOOST_NOEXCEPT
        : pinfo_(&inf)
    {}
    /// @endcond

public:
    /// Default constructor.
    type_index() 
        : pinfo_(&typeid(void))
    {}

    /// Factory method for constructing type_index instance for type T.
    /// Strips const, volatile and & modifiers from T.
    template <class T>
    static type_index construct() {
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
    const char* name() const {
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
    bool operator == (type_index const& rhs) const BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
        #ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
            return !std::strcmp(pinfo_->name(), rhs.pinfo_->name());
        #else
            return *pinfo_ == *rhs.pinfo_;
        #endif
    }

    bool operator != (type_index const& rhs) const BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
        return !(*this == rhs);
    }

    bool operator < (type_index const& rhs) const BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
        #ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
            return std::strcmp(pinfo_->name(), rhs.pinfo_->name()) < 0;
        #else
            return before(rhs);
        #endif
    }

    bool operator > (type_index const& rhs) const BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
        return (rhs < *this);
    }

    bool operator <= (type_index const& rhs) const BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
        return !(*this > rhs);
    }

    bool operator >= (type_index const& rhs) const BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
        return !(*this < rhs);
    }

    bool operator == (stl_type_info const& rhs) const BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
        #ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
            return !std::strcmp(pinfo_->name(), rhs.name());
        #else
            return *pinfo_ == rhs;
        #endif
    }

    bool operator != (stl_type_info const& rhs) const BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
        return !(*this == rhs);
    }

    bool operator < (stl_type_info const& rhs) const BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
        #ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
            return std::strcmp(pinfo_->name(), rhs.name()) < 0;
        #else
            return !!pinfo_->before(rhs);
        #endif
    }

    bool operator > (stl_type_info const& rhs) const BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
        #ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
            return std::strcmp(pinfo_->name(), rhs.name()) > 0;
        #else
            return !!rhs.before(*pinfo_);
        #endif
    }

    bool operator <= (stl_type_info const& rhs) const BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
        return !(*this > rhs);
    }

    bool operator >= (stl_type_info const& rhs) const BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
        return !(*this < rhs);
    }
#endif // BOOST_TYPE_INDEX_DOXYGEN_INVOKED

    /// Function for getting hash value
    std::size_t hash_code() const BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
#if _MSC_VER >= 1600 || (__GNUC__ == 4 && __GNUC_MINOR__ > 5 && defined(__GXX_EXPERIMENTAL_CXX0X__))
        return pinfo_->hash_code();
#else 
        return boost::hash_range(name(), name() + std::strlen(name()));
#endif 
    }
};

#ifndef BOOST_TYPE_INDEX_DOXYGEN_INVOKED

inline bool operator == (type_index::stl_type_info const& lhs, type_index const& rhs) BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
    return rhs == lhs; // Operation is commutative
}

inline bool operator != (type_index::stl_type_info const& lhs, type_index const& rhs) BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
    return rhs != lhs; // Operation is commutative
}

inline bool operator < (type_index::stl_type_info const& lhs, type_index const& rhs) BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
    return rhs > lhs;
}

inline bool operator > (type_index::stl_type_info const& lhs, type_index const& rhs) BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
    return rhs < lhs;
}

inline bool operator <= (type_index::stl_type_info const& lhs, type_index const& rhs) BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
    return rhs >= lhs;
}

inline bool operator >= (type_index::stl_type_info const& lhs, type_index const& rhs) BOOST_CLASSINFO_COMPARISON_NOEXCEPT {
    return rhs <= lhs;
}

#undef BOOST_CLASSINFO_COMPARISON_NOEXCEPT

#endif // BOOST_TYPE_INDEX_DOXYGEN_INVOKED



#ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
#undef BOOST_CLASSINFO_COMPARE_BY_NAMES
#endif

/// @endcond

/// Function, to get type_index for a type T. Strips const, volatile and & modifiers from T.
template <class T>
type_index type_id() {
    return type_index::construct<T>();
}

/// Function, that works exactly like C++ typeid(rtti_val) call, but returns boost::type_index.
/// This method available only with RTTI enabled.
template <class T>
type_index type_id_rtti_only(T& rtti_val) {
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
#endif

#endif

/// hash_value function overload for type_index.
inline std::size_t hash_value(type_index const& v) {
    return v.hash_code();
}

/// @endcond

#else
// BOOST_NO_RTTI is defined

typedef template_index type_index;

template <class T>
type_index type_id() {
    return template_index::construct<T>();
}

#endif


/* *************** template_index free functions ******************* */

/// @cond

#ifndef BOOST_NO_IOSTREAM
#ifdef BOOST_NO_TEMPLATED_IOSTREAMS
/// Ostream operator that will output demangled name
inline std::ostream& operator<<(std::ostream& ostr, template_index const& ind) {
    ostr << ind.name_demangled();
    return ostr;
}
#else
/// Ostream operator that will output demangled name
template <class CharT, class TriatT>
inline std::basic_ostream<CharT, TriatT>& operator<<(std::basic_ostream<CharT, TriatT>& ostr, template_index const& ind) {
    ostr << ind.name_demangled();
    return ostr;
}
#endif
#endif

/// hash_value function overload for template_index
inline std::size_t hash_value(template_index const& v) BOOST_NOEXCEPT {
    return v.hash_code();
}

/// @endcond

#ifdef BOOST_TYPE_INDEX_DOXYGEN_INVOKED

/// Output operators for type_index and template_index
std::basic_ostream& operator<<(std::basic_ostream, template_index/type_index const & rhs);

/// All possible operators for comparison of std::type_info's and type_index'es.
inline bool operator(type_index/std::type_info const& lhs, type_index/std::type_info const& rhs);

/// All possible operators for comparison of template_index'es.
inline bool operator(template_index const& lhs, template_index const& rhs);

/// All possible operators for comparison of type_index'es.
inline bool operator(type_index const& lhs, type_index const& rhs);

#endif // BOOST_TYPE_INDEX_DOXYGEN_INVOKED

} // namespace boost

#endif // BOOST_TYPE_INDEX_HPP

