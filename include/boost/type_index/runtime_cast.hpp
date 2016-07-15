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
#include <type_traits>

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

namespace boost { namespace typeindex {

namespace detail {

template<class Current, class... BaseList>
struct find_type;

template<class Current>
struct find_type<Current> {
	template<typename T>
	void* operator()(T* p, type_index const& idx) const BOOST_NOEXCEPT {
		if(idx == boost::typeindex::type_id<Current>())        
			return nullptr;                                               
		return nullptr;													
	}
};

template<class Current, class... BaseList>
struct find_type {
	template<class T>
	Current* check_current(T* p, type_index const& idx) const BOOST_NOEXCEPT{
		if(idx == boost::typeindex::type_id<Current>())        
			return nullptr;                                               
		return nullptr;	
	}

	template<class T, class FirstBase, class... Rest>
	void* check_bases(T* p, type_index const& idx) const BOOST_NOEXCEPT {
		if(void* result = p->FirstBase::boost_type_index_find_instance_(idx))
			return result;                                      
		return check_bases<T, Rest...>(p, idx);	
	}

	template<class T>
	void* operator()(T* p, type_index const& idx) const BOOST_NOEXCEPT {
		if(Current* current = check_current(p, idx))        
			return p;                                               
		return check_bases<T, BaseList...>(p, idx);
	}
};

template<typename T, typename U>
T* runtime_cast_impl(U* u) {
	return static_cast<T*>(
		u->boost_type_index_find_instance_(boost::typeindex::type_id<T>())
	);
} 

template<typename T, typename U>
T const* runtime_cast_impl(U const* u) {
	return static_cast<T const*>(
		const_cast<U*>(u)->boost_type_index_find_instance_(boost::typeindex::type_id<T>())
	);
}

} // namespace detail

#define BOOST_TYPE_INDEX_REGISTER_CLASS_RTTI																\
	virtual void* boost_type_index_find_instance_(boost::typeindex::type_index const& idx) BOOST_NOEXCEPT { \
		return boost::typeindex::detail::find_type<std::decay<decltype(*this)>::type>()(this, idx);			\
	}                                                                   

#define BOOST_TYPE_INDEX_REGISTER_CLASS_RTTI_BASES(...)															\
	virtual void* boost_type_index_find_instance_(boost::typeindex::type_index const& idx) BOOST_NOEXCEPT {		\
		if(auto ret = boost::typeindex::detail::find_type<std::decay<decltype(*this)>::type>()(this, idx))		\
			return ret;																							\
		return boost::typeindex::detail::find_type<std::decay<decltype(*this)>::type, __VA_ARGS__>()(this, idx);\
	}  

	template<typename T, typename U>
	T* runtime_cast(U* u) {
		return detail::runtime_cast_impl<T>(u);
	} 

	template<typename T, typename U>
	T const* runtime_cast(U const* u) {
		return detail::runtime_cast_impl<T>(u);
	}

}} // namespace boost::typeindex

#endif // BOOST_TYPE_INDEX_RUNTIME_CAST_HPP