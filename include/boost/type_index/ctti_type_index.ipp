//
// Copyright (c) Antony Polukhin, 2013-2014.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_CTTI_TYPE_INDEX_IPP
#define BOOST_TYPE_INDEX_CTTI_TYPE_INDEX_IPP

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
#include <boost/type_index/detail/compile_time_type_info.hpp>

#include <cstring>
#include <boost/static_assert.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/functional/hash_fwd.hpp>

namespace boost { namespace typeind { namespace detail {

struct ctti_data {
    const char* const typename_;
};

template <class T>
inline const ctti_data& ctti_construct() BOOST_NOEXCEPT {
    static const ctti_data result = { boost::detail::ctti<T>::n() };
    return result;
}


template <>
template <class T>
inline type_index_base<ctti_data> type_index_base<ctti_data>::construct() BOOST_NOEXCEPT {
    typedef BOOST_DEDUCED_TYPENAME boost::remove_reference<T>::type no_ref_t;
    typedef BOOST_DEDUCED_TYPENAME boost::remove_cv<no_ref_t>::type no_cvr_t;
    return ctti_construct<no_cvr_t>();
}


template <>
template <class T>
inline type_index_base<ctti_data> type_index_base<ctti_data>::construct_with_cvr() BOOST_NOEXCEPT {
    return ctti_construct<T>();
}


template <>
template <class T>
inline type_index_base<ctti_data> type_index_base<ctti_data>::construct_runtime(T& rtti_val) BOOST_NOEXCEPT {
    BOOST_STATIC_ASSERT_MSG(sizeof(T) && false, 
        "type_id_runtime(T&) and type_index::construct_runtime(T&) require RTTI");
    
    return ctti_construct<T>();
}


template <>
template <class T>
inline type_index_base<ctti_data> type_index_base<ctti_data>::construct_runtime(T* rtti_val) {
    BOOST_STATIC_ASSERT_MSG(sizeof(T) && false, 
        "type_id_runtime(T*) and type_index::construct_runtime(T*) require RTTI");

    return ctti_construct<T>();
}


template <>
inline const char* type_index_base<ctti_data>::raw_name() const BOOST_NOEXCEPT {
    return data_->typename_;
}


template <>
inline const char* type_index_base<ctti_data>::name() const BOOST_NOEXCEPT {
    return data_->typename_;
}

template <>
inline std::string type_index_base<ctti_data>::pretty_name() const {
    std::size_t len = std::strlen(raw_name() + ctti_skip_size_at_end);
    while (raw_name()[len - 1] == ' ') --len; // MSVC sometimes adds whitespaces
    return std::string(raw_name(), len);
}


template <>
inline bool type_index_base<ctti_data>::equal(const type_index_base<ctti_data>& rhs) const BOOST_NOEXCEPT {
    return raw_name() == rhs.raw_name() || !std::strcmp(raw_name(), rhs.raw_name());
}


template <>
inline bool type_index_base<ctti_data>::before(const type_index_base<ctti_data>& rhs) const BOOST_NOEXCEPT {
    return raw_name() != rhs.raw_name() && std::strcmp(raw_name(), rhs.raw_name()) < 0;
}


template <>
inline std::size_t type_index_base<ctti_data>::hash_code() const BOOST_NOEXCEPT {
    return boost::hash_range(raw_name(), raw_name() + std::strlen(raw_name() + detail::ctti_skip_size_at_end));
}


}}} // namespace boost::typeind::detail


#endif // BOOST_TYPE_INDEX_CTTI_TYPE_INDEX_IPP

