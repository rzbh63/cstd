// xhash internal header
#pragma once
#ifndef _XHASH_
#define _XHASH_
#ifndef RC_INVOKED
#include <cstring>
#include <cwchar>
#include <list>
#include <vector>

 #pragma pack(push,_CRT_PACKING)
 #pragma warning(push,3)
 #pragma push_macro("new")
 #undef new

 #pragma warning(disable: 4127)

namespace stdext {
using _STD basic_string;
using _STD less;
using _STD size_t;

		// TEMPLATE FUNCTION hash_value
 #define _HASH_SEED	(size_t)0xdeadbeef

template<class _Kty> inline
	size_t hash_value(const _Kty& _Keyval)
	{	// hash _Keyval to size_t value one-to-one
	return ((size_t)_Keyval ^ _HASH_SEED);
	}

template<class _Elem,
	class _Traits,
	class _Alloc> inline
	size_t hash_value(const basic_string<_Elem, _Traits, _Alloc>& _Str)
	{	// hash string to size_t value
	return (_STD _Hash_seq((const unsigned char *)_Str.c_str(),
		_Str.size() * sizeof (_Elem)));
	}

inline size_t hash_value(_In_z_ const char *_Str)
	{	// hash NTBS to size_t value
	return (_STD _Hash_seq((const unsigned char *)_Str,
		_CSTD strlen(_Str)));
	}

inline size_t hash_value(_In_z_ const wchar_t *_Str)
	{	// hash NTWCS to size_t value
	return (_STD _Hash_seq((const unsigned char *)_Str,
		_CSTD wcslen(_Str) * sizeof (wchar_t)));
	}

		// TEMPLATE CLASS hash_compare
template<class _Kty,
	class _Pr = less<_Kty> >
	class hash_compare
	{	// traits class for hash containers
public:
	enum
		{	// parameters for hash table
		bucket_size = 1		// 0 < bucket_size
		};

	hash_compare(_Pr _Pred = _Pr())
		: comp(_Pred)
		{	// construct with _Pred comparator
		}

	size_t operator()(const _Kty& _Keyval) const
		{	// hash _Keyval to size_t value by pseudorandomizing transform
		long _Quot = (long)(hash_value(_Keyval) & LONG_MAX);
		ldiv_t _Qrem = _CSTD ldiv(_Quot, 127773);

		_Qrem.rem = 16807 * _Qrem.rem - 2836 * _Qrem.quot;
		if (_Qrem.rem < 0)
			_Qrem.rem += LONG_MAX;
		return ((size_t)_Qrem.rem);
		}

	bool operator()(const _Kty& _Keyval1, const _Kty& _Keyval2) const
		{	// test if _Keyval1 ordered before _Keyval2
		return (comp(_Keyval1, _Keyval2));
		}

	_Pr comp;	// the comparator object
	};
}	// namespace stdext
_STD_BEGIN
using stdext::hash_compare;
using stdext::hash_value;
_STD_END

_STD_BEGIN
		// TEMPLATE CLASS _Uhash_compare
template<class _Kty,
	class _Hasher,
	class _Keyeq>
	class _Uhash_compare
	{	// traits class for unordered containers
public:
	enum
		{	// parameters for hash table
		bucket_size = 1	// 0 < bucket_size
		};

	_Uhash_compare(_Hasher _Hasharg = _Hasher(), _Keyeq _Keyeqarg = _Keyeq())
		: _Mypair(_One_then_variadic_args_t(), _Hasharg,
			_One_then_variadic_args_t(), _Keyeqarg, 0.0f)
		{	// construct with hasher and equality comparator
		}

	size_t operator()(const _Kty& _Keyval) const
		{	// hash _Keyval to size_t value
		return ((size_t)_Gethash()(_Keyval));
		}

	bool operator()(const _Kty& _Keyval1, const _Kty& _Keyval2) const
		{	// test if _Keyval1 NOT equal to _Keyval2
		return (!_Getkeyeq()(_Keyval1, _Keyval2));
		}

	const _Hasher& _Gethash() const _NOEXCEPT
		{	// return const reference to hash function
		return (_Mypair._Get_first());
		}

	const _Keyeq& _Getkeyeq() const _NOEXCEPT
		{	// return const reference to key equality predicate
		return (_Mypair._Get_second()._Get_first());
		}

	float& _Get_max_bucket_size() _NOEXCEPT
		{	// return reference to current maximum bucket size
		return (_Mypair._Get_second()._Get_second());
		}

	const float& _Get_max_bucket_size() const _NOEXCEPT
		{	// return const reference to current maximum bucket size
		return (_Mypair._Get_second()._Get_second());
		}

	_Compressed_pair<_Hasher, _Compressed_pair<_Keyeq, float> > _Mypair;
	};

		// TEMPLATE CLASS _Hash
template<class _Traits>
	class _Hash
	{	// hash table -- list with vector of iterators for quick access
public:
	typedef _Hash<_Traits> _Myt;

	typedef typename _Traits::key_type key_type;
	typedef typename _Traits::key_compare key_compare;
	typedef typename _Traits::value_compare value_compare;
	enum
		{	// various constants
		_Bucket_size = key_compare::bucket_size,
		_Min_buckets = 8,	// min_buckets = 2 ^^ N, 0 < N
		_Multi = _Traits::_Multi};
	typedef list<typename _Traits::value_type,
		typename _Traits::allocator_type> _Mylist;
	typedef typename _Mylist::_Alty _Alty;

	typedef typename _Mylist::value_type value_type;
	typedef typename _Mylist::allocator_type allocator_type;
	typedef typename _Mylist::size_type size_type;
	typedef typename _Mylist::difference_type difference_type;
	typedef typename _Mylist::pointer pointer;
	typedef typename _Mylist::const_pointer const_pointer;
	typedef typename _Mylist::reference reference;
	typedef typename _Mylist::const_reference const_reference;

	typedef typename _If<is_same<key_type, value_type>::value,
		typename _Mylist::const_iterator,
		typename _Mylist::iterator>::type iterator;
	typedef typename _Mylist::const_iterator const_iterator;

	typedef typename _If<is_same<key_type, value_type>::value,
		typename _Mylist::_Unchecked_const_iterator,
		typename _Mylist::_Unchecked_iterator>::type
		_Unchecked_iterator;
	typedef typename _Mylist::_Unchecked_const_iterator
		_Unchecked_const_iterator;

	typedef vector<_Unchecked_iterator,
		typename _Alty::template
			rebind<_Unchecked_iterator>::other> _Myvec;

	typedef pair<iterator, bool> _Pairib;
	typedef pair<iterator, iterator> _Pairii;
	typedef pair<const_iterator, const_iterator> _Paircc;

	_Hash(const key_compare& _Parg,
		const allocator_type& _Al)
		: _Traitsobj(_Parg),

			_List(_Al),
			_Vec(_Al)

		{	// construct empty hash table
		_Max_bucket_size() = _Bucket_size;
		_Init();
		}

	template<class _Any_alloc>
		_Hash(const _Myt& _Right, const _Any_alloc& _Al)
		: _Traitsobj(_Right._Traitsobj),
			_List(allocator_type(_Al)),
			_Vec(_Al)
		{	// construct hash table by copying right
		_Copy(_Right);
		}

	_Hash(_Myt&& _Right)
		: _Traitsobj(_Right._Traitsobj),

			_List(_STD move(_Right._List)),
			_Vec(_STD move(_Right._Vec)),
			_Mask(_Right._Mask),
			_Maxidx(_Right._Maxidx)

		{	// construct hash table by moving _Right
		_Right.clear();
		}

	_Hash(_Myt&& _Right, const allocator_type& _Al)
		: _Traitsobj(_Right._Traitsobj),

			_List(_STD move(_Right._List), _Al),
			_Vec(_STD move(_Right._Vec), _Al),
			_Mask(_Right._Mask),
			_Maxidx(_Right._Maxidx)

		{	// construct hash table by moving _Right, allocator
		_Right.clear();
		}

	_Myt& operator=(_Myt&& _Right)
		{	// assign by moving _Right
		if (this != &_Right)
			_Assign_rv(_STD forward<_Myt>(_Right));
		return (*this);
		}

	void _Assign_rv(_Myt&& _Right)
		{	// assign by moving _Right
		_Right.swap(*this);
		_Right.clear();
		}

	template<class... _Valty>
		_Pairib emplace(_Valty&&... _Val)
		{	// try to insert value_type(_Val...)
		_List.emplace_front(_STD forward<_Valty>(_Val)...);
		return (_Insert(_List.front(), _Unchecked_begin()));
		}

	template<class... _Valty>
		iterator emplace_hint(const_iterator, _Valty&&... _Val)
		{	// try to insert value_type(_Val...), ignore hint
		_List.emplace_front(_STD forward<_Valty>(_Val)...);
		return (_Insert(_List.front(), _Unchecked_begin()).first);
		}

	~_Hash() _NOEXCEPT
		{	// destroy hash table
//		_List.clear();	// speed orphaning of checked iterators
		}

	_Myt& operator=(const _Myt& _Right)
		{	// replace contents from _Right
		if (this != &_Right)
			_Copy(_Right);
		return (*this);
		}

	iterator begin() _NOEXCEPT
		{	// return iterator for beginning of mutable sequence
		return (_List.begin());
		}

	const_iterator begin() const _NOEXCEPT
		{	// return iterator for beginning of nonmutable sequence
		return (_List.begin());
		}

	iterator end() _NOEXCEPT
		{	// return iterator for end of mutable sequence
		return (_List.end());
		}

	const_iterator end() const _NOEXCEPT
		{	// return iterator for end of nonmutable sequence
		return (_List.end());
		}

	_Unchecked_iterator _Unchecked_begin()
		{	// return iterator for beginning of mutable sequence
		return (_List._Unchecked_begin());
		}

	_Unchecked_const_iterator _Unchecked_begin() const
		{	// return iterator for beginning of nonmutable sequence
		return (_List._Unchecked_begin());
		}

	_Unchecked_iterator _Unchecked_end()
		{	// return iterator for end of mutable sequence
		return (_List._Unchecked_end());
		}

	_Unchecked_const_iterator _Unchecked_end() const
		{	// return iterator for end of nonmutable sequence
		return (_List._Unchecked_end());
		}

	iterator _Make_iter(_Unchecked_const_iterator _Where) const
		{	// make iterator from _Unchecked_const_iterator
		return (_List._Make_iter(_Where));
		}

	iterator _Make_iter(const_iterator _Where) const
		{	// make iterator from const_iterator
		return (_List._Make_iter(_Where));
		}

	const_iterator cbegin() const _NOEXCEPT
		{	// return iterator for beginning of nonmutable sequence
		return (begin());
		}

	const_iterator cend() const _NOEXCEPT
		{	// return iterator for end of nonmutable sequence
		return (end());
		}

	size_type size() const _NOEXCEPT
		{	// return length of sequence
		return (_List.size());
		}

	size_type max_size() const _NOEXCEPT
		{	// return maximum possible length of sequence
		return (_List.max_size());
		}

	bool empty() const _NOEXCEPT
		{	// return true only if sequence is empty
		return (_List.empty());
		}

	allocator_type get_allocator() const _NOEXCEPT
		{	// return allocator object for values
		allocator_type _Ret(_List.get_allocator());
		return (_Ret);
		}

	key_compare key_comp() const
		{	// return object for comparing keys
		return (_Traitsobj);
		}

	value_compare value_comp() const
		{	// return object for comparing values
		return (value_compare(key_comp()));
		}

	typedef iterator local_iterator;
	typedef const_iterator const_local_iterator;

	size_type bucket_count() const _NOEXCEPT
		{	// return number of buckets
		return (_Maxidx);
		}

	size_type max_bucket_count() const _NOEXCEPT
		{	// return maximum number of buckets
		return (_Vec.max_size() / 2);
		}

	size_type bucket(const key_type& _Keyval) const
		{	// return bucket corresponding to _Key
		return (_Hashval(_Keyval));
		}

	size_type bucket_size(size_type _Bucket) const
		{	// return size of bucket _Bucket
		size_type _Ans = 0;
		if (_Bucket < _Maxidx)
			for (_Unchecked_const_iterator _Plist = _Begin(_Bucket);
				_Plist != _End(_Bucket); ++_Plist)
				++_Ans;
		return (_Ans);
		}

	local_iterator begin(size_type _Bucket)
		{	// return iterator for bucket _Bucket
		if (_Bucket < bucket_count())
			return (_Make_iter(_Begin(_Bucket)));
		else
			return (end());
		}

	const_local_iterator begin(size_type _Bucket) const
		{	// return iterator for bucket _Bucket
		if (_Bucket < bucket_count())
			return (_Make_iter(_Begin(_Bucket)));
		else
			return (end());
		}

	local_iterator end(size_type _Bucket)
		{	// return iterator for bucket following _Bucket
		if (_Bucket < bucket_count())
			return (_Make_iter(_End(_Bucket)));
		else
			return (end());
		}

	const_local_iterator end(size_type _Bucket) const
		{	// return iterator for bucket following _Bucket
		if (_Bucket < bucket_count())
			return (_Make_iter(_End(_Bucket)));
		else
			return (end());
		}

	const_local_iterator cbegin(size_type _Bucket) const
		{	// return iterator for bucket _Bucket
		if (_Bucket < bucket_count())
			return (_Make_iter(_Begin(_Bucket)));
		else
			return (end());
		}

	const_local_iterator cend(size_type _Bucket) const
		{	// return iterator for bucket following _Bucket
		if (_Bucket < bucket_count())
			return (_Make_iter(_End(_Bucket)));
		else
			return (end());
		}

	float load_factor() const _NOEXCEPT
		{	// return elements per bucket
		return ((float)size() / (float)bucket_count());
		}

	float max_load_factor() const _NOEXCEPT
		{	// return maximum elements per bucket
		return (_Max_bucket_size());
		}

	void max_load_factor(float _Newmax)
		{	// set new load factor
		if (_Newmax != _Newmax	// may detect a NaN
			|| _Newmax < 0)
			_Xout_of_range("invalid hash load factor");

		_Max_bucket_size() = _Newmax;
		}

	void rehash(size_type _Buckets)
		{	// rebuild table with at least _Buckets buckets
		size_type _Maxsize = _Vec.max_size() / 4;
		size_type _Newsize = _Min_buckets;

		for (; _Newsize < _Buckets && _Newsize < _Maxsize; )
			_Newsize *= 2;	// double until big enough
		if (_Newsize < _Buckets)
			_Xout_of_range("invalid hash bucket count");
		while (!(size() / max_load_factor() < _Newsize)
			&& _Newsize < _Maxsize)
			_Newsize *= 2;	// double until load factor okay

		_Init(_Newsize);
		_Reinsert();
		}

	void reserve(size_type _Maxcount)
		{	// rebuild table with room for _Maxcount elements
		rehash((size_type)((float)(_Maxcount / max_load_factor() + 0.5F)));
		}

	template<bool _Multi2 = _Multi,
		enable_if_t<!_Multi2, int> = 0>
		_Pairib insert(const value_type& _Val)
		{	// try to insert node with value _Val
		return (_Insert(_Val, _Nil()));
		}

	template<bool _Multi2 = _Multi,
		enable_if_t<_Multi2, int> = 0>
		iterator insert(const value_type& _Val)
		{	// try to insert node with value _Val
		return (_Insert(_Val, _Nil()).first);
		}

	template<bool _Multi2 = _Multi,
		enable_if_t<!_Multi2, int> = 0>
		_Pairib insert(value_type&& _Val)
		{	// try to insert node with value _Val, favoring right side
		return (_Insert(_STD forward<value_type>(_Val), _Nil()));
		}

	template<bool _Multi2 = _Multi,
		enable_if_t<_Multi2, int> = 0>
		iterator insert(value_type&& _Val)
		{	// try to insert node with value _Val, favoring right side
		return (_Insert(_STD forward<value_type>(_Val), _Nil()).first);
		}

	iterator insert(const_iterator,
		const value_type& _Val)
		{	// try to insert node with value _Val, ignore hint
		return (_Insert(_Val, _Nil()).first);
		}

	iterator insert(const_iterator, value_type&& _Val)
		{	// try to insert node with value _Val, ignore hint
		return (_Insert(_STD forward<value_type>(_Val), _Nil()).first);
		}

	template<class _Iter>
		void insert(_Iter _First, _Iter _Last)
		{	// insert [_First, _Last) at front, then put in place
		_DEBUG_RANGE(_First, _Last);
		for (; _First != _Last; ++_First)
			emplace(*_First);
		}

	void insert(_XSTD initializer_list<value_type> _Ilist)
		{	// insert initializer_list
		insert(_Ilist.begin(), _Ilist.end());
		}

	iterator erase(const_iterator _Plist)
		{	// erase element at _Plist
		size_type _Bucket = _Hashval(_Traits::_Kfn(*_Plist));

		_Erase_bucket(_Make_iter(_Plist), _Bucket);
		return (_List.erase(_Plist));
		}

	iterator erase(const_iterator _First, const_iterator _Last)
		{	// erase [_First, _Last)
		_DEBUG_RANGE(_First, _Last);
		if (_First == begin() && _Last == end())
			{	// erase all
			clear();
			return (begin());
			}
		else
			{	// partial erase, one at a time
			while (_First != _Last)
				erase(_First++);
			return (_Make_iter(_First));
			}
		}

	size_type erase(const key_type& _Keyval)
		{	// erase and count all that match _Keyval
		_Pairii _Where = equal_range(_Keyval);
		size_type _Num = _STD distance(_Where.first, _Where.second);
		erase(_Where.first, _Where.second);
		return (_Num);
		}

	void clear() _NOEXCEPT
		{	// erase all
		_List.clear();
		_Init();
		}

	iterator find(const key_type& _Keyval)
		{	// find an element in mutable hash table that matches _Keyval
		return (lower_bound(_Keyval));
		}

	const_iterator find(const key_type& _Keyval) const
		{	// find an element in nonmutable hash table that matches _Keyval
		return (lower_bound(_Keyval));
		}

	size_type count(const key_type& _Keyval) const
		{	// count all elements that match _Keyval
		_Paircc _Ans = equal_range(_Keyval);
		return (_STD distance(_Ans.first, _Ans.second));
		}

	iterator lower_bound(const key_type& _Keyval)
		{	// find leftmost not less than _Keyval in mutable hash table
		size_type _Bucket = _Hashval(_Keyval);
		for (_Unchecked_iterator _Where = _Begin(_Bucket);
			_Where != _End(_Bucket); ++_Where)
			if (!_Traitsobj(_Traits::_Kfn(*_Where), _Keyval))
				return (_Traitsobj(_Keyval,
					_Traits::_Kfn(*_Where)) ? end() : _Make_iter(_Where));
		return (end());
		}

	const_iterator lower_bound(const key_type& _Keyval) const
		{	// find leftmost not less than _Keyval in nonmutable hash table
		size_type _Bucket = _Hashval(_Keyval);
		for (_Unchecked_const_iterator _Where = _Begin(_Bucket);
			_Where != _End(_Bucket); ++_Where)
			if (!_Traitsobj(_Traits::_Kfn(*_Where), _Keyval))
				return (_Traitsobj(_Keyval,
					_Traits::_Kfn(*_Where)) ? end() : _Make_iter(_Where));
		return (end());
		}

	iterator upper_bound(const key_type& _Keyval)
		{	// find leftmost not greater than _Keyval in mutable hash table
		size_type _Bucket = _Hashval(_Keyval);
		for (_Unchecked_iterator _Where = _End(_Bucket);
			_Where != _Begin(_Bucket); )
			if (!_Traitsobj(_Keyval, _Traits::_Kfn(*--_Where)))
				return (_Traitsobj(_Traits::_Kfn(*_Where),
					_Keyval) ? end() : _Make_iter(++_Where));
		return (end());
		}

	const_iterator upper_bound(const key_type& _Keyval) const
		{	// find leftmost not greater than _Keyval in nonmutable hash table
		size_type _Bucket = _Hashval(_Keyval);
		for (_Unchecked_const_iterator _Where = _End(_Bucket);
			_Where != _Begin(_Bucket); )
			if (!_Traitsobj(_Keyval, _Traits::_Kfn(*--_Where)))
				return (_Traitsobj(_Traits::_Kfn(*_Where),
					_Keyval) ? end() : _Make_iter(++_Where));
		return (end());
		}

	_Pairii equal_range(const key_type& _Keyval)
		{	// find range equivalent to _Keyval in mutable hash table
		size_type _Bucket = _Hashval(_Keyval);
		for (_Unchecked_iterator _Where = _Begin(_Bucket);
			_Where != _End(_Bucket); ++_Where)
			if (!_Traitsobj(_Traits::_Kfn(*_Where), _Keyval))
				{	// found _First, look for end of range
				_Unchecked_iterator _First = _Where;
				for (; _Where != _End(_Bucket); ++_Where)
					if (_Traitsobj(_Keyval, _Traits::_Kfn(*_Where)))
						break;
				if (_First == _Where)
					break;
				return (_Pairii(_Make_iter(_First),
					_Make_iter(_Where)));
				}
		return (_Pairii(end(), end()));
		}

	_Paircc equal_range(const key_type& _Keyval) const
		{	// find range equivalent to _Keyval in nonmutable hash table
		size_type _Bucket = _Hashval(_Keyval);
		for (_Unchecked_const_iterator _Where = _Begin(_Bucket);
			_Where != _End(_Bucket); ++_Where)
			if (!_Traitsobj(_Traits::_Kfn(*_Where), _Keyval))
				{	// found _First, look for end of range
				_Unchecked_const_iterator _First = _Where;
				for (; _Where != _End(_Bucket); ++_Where)
					if (_Traitsobj(_Keyval, _Traits::_Kfn(*_Where)))
						break;
				if (_First == _Where)
					break;
				return (_Paircc(_Make_iter(_First),
					_Make_iter(_Where)));
				}
		return (_Paircc(end(), end()));
		}

	void swap(_Myt& _Right)
		{	// exchange contents with _Right
		if (this != &_Right)
			{	// different, worth swapping
			_Swap_adl(_Traitsobj, _Right._Traitsobj);

			this->_List.swap(_Right._List);
			this->_Vec.swap(_Right._Vec);
			_Swap_adl(this->_Mask, _Right._Mask);
			_Swap_adl(this->_Maxidx, _Right._Maxidx);
			}
		}

protected:
	template<class _Valty>
		_Unchecked_iterator _Buynode_if_nil(_Valty&&,
			_Unchecked_iterator _Plist)
		{	// node exists, just return it
		return (_Plist);
		}

	template<class _Valty>
		_Unchecked_iterator _Buynode_if_nil(_Valty&& _Val, _Nil)
		{	// node doesn't exist, make it
		_List.push_front(_STD forward<_Valty>(_Val));
		return (_Unchecked_begin());
		}

	void _Destroy_if_not_nil(_Unchecked_iterator _Plist)
		{	// node exists, destroy it
		_List.erase(_Make_iter(_Plist));
		}

	void _Destroy_if_not_nil(_Nil)
		{	// node doesn't exist, do nothing
		}

	template<class _Valty,
		class _Nodety>
		_Pairib _Insert(_Valty&& _Val, _Nodety _Pnode)
		{	// try to insert existing node with value _Val
		size_type _Bucket;
		_Unchecked_iterator _Where;

		_TRY_BEGIN
		_Bucket = _Hashval(_Traits::_Kfn(_Val));
		_Where = _End(_Bucket);
		for (; _Where != _Begin(_Bucket); )
			if (_Traitsobj(_Traits::_Kfn(_Val),
				_Traits::_Kfn(*--_Where)))
				;	// still too high in bucket list
			else if (_Multi
				|| (!_Traits::_Standard
					&& _Traitsobj(_Traits::_Kfn(*_Where),
						_Traits::_Kfn(_Val))))
				{	// found insertion point, back up to it
				++_Where;
				break;
				}
			else
				{	// discard new list element and return existing
				_Destroy_if_not_nil(_Pnode);
				return (_Pairib(_Make_iter(_Where), false));
				}
		_CATCH_ALL
		_Destroy_if_not_nil(_Pnode);
		_RERAISE;
		_CATCH_END

		_Unchecked_iterator _Plist =
			_Buynode_if_nil(_STD forward<_Valty>(_Val), _Pnode);
		_Unchecked_iterator _Next = _Plist;

		if (_Where != ++_Next)	// move element into place
			_List._Unchecked_splice(_Where, _Plist, _Next);

		_Insert_bucket(_Plist, _Where, _Bucket);

		_TRY_BEGIN
		_Check_size();
		_CATCH_ALL
		erase(_Make_iter(_Plist));
		_RERAISE;
		_CATCH_END

		return (_Pairib(_Make_iter(_Plist), true));
		}

	_Unchecked_iterator& _Vec_lo(size_type _Bucket)
		{	// return reference to begin() for _Bucket
		return (_Vec[2 * _Bucket]);
		}

	_Unchecked_const_iterator& _Vec_lo(size_type _Bucket) const
		{	// return reference to begin() for _Bucket
		return ((_Unchecked_const_iterator&)_Vec[2 * _Bucket]);
		}

	_Unchecked_iterator& _Vec_hi(size_type _Bucket)
		{	// return reference to end()-1 for _Bucket
		return (_Vec[2 * _Bucket + 1]);
		}

	_Unchecked_const_iterator& _Vec_hi(size_type _Bucket) const
		{	// return reference to end()-1 for _Bucket
		return ((_Unchecked_const_iterator&)_Vec[2 * _Bucket + 1]);
		}

	_Unchecked_iterator _Begin(size_type _Bucket)
		{	// return begin iterator for bucket _Bucket
		return (_Vec_lo(_Bucket));
		}

	_Unchecked_const_iterator _Begin(size_type _Bucket) const
		{	// return begin iterator for bucket _Bucket
		return (_Vec_lo(_Bucket));
		}

	_Unchecked_iterator _End(size_type _Bucket)
		{	// return end iterator for bucket _Bucket
		if (_Vec_lo(_Bucket) == _Unchecked_end())
			return (_Unchecked_end());
		else
			{	// point past last element
			_Unchecked_iterator _Ans = _Vec_hi(_Bucket);
			return (++_Ans);
			}
		}
	_Unchecked_const_iterator _End(size_type _Bucket) const
		{	// return end iterator for bucket _Bucket
		if (_Vec_lo(_Bucket) == _Unchecked_end())
			return (_Unchecked_end());
		else
			{	// point past last element
			_Unchecked_const_iterator _Ans = _Vec_hi(_Bucket);
			return (++_Ans);
			}
		}

	void _Erase_bucket(iterator _Plist_arg, size_type _Bucket)
		{	// fix iterators before erasing _Plist before _Where
		_Unchecked_iterator _Plist = _Plist_arg._Unchecked();
		if (_Vec_hi(_Bucket) == _Plist)
			if (_Vec_lo(_Bucket) == _Plist)
				{	// make bucket empty
				_Vec_lo(_Bucket) = _Unchecked_end();
				_Vec_hi(_Bucket) = _Unchecked_end();
				}
			else
				_Vec_hi(_Bucket) = --_Plist;	// move end back one element
		else if (_Vec_lo(_Bucket) == _Plist)
			_Vec_lo(_Bucket) = ++_Plist;	// move beginning up one element
		}

	void _Insert_bucket(_Unchecked_iterator _Plist,
		_Unchecked_iterator _Where, size_type _Bucket)
		{	// fix iterators after inserting _Plist before _Where
		if (_Vec_lo(_Bucket) == _Unchecked_end())
			{	// make bucket non-empty
			_Vec_lo(_Bucket) = _Plist;
			_Vec_hi(_Bucket) = _Plist;
			}
		else if (_Vec_lo(_Bucket) == _Where)
			_Vec_lo(_Bucket) = _Plist;	// move beginning back one element
		else if (++_Vec_hi(_Bucket) != _Plist)	// move end up one element
			--_Vec_hi(_Bucket);	// or not
		}

	void _Copy(const _Myt& _Right)
		{	// copy entire hash table
		_Mask = _Right._Mask;
		_Maxidx = _Right._Maxidx;
		_List.clear();

		_TRY_BEGIN
		_Traitsobj = _Right._Traitsobj;
		_Vec.assign(_Right._Vec.size(), _Unchecked_end());
		insert(_Right.begin(), _Right.end());
		_CATCH_ALL
		clear();	// list or compare copy failed, bail out
		_RERAISE;
		_CATCH_END
		}

	size_type _Hashval(const key_type& _Keyval) const
		{	// return hash value, masked to current table size
		return (_Traitsobj(_Keyval) & _Mask);
		}

	void _Init(size_type _Buckets = _Min_buckets)
		{	// initialize hash table with _Buckets buckets, leave list alone
		_Vec.reserve(2 * _Buckets);	// avoid curdling _Vec if exception occurs
		_Vec.assign(2 * _Buckets, _Unchecked_end());
		_Mask = _Buckets - 1;
		_Maxidx = _Buckets;
		}

	void _Check_size()
		{	// grow table as needed
		if (max_load_factor() < load_factor())

			{	// rehash to bigger table
			size_type _Newsize = bucket_count();

			if (_Newsize < 512)
				_Newsize *= 8;	// multiply by 8
			else if (_Newsize < _Vec.max_size() / 2)
				_Newsize *= 2;	// multiply safely by 2
			_Init(_Newsize);
			_Reinsert();
			}
		}

	void _Reinsert()
		{	// insert elements in [begin(), end())
		_Unchecked_iterator _Last = _Unchecked_end();
		if (_Unchecked_begin() != _Last)
			for (--_Last; ; )
				{	// reinsert elements in [begin(), _Last]
				_Unchecked_iterator _First = _Unchecked_begin();
				bool _Done = _First == _Last;
				_Insert(*_First, _First);
				if (_Done)
					break;
				}
		}

	float& _Max_bucket_size() _NOEXCEPT
		{	// return reference to current maximum bucket size
		return (_Traitsobj._Get_max_bucket_size());
		}

	const float& _Max_bucket_size() const _NOEXCEPT
		{	// return const reference to current maximum bucket size
		return (_Traitsobj._Get_max_bucket_size());
		}

	_Traits _Traitsobj;	// traits to customize behavior
	_Mylist _List;	// list of elements, must initialize before _Vec
	_Myvec _Vec;	// vector of list iterators, begin() then end()-1
	size_type _Mask;	// the key mask
	size_type _Maxidx;	// current maximum key value
	};

		// _Hash TEMPLATE OPERATORS
template<class _Traits> inline
	bool _Hash_equal_elements(
		const _Hash<_Traits>& _Left,
		const _Hash<_Traits>& _Right,
		false_type)
	{	// test for _Hash equality (same size, unique keys)
	typedef typename _Hash<_Traits>::const_iterator _Iter;
	for (_Iter _Next1 = _Left.begin(); _Next1 != _Left.end(); ++_Next1)
		{	// look for element with equivalent key
		_Iter _Next2 = _Right.find(_Traits::_Kfn(*_Next1));
		if (_Next2 == _Right.end()
			|| !(_Traits::_Nonkfn(*_Next1) == _Traits::_Nonkfn(*_Next2)))
			return (false);
		}
	return (true);
	}

template<class _Traits> inline
	bool _Hash_equal_elements(
		const _Hash<_Traits>& _Left,
		const _Hash<_Traits>& _Right,
		true_type)
	{	// test for _Hash equality (same size, equivalent keys)
	typedef typename _Hash<_Traits>::const_iterator _Iter;
	typedef pair<_Iter, _Iter> _Iter_pair;

	for (_Iter _Next1 = _Left.begin(); _Next1 != _Left.end(); )
		{	// look for elements with equivalent keys
		_Iter_pair _Lrange = _Left.equal_range(_Traits::_Kfn(*_Next1));
		_Iter_pair _Rrange = _Right.equal_range(_Traits::_Kfn(*_Next1));

		if (!_STD is_permutation(_Lrange.first, _Lrange.second,
			_Rrange.first, _Rrange.second))
			return (false);
		_Next1 = _Lrange.second;	// continue just past range
		}
	return (true);
	}

template<class _Traits> inline
	bool _Hash_equal(
		const _Hash<_Traits>& _Left,
		const _Hash<_Traits>& _Right)
	{	// test for _Hash equality
	return (_Left.size() == _Right.size() && _Hash_equal_elements(
		_Left, _Right, integral_constant<bool, _Traits::_Multi>()));
	}
_STD_END

 #pragma pop_macro("new")
 #pragma warning(pop)
 #pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _XHASH_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
