//
// Copyright (c) Antony Polukhin, 2013-2014.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_STL_TYPE_INDEX_HPP
#define BOOST_TYPE_INDEX_STL_TYPE_INDEX_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/// \file stl_type_index.hpp
/// \brief Contains boost::typeind::stl_type_index class.
///
/// boost::typeind::stl_type_index class can be used as a drop-in replacement 
/// for std::type_index.
///
/// It is used in situations when RTTI is enabled or typeid() method is available.
/// When typeid() is disabled or BOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY macro
/// is defined boost::typeind::ctti is usually used instead of boost::typeind::stl_type_index.

#include <boost/type_index/type_index_facade.hpp>


// MSVC is capable of calling typeid(T) even when RTTI is off
#if defined(BOOST_NO_RTTI) && !defined(BOOST_MSVC)
#error "File boost/type_index/stl_type_index.ipp is not usable when typeid() is not available."
#endif

#include <typeinfo>
#include <cstring>                                  // std::strcmp, std::strlen
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_volatile.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/if.hpp>
#include <boost/functional/hash_fwd.hpp>


#ifdef __GNUC__
#   include <cxxabi.h>                              // abi::__cxa_demangle
#endif

#if !defined(BOOST_MSVC)
#   include <boost/assert.hpp>
#   include <boost/detail/no_exceptions_support.hpp>
#endif

#if (defined(__EDG_VERSION__) && __EDG_VERSION__ < 245) \
        || (defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 744)
#   include <boost/type_traits/is_arithmetic.hpp>
#endif

namespace boost { namespace typeind {

/// \class stl_type_index
/// This class is a wrapper around std::type_info, that workarounds issues and provides
/// much more rich interface. For description of functions see type_index_facade.
///
/// This class requires typeid() to work. For cases when RTTI is disabled see ctti_type_index.
class stl_type_index
    : public type_index_facade<
        stl_type_index, 
        #ifdef BOOST_NO_STD_TYPEINFO
            type_info
        #else
            std::type_info
        #endif
    > 
{
public:
#ifdef BOOST_NO_STD_TYPEINFO
    typedef type_info type_info_t;
#else
    typedef std::type_info type_info_t;
#endif

private:
    const type_info_t* data_;

public:
    inline stl_type_index() BOOST_NOEXCEPT
        : data_(&typeid(void))
    {}

    inline stl_type_index(const type_info_t& data) BOOST_NOEXCEPT
        : data_(&data)
    {}

    inline const type_info_t&  type_info() const BOOST_NOEXCEPT;

    inline const char*  raw_name() const BOOST_NOEXCEPT;
    inline const char*  name() const BOOST_NOEXCEPT;
    inline std::string  pretty_name() const;

    inline std::size_t  hash_code() const BOOST_NOEXCEPT;
    inline bool         equal(const stl_type_index& rhs) const BOOST_NOEXCEPT;
    inline bool         before(const stl_type_index& rhs) const BOOST_NOEXCEPT;

    template <class T>
    inline static stl_type_index type_id() BOOST_NOEXCEPT;

    template <class T>
    inline static stl_type_index type_id_with_cvr() BOOST_NOEXCEPT;

    template <class T>
    inline static stl_type_index type_id_runtime(const T& value) BOOST_NOEXCEPT;
};

inline const stl_type_index::type_info_t& stl_type_index::type_info() const BOOST_NOEXCEPT {
    return *data_;
}


inline const char* stl_type_index::raw_name() const BOOST_NOEXCEPT {
#ifdef _MSC_VER
    return data_->raw_name();
#else
    return data_->name();
#endif
}

inline const char* stl_type_index::name() const BOOST_NOEXCEPT {
    return data_->name();
}

inline std::string stl_type_index::pretty_name() const {
#if defined(_MSC_VER)
    std::string ret = data_->name();
#else
    std::string ret;
    int status = 0;
    char* demang = abi::__cxa_demangle(raw_name(), NULL, 0, &status);
    BOOST_ASSERT(!status);

    BOOST_TRY {
        ret = demang; // may throw out of memory exception
    } BOOST_CATCH (...) {
        free(demang);
        BOOST_RETHROW;
    } BOOST_CATCH_END

    free(demang);
#endif

    std::string::size_type pos = ret.find("boost::typeind::detail::cvr_saver<");
    if (pos == std::string::npos) {
        return ret;
    }

    pos += sizeof("boost::typeind::detail::cvr_saver<") - 1;
    while (ret[pos] == ' ') {
        ++ pos;
    }

    std::string::size_type end = ret.rfind(">");
    while (ret[end - 1] == ' ') {
        -- end;
    }

    return ret.substr(pos, end - pos);
}


inline std::size_t stl_type_index::hash_code() const BOOST_NOEXCEPT {
#if _MSC_VER > 1600 || (__GNUC__ == 4 && __GNUC_MINOR__ > 5 && defined(__GXX_EXPERIMENTAL_CXX0X__))
    return data_->hash_code();
#else 
    return boost::hash_range(raw_name(), raw_name() + std::strlen(raw_name()));
#endif 
}


/// @cond

// for this compiler at least, cross-shared-library type_info
// comparisons don't work, so we are using typeid(x).name() instead.
# if (defined(__GNUC__) && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 5))) \
    || defined(_AIX) \
    || (defined(__sgi) && defined(__host_mips)) \
    || (defined(__hpux) && defined(__HP_aCC)) \
    || (defined(linux) && defined(__INTEL_COMPILER) && defined(__ICC))
#  define BOOST_CLASSINFO_COMPARE_BY_NAMES
# endif

/// @endcond

inline bool stl_type_index::equal(const stl_type_index& rhs) const BOOST_NOEXCEPT {
#ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
    return raw_name() == rhs.raw_name() || !std::strcmp(raw_name(), rhs.raw_name());
#else
    return *data_ == *rhs.data_;
#endif
}

inline bool stl_type_index::before(const stl_type_index& rhs) const BOOST_NOEXCEPT {
#ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
    return raw_name() != rhs.raw_name() && std::strcmp(raw_name(), rhs.raw_name()) < 0;
#else
    return !!data_->before(*rhs.data_);
#endif
}

#ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
#undef BOOST_CLASSINFO_COMPARE_BY_NAMES
#endif



template <class T>
inline stl_type_index stl_type_index::type_id() BOOST_NOEXCEPT {
    typedef BOOST_DEDUCED_TYPENAME boost::remove_reference<T>::type no_ref_t;
    typedef BOOST_DEDUCED_TYPENAME boost::remove_cv<no_ref_t>::type no_cvr_t;

    #  if (defined(__EDG_VERSION__) && __EDG_VERSION__ < 245) \
        || (defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 744)
            BOOST_STATIC_ASSERT_MSG( !boost::is_arithmetic<no_cvr_t>::type::value
                , "Your EDG-based compiler seems to mistakenly distinguish 'int' from 'signed int', in typeid() expressions.");
    #endif

    return typeid(no_cvr_t);
}

namespace detail {
    template <class T> class cvr_saver{};
}

template <class T>
inline stl_type_index stl_type_index::type_id_with_cvr() BOOST_NOEXCEPT {
    typedef typename boost::mpl::if_c<
        boost::is_reference<T>::value
            || boost::is_const<T>::value
            || boost::is_volatile<T>::value,
        detail::cvr_saver<T>,
        T
    >::type type;

    return typeid(type);
}


template <class T>
inline stl_type_index stl_type_index::type_id_runtime(const T& value) BOOST_NOEXCEPT {
#ifdef BOOST_NO_RTTI 
    return value.type_id_runtime();
#endif
    return typeid(value);
}

}} // namespace boost::typeind


#endif // BOOST_TYPE_INDEX_STL_TYPE_INDEX_HPP

