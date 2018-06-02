// hash_set extension header
#pragma once
#ifndef _HASH_SET_
#define _HASH_SET_
#ifndef RC_INVOKED
#include <xhash>

 #pragma pack(push,_CRT_PACKING)
 #pragma warning(push,3)
 #pragma push_macro("new")
 #undef new

 #ifndef _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
static_assert(false, "<hash_set> is deprecated and will be REMOVED. "
	"Please use <unordered_set>. You can define "
	"_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS "
	"to acknowledge that you have received this warning.");
 #endif /* _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS */

namespace stdext {
using _STD allocator;
using _STD enable_if;
using _STD equal_to;
using _STD hash;
using _STD is_convertible;
using _STD pair;
using _STD _Hash;

 #if _ITERATOR_DEBUG_LEVEL == 2
using _STD _Debug_range;
 #endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		// TEMPLATE CLASS _Hset_traits
template<class _Kty,	// key type (same as value type)
	class _Tr,	// comparator predicate type
	class _Alloc,	// actual allocator type (should be value allocator)
	bool _Mfl>	// true if multiple equivalent keys are permitted
	class _Hset_traits
		: public _Tr
	{	// traits required to make _Hash behave like a set
public:
	typedef _Kty key_type;
	typedef _Kty value_type;
	typedef _Tr key_compare;
	typedef _Alloc allocator_type;

	enum
		{	// make multi parameter visible as an enum constant
		_Multi = _Mfl};

	static constexpr bool _Standard = false;

	_Hset_traits(const _Tr& _Traits = _Tr())
		: _Tr(_Traits), _Max_buckets(0.0F)
		{	// construct with specified comparator
		}

	typedef key_compare value_compare;

	static const _Kty& _Kfn(const value_type& _Val)
		{	// return entire value as key
		return (_Val);
		}

	static int _Nonkfn(const value_type&)
		{	// extract "non-key" from element value (for container equality)
		return (0);
		}

	float& _Get_max_bucket_size() _NOEXCEPT
		{	// return reference to current maximum bucket size
		return (_Max_buckets);
		}

	const float& _Get_max_bucket_size() const _NOEXCEPT
		{	// return const reference to current maximum bucket size
		return (_Max_buckets);
		}

	float _Max_buckets;	// current maximum bucket size
	};

		// TEMPLATE CLASS hash_set
template<class _Kty,
	class _Tr = hash_compare<_Kty, less<_Kty> >,
	class _Alloc = allocator<_Kty> >
	class hash_set
		: public _Hash<_Hset_traits<_Kty, _Tr, _Alloc, false> >
	{	// hash table of key values, unique keys
public:
	typedef hash_set<_Kty, _Tr, _Alloc> _Myt;
	typedef _Hash<_Hset_traits<_Kty, _Tr, _Alloc, false> > _Mybase;
	typedef _Kty key_type;
	typedef _Tr key_compare;
	typedef typename _Mybase::value_compare value_compare;
	typedef typename _Mybase::allocator_type allocator_type;
	typedef typename _Mybase::size_type size_type;
	typedef typename _Mybase::difference_type difference_type;
	typedef typename _Mybase::pointer pointer;
	typedef typename _Mybase::const_pointer const_pointer;
	typedef typename _Mybase::reference reference;
	typedef typename _Mybase::const_reference const_reference;
	typedef typename _Mybase::iterator iterator;
	typedef typename _Mybase::const_iterator const_iterator;
	typedef typename _Mybase::value_type value_type;

	hash_set()
		: _Mybase(key_compare(), allocator_type())
		{	// construct empty set from defaults
		}

	explicit hash_set(const allocator_type& _Al)
		: _Mybase(key_compare(), _Al)
		{	// construct empty set from defaults, allocator
		}

	hash_set(const _Myt& _Right)
		: _Mybase(_Right,
			_Right._List._Getal().select_on_container_copy_construction())
		{	// construct set by copying _Right
		}

	hash_set(const _Myt& _Right, const allocator_type& _Al)
		: _Mybase(_Right, _Al)
		{	// construct set by copying _Right, allocator
		}

	explicit hash_set(const key_compare& _Traits)
		: _Mybase(_Traits, allocator_type())
		{	// construct empty set from comparator
		}

	hash_set(const key_compare& _Traits, const allocator_type& _Al)
		: _Mybase(_Traits, _Al)
		{	// construct empty set from comparator and allocator
		}

	template<class _Iter>
		hash_set(_Iter _First, _Iter _Last)
		: _Mybase(key_compare(), allocator_type())
		{	// construct set from sequence, defaults
		this->insert(_First, _Last);
		}

	template<class _Iter>
		hash_set(_Iter _First, _Iter _Last,
			const key_compare& _Traits)
		: _Mybase(_Traits, allocator_type())
		{	// construct set from sequence, comparator
		this->insert(_First, _Last);
		}

	template<class _Iter>
		hash_set(_Iter _First, _Iter _Last,
			const key_compare& _Traits, const allocator_type& _Al)
		: _Mybase(_Traits, _Al)
		{	// construct set from sequence, comparator, and allocator
		this->insert(_First, _Last);
		}

	_Myt& operator=(const _Myt& _Right)
		{	// assign by copying _Right
		_Mybase::operator=(_Right);
		return (*this);
		}

	hash_set(_Myt&& _Right)
		: _Mybase(_STD move(_Right))
		{	// construct set by moving _Right
		}

	hash_set(_Myt&& _Right, const allocator_type& _Al)
		: _Mybase(_STD move(_Right), _Al)
		{	// construct set by moving _Right, allocator
		}

	_Myt& operator=(_Myt&& _Right)
		{	// assign by moving _Right
		_Mybase::operator=(_STD move(_Right));
		return (*this);
		}

	void swap(_Myt& _Right)
		{	// exchange contents with non-movable _Right
		_Mybase::swap(_Right);
		}

	hash_set(_XSTD initializer_list<value_type> _Ilist)
		: _Mybase(key_compare(), allocator_type())
		{	// construct from initializer_list, defaults
		this->insert(_Ilist);
		}

	hash_set(_XSTD initializer_list<value_type> _Ilist,
			const key_compare& _Pred)
		: _Mybase(_Pred, allocator_type())
		{	// construct from initializer_list, comparator
		this->insert(_Ilist);
		}

	hash_set(_XSTD initializer_list<value_type> _Ilist,
			const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_Pred, _Al)
		{	// construct from initializer_list, comparator, and allocator
		this->insert(_Ilist);
		}

	_Myt& operator=(_XSTD initializer_list<value_type> _Ilist)
		{	// assign initializer_list
		this->clear();
		this->insert(_Ilist);
		return (*this);
		}

	typedef _STD reverse_iterator<iterator> reverse_iterator;
	typedef _STD reverse_iterator<const_iterator> const_reverse_iterator;

	reverse_iterator rbegin() _NOEXCEPT
		{	// return iterator for beginning of reversed mutable sequence
		return (reverse_iterator(this->end()));
		}

	const_reverse_iterator rbegin() const _NOEXCEPT
		{	// return iterator for beginning of reversed nonmutable sequence
		return (const_reverse_iterator(this->end()));
		}

	reverse_iterator rend() _NOEXCEPT
		{	// return iterator for end of reversed mutable sequence
		return (reverse_iterator(this->begin()));
		}

	const_reverse_iterator rend() const _NOEXCEPT
		{	// return iterator for end of reversed nonmutable sequence
		return (const_reverse_iterator(this->begin()));
		}

	const_reverse_iterator crbegin() const _NOEXCEPT
		{	// return iterator for beginning of reversed nonmutable sequence
		return (rbegin());
		}

	const_reverse_iterator crend() const _NOEXCEPT
		{	// return iterator for end of reversed nonmutable sequence
		return (rend());
		}
	};

template<class _Kty,
	class _Tr,
	class _Alloc> inline
	void swap(hash_set<_Kty, _Tr, _Alloc>& _Left,
		hash_set<_Kty, _Tr, _Alloc>& _Right)
	{	// swap _Left and _Right hash_sets
	_Left.swap(_Right);
	}

template<class _Kty,
	class _Tr,
	class _Alloc> inline
	bool operator==(
		const hash_set<_Kty, _Tr, _Alloc>& _Left,
		const hash_set<_Kty, _Tr, _Alloc>& _Right)
	{	// test for hash_set equality
	return (_STD _Hash_equal(_Left, _Right));
	}

template<class _Kty,
	class _Tr,
	class _Alloc> inline
	bool operator!=(
		const hash_set<_Kty, _Tr, _Alloc>& _Left,
		const hash_set<_Kty, _Tr, _Alloc>& _Right)
	{	// test for hash_set inequality
	return (!(_Left == _Right));
	}

		// TEMPLATE CLASS hash_multiset
template<class _Kty,
	class _Tr = hash_compare<_Kty, less<_Kty> >,
	class _Alloc = allocator<_Kty> >
	class hash_multiset
		: public _Hash<_Hset_traits<_Kty, _Tr, _Alloc, true> >
	{	// hash table of key values, non-unique keys
public:
	typedef hash_multiset<_Kty, _Tr, _Alloc> _Myt;
	typedef _Hash<_Hset_traits<_Kty, _Tr, _Alloc, true> > _Mybase;
	typedef _Kty key_type;
	typedef _Tr key_compare;
	typedef typename _Mybase::value_compare value_compare;
	typedef typename _Mybase::allocator_type allocator_type;
	typedef typename _Mybase::size_type size_type;
	typedef typename _Mybase::difference_type difference_type;
	typedef typename _Mybase::pointer pointer;
	typedef typename _Mybase::const_pointer const_pointer;
	typedef typename _Mybase::reference reference;
	typedef typename _Mybase::const_reference const_reference;
	typedef typename _Mybase::iterator iterator;
	typedef typename _Mybase::const_iterator const_iterator;
	typedef typename _Mybase::value_type value_type;

	hash_multiset()
		: _Mybase(key_compare(), allocator_type())
		{	// construct empty set from defaults
		}

	explicit hash_multiset(const allocator_type& _Al)
		: _Mybase(key_compare(), _Al)
		{	// construct empty set from defaults, allocator
		}

	hash_multiset(const _Myt& _Right)
		: _Mybase(_Right,
			_Right._List._Getal().select_on_container_copy_construction())
		{	// construct set by copying _Right
		}

	hash_multiset(const _Myt& _Right, const allocator_type& _Al)
		: _Mybase(_Right, _Al)
		{	// construct set by copying _Right, allocator
		}

	explicit hash_multiset(const key_compare& _Traits)
		: _Mybase(_Traits, allocator_type())
		{	// construct empty set from comparator
		}

	hash_multiset(const key_compare& _Traits,
		const allocator_type& _Al)
		: _Mybase(_Traits, _Al)
		{	// construct empty set from comparator and allocator
		}

	template<class _Iter>
		hash_multiset(_Iter _First, _Iter _Last)
		: _Mybase(key_compare(), allocator_type())
		{	// construct from sequence, defaults
		this->insert(_First, _Last);
		}

	template<class _Iter>
		hash_multiset(_Iter _First, _Iter _Last,
			const key_compare& _Traits)
		: _Mybase(_Traits, allocator_type())
		{	// construct from sequence, comparator
		this->insert(_First, _Last);
		}

	template<class _Iter>
		hash_multiset(_Iter _First, _Iter _Last,
			const key_compare& _Traits, const allocator_type& _Al)
		: _Mybase(_Traits, _Al)
		{	// construct from sequence, comparator, and allocator
		this->insert(_First, _Last);
		}

	_Myt& operator=(const _Myt& _Right)
		{	// assign by copying _Right
		_Mybase::operator=(_Right);
		return (*this);
		}

	hash_multiset(_Myt&& _Right)
		: _Mybase(_STD move(_Right))
		{	// construct set by moving _Right
		}

	hash_multiset(_Myt&& _Right, const allocator_type& _Al)
		: _Mybase(_STD move(_Right), _Al)
		{	// construct set by moving _Right, allocator
		}

	_Myt& operator=(_Myt&& _Right)
		{	// assign by moving _Right
		_Mybase::operator=(_STD move(_Right));
		return (*this);
		}

	void swap(_Myt& _Right)
		{	// exchange contents with non-movable _Right
		_Mybase::swap(_Right);
		}

	hash_multiset(_XSTD initializer_list<value_type> _Ilist)
		: _Mybase(key_compare(), allocator_type())
		{	// construct from initializer_list, defaults
		this->insert(_Ilist);
		}

	hash_multiset(_XSTD initializer_list<value_type> _Ilist,
			const key_compare& _Pred)
		: _Mybase(_Pred, allocator_type())
		{	// construct from initializer_list, comparator
		this->insert(_Ilist);
		}

	hash_multiset(_XSTD initializer_list<value_type> _Ilist,
			const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_Pred, _Al)
		{	// construct from initializer_list, comparator, and allocator
		this->insert(_Ilist);
		}

	_Myt& operator=(_XSTD initializer_list<value_type> _Ilist)
		{	// assign initializer_list
		this->clear();
		this->insert(_Ilist);
		return (*this);
		}

	typedef _STD reverse_iterator<iterator> reverse_iterator;
	typedef _STD reverse_iterator<const_iterator> const_reverse_iterator;

	reverse_iterator rbegin() _NOEXCEPT
		{	// return iterator for beginning of reversed mutable sequence
		return (reverse_iterator(this->end()));
		}

	const_reverse_iterator rbegin() const _NOEXCEPT
		{	// return iterator for beginning of reversed nonmutable sequence
		return (const_reverse_iterator(this->end()));
		}

	reverse_iterator rend() _NOEXCEPT
		{	// return iterator for end of reversed mutable sequence
		return (reverse_iterator(this->begin()));
		}

	const_reverse_iterator rend() const _NOEXCEPT
		{	// return iterator for end of reversed nonmutable sequence
		return (const_reverse_iterator(this->begin()));
		}

	const_reverse_iterator crbegin() const _NOEXCEPT
		{	// return iterator for beginning of reversed nonmutable sequence
		return (rbegin());
		}

	const_reverse_iterator crend() const _NOEXCEPT
		{	// return iterator for end of reversed nonmutable sequence
		return (rend());
		}
	};

template<class _Kty,
	class _Tr,
	class _Alloc> inline
	void swap(hash_multiset<_Kty, _Tr, _Alloc>& _Left,
		hash_multiset<_Kty, _Tr, _Alloc>& _Right)
	{	// swap _Left and _Right hash_multisets
	_Left.swap(_Right);
	}

template<class _Kty,
	class _Tr,
	class _Alloc> inline
	bool operator==(
		const hash_multiset<_Kty, _Tr, _Alloc>& _Left,
		const hash_multiset<_Kty, _Tr, _Alloc>& _Right)
	{	// test for hash_multiset equality
	return (_STD _Hash_equal(_Left, _Right));
	}

template<class _Kty,
	class _Tr,
	class _Alloc> inline
	bool operator!=(
		const hash_multiset<_Kty, _Tr, _Alloc>& _Left,
		const hash_multiset<_Kty, _Tr, _Alloc>& _Right)
	{	// test for hash_multiset inequality
	return (!(_Left == _Right));
	}
}	// namespace stdext
_STD_BEGIN
using stdext::hash_set;
using stdext::hash_multiset;
_STD_END

 #pragma pop_macro("new")
 #pragma warning(pop)
 #pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _HASH_SET_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
