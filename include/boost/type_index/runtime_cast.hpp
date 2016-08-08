//
// Copyright (c) CHris Glover, 2016.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_RUNTIME_CAST_HPP
#define BOOST_TYPE_INDEX_RUNTIME_CAST_HPP

/// \file runtime_cast.hpp
/// \brief Contains boost::typeindex::ctti_type_index class that is constexpr if C++14 constexpr is supported by compiler.
///
/// boost::typeindex::runtime_cast class can be used as a replacement
/// for dynamic_cast on platforms where dynamic_cast is not available
/// or undesirable at a global level.

#include <boost/type_index.hpp>
#include <boost/throw_exception.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/type_traits/decay.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <typeinfo>

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

namespace boost { namespace typeindex {

namespace detail {

template<typename T, typename U>
T* runtime_cast_impl(U* u, boost::true_type) {
    return u;
} 

template<typename T, typename U>
T const* runtime_cast_impl(U const* u, boost::true_type) {
    return u;
}

template<typename T, typename U>
T* runtime_cast_impl(U* u, boost::false_type) {
    return const_cast<T*>(static_cast<T const*>(
        u->boost_type_index_find_instance_(boost::typeindex::type_id<T>())
    ));
} 

template<typename T, typename U>
T const* runtime_cast_impl(U const* u, boost::false_type) {
    return static_cast<T const*>(u->boost_type_index_find_instance_(boost::typeindex::type_id<T>()));
}

template<typename T>
type_index get_type_for_value(T const&) {
    return type_id<typename boost::decay<T>::type>();
}

} // namespace detail

#define BOOST_TYPE_INDEX_CHECK_BASE_(r, data, Base) \
    if(void const* ret_val = this->Base::boost_type_index_find_instance_(idx)) return ret_val;

#define BOOST_TYPE_INDEX_CHECK_BASES(base_list) \
    BOOST_PP_SEQ_FOR_EACH(BOOST_TYPE_INDEX_CHECK_BASE_, _, base_list)

#define BOOST_TYPE_INDEX_REGISTER_CLASS_RTTI                                                                              \
    virtual void const* boost_type_index_find_instance_(boost::typeindex::type_index const& idx) const BOOST_NOEXCEPT {   \
        if(idx == boost::typeindex::detail::get_type_for_value(*this))                                                    \
            return this;                                                                                                  \
        return NULL;                                                                                                      \
    }                                                                   

#define BOOST_TYPE_INDEX_REGISTER_CLASS_RTTI_BASES(base_list)                                                             \
    virtual void const* boost_type_index_find_instance_(boost::typeindex::type_index const& idx) const BOOST_NOEXCEPT {   \
        if(idx == boost::typeindex::detail::get_type_for_value(*this))                                                    \
            return this;                                                                                                  \
         BOOST_TYPE_INDEX_CHECK_BASES(base_list)                                                                          \
         return NULL;                                                                                                     \
    }  

    template<typename T, typename U>
    T runtime_cast(U* u) BOOST_NOEXCEPT {
        typedef typename boost::remove_pointer<T>::type impl_type;
        return detail::runtime_cast_impl<impl_type>(u, boost::is_base_and_derived<T, U>());
    } 

    template<typename T, typename U>
    T runtime_cast(U const* u) BOOST_NOEXCEPT {
        typedef typename boost::remove_pointer<T>::type impl_type;
        return detail::runtime_cast_impl<impl_type>(u, boost::is_base_and_derived<T, U>());
    }

    template<typename T, typename U>
    T runtime_cast(U& u) {
        typedef typename boost::remove_reference<T>::type impl_type;
        impl_type* value = detail::runtime_cast_impl<impl_type>(&u, boost::is_base_and_derived<T, U>());
        if(!value)
            boost::throw_exception(std::bad_cast());
        return *value;
    } 

    template<typename T, typename U>
    T runtime_cast(U const& u) {
        typedef typename boost::remove_reference<T>::type impl_type;
        impl_type* value = detail::runtime_cast_impl<impl_type>(&u, boost::is_base_and_derived<T, U>());
        if(!value)
            boost::throw_exception(std::bad_cast());
        return *value;
    }

    template<typename T, typename U>
    T* runtime_pointer_cast(U* u) BOOST_NOEXCEPT {
        return detail::runtime_cast_impl<T>(u, boost::is_base_and_derived<T, U>());
    } 

    template<typename T, typename U>
    T const* runtime_pointer_cast(U const* u) BOOST_NOEXCEPT {
        return detail::runtime_cast_impl<T>(u, boost::is_base_and_derived<T, U>());
    }

}} // namespace boost::typeindex

#endif // BOOST_TYPE_INDEX_RUNTIME_CAST_HPP