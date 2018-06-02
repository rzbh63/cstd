// unordered_set standard header
#pragma once
#ifndef _UNORDERED_SET_
#define _UNORDERED_SET_
#ifndef RC_INVOKED
#include <xhash>

 #pragma pack(push,_CRT_PACKING)
 #pragma warning(push,3)
 #pragma push_macro("new")
 #undef new

_STD_BEGIN
		// TEMPLATE CLASS _Uset_traits
template<class _Kty,	// key type (same as value type)
	class _Tr,	// comparator predicate type
	class _Alloc,	// actual allocator type (should be value allocator)
	bool _Mfl>	// true if multiple equivalent keys are permitted
	class _Uset_traits
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

	static constexpr bool _Standard = true;

	_Uset_traits(const _Tr& _Traits = _Tr())
		: _Tr(_Traits)
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
	};

		// TEMPLATE CLASS unordered_set
template<class _Kty,
	class _Hasher = hash<_Kty>,
	class _Keyeq = equal_to<_Kty>,
	class _Alloc = allocator<_Kty> >
	class unordered_set
		: public _Hash<_Uset_traits<_Kty,
			_Uhash_compare<_Kty, _Hasher, _Keyeq>, _Alloc, false> >
	{	// hash table of key values, unique keys
public:
	typedef unordered_set<_Kty, _Hasher, _Keyeq, _Alloc> _Myt;
	typedef _Uhash_compare<_Kty, _Hasher, _Keyeq> _Mytraits;
	typedef _Hash<_Uset_traits<_Kty,
		_Mytraits, _Alloc, false> > _Mybase;
	typedef _Hasher hasher;
	typedef _Kty key_type;
	typedef _Keyeq key_equal;
	typedef _Mytraits key_compare;	// extra

//	typedef typename _Mybase::value_compare value_compare;
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

	typedef typename _Mybase::iterator local_iterator;
	typedef typename _Mybase::const_iterator const_local_iterator;

	typedef typename _Mybase::_Alty _Alty;

	unordered_set()
		: _Mybase(key_compare(), allocator_type())
		{	// construct empty set from defaults
		}

	explicit unordered_set(const allocator_type& _Al)
		: _Mybase(key_compare(), _Al)
		{	// construct empty set from defaults, allocator
		}

	unordered_set(const _Myt& _Right)
		: _Mybase(_Right,
			_Right._List._Getal().select_on_container_copy_construction())
		{	// construct set by copying _Right
		}

	unordered_set(const _Myt& _Right, const allocator_type& _Al)
		: _Mybase(_Right, _Al)
		{	// construct set by copying _Right, allocator
		}

	explicit unordered_set(size_type _Buckets)
		: _Mybase(key_compare(), allocator_type())
		{	// construct empty set from defaults, ignore initial size
		this->rehash(_Buckets);
		}

	unordered_set(size_type _Buckets, const hasher& _Hasharg)
		: _Mybase(key_compare(_Hasharg), allocator_type())
		{	// construct empty set from hasher
		this->rehash(_Buckets);
		}

	unordered_set(size_type _Buckets, const hasher& _Hasharg,
		const _Keyeq& _Keyeqarg)
		: _Mybase(key_compare(_Hasharg, _Keyeqarg), allocator_type())
		{	// construct empty set from hasher and equality comparator
		this->rehash(_Buckets);
		}

	unordered_set(size_type _Buckets, const hasher& _Hasharg,
		const _Keyeq& _Keyeqarg, const allocator_type& _Al)
		: _Mybase(key_compare(_Hasharg, _Keyeqarg), _Al)
		{	// construct empty set from hasher and equality comparator
		this->rehash(_Buckets);
		}

	template<class _Iter>
		unordered_set(_Iter _First, _Iter _Last)
		: _Mybase(key_compare(), allocator_type())
		{	// construct set from sequence, defaults
		this->insert(_First, _Last);
		}

	template<class _Iter>
		unordered_set(_Iter _First, _Iter _Last,
			size_type _Buckets)
		: _Mybase(key_compare(), allocator_type())
		{	// construct set from sequence, ignore initial size
		this->rehash(_Buckets);
		this->insert(_First, _Last);
		}

	template<class _Iter>
		unordered_set(_Iter _First, _Iter _Last,
			size_type _Buckets, const hasher& _Hasharg)
		: _Mybase(key_compare(_Hasharg), allocator_type())
		{	// construct set from sequence, comparator
		this->rehash(_Buckets);
		this->insert(_First, _Last);
		}

	template<class _Iter>
		unordered_set(_Iter _First, _Iter _Last,
			size_type _Buckets, const hasher& _Hasharg,
			const _Keyeq& _Keyeqarg)
		: _Mybase(key_compare(_Hasharg, _Keyeqarg), allocator_type())
		{	// construct set from sequence, comparator, and allocator
		this->rehash(_Buckets);
		this->insert(_First, _Last);
		}

	template<class _Iter>
		unordered_set(_Iter _First, _Iter _Last,
			size_type _Buckets, const hasher& _Hasharg,
			const _Keyeq& _Keyeqarg, const allocator_type& _Al)
		: _Mybase(key_compare(_Hasharg, _Keyeqarg), _Al)
		{	// construct set from sequence, comparator, and allocator
		this->rehash(_Buckets);
		this->insert(_First, _Last);
		}

	_Myt& operator=(const _Myt& _Right)
		{	// assign by copying _Right
		_Mybase::operator=(_Right);
		return (*this);
		}

	unordered_set(_Myt&& _Right)
		: _Mybase(_STD move(_Right))
		{	// construct set by moving _Right
		}

	unordered_set(_Myt&& _Right, const allocator_type& _Al)
		: _Mybase(_STD move(_Right), _Al)
		{	// construct set by moving _Right, allocator
		}

	_Myt& operator=(_Myt&& _Right)
		_NOEXCEPT_OP(_Alty::is_always_equal::value
			&& is_nothrow_move_assignable<_Hasher>::value
			&& is_nothrow_move_assignable<_Keyeq>::value)
		{	// assign by moving _Right
		_Mybase::operator=(_STD move(_Right));
		return (*this);
		}

	void swap(_Myt& _Right)
		_NOEXCEPT_OP(_Alty::is_always_equal::value
			&& _Is_nothrow_swappable<_Hasher>::value
			&& _Is_nothrow_swappable<_Keyeq>::value)
		{	// exchange contents with non-movable _Right
		_Mybase::swap(_Right);
		}

	unordered_set(_XSTD initializer_list<value_type> _Ilist)
		: _Mybase(key_compare(), allocator_type())
		{	// construct set from initializer_list, defaults
		this->insert(_Ilist);
		}

	unordered_set(_XSTD initializer_list<value_type> _Ilist,
		size_type _Buckets)
		: _Mybase(key_compare(), allocator_type())
		{	// construct set from initializer_list
		this->rehash(_Buckets);
		this->insert(_Ilist);
		}

	unordered_set(_XSTD initializer_list<value_type> _Ilist,
		size_type _Buckets, const hasher& _Hasharg)
		: _Mybase(key_compare(_Hasharg), allocator_type())
		{	// construct set from initializer_list, hasher
		this->rehash(_Buckets);
		this->insert(_Ilist);
		}

	unordered_set(_XSTD initializer_list<value_type> _Ilist,
		size_type _Buckets, const hasher& _Hasharg,
			const _Keyeq& _Keyeqarg)
		: _Mybase(key_compare(_Hasharg, _Keyeqarg), allocator_type())
		{	// construct set from initializer_list, comparator
		this->rehash(_Buckets);
		this->insert(_Ilist);
		}

	unordered_set(_XSTD initializer_list<value_type> _Ilist,
		size_type _Buckets, const hasher& _Hasharg,
			const _Keyeq& _Keyeqarg, const allocator_type& _Al)
		: _Mybase(key_compare(_Hasharg, _Keyeqarg), _Al)
		{	// construct from initializer_list, allocator
		this->rehash(_Buckets);
		this->insert(_Ilist);
		}

	_Myt& operator=(_XSTD initializer_list<value_type> _Ilist)
		{	// assign initializer_list
		this->clear();
		this->insert(_Ilist);
		return (*this);
		}

	hasher hash_function() const
		{	// return hasher object
		return (this->_Traitsobj._Gethash());
		}

	key_equal key_eq() const
		{	// return equality comparator object
		return (this->_Traitsobj._Getkeyeq());
		}
	};

template<class _Kty,
	class _Hasher,
	class _Keyeq,
	class _Alloc>
	void swap(unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& _Left,
		unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& _Right)
		_NOEXCEPT_OP(_NOEXCEPT_OP(_Left.swap(_Right)))
	{	// swap _Left and _Right unordered_sets
	_Left.swap(_Right);
	}

template<class _Kty,
	class _Hasher,
	class _Keyeq,
	class _Alloc> inline
	bool operator==(
		const unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& _Left,
		const unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& _Right)
	{	// test for unordered_set equality
	return (_Hash_equal(_Left, _Right));
	}

template<class _Kty,
	class _Hasher,
	class _Keyeq,
	class _Alloc> inline
	bool operator!=(
		const unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& _Left,
		const unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& _Right)
	{	// test for unordered_set inequality
	return (!(_Left == _Right));
	}

		// TEMPLATE CLASS unordered_multiset
template<class _Kty,
	class _Hasher = hash<_Kty>,
	class _Keyeq = equal_to<_Kty>,
	class _Alloc = allocator<_Kty> >
	class unordered_multiset
		: public _Hash<_Uset_traits<_Kty,
			_Uhash_compare<_Kty, _Hasher, _Keyeq>, _Alloc, true> >
	{	// hash table of key values, non-unique keys
public:
	typedef unordered_multiset<_Kty, _Hasher, _Keyeq, _Alloc> _Myt;
	typedef _Uhash_compare<_Kty, _Hasher, _Keyeq> _Mytraits;
	typedef _Hash<_Uset_traits<_Kty,
		_Mytraits, _Alloc, true> > _Mybase;
	typedef _Hasher hasher;
	typedef _Kty key_type;
	typedef _Keyeq key_equal;
	typedef _Mytraits key_compare;	// extra

//	typedef typename _Mybase::value_compare value_compare;
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

	typedef typename _Mybase::_Alty _Alty;

	unordered_multiset()
		: _Mybase(key_compare(), allocator_type())
		{	// construct empty set from defaults
		}

	explicit unordered_multiset(const allocator_type& _Al)
		: _Mybase(key_compare(), _Al)
		{	// construct empty set from defaults, allocator
		}

	unordered_multiset(const _Myt& _Right)
		: _Mybase(_Right,
			_Right._List._Getal().select_on_container_copy_construction())
		{	// construct set by copying _Right
		}

	unordered_multiset(const _Myt& _Right, const allocator_type& _Al)
		: _Mybase(_Right, _Al)
		{	// construct set by copying _Right, allocator
		}

	explicit unordered_multiset(size_type _Buckets)
		: _Mybase(key_compare(), allocator_type())
		{	// construct empty set from defaults, ignore initial size
		this->rehash(_Buckets);
		}

	unordered_multiset(size_type _Buckets, const hasher& _Hasharg)
		: _Mybase(key_compare(_Hasharg), allocator_type())
		{	// construct empty set from hasher
		this->rehash(_Buckets);
		}

	unordered_multiset(size_type _Buckets, const hasher& _Hasharg,
		const _Keyeq& _Keyeqarg)
		: _Mybase(key_compare(_Hasharg, _Keyeqarg), allocator_type())
		{	// construct empty set from hasher and equality comparator
		this->rehash(_Buckets);
		}

	unordered_multiset(size_type _Buckets, const hasher& _Hasharg,
		const _Keyeq& _Keyeqarg, const allocator_type& _Al)
		: _Mybase(key_compare(_Hasharg, _Keyeqarg), _Al)
		{	// construct empty set from hasher and equality comparator
		this->rehash(_Buckets);
		}

	template<class _Iter>
		unordered_multiset(_Iter _First, _Iter _Last)
		: _Mybase(key_compare(), allocator_type())
		{	// construct set from sequence, defaults
		this->insert(_First, _Last);
		}

	template<class _Iter>
		unordered_multiset(_Iter _First, _Iter _Last,
			size_type _Buckets)
		: _Mybase(key_compare(), allocator_type())
		{	// construct set from sequence, defaults, ignore initial size
		this->rehash(_Buckets);
		this->insert(_First, _Last);
		}

	template<class _Iter>
		unordered_multiset(_Iter _First, _Iter _Last,
			size_type _Buckets, const hasher& _Hasharg)
		: _Mybase(key_compare(_Hasharg), allocator_type())
		{	// construct set from sequence, comparator
		this->rehash(_Buckets);
		this->insert(_First, _Last);
		}

	template<class _Iter>
		unordered_multiset(_Iter _First, _Iter _Last,
			size_type _Buckets, const hasher& _Hasharg,
			const _Keyeq& _Keyeqarg)
		: _Mybase(key_compare(_Hasharg, _Keyeqarg), allocator_type())
		{	// construct set from sequence, comparator, and allocator
		this->rehash(_Buckets);
		this->insert(_First, _Last);
		}

	template<class _Iter>
		unordered_multiset(_Iter _First, _Iter _Last,
			size_type _Buckets, const hasher& _Hasharg,
			const _Keyeq& _Keyeqarg, const allocator_type& _Al)
		: _Mybase(key_compare(_Hasharg, _Keyeqarg), _Al)
		{	// construct set from sequence, comparator, and allocator
		this->rehash(_Buckets);
		this->insert(_First, _Last);
		}

	_Myt& operator=(const _Myt& _Right)
		{	// assign by copying _Right
		_Mybase::operator=(_Right);
		return (*this);
		}

	unordered_multiset(_Myt&& _Right)
		: _Mybase(_STD move(_Right))
		{	// construct set by moving _Right
		}

	unordered_multiset(_Myt&& _Right, const allocator_type& _Al)
		: _Mybase(_STD move(_Right), _Al)
		{	// construct set by moving _Right, allocator
		}

	_Myt& operator=(_Myt&& _Right)
		_NOEXCEPT_OP(_Alty::is_always_equal::value
			&& is_nothrow_move_assignable<_Hasher>::value
			&& is_nothrow_move_assignable<_Keyeq>::value)
		{	// assign by moving _Right
		_Mybase::operator=(_STD move(_Right));
		return (*this);
		}

	template<class... _Valty>
		iterator emplace(_Valty&&... _Val)
		{	// try to insert value_type(_Val...), favoring right side
		return (_Mybase::emplace(_STD forward<_Valty>(_Val)...).first);
		}

	void swap(_Myt& _Right)
		_NOEXCEPT_OP(_Alty::is_always_equal::value
			&& _Is_nothrow_swappable<_Hasher>::value
			&& _Is_nothrow_swappable<_Keyeq>::value)
		{	// exchange contents with non-movable _Right
		_Mybase::swap(_Right);
		}

	unordered_multiset(_XSTD initializer_list<value_type> _Ilist)
		: _Mybase(key_compare(), allocator_type())
		{	// construct set from initializer_list, defaults
		this->insert(_Ilist);
		}

	unordered_multiset(_XSTD initializer_list<value_type> _Ilist,
		size_type _Buckets)
		: _Mybase(key_compare(), allocator_type())
		{	// construct set from initializer_list
		this->rehash(_Buckets);
		this->insert(_Ilist);
		}

	unordered_multiset(_XSTD initializer_list<value_type> _Ilist,
		size_type _Buckets, const hasher& _Hasharg)
		: _Mybase(key_compare(_Hasharg), allocator_type())
		{	// construct set from initializer_list, hasher
		this->rehash(_Buckets);
		this->insert(_Ilist);
		}

	unordered_multiset(_XSTD initializer_list<value_type> _Ilist,
		size_type _Buckets, const hasher& _Hasharg,
			const _Keyeq& _Keyeqarg)
		: _Mybase(key_compare(_Hasharg, _Keyeqarg), allocator_type())
		{	// construct set from initializer_list, comparator
		this->rehash(_Buckets);
		this->insert(_Ilist);
		}

	unordered_multiset(_XSTD initializer_list<value_type> _Ilist,
		size_type _Buckets, const hasher& _Hasharg,
			const _Keyeq& _Keyeqarg, const allocator_type& _Al)
		: _Mybase(key_compare(_Hasharg, _Keyeqarg), _Al)
		{	// construct set from initializer_list, allocator
		this->rehash(_Buckets);
		this->insert(_Ilist);
		}

	_Myt& operator=(_XSTD initializer_list<value_type> _Ilist)
		{	// assign initializer_list
		this->clear();
		this->insert(_Ilist);
		return (*this);
		}

	hasher hash_function() const
		{	// return hasher object
		return (this->_Traitsobj._Gethash());
		}

	key_equal key_eq() const
		{	// return equality comparator object
		return (this->_Traitsobj._Getkeyeq());
		}
	};

template<class _Kty,
	class _Hasher,
	class _Keyeq,
	class _Alloc>
	void swap(unordered_multiset<_Kty, _Hasher, _Keyeq, _Alloc>& _Left,
		unordered_multiset<_Kty, _Hasher, _Keyeq, _Alloc>& _Right)
		_NOEXCEPT_OP(_NOEXCEPT_OP(_Left.swap(_Right)))
	{	// swap _Left and _Right unordered_multisets
	_Left.swap(_Right);
	}

template<class _Kty,
	class _Hasher,
	class _Keyeq,
	class _Alloc> inline
	bool operator==(
		const unordered_multiset<_Kty, _Hasher, _Keyeq, _Alloc>& _Left,
		const unordered_multiset<_Kty, _Hasher, _Keyeq, _Alloc>& _Right)
	{	// test for unordered_multiset equality
	return (_Hash_equal(_Left, _Right));
	}

template<class _Kty,
	class _Hasher,
	class _Keyeq,
	class _Alloc> inline
	bool operator!=(
		const unordered_multiset<_Kty, _Hasher, _Keyeq, _Alloc>& _Left,
		const unordered_multiset<_Kty, _Hasher, _Keyeq, _Alloc>& _Right)
	{	// test for unordered_multiset inequality
	return (!(_Left == _Right));
	}

#if _HAS_TR1_NAMESPACE
namespace tr1 {	// TR1 ADDITIONS
using _STD unordered_multiset;
using _STD unordered_set;
}	// namespace tr1
#endif /* _HAS_TR1_NAMESPACE */
_STD_END
 #pragma pop_macro("new")
 #pragma warning(pop)
 #pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _UNORDERED_SET_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
