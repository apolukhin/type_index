//
// Copyright (c) Antony Polukhin, 2012-2014.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_TYPE_INDEX_HPP
#define BOOST_TYPE_INDEX_HPP

/// \file boost/type_index.hpp
/// \brief Includes minimal set of headers required to use the Boost.TypeIndex library.
///
/// By inclusion of this file most optimal type index classes will be included and used 
/// as a boost::typeindex::type_index and boost::typeindex::type_info.

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/config.hpp>

#if defined(BOOST_TYPE_INDEX_USER_TYPEINDEX)
#   include BOOST_TYPE_INDEX_USER_TYPEINDEX
#elif (!defined(BOOST_NO_RTTI) && !defined(BOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY)) || defined(BOOST_MSVC)
#   include <boost/type_index/stl_type_index.hpp>
#   ifdef BOOST_NO_RTTI
#       include <boost/type_index/stl_register_class.hpp>
#   endif
#else
#   include <boost/type_index/ctti_type_index.hpp>
#   include <boost/type_index/ctti_register_class.hpp>
#endif

namespace boost { namespace typeindex {

#if defined(BOOST_TYPE_INDEX_DOXYGEN_INVOKED)
    /// Depending on a compiler flags, optimal implementation of type_index will be used 
    /// as a default boost::typeindex::type_index.
    ///
    /// Could be a boost::typeindex::stl_type_index, boost::typeindex::ctti_type_index or 
    /// user defined type_index class.
    typedef platform-specific type_index;
#elif defined(BOOST_TYPE_INDEX_USER_TYPEINDEX)
    // Nothing to do
#elif (!defined(BOOST_NO_RTTI) && !defined(BOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY)) || defined(BOOST_MSVC)
    typedef boost::typeindex::stl_type_index type_index;
#   ifdef BOOST_NO_RTTI
#       define BOOST_TYPE_INDEX_REGISTER_CLASS BOOST_TYPE_INDEX_REGISTER_STL_CLASS
#   else
#       define BOOST_TYPE_INDEX_REGISTER_CLASS
#   endif
#else 
    typedef boost::typeindex::ctti_type_index type_index;
#   define BOOST_TYPE_INDEX_REGISTER_CLASS BOOST_TYPE_INDEX_REGISTER_CTTI_CLASS
#endif

/// Depending on a compiler flags, optimal implementation of type_info will be used 
/// as a default boost::typeindex::type_info.
///
/// Could be a std::type_info, boost::typeindex::detail::ctti_data or 
/// some user defined class.
///
/// type_info \b is \b not copyable or default constructible. It is \b not assignable too!
typedef type_index::type_info_t type_info;

#if defined(BOOST_TYPE_INDEX_DOXYGEN_INVOKED)

/// \def BOOST_TYPE_INDEX_USER_TYPEINDEX
/// BOOST_TYPE_INDEX_USER_TYPEINDEX can be defined to the path to header file
/// with user provided implementation of type_index.
///
/// See "Making own type_index" section of documentation for usage example.
#define BOOST_TYPE_INDEX_USER_TYPEINDEX <full/absolute/path/to/header/with/type_index.hpp>


/// \def BOOST_TYPE_INDEX_REGISTER_CLASS
/// \def BOOST_TYPE_INDEX_REGISTER_CLASSis used to help to emulate RTTI.
/// Put this macro into the public section of polymorphic class to allow runtime type detection.
///
/// Depending on the typeid() availability this macro will expand to nothing or to virtual helper function
/// `virtual const type_info& type_id_runtime() const`.
///
/// \b Example:
/// \code
/// class A {
/// public:
///     BOOST_TYPE_INDEX_REGISTER_CLASS
///     virtual ~A(){}
/// };
///
/// struct B: public A {
///     BOOST_TYPE_INDEX_REGISTER_CLASS
/// };
///
/// struct C: public B {
///     BOOST_TYPE_INDEX_REGISTER_CLASS
/// };
///
/// ...
///
/// C c1;
/// A* pc1 = &c1;
/// assert(boost::typeindex::type_id<C>() == boost::typeindex::type_id_runtime(*pc1));
/// \endcode
#define BOOST_TYPE_INDEX_REGISTER_CLASS nothing-or-some-virtual-functions

/// \def BOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY
/// BOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY is a helper macro that must be defined if mixing
/// RTTI on/off modules. See "Mixing sources with RTTI on and RTTI off" section of documentation
/// for more info.
#define BOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY

#endif // defined(BOOST_TYPE_INDEX_DOXYGEN_INVOKED)


/// Function to get boost::type_index for a type T.
/// Removes const, volatile && and & modifiers from T.
///
/// \b Example:
/// \code
/// type_index ti = type_id<int&>();
/// std::cout << ti.pretty_name();  // Outputs 'int'
/// \endcode
///
/// \tparam T Type for which type_index must be created.
/// \throw Nothing.
/// \return boost::typeindex::type_index with information about the specified type T.
template <class T>
inline type_index type_id() BOOST_NOEXCEPT {
    return type_index::type_id<T>();
}

/// Function for constructing boost::type_index instance for type T. 
/// Does not remove const, volatile, & and && modifiers from T.
///
/// If T has no const, volatile, & and && modifiers, then returns exactly 
/// the same result as in case of calling `type_id<T>()`.
///
/// \b Example:
/// \code
/// type_index ti = type_id_with_cvr<int&>();
/// std::cout << ti.pretty_name();  // Outputs 'int&'
/// \endcode
///
/// \tparam T Type for which type_index must be created.
/// \throw Nothing.
/// \return boost::typeindex::type_index with information about the specified type T.
template <class T>
inline type_index type_id_with_cvr() BOOST_NOEXCEPT {
    return type_index::type_id_with_cvr<T>();
}

/// Function that works exactly like C++ typeid(rtti_val) call, but returns boost::type_index.
///
/// Retunrs runtime information about specified type.
///
/// \b Requirements: RTTI available or Base and Derived classes must be marked with BOOST_TYPE_INDEX_REGISTER_CLASS.
///
/// \b Example:
/// \code
/// struct Base { virtual ~Base(){} };
/// struct Derived: public Base  {};
/// ...
/// Derived d;
/// Base& b = d;
/// type_index ti = type_id_runtime(b);
/// std::cout << ti.pretty_name();  // Outputs 'Derived'
/// \endcode
///
/// \param runtime_val Varaible which runtime type must be returned.
/// \throw Nothing.
/// \return boost::typeindex::type_index with information about the specified variable.
template <class T>
inline type_index type_id_runtime(const T& runtime_val) BOOST_NOEXCEPT {
    return type_index::type_id_runtime(runtime_val);
}

}} // namespace boost::typeindex



#endif // BOOST_TYPE_INDEX_HPP

