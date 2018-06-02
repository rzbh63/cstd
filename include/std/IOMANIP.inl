// iomanip standard header

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _IOMANIP_
#define _IOMANIP_
#include <istream>
#include <xstddef>

#ifdef  _MSC_VER
#pragma pack(push,8)
#endif  /* _MSC_VER */
_STD_BEGIN
		// TEMPLATE FUNCTION setfill
template<class _E>
	struct _Fillobj {
	_Fillobj(_E _Charg)
		: _Ch(_Charg) {}
	_E _Ch;
	};
template<class _E> inline
	_Fillobj<_E> __cdecl setfill(_E _C)
	{return (_Fillobj<_E>(_C)); }
template<class _E, class _Tr> inline
	basic_istream<_E, _Tr>& __cdecl operator>>(
		basic_istream<_E, _Tr>& _I, const _Fillobj<_E>& _X)
		{_I.fill(_X._Ch);
		return (_I); }
template<class _E, class _Tr> inline
	basic_ostream<_E, _Tr>& __cdecl operator<<(
		basic_ostream<_E, _Tr>& _O, const _Fillobj<_E>& _X)
		{_O.fill(_X._Ch);
		return (_O); }
		// TEMPLATE STRUCT _Smanip
template<class _Tm>
	struct _Smanip {
	_Smanip(void (__cdecl *_F)(ios_base&, _Tm), _Tm _A)
		: _Pf(_F), _Manarg(_A) {}
	void (__cdecl *_Pf)(ios_base&, _Tm);
	_Tm _Manarg;
	};
template<class _E, class _Tr, class _Tm> inline
	basic_istream<_E, _Tr>& __cdecl operator>>(
		basic_istream<_E, _Tr>& _I, const _Smanip<_Tm>& _M)
	{(*_M._Pf)(_I, _M._Manarg);
	return (_I); }
template<class _E, class _Tr, class _Tm> inline
	basic_ostream<_E, _Tr>& __cdecl operator<<(
		basic_ostream<_E, _Tr>& _O, const _Smanip<_Tm>& _M)
	{(*_M._Pf)(_O, _M._Manarg);
	return (_O); }
		// INSTANTIATIONS
_CRTIMP _Smanip<ios_base::fmtflags>
	__cdecl resetiosflags(ios_base::fmtflags);
_CRTIMP _Smanip<ios_base::fmtflags>
	__cdecl setiosflags(ios_base::fmtflags);
_CRTIMP _Smanip<int> __cdecl setbase(int);
_CRTIMP _Smanip<streamsize> __cdecl setprecision(streamsize);
_CRTIMP _Smanip<streamsize> __cdecl setw(streamsize);
_STD_END
#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif /* _IOMANIP_ */

/*
 * Copyright (c) 1994 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
