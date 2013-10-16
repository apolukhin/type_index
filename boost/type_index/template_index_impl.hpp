//
// Copyright (c) Antony Polukhin, 2012-2013.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_TEMPLATE_INDEX_IMPL_HPP
#define BOOST_TYPE_INDEX_TEMPLATE_INDEX_IMPL_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/// \file template_index_impl.hpp
/// \brief Contains implementation of template_index class.
///
/// Here is defined the template_index class, that is used instead of type_index class in situations when RTTI is disabled.
/// Consider including `<boost/type_index/type_index_minimal.hpp>` or `<boost/type_index.hpp>` instead of this file.

#include <cstring>
#include <string>
#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/current_function.hpp>
#include <boost/functional/hash_fwd.hpp>

#if !defined(BOOST_NO_IOSTREAM)
#if !defined(BOOST_NO_IOSFWD)
#include <iosfwd>               // for std::basic_ostream
#else
#include <ostream>
#endif
#endif


namespace boost {

namespace detail {
#if defined(BOOST_TYPE_INDEX_CTTI_BEGIN_SKIP) && defined(BOOST_TYPE_INDEX_CTTI_END_SKIP)
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_begin = BOOST_TYPE_INDEX_CTTI_BEGIN_SKIP); // skip user specified bytes count
    BOOST_STATIC_CONSTANT(std::size_t, ctti_skip_size_at_end = BOOST_TYPE_INDEX_CTTI_END_SKIP);     // skip user specified bytes count
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
            return std::strlen(this_type::n() + detail::ctti_skip_size_at_end);
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
        return std::string(name_, std::strlen(name_ + detail::ctti_skip_size_at_end));
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
        return boost::hash_range(name_, name_ + std::strlen(name_ + detail::ctti_skip_size_at_end));
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
/// If T has no const, volatile, & and && modifiers, then returns exactly 
/// the same result as in case of calling `template_id<T>()`.
template <class T>
template_index template_id_with_cvr() BOOST_NOEXCEPT {
    return template_index::construct_with_cvr<T>();
}

/* *************** template_index free functions ******************* */

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
#endif // BOOST_NO_TEMPLATED_IOSTREAMS
#endif // BOOST_NO_IOSTREAM

/// hash_value function overload for template_index
inline std::size_t hash_value(template_index const& v) BOOST_NOEXCEPT {
    return v.hash_code();
}

} // namespace boost

#endif // BOOST_TYPE_INDEX_TEMPLATE_INDEX_IMPL_HPP

