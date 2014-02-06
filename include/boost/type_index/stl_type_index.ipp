//
// Copyright (c) Antony Polukhin, 2013-2014.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_STL_TYPE_INDEX_IPP
#define BOOST_TYPE_INDEX_STL_TYPE_INDEX_IPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/// \file stl_type_index.ipp
/// \brief Contains specialization of boost::type_index_base<std::type_info>.
///
/// boost::type_index_base<std::type_info> class can be used as a drop-in replacement 
/// for std::type_index.
///
/// It is used in situations when RTTI is enabled or typeid() method is available.
/// When typeid() is not is disabled or BOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY macro is defined boost::template_info
/// is usually used instead of it (some compilers allow calling typeid(T)
/// even if RTTI is disabled, those copilers will continue to use boost::type_info class).

#include <boost/type_index/type_index_base.hpp>


// MSVC is capable of calling typeid(T) even when RTTI is off
#if defined(BOOST_NO_RTTI) && !defined(BOOST_MSVC)
#error "File boost/type_index/stl_type_index.ipp is not usable when typeid() is not available."
#endif

#include <typeinfo>

#ifdef __GNUC__
#include <cxxabi.h>
#endif


#include <cstring>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_volatile.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/if.hpp>
#include <boost/detail/no_exceptions_support.hpp>
#include <boost/functional/hash_fwd.hpp>



namespace boost { namespace typeind { namespace detail {

template <>
template <class T>
type_index_base<std::type_info> type_index_base<std::type_info>::construct() BOOST_NOEXCEPT {
    typedef BOOST_DEDUCED_TYPENAME boost::remove_reference<T>::type no_ref_t;
    typedef BOOST_DEDUCED_TYPENAME boost::remove_cv<no_ref_t>::type no_cvr_t;

    #  if (defined(__EDG_VERSION__) && __EDG_VERSION__ < 245) \
        || (defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 744)
            BOOST_STATIC_ASSERT_MSG( !boost::is_arithmetic<no_cvr_t>::type::value
                , "Your EDG-based compiler seems to mistakenly distinguish 'int' from 'signed int', in typeid() expressions.");
    #endif

    return typeid(no_cvr_t);
}


template <>
template <class T>
const type_index_base<std::type_info>& type_index_base<std::type_info>::construct_with_cvr() BOOST_NOEXCEPT {
    typedef typename boost::mpl::if_c<
        boost::is_reference<T>::value
            || boost::is_const<T>::value
            || boost::is_volatile<T>::value,
        detail::cvr_saver<T>,
        T
    >::type type;

    return typeid(type);
}


template <>
template <class T>
type_index_base<std::type_info>& type_index_base<std::type_info>::construct_runtime(T& rtti_val) BOOST_NOEXCEPT {
#ifdef BOOST_NO_RTTI 
    BOOST_STATIC_ASSERT_MSG(sizeof(T) && false, 
        "type_id_runtime(T&) and type_index::construct_runtime(T&) require RTTI");
#endif
    
    return typeid(rtti_val);
}


template <>
template <class T>
type_index_base<std::type_info> type_index_base<std::type_info>::construct_runtime(T* rtti_val) {
#ifdef BOOST_NO_RTTI 
    BOOST_STATIC_ASSERT_MSG(sizeof(T) && false, 
        "type_id_runtime(T*) and type_index::construct_runtime(T*) require RTTI");
#endif
    return typeid(rtti_val);
}


template <>
const char* type_index_base<std::type_info>::raw_name() const BOOST_NOEXCEPT {
#ifdef _MSC_VER
    return data_->raw_name();
#else
    return data_->name();
#endif
}


template <>
const char* type_index_base<std::type_info>::name() const BOOST_NOEXCEPT {
    return data_->name();
}

template <>
std::string type_index_base<std::type_info>::pretty_name() const {
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
    BOOST_ASSERT(end != std::string::npos);
    while (ret[end - 1] == ' ') {
        -- end;
    }

    return ret.substr(pos, end - pos);
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


template <>
bool type_index_base<std::type_info>::equal(const type_index_base<std::type_info>& rhs) const BOOST_NOEXCEPT {
#ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
    return raw_name() == rhs.raw_name() || !std::strcmp(raw_name(), rhs.raw_name());
#else
    return *data_ == *rhs.data_;
#endif
}


template <>
bool type_index_base<std::type_info>::before(const type_index_base<std::type_info>& rhs) const BOOST_NOEXCEPT {
#ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
    return raw_name() != rhs.raw_name() && std::strcmp(raw_name(), rhs.raw_name()) < 0;
#else
    return !!data_->before(*rhs.data_);
#endif
}


#ifdef BOOST_CLASSINFO_COMPARE_BY_NAMES
#undef BOOST_CLASSINFO_COMPARE_BY_NAMES
#endif


template <>
std::size_t type_index_base<std::type_info>::hash_code() const BOOST_NOEXCEPT {
#if _MSC_VER > 1600 || (__GNUC__ == 4 && __GNUC_MINOR__ > 5 && defined(__GXX_EXPERIMENTAL_CXX0X__))
    return data_->hash_code();
#else 
    return boost::hash_range(raw_name(), raw_name() + std::strlen(name()));
#endif 
}


}}} // namespace boost::typeind::detail


#endif // BOOST_TYPE_INDEX_STL_TYPE_INDEX_IPP

