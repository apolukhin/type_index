//
// Copyright (c) Antony Polukhin, 2013-2014.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_CTTI_TYPE_INDEX_HPP
#define BOOST_TYPE_INDEX_CTTI_TYPE_INDEX_HPP

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

#include <boost/type_index/type_index_facade.hpp>
#include <boost/type_index/detail/compile_time_type_info.hpp>

#include <cstring>
#include <boost/static_assert.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/functional/hash_fwd.hpp>

namespace boost { namespace typeind { namespace detail {

struct ctti_data {
    const char* typename_;
};

template <class T>
inline const ctti_data& ctti_construct() BOOST_NOEXCEPT {
    static const ctti_data result = { boost::detail::ctti<T>::n() };
    return result;
}


class ctti_type_index: public type_index_facade<ctti_type_index, ctti_data> {
    const ctti_data* data_;

public:
    typedef ctti_data type_info_t;

    inline ctti_type_index(const type_info_t& data) BOOST_NOEXCEPT
        : data_(&data)
    {}

    inline const type_info_t&  type_info() const BOOST_NOEXCEPT;

    inline const char*  raw_name() const BOOST_NOEXCEPT;
    inline const char*  name() const BOOST_NOEXCEPT;
    inline std::string  pretty_name() const;

    inline std::size_t  hash_code() const BOOST_NOEXCEPT;
    inline bool         equal(const ctti_type_index& rhs) const BOOST_NOEXCEPT;
    inline bool         before(const ctti_type_index& rhs) const BOOST_NOEXCEPT;

    template <class T>
    inline static ctti_type_index construct() BOOST_NOEXCEPT;

    template <class T>
    inline static ctti_type_index construct_with_cvr() BOOST_NOEXCEPT;

    template <class T>
    inline static ctti_type_index construct_runtime(const T* variable) BOOST_NOEXCEPT;

    template <class T>
    inline static ctti_type_index construct_runtime(const T& variable) BOOST_NOEXCEPT;
};


template <class T>
inline ctti_type_index ctti_type_index::construct() BOOST_NOEXCEPT {
    typedef BOOST_DEDUCED_TYPENAME boost::remove_reference<T>::type no_ref_t;
    typedef BOOST_DEDUCED_TYPENAME boost::remove_cv<no_ref_t>::type no_cvr_t;
    return ctti_construct<no_cvr_t>();
}



template <class T>
inline ctti_type_index ctti_type_index::construct_with_cvr() BOOST_NOEXCEPT {
    return ctti_construct<T>();
}


template <class T>
inline ctti_type_index ctti_type_index::construct_runtime(const T* rtti_val) BOOST_NOEXCEPT {
    BOOST_STATIC_ASSERT_MSG(sizeof(T) && false, 
        "type_id_runtime(const T*) and type_index::construct_runtime(const T*) require RTTI");

    return ctti_construct<T>();
}

template <class T>
inline ctti_type_index ctti_type_index::construct_runtime(const T& rtti_val) BOOST_NOEXCEPT {
    BOOST_STATIC_ASSERT_MSG(sizeof(T) && false, 
        "type_id_runtime(const T&) and type_index::construct_runtime(const T&) require RTTI");

    return ctti_construct<T>();
}


inline const char* ctti_type_index::raw_name() const BOOST_NOEXCEPT {
    return data_->typename_;
}


inline const char* ctti_type_index::name() const BOOST_NOEXCEPT {
    return data_->typename_;
}

inline std::string ctti_type_index::pretty_name() const {
    std::size_t len = std::strlen(raw_name() + ctti_skip_size_at_end);
    while (raw_name()[len - 1] == ' ') --len; // MSVC sometimes adds whitespaces
    return std::string(raw_name(), len);
}


inline bool ctti_type_index::equal(const ctti_type_index& rhs) const BOOST_NOEXCEPT {
    return raw_name() == rhs.raw_name() || !std::strcmp(raw_name(), rhs.raw_name());
}


inline bool ctti_type_index::before(const ctti_type_index& rhs) const BOOST_NOEXCEPT {
    return raw_name() != rhs.raw_name() && std::strcmp(raw_name(), rhs.raw_name()) < 0;
}


inline std::size_t ctti_type_index::hash_code() const BOOST_NOEXCEPT {
    return boost::hash_range(raw_name(), raw_name() + std::strlen(raw_name() + detail::ctti_skip_size_at_end));
}


}}} // namespace boost::typeind::detail


#endif // BOOST_TYPE_INDEX_CTTI_TYPE_INDEX_IPP

