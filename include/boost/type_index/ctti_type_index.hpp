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

/// \file ctti_type_index.hpp
/// \brief Contains boost::typeind::ctti_type_index class.
///
/// boost::typeind::ctti_type_index class can be used as a drop-in replacement 
/// for std::type_index.
///
/// It is used in situations when typeid() method is not available or 
/// BOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY macro is defined.

#include <boost/type_index/type_index_facade.hpp>
#include <boost/type_index/detail/compile_time_type_info.hpp>

#include <cstring>
#include <boost/static_assert.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost { namespace typeind {

namespace detail {

// That's the most trickiest part of the TypeIndex library:
//      1) we do not whant to give user ability to manually construct and compare `struct-that-represents-type`
//      2) we need to distinguish beteween `struct-that-represents-type` and `const char*`
//      3) we need a thread-safe way to have references to instances `struct-that-represents-type`
//      4) we need a compile-time control to make shure that user does not copy or 
// default construct `struct-that-represents-type`
//
// Solution would be a forward declared structure.
struct ctti_data;

} // namespace detail

/// Helper method for getting detail::ctti_data of a tempalte patameter T.
template <class T>
inline const detail::ctti_data& ctti_construct() BOOST_NOEXCEPT {
    // Standard C++11, 5.2.10 Reinterpret cast: Converting a prvalue of type “pointer to T1” to the
    // type “pointer to T2” (where T1 and T2 are object types and where the alignment requirements of T2 are no
    // stricter than those of T1) and back to its original type yields the original pointer value.
    return *reinterpret_cast<const detail::ctti_data*>(boost::detail::ctti<T>::n());
}

/// \class ctti_type_index
/// This class is a wrapper that pretends to work exactly like stl_type_index, but does 
/// not require RTTI support. For description of functions see type_index_facade.
///
/// This class produces slightly longer type names, so consider using stl_type_index 
/// in situations when typeid() is working.
class ctti_type_index: public type_index_facade<ctti_type_index, detail::ctti_data> {
    const detail::ctti_data* data_;

public:
    typedef detail::ctti_data type_info_t;

    inline ctti_type_index() BOOST_NOEXCEPT
        : data_(&ctti_construct<void>())
    {}

    inline ctti_type_index(const type_info_t& data) BOOST_NOEXCEPT
        : data_(&data)
    {}

    inline const type_info_t&  type_info() const BOOST_NOEXCEPT;
    inline const char*  raw_name() const BOOST_NOEXCEPT;
    inline std::string  pretty_name() const;
    inline std::size_t  hash_code() const BOOST_NOEXCEPT;

    template <class T>
    inline static ctti_type_index type_id() BOOST_NOEXCEPT;

    template <class T>
    inline static ctti_type_index type_id_with_cvr() BOOST_NOEXCEPT;

    template <class T>
    inline static ctti_type_index type_id_runtime(const T& variable) BOOST_NOEXCEPT;
};


inline const ctti_type_index::type_info_t& ctti_type_index::type_info() const BOOST_NOEXCEPT {
    return *data_;
}


template <class T>
inline ctti_type_index ctti_type_index::type_id() BOOST_NOEXCEPT {
    typedef BOOST_DEDUCED_TYPENAME boost::remove_reference<T>::type no_ref_t;
    typedef BOOST_DEDUCED_TYPENAME boost::remove_cv<no_ref_t>::type no_cvr_t;
    return ctti_construct<no_cvr_t>();
}



template <class T>
inline ctti_type_index ctti_type_index::type_id_with_cvr() BOOST_NOEXCEPT {
    return ctti_construct<T>();
}


template <class T>
inline ctti_type_index ctti_type_index::type_id_runtime(const T& variable) BOOST_NOEXCEPT {
    return variable.type_id_runtime();
}


inline const char* ctti_type_index::raw_name() const BOOST_NOEXCEPT {
    return reinterpret_cast<const char*>(data_);
}


inline std::string ctti_type_index::pretty_name() const {
    std::size_t len = std::strlen(raw_name() + detail::ctti_skip_size_at_end);
    while (raw_name()[len - 1] == ' ') --len; // MSVC sometimes adds whitespaces
    return std::string(raw_name(), len);
}


inline std::size_t ctti_type_index::hash_code() const BOOST_NOEXCEPT {
    return boost::hash_range(raw_name(), raw_name() + std::strlen(raw_name() + detail::ctti_skip_size_at_end));
}


}} // namespace boost::typeind

#endif // BOOST_TYPE_INDEX_CTTI_TYPE_INDEX_HPP

