//
// Copyright (c) Antony Polukhin, 2012-2013.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_TEMPLATE_INFO_IMPL_HPP
#define BOOST_TYPE_INDEX_TEMPLATE_INFO_IMPL_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/// \file template_info_impl.hpp
/// \brief Contains implementation of boost::template_info class.
///
/// boost::template_info class is used instead of boost::type_info in situations when RTTI is disabled.
///
/// Consider including <boost/type_index/type_info.hpp> or <boost/type_index.hpp> instead of this file.

#include <cstring>
#include <string>
#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/functional/hash_fwd.hpp>

namespace boost {

namespace detail {
#if defined(BOOST_TYPE_INDEX_DOXYGEN_INVOKED)

/// \def BOOST_TYPE_INDEX_FUNCTION_SIGNATURE
/// BOOST_TYPE_INDEX_FUNCTION_SIGNATURE is used by boost::template_info class to
/// deduce the name of a template parameter. If your compiler is not recognized 
/// by the TypeIndex library and you wish to work with boost::template_info, you may 
/// define this macro by yourself.
///
/// BOOST_TYPE_INDEX_FUNCTION_SIGNATURE must be defined to a compiler specific macro, 
/// that outputs the WHOLE function signature, including template parameters.
///
/// If your compiler is not recognised and BOOST_TYPE_INDEX_FUNCTION_SIGNATURE is not defined,
/// then a compile-time error will arise at any attempt to use boost::template_info class.
#define BOOST_TYPE_INDEX_FUNCTION_SIGNATURE BOOST_CURRENT_FUNCTION

#elif defined(BOOST_TYPE_INDEX_FUNCTION_SIGNATURE)

template <class T> 
inline void lazy_function_signature_assert() BOOST_NOEXCEPT {}

#elif defined(__FUNCSIG__)

template <class T> 
inline void lazy_function_signature_assert() BOOST_NOEXCEPT {}
#define BOOST_TYPE_INDEX_FUNCTION_SIGNATURE __FUNCSIG__

#elif defined(__PRETTY_FUNCTION__) \
        || defined(__GNUC__) \
        || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) \
        || (defined(__ICC) && (__ICC >= 600)) \
        || defined(__ghs__) \
        || defined(__DMC__)

template <class T> 
inline void lazy_function_signature_assert() BOOST_NOEXCEPT {}
#define BOOST_TYPE_INDEX_FUNCTION_SIGNATURE __PRETTY_FUNCTION__

#else

template <class T> 
inline void lazy_function_signature_assert() BOOST_NOEXCEPT {
    BOOST_STATIC_ASSERT_MSG(
        sizeof(T) && false,
        "TypeIndex library could not detect your compiler. "
        "Please make the BOOST_TYPE_INDEX_FUNCTION_SIGNATURE macro use "
        "correct compiler macro for getting the whole function name. "
        "Do not forget to also define BOOST_TYPE_INDEX_CTTI_BEGIN_SKIP and "
        "BOOST_TYPE_INDEX_CTTI_END_SKIP."
    );
}

#endif

#if defined(BOOST_TYPE_INDEX_CTTI_BEGIN_SKIP) && defined(BOOST_TYPE_INDEX_CTTI_END_SKIP)
    // skip user specified bytes count
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_begin = BOOST_TYPE_INDEX_CTTI_BEGIN_SKIP);
    // skip user specified bytes count
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_end = BOOST_TYPE_INDEX_CTTI_END_SKIP);
#elif defined _MSC_VER
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
            lazy_function_signature_assert<T>();
            return BOOST_TYPE_INDEX_FUNCTION_SIGNATURE + detail::ctti_skip_size_at_begin;
        }

        /// Returns raw name
        static const char* name() BOOST_NOEXCEPT {
            return this_type::n();
        }
    };
} // namespace detail

/// Copyable type_info that does not require RTTI and could store const,
/// volatile and references if constructed via construct_with_cvr()
class template_info: boost::noncopyable {
private:
    const char* name_;

    /// @cond
    explicit template_info(const char* name) BOOST_NOEXCEPT
        : name_(name)
    {}
    /// @endcond

public:
    /// Factory method for constructing template_index instance for type T.
    /// Strips const, volatile and & modifiers from T
    template <class T>
    static const template_info& construct() BOOST_NOEXCEPT {
        typedef BOOST_DEDUCED_TYPENAME boost::remove_reference<T>::type no_ref_t;
        typedef BOOST_DEDUCED_TYPENAME boost::remove_cv<no_ref_t>::type no_cvr_t;

        #  if (defined(__EDG_VERSION__) && __EDG_VERSION__ < 245) \
            || (defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 744)
                BOOST_STATIC_ASSERT_MSG( !boost::is_arithmetic<no_cvr_t>::type::value
                    , "Your EDG-based compiler seems to mistakenly distinguish 'int' from 'signed int', in typeid() expressions.");
        #endif

        static const template_index ret(detail::ctti<no_cvr_t>::name())
        return ret;
    }

    /// Factory method for constructing template_index instance for type T.
    /// Does not strip const, volatile and & modifiers from T
    template <class T>
    static const template_info& construct_with_cvr() BOOST_NOEXCEPT {
        #  if (defined(__EDG_VERSION__) && __EDG_VERSION__ < 245) \
            || (defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 744)
                BOOST_STATIC_ASSERT_MSG( !boost::is_arithmetic<T>::type::value
                    , "Your EDG-based compiler seems to mistakenly distinguish 'int' from 'signed int', in typeid() expressions.");
        #endif

        static const template_index ret(detail::ctti<T>::name())
        return ret;
    }


    /// Returns true if the type precedes the type of rhs in the collation order.
    /// The collation order is just an internal order.
    bool before(const template_info& rhs) const BOOST_NOEXCEPT {
        return std::strcmp(name(), rhs.name()) < 0;
    }

    /// Returns raw name
    const char* name() const BOOST_NOEXCEPT {
        return name_;
    }

    /// Returns user-friendly name
    std::string name_demangled() const {
        return std::string(name_, std::strlen(name_ + detail::ctti_skip_size_at_end));
    }

    bool operator == (const template_info& rhs) const BOOST_NOEXCEPT {
        return !std::strcmp(name_, rhs.name());
    }

    bool operator != (const template_info& rhs) const BOOST_NOEXCEPT {
        return !!std::strcmp(name_, rhs.name());
    }
    
    /// Function for getting hash value
    std::size_t hash_code() const BOOST_NOEXCEPT {
        return boost::hash_range(name_, name_ + std::strlen(name_ + detail::ctti_skip_size_at_end));
    }
};

/// Method for constructing template_index instance for type T.
/// Strips const, volatile and & modifiers from T.
template <class T>
inline const template_info& template_id() BOOST_NOEXCEPT {
    return template_index::construct<T>();
}

/// Factory method for constructing template_index instance for type T.
/// Does not strip const, volatile and & modifiers from T.
/// If T has no const, volatile, & and && modifiers, then returns exactly 
/// the same result as in case of calling `template_id<T>()`.
template <class T>
inline const template_info& template_id_with_cvr() BOOST_NOEXCEPT {
    return template_index::construct_with_cvr<T>();
}

/* *************** template_info free functions ******************* */
/// hash_value function overload for template_index
inline std::size_t hash_value(template_info const& v) BOOST_NOEXCEPT {
    return v.hash_code();
}

} // namespace boost

#endif // BOOST_TYPE_INDEX_TEMPLATE_INDEX_IMPL_HPP

