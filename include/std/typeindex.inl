// typeindex standard header
#pragma once
#ifndef _TYPEINDEX_
#define _TYPEINDEX_
#ifndef RC_INVOKED
#include <typeinfo>

 #pragma pack(push,_CRT_PACKING)
 #pragma warning(push,3)
 #pragma push_macro("new")
 #undef new

 #ifndef _XSTD2
  #define _XSTD2
 #endif /* _XSTD2 */

_STD_BEGIN
class type_index
	{	// wraps a typeinfo for indexing
public:
	type_index(const _XSTD2 type_info& _Tinfo) _NOEXCEPT
		: _Tptr(&_Tinfo)
		{	// construct from type_info
		}

	size_t hash_code() const _NOEXCEPT
		{	// return hash value
		return (_Tptr->hash_code());
		}

	const char *name() const _NOEXCEPT
		{	// return name
		return (_Tptr->name());
		}

	bool operator==(const type_index& _Right) const _NOEXCEPT
		{	// test if *this == _Right
		return (*_Tptr == *_Right._Tptr);
		}

	bool operator!=(const type_index& _Right) const _NOEXCEPT
		{	// test if *this != _Right
		return (!(*this == _Right));
		}

	bool operator<(const type_index& _Right) const _NOEXCEPT
		{	// test if *this < _Right
		return (_Tptr->before(*_Right._Tptr));
		}

	bool operator>=(const type_index& _Right) const _NOEXCEPT
		{	// test if *this >= _Right
		return (!(*this < _Right));
		}

	bool operator>(const type_index& _Right) const _NOEXCEPT
		{	// test if *this > _Right
		return (_Right < *this);
		}

	bool operator<=(const type_index& _Right) const _NOEXCEPT
		{	// test if *this <= _Right
		return (!(_Right < *this));
		}

private:
	const _XSTD2 type_info *_Tptr;
	};

	// TEMPLATE STRUCT SPECIALIZATION hash
template<>
	struct hash<type_index>
	{	// hash functor for type_index
	typedef type_index argument_type;
	typedef size_t result_type;

	size_t operator()(const argument_type& _Keyval) const
		{	// hash _Keyval to size_t value by pseudorandomizing transform
		return (_Keyval.hash_code());
		}
	};
_STD_END

 #pragma pop_macro("new")
 #pragma warning(pop)
 #pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _TYPEINDEX_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
