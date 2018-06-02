// algorithm standard header

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _ALGORITHM_
#define _ALGORITHM_
#include <iterator>
#include <memory>
#include <xutility>

#ifdef  _MSC_VER
#pragma pack(push,8)
#endif  /* _MSC_VER */
_STD_BEGIN
const int _CHUNK_SIZE = 7;
const int _SORT_MAX = 16;
		// TEMPLATE FUNCTION _Median
template<class _Ty> inline
	_Ty _Median(_Ty _X, _Ty _Y, _Ty _Z)
		{if (_X < _Y)
			return (_Y < _Z ? _Y : _X < _Z ? _Z : _X);
		else
			return (_X < _Z ? _X : _Y < _Z ? _Z : _Y); }
		// TEMPLATE FUNCTION _Median WITH PRED
template<class _Ty, class _Pr> inline
	_Ty _Median(_Ty _X, _Ty _Y, _Ty _Z, _Pr _P)
		{if (_P(_X, _Y))
			return (_P(_Y, _Z) ? _Y : _P(_X, _Z) ? _Z : _X);
		else
			return (_P(_X, _Z) ? _X : _P(_Y, _Z) ? _Z : _Y); }
		// TEMPLATE FUNCTION for_each
template<class _II, class _Fn> inline
	_Fn for_each(_II _F, _II _L, _Fn _Op)
	{for (; _F != _L; ++_F)
		_Op(*_F);
	return (_Op); }
		// TEMPLATE FUNCTION find
template<class _II, class _Ty> inline
	_II find(_II _F, _II _L, const _Ty& _V)
	{for (; _F != _L; ++_F)
		if (*_F == _V)
			break;
	return (_F); }
		// TEMPLATE FUNCTION find_if
template<class _II, class _Pr> inline
	_II find_if(_II _F, _II _L, _Pr _P)
	{for (; _F != _L; ++_F)
		if (_P(*_F))
			break;
	return (_F); }
		// TEMPLATE FUNCTION adjacent_find
template<class _FI> inline
	_FI adjacent_find(_FI _F, _FI _L)
	{for (_FI _Fb; (_Fb = _F) != _L && ++_F != _L; )
		if (*_Fb == *_F)
			return (_Fb);
	return (_L); }
		// TEMPLATE FUNCTION adjacent_find WITH PRED
template<class _FI, class _Pr> inline
	_FI adjacent_find(_FI _F, _FI _L, _Pr _P)
	{for (_FI _Fb; (_Fb = _F) != _L && ++_F != _L; )
		if (_P(*_Fb, *_F))
			return (_Fb);
	return (_L); }
		// TEMPLATE FUNCTION count
template<class _II, class _Ty> inline
	_CNTSIZ(_II) count(_II _F, _II _L, const _Ty& _V)
	{_CNTSIZ(_II) _N = 0;
	for (; _F != _L; ++_F)
		if (*_F == _V)
			++_N;
	return (_N); }
		// TEMPLATE FUNCTION count_if
template<class _II, class _Pr> inline
	_CNTSIZ(_II) count_if(_II _F, _II _L, _Pr _P)
	{_CNTSIZ(_II) _N = 0;
	for (; _F != _L; ++_F)
		if (_P(*_F))
			++_N;
	return (_N); }
		// TEMPLATE FUNCTION search
template<class _FI1, class _FI2> inline
	_FI1 search(_FI1 _F1, _FI1 _L1, _FI2 _F2, _FI2 _L2)
	{return (_Search(_F1, _L1, _F2, _L2,
		_Dist_type(_F1), _Dist_type(_F2))); }
template<class _FI1, class _FI2, class _Pd1, class _Pd2> inline
	_FI1 _Search(_FI1 _F1, _FI1 _L1, _FI2 _F2, _FI2 _L2,
		_Pd1 *, _Pd2 *)
	{_Pd1 _D1 = 0;
	_Distance(_F1, _L1, _D1);
	_Pd2 _D2 = 0;
	_Distance(_F2, _L2, _D2);
	for (; _D2 <= _D1; ++_F1, --_D1)
		{_FI1 _X1 = _F1;
		for (_FI2 _X2 = _F2; ; ++_X1, ++_X2)
			if (_X2 == _L2)
				return (_F1);
			else if (!(*_X1 == *_X2))
				break; }
	return (_L1); }
		// TEMPLATE FUNCTION search WITH PRED
template<class _FI1, class _FI2, class _Pr> inline
	_FI1 search(_FI1 _F1, _FI1 _L1, _FI2 _F2, _FI2 _L2, _Pr _P)
	{return (_Search(_F1, _L1, _F2, _L2, _P,
		_Dist_type(_F1), _Dist_type(_F2))); }
template<class _FI1, class _FI2, class _Pd1, class _Pd2,
	class _Pr> inline
	_FI1 _Search(_FI1 _F1, _FI1 _L1, _FI2 _F2, _FI2 _L2,
		_Pr _P, _Pd1 *, _Pd2 *)
	{_Pd1 _D1 = 0;
	_Distance(_F1, _L1, _D1);
	_Pd2 _D2 = 0;
	_Distance(_F2, _L2, _D2);
	for (; _D2 <= _D1; ++_F1, --_D1)
		{_FI1 _X1 = _F1;
		for (_FI2 _X2 = _F2; ; ++_X1, ++_X2)
			if (_X2 == _L2)
				return (_F1);
			else if (!_P(*_X1, *_X2))
				break; }
	return (_L1); }
		// TEMPLATE FUNCTION search_n
template<class _FI1, class _Pd2, class _Ty> inline
	_FI1 search_n(_FI1 _F1, _FI1 _L1, _Pd2 _N, const _Ty& _V)
	{return (_Search_n(_F1, _L1, _N, _V, _Dist_type(_F1))); }
template<class _FI1, class _Pd2, class _Ty, class _Pd1> inline
	_FI1 _Search_n(_FI1 _F1, _FI1 _L1,
		_Pd2 _N, const _Ty& _V, _Pd1 *)
	{_Pd1 _D1 = 0;
	_Distance(_F1, _L1, _D1);
	for (; _N <= _D1; ++_F1, --_D1)
		{_FI1 _X1 = _F1;
		for (_Pd2 _D2 = _N; ; ++_X1, --_D2)
			if (_D2 == 0)
				return (_F1);
			else if (!(*_X1 == _V))
				break; }
	return (_L1); }
		// TEMPLATE FUNCTION search_n WITH PRED
template<class _FI1, class _Pd2, class _Ty, class _Pr> inline
	_FI1 search_n(_FI1 _F1, _FI1 _L1,
		_Pd2 _N, const _Ty& _V, _Pr _P)
	{return (_Search_n(_F1, _L1,
		_N, _V, _P, _Dist_type(_F1))); }
template<class _FI1, class _Pd2,
	class _Ty, class _Pd1, class _Pr> inline
	_FI1 _Search_n(_FI1 _F1, _FI1 _L1,
		_Pd2 _N, const _Ty& _V, _Pr _P, _Pd1 *)
	{_Pd1 _D1 = 0;
	_Distance(_F1, _L1, _D1);
	for (; _N <= _D1; ++_F1, --_D1)
		{_FI1 _X1 = _F1;
		for (_Pd2 _D2 = _N; ; ++_X1, --_D2)
			if (_D2 == 0)
				return (_F1);
			else if (!_P(*_X1, _V))
				break; }
	return (_L1); }
		// TEMPLATE FUNCTION find_end
template<class _FI1, class _FI2> inline
	_FI1 find_end(_FI1 _F1, _FI1 _L1, _FI2 _F2, _FI2 _L2)
	{return (_Find_end(_F1, _L1, _F2, _L2,
		_Dist_type(_F1), _Dist_type(_F2))); }
template<class _FI1, class _FI2, class _Pd1, class _Pd2> inline
	_FI1 _Find_end(_FI1 _F1, _FI1 _L1, _FI2 _F2, _FI2 _L2,
		_Pd1 *, _Pd2 *)
	{_Pd1 _D1 = 0;
	_Distance(_F1, _L1, _D1);
	_Pd2 _D2 = 0;
	_Distance(_F2, _L2, _D2);
	_FI1 _Ans = _L1;
	if (0 < _D2)
		for (; _D2 <= _D1; ++_F1, --_D1)
			{_FI1 _X1 = _F1;
			for (_FI2 _X2 = _F2; ; ++_X1)
				if (!(*_X1 == *_X2))
					break;
				else if (++_X2 == _L2)
					{_Ans = _F1;
					break; }}
	return (_Ans); }
		// TEMPLATE FUNCTION find_end WITH PRED
template<class _FI1, class _FI2, class _Pr> inline
	_FI1 find_end(_FI1 _F1, _FI1 _L1, _FI2 _F2, _FI2 _L2, _Pr _P)
	{return (_Find_end(_F1, _L1, _F2, _L2, _P,
		_Dist_type(_F1), _Dist_type(_F2))); }
template<class _FI1, class _FI2, class _Pd1, class _Pd2,
	class _Pr> inline
	_FI1 _Find_end(_FI1 _F1, _FI1 _L1, _FI2 _F2, _FI2 _L2, _Pr _P,
		_Pd1 *, _Pd2 *)
	{_Pd1 _D1 = 0;
	_Distance(_F1, _L1, _D1);
	_Pd2 _D2 = 0;
	_Distance(_F2, _L2, _D2);
	_FI1 _Ans = _L1;
	if (0 < _D2)
		for (; _D2 <= _D1; ++_F1, --_D1)
			{_FI1 _X1 = _F1;
			for (_FI2 _X2 = _F2; ; ++_X1)
				if (!_P(*_X1, *_X2))
					break;
				else if (++_X2 == _L2)
					{_Ans = _F1;
					break; }}
	return (_Ans); }
		// TEMPLATE FUNCTION find_first_of
template<class _FI1, class _FI2> inline
	_FI1 find_first_of(_FI1 _F1, _FI1 _L1, _FI2 _F2, _FI2 _L2)
	{for (; _F1 != _L1; ++_F1)
		for (_FI2 _X2 = _F2; _X2 != _L2; ++_X2)
			if (*_F1 == *_X2)
				return (_F1);
	return (_F1); }
		// TEMPLATE FUNCTION find_first_of WITH PRED
template<class _FI1, class _FI2, class _Pr> inline
	_FI1 find_first_of(_FI1 _F1, _FI1 _L1, _FI2 _F2, _FI2 _L2,
		_Pr _P)
	{for (; _F1 != _L1; ++_F1)
		for (_FI2 _X2 = _F2; _X2 != _L2; ++_X2)
			if (_P(*_F1, *_X2))
				return (_F1);
	return (_F1); }
		// TEMPLATE FUNCTION iter_swap
template<class _FI1, class _FI2> inline
	void iter_swap(_FI1 _X, _FI2 _Y)
	{_Iter_swap(_X, _Y, _Val_type(_X)); }
template<class _FI1, class _FI2, class _Ty> inline
	void _Iter_swap(_FI1 _X, _FI2 _Y, _Ty *)
	{_Ty _Tmp = *_X;
	*_X = *_Y, *_Y = _Tmp; }
		// TEMPLATE FUNCTION swap_ranges
template<class _FI1, class _FI2> inline
	_FI2 swap_ranges(_FI1 _F, _FI1 _L, _FI2 _X)
	{for (; _F != _L; ++_F, ++_X)
		iter_swap(_F, _X);
	return (_X); }
		// TEMPLATE FUNCTION transform WITH UNARY OP
template<class _II, class _OI, class _Uop> inline
	_OI transform(_II _F, _II _L, _OI _X, _Uop _U)
	{for (; _F != _L; ++_F, ++_X)
		*_X = _U(*_F);
	return (_X); }
		// TEMPLATE FUNCTION transform WITH BINARY OP
template<class _II1, class _II2, class _OI, class _Bop> inline
	_OI transform(_II1 _F1, _II1 _L1, _II2 _F2, _OI _X, _Bop _B)
	{for (; _F1 != _L1; ++_F1, ++_F2, ++_X)
		*_X = _B(*_F1, *_F2);
	return (_X); }
		// TEMPLATE FUNCTION replace
template<class _FI, class _Ty> inline
	void replace(_FI _F, _FI _L, const _Ty& _Vo, const _Ty& _Vn)
	{for (; _F != _L; ++_F)
		if (*_F == _Vo)
			*_F = _Vn; }
		// TEMPLATE FUNCTION replace_if
template<class _FI, class _Pr, class _Ty> inline
	void replace_if(_FI _F, _FI _L, _Pr _P, const _Ty& _V)
	{for (; _F != _L; ++_F)
		if (_P(*_F))
			*_F = _V; }
		// TEMPLATE FUNCTION replace_copy
template<class _II, class _OI, class _Ty> inline
	_OI replace_copy(_II _F, _II _L, _OI _X,
		const _Ty& _Vo, const _Ty& _Vn)
	{for (; _F != _L; ++_F, ++_X)
		*_X = *_F == _Vo ? _Vn : *_F;
		return (_X); }
		// TEMPLATE FUNCTION replace_copy_if
template<class _II, class _OI, class _Pr, class _Ty> inline
	_OI replace_copy_if(_II _F, _II _L, _OI _X,
		_Pr _P, const _Ty& _V)
	{for (; _F != _L; ++_F, ++_X)
		*_X = _P(*_F) ? _V : *_F;
		return (_X); }
		// TEMPLATE FUNCTION generate
template<class _FI, class _Gen> inline
	void generate(_FI _F, _FI _L, _Gen _G)
	{for (; _F != _L; ++_F)
		*_F = _G(); }
		// TEMPLATE FUNCTION generate_n
template<class _OI, class _Pd, class _Gen> inline
	void generate_n(_OI _F, _Pd _N, _Gen _G)
	{for (; 0 < _N; --_N, ++_F)
		*_F = _G(); }
		// TEMPLATE FUNCTION remove
template<class _FI, class _Ty> inline
	_FI remove(_FI _F, _FI _L, const _Ty& _V)
	{_F = find(_F, _L, _V);
	if (_F == _L)
		return (_F);
	else
		{_FI _Fb = _F;
		return (remove_copy(++_F, _L, _Fb, _V)); }}
		// TEMPLATE FUNCTION remove_if
template<class _FI, class _Pr> inline
	_FI remove_if(_FI _F, _FI _L, _Pr _P)
	{_F = find_if(_F, _L, _P);
	if (_F == _L)
		return (_F);
	else
		{_FI _Fb = _F;
		return (remove_copy_if(++_F, _L, _Fb, _P)); }}
		// TEMPLATE FUNCTION remove_copy
template<class _II, class _OI, class _Ty> inline
	_OI remove_copy(_II _F, _II _L, _OI _X, const _Ty& _V)
	{for (; _F != _L; ++_F)
		if (!(*_F == _V))
			*_X++ = *_F;
	return (_X); }
		// TEMPLATE FUNCTION remove_copy_if
template<class _II, class _OI, class _Pr> inline
	_OI remove_copy_if(_II _F, _II _L, _OI _X, _Pr _P)
	{for (; _F != _L; ++_F)
		if (!_P(*_F))
			*_X++ = *_F;
	return (_X); }
		// TEMPLATE FUNCTION unique
template<class _FI> inline
	_FI unique(_FI _F, _FI _L)
	{_F = adjacent_find(_F, _L);
	return (unique_copy(_F, _L, _F)); }
		// TEMPLATE FUNCTION unique WITH PRED
template<class _FI, class _Pr> inline
	_FI unique(_FI _F, _FI _L, _Pr _P)
	{_F = adjacent_find(_F, _L, _P);
	return (unique_copy(_F, _L, _F, _P)); }
		// TEMPLATE FUNCTION unique_copy
template<class _II, class _OI> inline
	_OI unique_copy(_II _F, _II _L, _OI _X)
	{return (_F == _L ? _X :
		_Unique_copy(_F, _L, _X, _Iter_cat(_F))); }
template<class _II, class _OI> inline
	_OI _Unique_copy(_II _F, _II _L, _OI _X, input_iterator_tag)
	{return (_Unique_copy(_F, _L, _X, _Val_type(_F))); }
template<class _II, class _OI, class _Ty> inline
	_OI _Unique_copy(_II _F, _II _L, _OI _X, _Ty *)
	{_Ty _V = *_F;
	for (*_X++ = _V; ++_F != _L; )
		if (!(_V == *_F))
			_V = *_F, *_X++ = _V;
	return (_X); }
template<class _FI, class _OI> inline
	_OI _Unique_copy(_FI _F, _FI _L, _OI _X, forward_iterator_tag)
	{_FI _Fb = _F;
	for (*_X++ = *_Fb; ++_F != _L; )
		if (!(*_Fb == *_F))
			_Fb = _F, *_X++ = *_Fb;
	return (_X); }
template<class _BI, class _OI> inline
	_OI _Unique_copy(_BI _F, _BI _L, _OI _X,
		bidirectional_iterator_tag)
	{return (_Unique_copy(_F, _L, _X, forward_iterator_tag())); }
template<class _RI, class _OI> inline
	_OI _Unique_copy(_RI _F, _RI _L, _OI _X,
		random_access_iterator_tag)
	{return (_Unique_copy(_F, _L, _X, forward_iterator_tag())); }
		// TEMPLATE FUNCTION unique_copy WITH PRED
template<class _II, class _OI, class _Pr> inline
	_OI unique_copy(_II _F, _II _L, _OI _X, _Pr _P)
	{return (_F == _L ? _X :
		_Unique_copy(_F, _L, _X, _P, _Iter_cat(_F))); }
template<class _II, class _OI, class _Pr> inline
	_OI _Unique_copy(_II _F, _II _L, _OI _X, _Pr _P,
		input_iterator_tag)
	{return (_Unique_copy(_F, _L, _X, _P, _Val_type(_F))); }
template<class _II, class _OI, class _Ty, class _Pr> inline
	_OI _Unique_copy(_II _F, _II _L, _OI _X, _Pr _P, _Ty *)
	{_Ty _V = *_F;
	for (*_X++ = _V; ++_F != _L; )
		if (!_P(_V, *_F))
			_V = *_F, *_X++ = _V;
	return (_X); }
template<class _FI, class _OI, class _Pr> inline
	_OI _Unique_copy(_FI _F, _FI _L, _OI _X, _Pr _P,
		forward_iterator_tag)
	{_FI _Fb = _F;
	for (*_X++ = *_Fb; ++_F != _L; )
		if (!_P(*_Fb, *_F))
			_Fb = _F, *_X++ = *_Fb;
	return (_X); }
template<class _BI, class _OI, class _Pr> inline
	_OI _Unique_copy(_BI _F, _BI _L, _OI _X, _Pr _P,
		bidirectional_iterator_tag)
	{return (_Unique_copy(_F, _L, _X, _P,
		forward_iterator_tag())); }
template<class _RI, class _OI, class _Pr> inline
	_OI _Unique_copy(_RI _F, _RI _L, _OI _X, _Pr _P,
		random_access_iterator_tag)
	{return (_Unique_copy(_F, _L, _X, _P,
		forward_iterator_tag())); }
		// TEMPLATE FUNCTION reverse
template<class _BI> inline
	void reverse(_BI _F, _BI _L)
	{_Reverse(_F, _L, _Iter_cat(_F)); }
template<class _BI> inline
	void _Reverse(_BI _F, _BI _L, bidirectional_iterator_tag)
	{for (; _F != _L && _F != --_L; ++_F)
		iter_swap(_F, _L); }
template<class _RI> inline
	void _Reverse(_RI _F, _RI _L, random_access_iterator_tag)
	{for (; _F < _L; ++_F)
		iter_swap(_F, --_L); }
		// TEMPLATE FUNCTION reverse_copy
template<class _BI, class _OI> inline
	_OI reverse_copy(_BI _F, _BI _L, _OI _X)
	{for (; _F != _L; ++_X)
		*_X = *--_L;
	return (_X); }
		// TEMPLATE FUNCTION rotate
template<class _FI> inline
	void rotate(_FI _F, _FI _M, _FI _L)
	{if (_F != _M && _M != _L)
		_Rotate(_F, _M, _L, _Iter_cat(_F)); }
template<class _FI> inline
	void _Rotate(_FI _F, _FI _M, _FI _L,
		forward_iterator_tag)
	{for (_FI _X = _M; ; )
		{iter_swap(_F, _X);
		if (++_F == _M)
			if (++_X == _L)
				break;
			else
				_M = _X;
		else if (++_X == _L)
			_X = _M; }}
template<class _BI> inline
	void _Rotate(_BI _F, _BI _M, _BI _L,
		bidirectional_iterator_tag)
	{reverse(_F, _M);
	reverse(_M, _L);
	reverse(_F, _L); }
template<class _RI> inline
	void _Rotate(_RI _F, _RI _M, _RI _L,
			random_access_iterator_tag)
	{_Rotate(_F, _M, _L, _Dist_type(_F), _Val_type(_F)); }
template<class _RI, class _Pd, class _Ty> inline
	void _Rotate(_RI _F, _RI _M, _RI _L, _Pd *, _Ty *)
	{_Pd _D = _M - _F;
	_Pd _N = _L - _F;
	for (_Pd _I = _D; _I != 0; )
		{_Pd _J = _N % _I;
		_N = _I, _I = _J; }
	if (_N < _L - _F)
		for (; 0 < _N; --_N)
			{_RI _X = _F + _N;
			_RI _Y = _X;
			_Ty _V = *_X;
			_RI _Z = _Y + _D == _L ? _F : _Y + _D;
			while (_Z != _X)
				{*_Y = *_Z;
				_Y = _Z;
				_Z = _D < _L - _Z ? _Z + _D
					: _F + (_D - (_L - _Z)); }
			*_Y = _V; }}
		// TEMPLATE FUNCTION rotate_copy
template<class _FI, class _OI> inline
	_OI rotate_copy(_FI _F, _FI _M, _FI _L, _OI _X)
	{return (copy(_F, _M, copy(_M, _L, _X))); }
		// TEMPLATE FUNCTION random_shuffle
template<class _RI> inline
	void random_shuffle(_RI _F, _RI _L)
	{if (_F != _L)
		_Random_shuffle(_F, _L, _Dist_type(_F)); }
template<class _RI, class _Pd> inline
	void _Random_shuffle(_RI _F, _RI _L, _Pd *)
	{const int _RBITS = 15;
	const int _RMAX = (1U << _RBITS) - 1;
	_RI _X = _F;
	for (_Pd _D = 1; ++_X != _L; ++_D)
		{unsigned long _Rm = _RMAX;
		unsigned long _Rn = rand() & _RMAX;
		for (; _Rm < _D && _Rm != ~0UL;
			_Rm = _Rm << _RBITS | _RMAX)
			_Rn = _Rn << _RBITS | _RMAX;
		iter_swap(_X, _F + _Pd(_Rn % _D)); }}
template<class _RI, class _Pf> inline
	void random_shuffle(_RI _F, _RI _L, _Pf& _R)
	{if (_F != _L)
		_Random_shuffle(_F, _L, _R, _Dist_type(_F)); }
template<class _RI, class _Pf, class _Pd> inline
	void _Random_shuffle(_RI _F, _RI _L, _Pf& _R, _Pd *)
	{_RI _X = _F;
	for (_Pd _D = 1; ++_X != _L; ++_D)
		iter_swap(_X, _F + _Pd(_R(_D))); }
		// TEMPLATE FUNCTION partition
template<class _BI, class _Pr> inline
	_BI partition(_BI _F, _BI _L, _Pr _P)
	{for (; ; ++_F)
		{for (; _F != _L && _P(*_F); ++_F)
			;
		if (_F == _L)
			break;
		for (; _F != --_L && !_P(*_L); )
			;
		if (_F == _L)
			break;
		iter_swap(_F, _L); }
	return (_F); }
		// TEMPLATE FUNCTION stable_partition
template<class _FI, class _Pr> inline
	_FI stable_partition(_FI _F, _FI _L, _Pr _P)
	{return (_F == _L ? _F : _Stable_partition(_F, _L, _P,
		_Dist_type(_F), _Val_type(_F))); }
template<class _FI, class _Pr, class _Pd, class _Ty> inline
	_FI _Stable_partition(_FI _F, _FI _L, _Pr _P, _Pd *, _Ty *)
	{_Pd _N = 0;
	_Distance(_F, _L, _N);
	_Temp_iterator<_Ty> _Xb(_N);
	return (_Stable_partition(_F, _L, _P, _N, _Xb)); }
template<class _FI, class _Pr, class _Pd, class _Ty> inline
	_FI _Stable_partition(_FI _F, _FI _L, _Pr _P, _Pd _N,
		_Temp_iterator<_Ty>& _Xb)
	{if (_N == 1)
		return (_P(*_F) ? _L : _F);
	else if (_N <= _Xb._Maxlen())
		{_FI _X = _F;
		for (_Xb._Init(); _F != _L; ++_F)
			if (_P(*_F))
				*_X++ = *_F;
			else
				*_Xb++ = *_F;
		copy(_Xb._First(), _Xb._Last(), _X);
		return (_X); }
	else
		{_FI _M = _F;
		advance(_M, _N / 2);
		_FI _Lp = _Stable_partition(_F, _M, _P, _N / 2, _Xb);
		_FI _Rp = _Stable_partition(_M, _L, _P, _N - _N / 2, _Xb);
		_Pd _D1 = 0;
		_Distance(_Lp, _M, _D1);
		_Pd _D2 = 0;
		_Distance(_M, _Rp, _D2);
		return (_Buffered_rotate(_Lp, _M, _Rp, _D1, _D2, _Xb)); }}
		// TEMPLATE FUNCTION sort
template<class _RI> inline
	void sort(_RI _F, _RI _L)
	{_Sort_0(_F, _L, _Val_type(_F)); }
template<class _RI, class _Ty> inline
	void _Sort_0(_RI _F, _RI _L, _Ty *)
	{if (_L - _F <= _SORT_MAX)
		_Insertion_sort(_F, _L);
	else
		{_Sort(_F, _L, (_Ty *)0);
		_Insertion_sort(_F, _F + _SORT_MAX);
		for (_F += _SORT_MAX; _F != _L; ++_F)
			_Unguarded_insert(_F, _Ty(*_F)); }}
template<class _RI, class _Ty> inline
	void _Sort(_RI _F, _RI _L, _Ty *)
	{for (; _SORT_MAX < _L - _F; )
		{_RI _M = _Unguarded_partition(_F, _L, _Median(_Ty(*_F),
			_Ty(*(_F + (_L - _F) / 2)), _Ty(*(_L - 1))));
		if (_L - _M <= _M - _F)
			_Sort(_M, _L, _Val_type(_F)), _L = _M;
		else
			_Sort(_F, _M, _Val_type(_F)), _F = _M; }}
template<class _RI, class _Ty> inline
	_RI _Unguarded_partition(_RI _F, _RI _L, _Ty _Piv)
	{for (; ; ++_F)
		{for (; *_F < _Piv; ++_F)
			;
		for (; _Piv < *--_L; )
			;
		if (_L <= _F)
			return (_F);
		iter_swap(_F, _L); }}
template<class _RI> inline
	void _Insertion_sort(_RI _F, _RI _L)
	{_Insertion_sort_1(_F, _L, _Val_type(_F)); }
template<class _BI, class _Ty> inline
	void _Insertion_sort_1(_BI _F, _BI _L, _Ty *)
	{if (_F != _L)
		for (_BI _M = _F; ++_M != _L; )
			{_Ty _V = *_M;
			if (!(_V < *_F))
				_Unguarded_insert(_M, _V);
			else
				{copy_backward(_F, _M, _M + 1);
				*_F = _V; }}}
template<class _BI, class _Ty> inline
	void _Unguarded_insert(_BI _L, _Ty _V)
	{for (_BI _M = _L; _V < *--_M; _L = _M)
		*_L = *_M;
	*_L = _V; }
		// TEMPLATE FUNCTION sort WITH PRED
template<class _RI, class _Pr> inline
	void sort(_RI _F, _RI _L, _Pr _P)
	{_Sort_0(_F, _L, _P, _Val_type(_F)); }
template<class _RI, class _Ty, class _Pr> inline
	void _Sort_0(_RI _F, _RI _L, _Pr _P, _Ty *)
	{if (_L - _F <= _SORT_MAX)
		_Insertion_sort(_F, _L, _P);
	else
		{_Sort(_F, _L, _P, (_Ty *)0);
		_Insertion_sort(_F, _F + _SORT_MAX, _P);
		for (_F += _SORT_MAX; _F != _L; ++_F)
			_Unguarded_insert(_F, _Ty(*_F), _P); }}
template<class _RI, class _Ty, class _Pr> inline
	void _Sort(_RI _F, _RI _L, _Pr _P, _Ty *)
	{for (; _SORT_MAX < _L - _F; )
		{_RI _M = _Unguarded_partition(_F, _L, _Median(_Ty(*_F),
			_Ty(*(_F + (_L - _F) / 2)), _Ty(*(_L - 1)), _P), _P);
		if (_L - _M <= _M - _F)
			_Sort(_M, _L, _P, _Val_type(_F)), _L = _M;
		else
			_Sort(_F, _M, _P, _Val_type(_F)), _F = _M; }}
template<class _RI, class _Ty, class _Pr> inline
	_RI _Unguarded_partition(_RI _F, _RI _L, _Ty _Piv, _Pr _P)
	{for (; ; ++_F)
		{for (; _P(*_F, _Piv); ++_F)
			;
		for (; _P(_Piv, *--_L); )
			;
		if (_L <= _F)
			return (_F);
		iter_swap(_F, _L); }}
template<class _RI, class _Pr> inline
	void _Insertion_sort(_RI _F, _RI _L, _Pr _P)
	{_Insertion_sort_1(_F, _L, _P, _Val_type(_F)); }
template<class _RI, class _Ty, class _Pr> inline
	void _Insertion_sort_1(_RI _F, _RI _L, _Pr _P, _Ty *)
	{if (_F != _L)
		for (_RI _M = _F; ++_M != _L; )
			{_Ty _V = *_M;
			if (!_P(_V, *_F))
				_Unguarded_insert(_M, _V, _P);
			else
				{copy_backward(_F, _M, _M + 1);
				*_F = _V; }}}
template<class _RI, class _Ty, class _Pr> inline
	void _Unguarded_insert(_RI _L, _Ty _V, _Pr _P)
	{for (_RI _M = _L; _P(_V, *--_M); _L = _M)
		*_L = *_M;
	*_L = _V; }
		// TEMPLATE FUNCTION stable_sort
template<class _BI> inline
	void stable_sort(_BI _F, _BI _L)
	{if (_F != _L)
		_Stable_sort(_F, _L, _Dist_type(_F), _Val_type(_F)); }
template<class _BI, class _Pd, class _Ty> inline
	void _Stable_sort(_BI _F, _BI _L, _Pd *, _Ty *)
	{_Pd _N = 0;
	_Distance(_F, _L, _N);
	_Temp_iterator<_Ty> _Xb(_N);
	_Stable_sort(_F, _L, _N, _Xb); }
template<class _BI, class _Pd, class _Ty> inline
	void _Stable_sort(_BI _F, _BI _L, _Pd _N,
		_Temp_iterator<_Ty>& _Xb)
	{if (_N <= _SORT_MAX)
		_Insertion_sort(_F, _L);
	else
		{_Pd _N2 = (_N + 1) / 2;
		_BI _M = _F;
		advance(_M, _N2);
		if (_N2 <= _Xb._Maxlen())
			{_Buffered_merge_sort(_F, _M, _N2, _Xb);
			_Buffered_merge_sort(_M, _L, _N - _N2, _Xb); }
		else
			{_Stable_sort(_F, _M, _N2, _Xb);
			_Stable_sort(_M, _L, _N - _N2, _Xb); }
		_Buffered_merge(_F, _M, _L, _N2, _N - _N2, _Xb); }}
template<class _BI, class _Pd, class _Ty> inline
	void _Buffered_merge_sort(_BI _F, _BI _L, _Pd _N,
		_Temp_iterator<_Ty>& _Xb)
	{_BI _M = _F;
	for (_Pd _I = _N; _CHUNK_SIZE <= _I; _I -= _CHUNK_SIZE)
		{_BI _Mn = _M;
		advance(_Mn, (int)_CHUNK_SIZE);
		_Insertion_sort(_M, _Mn);
		_M = _Mn; }
	_Insertion_sort(_M, _L);
	for (_Pd _D = _CHUNK_SIZE; _D < _N; _D *= 2)
		{_BI _Ft = _F;
		_Chunked_merge(_F, _L, _Xb._Init(), _D, _N);
		_Chunked_merge(_Xb._First(), _Xb._Last(), _Ft,
			_D *= 2, _N); }}
template<class _BI, class _OI, class _Pd> inline
	void _Chunked_merge(_BI _F, _BI _L, _OI& _X, _Pd _D, _Pd _N)
	{_Pd _D2 = _D * 2;
	for (; _D2 <= _N; _N -= _D2)
		{_BI _F1 = _F;
		advance(_F1, _D);
		_BI _F2 = _F1;
		advance(_F2, _D);
		_X = merge(_F, _F1, _F1, _F2, _X);
		_F = _F2; }
	if (_N <= _D)
		copy(_F, _L, _X);
	else
		{_BI _F1 = _F;
		advance(_F1, _D);
		merge(_F, _F1, _F1, _L, _X); }}
		// TEMPLATE FUNCTION stable_sort WITH PRED
template<class _BI, class _Pr> inline
	void stable_sort(_BI _F, _BI _L, _Pr _P)
	{if (_F != _L)
		_Stable_sort(_F, _L,
			_Dist_type(_F), _Val_type(_F), _P); }
template<class _BI, class _Pd, class _Ty, class _Pr> inline
	void _Stable_sort(_BI _F, _BI _L, _Pd *, _Ty *, _Pr _P)
	{_Pd _N = 0;
	_Distance(_F, _L, _N);
	_Temp_iterator<_Ty> _Xb(_N);
	_Stable_sort(_F, _L, _N, _Xb, _P); }
template<class _BI, class _Pd, class _Ty, class _Pr> inline
	void _Stable_sort(_BI _F, _BI _L, _Pd _N,
		_Temp_iterator<_Ty>& _Xb, _Pr _P)
	{if (_N <= _SORT_MAX)
		_Insertion_sort(_F, _L, _P);
	else
		{_Pd _N2 = (_N + 1) / 2;
		_BI _M = _F;
		advance(_M, _N2);
		if (_N2 <= _Xb._Maxlen())
			{_Buffered_merge_sort(_F, _M, _N2, _Xb, _P);
			_Buffered_merge_sort(_M, _L, _N - _N2, _Xb, _P); }
		else
			{_Stable_sort(_F, _M, _N2, _Xb, _P);
			_Stable_sort(_M, _L, _N - _N2, _Xb, _P); }
		_Buffered_merge(_F, _M, _L, _N2, _N - _N2, _Xb, _P); }}
template<class _BI, class _Pd, class _Ty, class _Pr> inline
	void _Buffered_merge_sort(_BI _F, _BI _L, _Pd _N,
		_Temp_iterator<_Ty>& _Xb, _Pr _P)
	{_BI _M = _F;
	for (_Pd _I = _N; _CHUNK_SIZE <= _I; _I -= _CHUNK_SIZE)
		{_BI _Mn = _M;
		advance(_Mn, (int)_CHUNK_SIZE);
		_Insertion_sort(_M, _Mn, _P);
		_M = _Mn; }
	_Insertion_sort(_M, _L, _P);
	for (_Pd _D = _CHUNK_SIZE; _D < _N; _D *= 2)
		{_BI _Ft = _F;
		_Chunked_merge(_F, _L, _Xb._Init(), _D, _N, _P);
		_Chunked_merge(_Xb._First(), _Xb._Last(), _Ft,
			_D *= 2, _N, _P); }}
template<class _BI, class _OI, class _Pd, class _Pr> inline
	void _Chunked_merge(_BI _F, _BI _L, _OI& _X,
		_Pd _D, _Pd _N, _Pr _P)
	{_Pd _D2 = _D * 2;
	for (; _D2 <= _N; _N -= _D2)
		{_BI _F1 = _F;
		advance(_F1, _D);
		_BI _F2 = _F1;
		advance(_F2, _D);
		_X = merge(_F, _F1, _F1, _F2, _X, _P);
		_F = _F2; }
	if (_N <= _D)
		copy(_F, _L, _X);
	else
		{_BI _F1 = _F;
		advance(_F1, _D);
		merge(_F, _F1, _F1, _L, _X, _P); }}
		// TEMPLATE FUNCTION partial_sort
template<class _RI> inline
	void partial_sort(_RI _F, _RI _M, _RI _L)
	{_Partial_sort(_F, _M, _L, _Val_type(_F)); }
template<class _RI, class _Ty> inline
	void _Partial_sort(_RI _F, _RI _M, _RI _L, _Ty *)
	{make_heap(_F, _M);
	for (_RI _I = _M; _I < _L; ++_I)
		if (*_I < *_F)
			_Pop_heap(_F, _M, _I, _Ty(*_I), _Dist_type(_F));
	sort_heap(_F, _M); }
		// TEMPLATE FUNCTION partial_sort WITH PRED
template<class _RI, class _Pr> inline
	void partial_sort(_RI _F, _RI _M, _RI _L, _Pr _P)
	{_Partial_sort(_F, _M, _L, _P, _Val_type(_F)); }
template<class _RI, class _Ty, class _Pr> inline
	void _Partial_sort(_RI _F, _RI _M, _RI _L, _Pr _P, _Ty *)
	{make_heap(_F, _M, _P);
	for (_RI _I = _M; _I < _L; ++_I)
		if (_P(*_I, *_F))
			_Pop_heap(_F, _M, _I, _Ty(*_I), _P, _Dist_type(_F));
	sort_heap(_F, _M, _P); }
		// TEMPLATE FUNCTION partial_sort_copy
template<class _II, class _RI> inline
	_RI partial_sort_copy(_II _F1, _II _L1, _RI _F2, _RI _L2)
	{return (_Partial_sort_copy(_F1, _L1, _F2, _L2,
		_Dist_type(_F2), _Val_type(_F1))); }
template<class _II, class _RI, class _Pd, class _Ty> inline
	_RI _Partial_sort_copy(_II _F1, _II _L1, _RI _F2, _RI _L2,
		_Pd *, _Ty *)
	{_RI _X = _F2;
	if (_X != _L2)
		{for (; _F1 != _L1 && _X != _L2; ++_F1, ++_X)
			*_X = *_F1;
		make_heap(_F2, _X);
		for (; _F1 != _L1; ++_F1)
			if (*_F1 < *_F2)
				_Adjust_heap(_F2, _Pd(0), _Pd(_X - _F2),
					_Ty(*_F1));
		sort_heap(_F2, _X); }
	return (_X); }
		// TEMPLATE FUNCTION partial_sort_copy WITH PRED
template<class _II, class _RI, class _Pr> inline
	_RI partial_sort_copy(_II _F1, _II _L1, _RI _F2, _RI _L2,
		_Pr _P)
	{return (_Partial_sort_copy(_F1, _L1, _F2, _L2, _P,
		_Dist_type(_F2), _Val_type(_F1))); }
template<class _II, class _RI, class _Pd,
	class _Ty, class _Pr> inline
	_RI _Partial_sort_copy(_II _F1, _II _L1, _RI _F2, _RI _L2,
		_Pr _P, _Pd *, _Ty *)
	{_RI _X = _F2;
	if (_X != _L2)
		{for (; _F1 != _L1 && _X != _L2; ++_F1, ++_X)
			*_X = *_F1;
		make_heap(_F2, _X, _P);
		for (; _F1 != _L1; ++_F1)
			if (_P(*_F1, *_F2))
				_Adjust_heap(_F2, _Pd(0), _Pd(_X - _F2),
					_Ty(*_F1), _P);
		sort_heap(_F2, _X, _P); }
	return (_X); }
		// TEMPLATE FUNCTION nth_element
template<class _RI> inline
	void nth_element(_RI _F, _RI _Nth, _RI _L)
	{_Nth_element(_F, _Nth, _L, _Val_type(_F)); }
template<class _RI, class _Ty> inline
	void _Nth_element(_RI _F, _RI _Nth, _RI _L, _Ty *)
	{for (; _SORT_MAX < _L - _F; )
		{_RI _M = _Unguarded_partition(_F, _L, _Median(_Ty(*_F),
			_Ty(*(_F + (_L - _F) / 2)), _Ty(*(_L - 1))));
		if (_M <= _Nth)
			_F = _M;
		else
			_L = _M; }
	_Insertion_sort(_F, _L); }
		// TEMPLATE FUNCTION nth_element WITH PRED
template<class _RI, class _Pr> inline
	void nth_element(_RI _F, _RI _Nth, _RI _L, _Pr _P)
	{_Nth_element(_F, _Nth, _L, _P, _Val_type(_F)); }
template<class _RI, class _Ty, class _Pr> inline
	void _Nth_element(_RI _F, _RI _Nth, _RI _L, _Pr _P, _Ty *)
	{for (; _SORT_MAX < _L - _F; )
		{_RI _M = _Unguarded_partition(_F, _L, _Median(_Ty(*_F),
			_Ty(*(_F + (_L - _F) / 2)), _Ty(*(_L - 1)), _P), _P);
		if (_M <= _Nth)
			_F = _M;
		else
			_L = _M; }
	_Insertion_sort(_F, _L, _P); }
		// TEMPLATE FUNCTION lower_bound
template<class _FI, class _Ty> inline
	_FI lower_bound(_FI _F, _FI _L, const _Ty& _V)
	{return (_Lower_bound(_F, _L, _V, _Dist_type(_F))); }
template<class _FI, class _Ty, class _Pd> inline
	_FI _Lower_bound(_FI _F, _FI _L, const _Ty& _V, _Pd *)
	{_Pd _N = 0;
	_Distance(_F, _L, _N);
	for (; 0 < _N; )
		{_Pd _N2 = _N / 2;
		_FI _M = _F;
		advance(_M, _N2);
		if (*_M < _V)
			_F = ++_M, _N -= _N2 + 1;
		else
			_N = _N2; }
	return (_F); } 
		// TEMPLATE FUNCTION lower_bound WITH PRED
template<class _FI, class _Ty, class _Pr> inline
	_FI lower_bound(_FI _F, _FI _L, const _Ty& _V, _Pr _P)
	{return (_Lower_bound(_F, _L, _V, _P, _Dist_type(_F))); }
template<class _FI, class _Ty, class _Pd, class _Pr> inline
	_FI _Lower_bound(_FI _F, _FI _L, const _Ty& _V, _Pr _P, _Pd *)
	{_Pd _N = 0;
	_Distance(_F, _L, _N);
	for (; 0 < _N; )
		{_Pd _N2 = _N / 2;
		_FI _M = _F;
		advance(_M, _N2);
		if (_P(*_M, _V))
			_F = ++_M, _N -= _N2 + 1;
		else
			_N = _N2; }
	return (_F); } 
		// TEMPLATE FUNCTION upper_bound
template<class _FI, class _Ty> inline
	_FI upper_bound(_FI _F, _FI _L, const _Ty& _V)
	{return (_Upper_bound(_F, _L, _V, _Dist_type(_F))); }
template<class _FI, class _Ty, class _Pd> inline
	_FI _Upper_bound(_FI _F, _FI _L, const _Ty& _V, _Pd *)
	{_Pd _N = 0;
	_Distance(_F, _L, _N);
	for (; 0 < _N; )
		{_Pd _N2 = _N / 2;
		_FI _M = _F;
		advance(_M, _N2);
		if (!(_V < *_M))
			_F = ++_M, _N -= _N2 + 1;
		else
			_N = _N2; }
	return (_F); } 
		// TEMPLATE FUNCTION upper_bound WITH PRED
template<class _FI, class _Ty, class _Pr> inline
	_FI upper_bound(_FI _F, _FI _L, const _Ty& _V, _Pr _P)
	{return (_Upper_bound(_F, _L, _V, _P, _Dist_type(_F))); }
template<class _FI, class _Ty, class _Pd, class _Pr> inline
	_FI _Upper_bound(_FI _F, _FI _L, const _Ty& _V, _Pr _P, _Pd *)
	{_Pd _N = 0;
	_Distance(_F, _L, _N);
	for (; 0 < _N; )
		{_Pd _N2 = _N / 2;
		_FI _M = _F;
		advance(_M, _N2);
		if (!_P(_V, *_M))
			_F = ++_M, _N -= _N2 + 1;
		else
			_N = _N2; }
	return (_F); } 
		// TEMPLATE FUNCTION equal_range
template<class _FI, class _Ty> inline
	pair<_FI, _FI> equal_range(_FI _F, _FI _L, const _Ty& _V)
	{return (_Equal_range(_F, _L, _V, _Dist_type(_F))); }
template<class _FI, class _Ty, class _Pd> inline
	pair<_FI, _FI> _Equal_range(_FI _F, _FI _L,
		const _Ty& _V, _Pd *)
	{_Pd _N = 0;
	_Distance(_F, _L, _N);
	for (; 0 < _N; )
		{_Pd _N2 = _N / 2;
		_FI _M = _F;
		advance(_M, _N2);
		if (*_M < _V)
			_F = ++_M, _N -= _N2 + 1;
		else if (_V < *_M)
			_N = _N2;
		else
			{_FI _F2 = lower_bound(_F, _M, _V);
			advance(_F, _N);
			_FI _L2 = upper_bound(++_M, _F, _V);
			return (pair<_FI, _FI>(_F2, _L2)); }} 
	return (pair<_FI, _FI>(_F, _F)); } 
		// TEMPLATE FUNCTION equal_range WITH PRED
template<class _FI, class _Ty, class _Pr> inline
	pair<_FI, _FI> equal_range(_FI _F, _FI _L, const _Ty& _V,
		_Pr _P)
	{return (_Equal_range(_F, _L, _V, _P, _Dist_type(_F))); }
template<class _FI, class _Ty, class _Pd, class _Pr> inline
	pair<_FI, _FI> _Equal_range(_FI _F, _FI _L, const _Ty& _V,
		_Pr _P, _Pd *)
	{_Pd _N = 0;
	_Distance(_F, _L, _N);
	for (; 0 < _N; )
		{_Pd _N2 = _N / 2;
		_FI _M = _F;
		advance(_M, _N2);
		if (_P(*_M, _V))
			_F = ++_M, _N -= _N2 + 1;
		else if (_P(_V, *_M))
			_N = _N2;
		else
			{_FI _F2 = lower_bound(_F, _M, _V, _P);
			advance(_F, _N);
			_FI _L2 = upper_bound(++_M, _F, _V, _P);
			return (pair<_FI, _FI>(_F2, _L2)); }}
	return (pair<_FI, _FI>(_F, _F)); } 
		// TEMPLATE FUNCTION binary_search
template<class _FI, class _Ty> inline
	bool binary_search(_FI _F, _FI _L, const _Ty& _V)
	{_FI _I = lower_bound(_F, _L, _V);
	return (_I != _L && !(_V < *_I)); }
		// TEMPLATE FUNCTION binary_search WITH PRED
template<class _FI, class _Ty, class _Pr> inline
	bool binary_search(_FI _F, _FI _L, const _Ty& _V, _Pr _P)
	{_FI _I = lower_bound(_F, _L, _V, _P);
	return (_I != _L && !_P(_V, *_I)); }
		// TEMPLATE FUNCTION merge
template<class _II1, class _II2, class _OI> inline
	_OI merge(_II1 _F1, _II1 _L1, _II2 _F2, _II2 _L2, _OI _X)
	{for (; _F1 != _L1 && _F2 != _L2; ++_X)
		if (*_F2 < *_F1)
			*_X = *_F2++;
		else
			*_X = *_F1++;
	return (copy(_F2, _L2, copy(_F1, _L1, _X))); } 
		// TEMPLATE FUNCTION merge WITH PRED
template<class _II1, class _II2, class _OI, class _Pr> inline
	_OI merge(_II1 _F1, _II1 _L1, _II2 _F2, _II2 _L2, _OI _X,
		_Pr _P)
	{for (; _F1 != _L1 && _F2 != _L2; ++_X)
		if (_P(*_F2, *_F1))
			*_X = *_F2++;
		else
			*_X = *_F1++;
	return (copy(_F2, _L2, copy(_F1, _L1, _X))); } 
		// TEMPLATE FUNCTION inplace_merge
template<class _BI> inline
	void inplace_merge(_BI _F, _BI _M, _BI _L)
	{if (_F != _L)
		_Inplace_merge(_F, _M, _L,
			_Dist_type(_F), _Val_type(_F)); }
template<class _BI, class _Pd, class _Ty> inline
	void _Inplace_merge(_BI _F, _BI _M, _BI _L, _Pd *, _Ty *)
	{_Pd _D1 = 0;
	_Distance(_F, _M, _D1);
	_Pd _D2 = 0;
	_Distance(_M, _L, _D2);
	_Temp_iterator<_Ty> _Xb(_D1 < _D2 ? _D1 : _D2);
	_Buffered_merge(_F, _M, _L, _D1, _D2, _Xb); }
template<class _BI, class _Pd, class _Ty> inline
	void _Buffered_merge(_BI _F, _BI _M, _BI _L,
		_Pd _D1, _Pd _D2, _Temp_iterator<_Ty>& _Xb)
	{if (_D1 == 0 || _D2 == 0)
		;
	else if (_D1 + _D2 == 2)
		{if (*_M < *_F)
			iter_swap(_F, _M); }
	else if (_D1 <= _D2 && _D1 <= _Xb._Maxlen())
		{copy(_F, _M, _Xb._Init());
		merge(_Xb._First(), _Xb._Last(), _M, _L, _F); }
	else if (_D2 <= _Xb._Maxlen())
		{copy(_M, _L, _Xb._Init());
		_Merge_backward(_F, _M, _Xb._First(), _Xb._Last(), _L); }
	else
		{_BI _Fn, _Ln;
		_Pd _D1n = 0;
		_Pd _D2n = 0;
		if (_D2 < _D1)
			{_D1n = _D1 / 2;
			_Fn = _F;
			advance(_Fn, _D1n);
			_Ln = lower_bound(_M, _L, _Ty(*_Fn));
			_Distance(_M, _Ln, _D2n); }
		else
			{_D2n = _D2 / 2;
			_Ln = _M;
			advance(_Ln, _D2n);
			_Fn = upper_bound(_F, _M, _Ty(*_Ln));
			_Distance(_F, _Fn, _D1n); }
		_BI _Mn = _Buffered_rotate(_Fn, _M, _Ln,
			_D1 - _D1n, _D2n, _Xb);
		_Buffered_merge(_F, _Fn, _Mn, _D1n, _D2n, _Xb);
		_Buffered_merge(_Mn, _Ln, _L,
			_D1 - _D1n, _D2 - _D2n, _Xb); }}
template<class _BI1, class _BI2, class _BI3> inline
	_BI3 _Merge_backward(_BI1 _F1, _BI1 _L1, _BI2 _F2, _BI2 _L2,
		_BI3 _X)
	{for (; ; )
		if (_F1 == _L1)
			return (copy_backward(_F2, _L2, _X));
		else if (_F2 == _L2)
			return (copy_backward(_F1, _L1, _X));
		else if (*--_L2 < *--_L1)
			*--_X = *_L1, ++_L2;
		else
			*--_X = *_L2, ++_L1; }
template<class _BI, class _Pd, class _Ty> inline
	_BI _Buffered_rotate(_BI _F, _BI _M, _BI _L,
		_Pd _D1, _Pd _D2, _Temp_iterator<_Ty>& _Xb)
	{if (_D1 <= _D2 && _D1 <= _Xb._Maxlen())
		{copy(_F, _M, _Xb._Init());
		copy(_M, _L, _F);
		return (copy_backward(_Xb._First(), _Xb._Last(), _L)); }
	else if (_D2 <= _Xb._Maxlen())
		{copy(_M, _L, _Xb._Init());
		copy_backward(_F, _M, _L);
		return (copy(_Xb._First(), _Xb._Last(), _F)); }
	else
		{rotate(_F, _M, _L);
		advance(_F, _D2);
		return (_F); }}
		// TEMPLATE FUNCTION inplace_merge WITH PRED
template<class _BI, class _Pr> inline
	void inplace_merge(_BI _F, _BI _M, _BI _L, _Pr _P)
	{if (_F != _L)
		_Inplace_merge(_F, _M, _L, _P,
			_Dist_type(_F), _Val_type(_F)); }
template<class _BI, class _Pd, class _Ty, class _Pr> inline
	void _Inplace_merge(_BI _F, _BI _M, _BI _L, _Pr _P,
		_Pd *, _Ty *)
	{_Pd _D1 = 0;
	_Distance(_F, _M, _D1);
	_Pd _D2 = 0;
	_Distance(_M, _L, _D2);
	_Temp_iterator<_Ty> _Xb(_D1 < _D2 ? _D1 : _D2);
	_Buffered_merge(_F, _M, _L, _D1, _D2, _Xb, _P); }
template<class _BI, class _Pd, class _Ty, class _Pr> inline
	void _Buffered_merge(_BI _F, _BI _M, _BI _L,
		_Pd _D1, _Pd _D2, _Temp_iterator<_Ty>& _Xb, _Pr _P)
	{if (_D1 == 0 || _D2 == 0)
		;
	else if (_D1 + _D2 == 2)
		{if (_P(*_M, *_F))
			iter_swap(_F, _M); }
	else if (_D1 <= _D2 && _D1 <= _Xb._Maxlen())
		{copy(_F, _M, _Xb._Init());
		merge(_Xb._First(), _Xb._Last(), _M, _L, _F, _P); }
	else if (_D2 <= _Xb._Maxlen())
		{copy(_M, _L, _Xb._Init());
		_Merge_backward(_F, _M, _Xb._First(), _Xb._Last(),
			_L, _P); }
	else
		{_BI _Fn, _Ln;
		_Pd _D1n = 0;
		_Pd _D2n = 0;
		if (_D2 < _D1)
			{_D1n = _D1 / 2;
			_Fn = _F;
			advance(_Fn, _D1n);
			_Ln = lower_bound(_M, _L, _Ty(*_Fn), _P);
			_Distance(_M, _Ln, _D2n); }
		else
			{_D2n = _D2 / 2;
			_Ln = _M;
			advance(_Ln, _D2n);
			_Fn = upper_bound(_F, _M, _Ty(*_Ln), _P);
			_Distance(_F, _Fn, _D1n); }
		_BI _Mn = _Buffered_rotate(_Fn, _M, _Ln,
			_D1 - _D1n, _D2n, _Xb);
		_Buffered_merge(_F, _Fn, _Mn, _D1n, _D2n, _Xb, _P);
		_Buffered_merge(_Mn, _Ln, _L,
			_D1 - _D1n, _D2 - _D2n, _Xb, _P); }}
template<class _BI1, class _BI2, class _BI3, class _Pr> inline
	_BI3 _Merge_backward(_BI1 _F1, _BI1 _L1, _BI2 _F2, _BI2 _L2,
		_BI3 _X, _Pr _P)
	{for (; ; )
		if (_F1 == _L1)
			return (copy_backward(_F2, _L2, _X));
		else if (_F2 == _L2)
			return (copy_backward(_F1, _L1, _X));
		else if (_P(*--_L2, *--_L1))
			*--_X = *_L1, ++_L2;
		else
			*--_X = *_L2, ++_L1; }
		// TEMPLATE FUNCTION includes
template<class _II1, class _II2> inline
	bool includes(_II1 _F1, _II1 _L1, _II2 _F2, _II2 _L2)
	{for (; _F1 != _L1 && _F2 != _L2; )
		if (*_F2 < *_F1)
			return (false);
		else if (*_F1 < *_F2)
			++_F1;
		else
			++_F2;
	return (_F2 == _L2); }
		// TEMPLATE FUNCTION includes WITH PRED
template<class _II1, class _II2, class _Pr> inline
	bool includes(_II1 _F1, _II1 _L1, _II2 _F2, _II2 _L2, _Pr _P)
	{for (; _F1 != _L1 && _F2 != _L2; )
		if (_P(*_F2, *_F1))
			return (false);
		else if (_P(*_F1, *_F2))
			++_F1;
		else
			++_F2;
	return (_F2 == _L2); }
		// TEMPLATE FUNCTION set_union
template<class _II1, class _II2, class _OI> inline
	_OI set_union(_II1 _F1, _II1 _L1, _II2 _F2, _II2 _L2, _OI _X)
	{for (; _F1 != _L1 && _F2 != _L2; )
		if (*_F1 < *_F2)
			*_X++ = *_F1++;
		else if (*_F2 < *_F1)
			*_X++ = *_F2++;
		else
			*_X++ = *_F1++, ++_F2;
	return (copy(_F2, _L2, copy(_F1, _L1, _X))); }
		// TEMPLATE FUNCTION set_union WITH PRED
template<class _II1, class _II2, class _OI, class _Pr> inline
	_OI set_union(_II1 _F1, _II1 _L1, _II2 _F2, _II2 _L2, _OI _X,
		_Pr _P)
	{for (; _F1 != _L1 && _F2 != _L2; )
		if (_P(*_F1, *_F2))
			*_X++ = *_F1++;
		else if (_P(*_F2, *_F1))
			*_X++ = *_F2++;
		else
			*_X++ = *_F1++, ++_F2;
	return (copy(_F2, _L2, copy(_F1, _L1, _X))); }
		// TEMPLATE FUNCTION set_intersection
template<class _II1, class _II2, class _OI> inline
	_OI set_intersection(_II1 _F1, _II1 _L1, _II2 _F2, _II2 _L2,
		_OI _X)
	{for (; _F1 != _L1 && _F2 != _L2; )
		if (*_F1 < *_F2)
			++_F1;
		else if (*_F2 < *_F1)
			++_F2;
		else
			*_X++ = *_F1++, ++_F2;
	return (_X); }
		// TEMPLATE FUNCTION set_intersection WITH PRED
template<class _II1, class _II2, class _OI, class _Pr> inline
	_OI set_intersection(_II1 _F1, _II1 _L1, _II2 _F2, _II2 _L2,
		_OI _X, _Pr _P)
	{for (; _F1 != _L1 && _F2 != _L2; )
		if (_P(*_F1, *_F2))
			++_F1;
		else if (_P(*_F2, *_F1))
			++_F2;
		else
			*_X++ = *_F1++, ++_F2;
	return (_X); }
		// TEMPLATE FUNCTION set_difference
template<class _II1, class _II2, class _OI> inline
	_OI set_difference(_II1 _F1, _II1 _L1, _II2 _F2, _II2 _L2,
		_OI _X)
	{for (; _F1 != _L1 && _F2 != _L2; )
		if (*_F1 < *_F2)
			*_X++ = *_F1++;
		else if (*_F2 < *_F1)
			++_F2;
		else
			++_F1, ++_F2;
	return (copy(_F1, _L1, _X)); }
		// TEMPLATE FUNCTION set_difference WITH PRED
template<class _II1, class _II2, class _OI, class _Pr> inline
	_OI set_difference(_II1 _F1, _II1 _L1, _II2 _F2, _II2 _L2,
		_OI _X, _Pr _P)
	{for (; _F1 != _L1 && _F2 != _L2; )
		if (_P(*_F1, *_F2))
			*_X++ = *_F1++;
		else if (_P(*_F2, *_F1))
			++_F2;
		else
			++_F1, ++_F2;
	return (copy(_F1, _L1, _X)); }
		// TEMPLATE FUNCTION set_symmetric_difference
template<class _II1, class _II2, class _OI> inline
	_OI set_symmetric_difference(_II1 _F1, _II1 _L1, _II2 _F2,
		_II2 _L2, _OI _X)
	{for (; _F1 != _L1 && _F2 != _L2; )
		if (*_F1 < *_F2)
			*_X++ = *_F1++;
		else if (*_F2 < *_F1)
			*_X++ = *_F2++;
		else
			++_F1, ++_F2;
	return (copy(_F2, _L2, copy(_F1, _L1, _X))); }
		// TEMPLATE FUNCTION set_symmetric_difference WITH PRED
template<class _II1, class _II2, class _OI, class _Pr> inline
	_OI set_symmetric_difference(_II1 _F1, _II1 _L1, _II2 _F2,
		_II2 _L2, _OI _X, _Pr _P)
	{for (; _F1 != _L1 && _F2 != _L2; )
		if (_P(*_F1, *_F2))
			*_X++ = *_F1++;
		else if (_P(*_F2, *_F1))
			*_X++ = *_F2++;
		else
			++_F1, ++_F2;
	return (copy(_F2, _L2, copy(_F1, _L1, _X))); }
		// TEMPLATE FUNCTION push_heap
template<class _RI> inline
	void push_heap(_RI _F, _RI _L)
	{_Push_heap_0(_F, _L, _Dist_type(_F), _Val_type(_F)); }
template<class _RI, class _Pd, class _Ty> inline
	void _Push_heap_0(_RI _F, _RI _L, _Pd *, _Ty *)
	{_Push_heap(_F, _Pd(_L - _F - 1), _Pd(0), _Ty(*(_L - 1))); }
template<class _RI, class _Pd, class _Ty> inline
	void _Push_heap(_RI _F, _Pd _H, _Pd _J, _Ty _V)
	{for (_Pd _I = (_H - 1) / 2; _J < _H && *(_F + _I) < _V;
		_I = (_H - 1) / 2)
		*(_F + _H) = *(_F + _I), _H = _I;
	*(_F + _H) = _V; }
		// TEMPLATE FUNCTION push_heap WITH PRED
template<class _RI, class _Pr> inline
	void push_heap(_RI _F, _RI _L, _Pr _P)
	{_Push_heap_0(_F, _L, _P,
		_Dist_type(_F), _Val_type(_F)); }
template<class _RI, class _Pd, class _Ty, class _Pr> inline
	void _Push_heap_0(_RI _F, _RI _L, _Pr _P, _Pd *, _Ty *)
	{_Push_heap(_F, _Pd(_L - _F - 1), _Pd(0),
		_Ty(*(_L - 1)), _P); }
template<class _RI, class _Pd, class _Ty, class _Pr> inline
	void _Push_heap(_RI _F, _Pd _H, _Pd _J, _Ty _V, _Pr _P)
	{for (_Pd _I = (_H - 1) / 2; _J < _H && _P(*(_F + _I), _V);
		_I = (_H - 1) / 2)
		*(_F + _H) = *(_F + _I), _H = _I;
	*(_F + _H) = _V; }
		// TEMPLATE FUNCTION pop_heap
template<class _RI> inline
	void pop_heap(_RI _F, _RI _L)
	{_Pop_heap_0(_F, _L, _Val_type(_F)); }
template<class _RI, class _Ty> inline
	void _Pop_heap_0(_RI _F, _RI _L, _Ty *)
	{_Pop_heap(_F, _L - 1, _L - 1, _Ty(*(_L - 1)),
		_Dist_type(_F)); }
template<class _RI, class _Pd, class _Ty> inline
	void _Pop_heap(_RI _F, _RI _L, _RI _X, _Ty _V, _Pd *)
	{*_X = *_F;
	_Adjust_heap(_F, _Pd(0), _Pd(_L - _F), _V); }
template<class _RI, class _Pd, class _Ty> inline
	void _Adjust_heap(_RI _F, _Pd _H, _Pd _N, _Ty _V)
	{_Pd _J = _H;
	_Pd _K = 2 * _H + 2;
	for (; _K < _N; _K = 2 * _K + 2)
		{if (*(_F + _K) < *(_F + (_K - 1)))
			--_K;
		*(_F + _H) = *(_F + _K), _H = _K; }
	if (_K == _N)
		*(_F + _H) = *(_F + (_K - 1)), _H = _K - 1;
	_Push_heap(_F, _H, _J, _V); }
		// TEMPLATE FUNCTION pop_heap WITH PRED
template<class _RI, class _Pr> inline
	void pop_heap(_RI _F, _RI _L, _Pr _P)
	{_Pop_heap_0(_F, _L, _P, _Val_type(_F)); }
template<class _RI, class _Ty, class _Pr> inline
	void _Pop_heap_0(_RI _F, _RI _L, _Pr _P, _Ty *)
	{_Pop_heap(_F, _L - 1, _L - 1, _Ty(*(_L - 1)), _P,
		_Dist_type(_F)); }
template<class _RI, class _Pd, class _Ty, class _Pr> inline
	void _Pop_heap(_RI _F, _RI _L, _RI _X, _Ty _V, _Pr _P, _Pd *)
	{*_X = *_F;
	_Adjust_heap(_F, _Pd(0), _Pd(_L - _F), _V, _P); }
template<class _RI, class _Pd, class _Ty, class _Pr> inline
	void _Adjust_heap(_RI _F, _Pd _H, _Pd _N, _Ty _V, _Pr _P)
	{_Pd _J = _H;
	_Pd _K = 2 * _H + 2;
	for (; _K < _N; _K = 2 * _K + 2)
		{if (_P(*(_F + _K), *(_F + (_K - 1))))
			--_K;
		*(_F + _H) = *(_F + _K), _H = _K; }
	if (_K == _N)
		*(_F + _H) = *(_F + (_K - 1)), _H = _K - 1;
	_Push_heap(_F, _H, _J, _V, _P); }
		// TEMPLATE FUNCTION make_heap
template<class _RI> inline
	void make_heap(_RI _F, _RI _L)
	{if (2 <= _L - _F)
		_Make_heap(_F, _L, _Dist_type(_F), _Val_type(_F)); }
template<class _RI, class _Pd, class _Ty> inline
	void _Make_heap(_RI _F, _RI _L, _Pd *, _Ty *)
	{_Pd _N = _L - _F;
	for (_Pd _H = _N / 2; 0 < _H; )
		--_H, _Adjust_heap(_F, _H, _N, _Ty(*(_F + _H))); }
		// TEMPLATE FUNCTION make_heap WITH PRED
template<class _RI, class _Pr> inline
	void make_heap(_RI _F, _RI _L, _Pr _P)
	{if (2 <= _L - _F)
		_Make_heap(_F, _L, _P,
			_Dist_type(_F), _Val_type(_F)); }
template<class _RI, class _Pd, class _Ty, class _Pr> inline
	void _Make_heap(_RI _F, _RI _L, _Pr _P, _Pd *, _Ty *)
	{_Pd _N = _L - _F;
	for (_Pd _H = _N / 2; 0 < _H; )
		--_H, _Adjust_heap(_F, _H, _N, _Ty(*(_F + _H)), _P); }
		// TEMPLATE FUNCTION sort_heap
template<class _RI> inline
	void sort_heap(_RI _F, _RI _L)
	{for (; 1 < _L - _F; --_L)
		pop_heap(_F, _L); }
		// TEMPLATE FUNCTION sort_heap WITH PRED
template<class _RI, class _Pr> inline
	void sort_heap(_RI _F, _RI _L, _Pr _P)
	{for (; 1 < _L - _F; --_L)
		pop_heap(_F, _L, _P); }
		// TEMPLATE FUNCTION max_element
template<class _FI> inline
	_FI max_element(_FI _F, _FI _L)
	{_FI _X = _F;
	if (_F != _L)
		for (; ++_F != _L; )
			if (*_X < *_F)
				_X = _F;
	return (_X); }
		// TEMPLATE FUNCTION max_element WITH PRED
template<class _FI, class _Pr> inline
	_FI max_element(_FI _F, _FI _L, _Pr _P)
	{_FI _X = _F;
	if (_F != _L)
		for (; ++_F != _L; )
			if (_P(*_X, *_F))
				_X = _F;
	return (_X); }
		// TEMPLATE FUNCTION min_element
template<class _FI> inline
	_FI min_element(_FI _F, _FI _L)
	{_FI _X = _F;
	if (_F != _L)
		for (; ++_F != _L; )
			if (*_F < *_X)
				_X = _F;
	return (_X); }
		// TEMPLATE FUNCTION min_element WITH PRED
template<class _FI, class _Pr> inline
	_FI min_element(_FI _F, _FI _L, _Pr _P)
	{_FI _X = _F;
	if (_F != _L)
		for (; ++_F != _L; )
			if (_P(*_F, *_X))
				_X = _F;
	return (_X); }
		// TEMPLATE FUNCTION next_permutation
template<class _BI> inline
	bool next_permutation(_BI _F, _BI _L)
	{_BI _I = _L;
	if (_F == _L || _F == --_I)
		return (false);
	for (; ; )
		{_BI _Ip = _I;
		if (*--_I < *_Ip)
			{_BI _J = _L;
			for (; !(*_I < *--_J); )
				;
			iter_swap(_I, _J);
			reverse(_Ip, _L);
			return (true); }
		if (_I == _F)
			{reverse(_F, _L);
			return (false); }}}
		// TEMPLATE FUNCTION next_permutation WITH PRED
template<class _BI, class _Pr> inline
	bool next_permutation(_BI _F, _BI _L, _Pr _P)
	{_BI _I = _L;
	if (_F == _L || _F == --_I)
		return (false);
	for (; ; )
		{_BI _Ip = _I;
		if (_P(*--_I, *_Ip))
			{_BI _J = _L;
			for (; !_P(*_I, *--_J); )
				;
			iter_swap(_I, _J);
			reverse(_Ip, _L);
			return (true); }
		if (_I == _F)
			{reverse(_F, _L);
			return (false); }}}
		// TEMPLATE FUNCTION prev_permutation
template<class _BI> inline
	bool prev_permutation(_BI _F, _BI _L)
	{_BI _I = _L;
	if (_F == _L || _F == --_I)
		return (false);
	for (; ; )
		{_BI _Ip = _I;
		if (!(*--_I < *_Ip))
			{_BI _J = _L;
			for (; *_I < *--_J; )
				;
			iter_swap(_I, _J);
			reverse(_Ip, _L);
			return (true); }
		if (_I == _F)
			{reverse(_F, _L);
			return (false); }}}
		// TEMPLATE FUNCTION prev_permutation WITH PRED
template<class _BI, class _Pr> inline
	bool prev_permutation(_BI _F, _BI _L, _Pr _P)
	{_BI _I = _L;
	if (_F == _L || _F == --_I)
		return (false);
	for (; ; )
		{_BI _Ip = _I;
		if (!_P(*--_I, *_Ip))
			{_BI _J = _L;
			for (; _P(*_I, *--_J); )
				;
			iter_swap(_I, _J);
			reverse(_Ip, _L);
			return (true); }
		if (_I == _F)
			{reverse(_F, _L);
			return (false); }}}
_STD_END
#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif /* _ALGORITHM_ */

/*
 * Copyright (c) 1995 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */

/*
 * This file is derived from software bearing the following
 * restrictions:
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this
 * software and its documentation for any purpose is hereby
 * granted without fee, provided that the above copyright notice
 * appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation.
 * Hewlett-Packard Company makes no representations about the
 * suitability of this software for any purpose. It is provided
 * "as is" without express or implied warranty.
 */
