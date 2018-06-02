// complex standard header

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _COMPLEX_
#define _COMPLEX_
#include <ymath.h>
#include <cmath>
#include <sstream>
#include <xutility>

#ifdef  _MSC_VER
#pragma pack(push,8)
#endif  /* _MSC_VER */
_STD_BEGIN
#define __STD_COMPLEX
		// TEMPLATE CLASS _Ctr
template<class _Ty> class _Ctr {
public:
	static _Ty _Cosh(_Ty _X, _Ty _Y)
		{return (::_Cosh((double)_X, (double)_Y)); }
	static short _Exp(_Ty *_P, _Ty _Y, short _E)
		{double _W = (double)*_P;
		short _Ans = ::_Exp(&_W, (double)_Y, _E);
		*_P = (_Ty)_W;
		return (_Ans); }
	static _Ty _Infv(_Ty)
		{return (_Inf._D); }
	static bool _Isinf(_Ty _X)
		{double _W = (double)_X;
		return (_Dtest(&_W) == _INFCODE); }
	static bool _Isnan(_Ty _X)
		{double _W = (double)_X;
		return (_Dtest(&_W) == _NANCODE); }
	static _Ty _Nanv(_Ty)
		{return (_Nan._D); }
	static _Ty _Sinh(_Ty _X, _Ty _Y)
		{return (::_Sinh((double)_X, (double)_Y)); }
	static _Ty atan2(_Ty _Y, _Ty _X)
		{return (::atan2((double)_Y, (double)_X)); }
	static _Ty cos(_Ty _X)
		{return (::cos((double)_X)); }
	static _Ty exp(_Ty _X)
		{return (::exp((double)_X)); }
	static _Ty ldexp(_Ty _R, int _E)
		{return (::ldexp((double)_R, _E)); }
	static _Ty log(_Ty _X)
		{return (::log((double)_X)); }
	static _Ty pow(_Ty _X, _Ty _Y)
		{return (::pow((double)_X, (double)_Y)); }
	static _Ty sin(_Ty _X)
		{return (::sin((double)_X)); }
	static _Ty sqrt(_Ty _X)
		{return (::sqrt((double)_X)); }
	};
		// CLASS _Ctr<long double>
class _CRTIMP _Ctr<long double> {
public:
	typedef long double _Ty;
	static _Ty _Cosh(_Ty _X, _Ty _Y)
		{return (_LCosh(_X, _Y)); }
	static short _Exp(_Ty *_P, _Ty _Y, short _E)
		{return (_LExp(_P, _Y, _E)); }
	static _Ty _Infv(_Ty)
		{return (_LInf._L); }
	static bool _Isinf(_Ty _X)
		{return (_LDtest(&_X) == _INFCODE); }
	static bool _Isnan(_Ty _X)
		{return (_LDtest(&_X) == _NANCODE); }
	static _Ty _Nanv(_Ty)
		{return (_LNan._L); }
	static _Ty _Sinh(_Ty _X, _Ty _Y)
		{return (_LSinh(_X, _Y)); }
	static _Ty atan2(_Ty _Y, _Ty _X)
		{return (atan2l(_Y, _X)); }
	static _Ty cos(_Ty _X)
		{return (cosl(_X)); }
	static _Ty exp(_Ty _X)
		{return (expl(_X)); }
	static _Ty ldexp(_Ty _R, int _E)
		{return (ldexpl(_R, _E)); }
	static _Ty log(_Ty _X)
		{return (logl(_X)); }
	static _Ty pow(_Ty _X, _Ty _Y)
		{return (powl(_X, _Y)); }
	static _Ty sin(_Ty _X)
		{return (sinl(_X)); }
	static _Ty sqrt(_Ty _X)
		{return (sqrtl(_X)); }
	};
		// CLASS _Ctr<double>
class _CRTIMP _Ctr<double> {
public:
	typedef double _Ty;
	static _Ty _Cosh(_Ty _X, _Ty _Y)
		{return (::_Cosh(_X, _Y)); }
	static short _Exp(_Ty *_P, _Ty _Y, short _E)
		{return (::_Exp(_P, _Y, _E)); }
	static _Ty _Infv(_Ty)
		{return (_Inf._D); }
	static bool _Isinf(_Ty _X)
		{return (_Dtest(&_X) == _INFCODE); }
	static bool _Isnan(_Ty _X)
		{return (_Dtest(&_X) == _NANCODE); }
	static _Ty _Nanv(_Ty)
		{return (_Nan._D); }
	static _Ty _Sinh(_Ty _X, _Ty _Y)
		{return (::_Sinh(_X, _Y)); }
	static _Ty atan2(_Ty _Y, _Ty _X)
		{return (::atan2(_Y, _X)); }
	static _Ty cos(_Ty _X)
		{return (::cos(_X)); }
	static _Ty exp(_Ty _X)
		{return (::exp(_X)); }
	static _Ty ldexp(_Ty _R, int _E)
		{return (::ldexp(_R, _E)); }
	static _Ty log(_Ty _X)
		{return (::log(_X)); }
	static _Ty pow(_Ty _X, _Ty _Y)
		{return (::pow(_X, _Y)); }
	static _Ty sin(_Ty _X)
		{return (::sin(_X)); }
	static _Ty sqrt(_Ty _X)
		{return (::sqrt(_X)); }
	};
class _CRTIMP _Ctr<float> {
public:
	typedef float _Ty;
	static _Ty _Cosh(_Ty _X, _Ty _Y)
		{return (_FCosh(_X, _Y)); }
	static short _Exp(_Ty *_P, _Ty _Y, short _E)
		{return (_FExp(_P, _Y, _E)); }
	static _Ty _Infv(_Ty)
		{return (_FInf._F); }
	static bool _Isinf(_Ty _X)
		{return (_FDtest(&_X) == _INFCODE); }
	static bool _Isnan(_Ty _X)
		{return (_FDtest(&_X) == _NANCODE); }
	static _Ty _Nanv(_Ty)
		{return (_FNan._F); }
	static _Ty _Sinh(_Ty _X, _Ty _Y)
		{return (_FSinh(_X, _Y)); }
	static _Ty atan2(_Ty _Y, _Ty _X)
		{return (atan2f(_Y, _X)); }
	static _Ty cos(_Ty _X)
		{return (cosf(_X)); }
	static _Ty exp(_Ty _X)
		{return (expf(_X)); }
	static _Ty ldexp(_Ty _R, int _E)
		{return (ldexpf(_R, _E)); }
	static _Ty log(_Ty _X)
		{return (logf(_X)); }
	static _Ty pow(_Ty _X, _Ty _Y)
		{return (powf(_X, _Y)); }
	static _Ty sin(_Ty _X)
		{return (sinf(_X)); }
	static _Ty sqrt(_Ty _X)
		{return (sqrtf(_X)); }
	};
		// TEMPLATE CLASS _Complex_base
template<class _Ty> class complex;
class _CRTIMP complex<float>;
class _CRTIMP complex<double>;
class _CRTIMP complex<long double>;
template<class _Ty>
	class _Complex_base {
public:
	typedef _Complex_base<_Ty> _Myt;
	typedef _Ctr<_Ty> _Myctr;
	typedef _Ty value_type;
	_Complex_base(const _Ty& _R, const _Ty& _I)
		: _Re(_R), _Im(_I) {}
	_Myt& operator+=(const _Ty& _X)
		{_Re = _Re + _X;
		return (*this); }
	_Myt& operator-=(const _Ty& _X)
		{_Re = _Re - _X;
		return (*this); }
	_Myt& operator*=(const _Ty& _X)
		{_Re = _Re * _X;
		_Im = _Im * _X;
		return (*this); }
	_Myt& operator/=(const _Ty& _X)
		{_Re = _Re / _X;
		_Im = _Im / _X;
		return (*this); }
	_Ty real(const _Ty& _X)
		{return (_Re = _X); }
	_Ty imag(const _Ty& _X)
		{return (_Im = _X); }
	_Ty real() const
		{return (_Re); }
	_Ty imag() const
		{return (_Im); }
protected:
	_Ty _Re, _Im;
	};


		// CLASS complex<float>
class _CRTIMP complex<float> : public _Complex_base<float> {
public:
	typedef float _Ty;
	explicit complex(const complex<double>&);
	explicit complex(const complex<long double>&);
	complex(const _Ty& _R = 0, const _Ty& _I = 0)
		: _Complex_base<_Ty>(_R, _I) {}
	complex<_Ty>& operator=(const _Ty& _X)
		{_Re = _X;
		_Im = 0;
		return (*this); }
	};
		// CLASS complex<double>
class _CRTIMP complex<double> : public _Complex_base<double> {
public:
	typedef double _Ty;
	complex(const complex<float>&);
	explicit complex(const complex<long double>&);
	complex(const _Ty& _R = 0, const _Ty& _I = 0)
		: _Complex_base<_Ty>(_R, _I) {}
	complex<_Ty>& operator=(const _Ty& _X)
		{_Re = _X;
		_Im = 0;
		return (*this); }
	};
		// CLASS complex<long double>
class _CRTIMP complex<long double> : public _Complex_base<long double> {
public:
	typedef long double _Ty;
	complex(const complex<float>&);
	complex(const complex<double>&);
	complex(const _Ty& _R = 0, const _Ty& _I = 0)
		: _Complex_base<_Ty>(_R, _I) {}
	complex<_Ty>& operator=(const _Ty& _X)
		{_Re = _X;
		_Im = 0;
		return (*this); }
	};
		// CONSTRUCTORS FOR complex SPECIALIZATIONS
inline complex<float>::complex(const complex<double>& _X)
	: _Complex_base<float>((_Ty)_X.real(), (_Ty)_X.imag()) {}
inline complex<float>::complex(const complex<long double>& _X)
	: _Complex_base<float>((_Ty)_X.real(), (_Ty)_X.imag()) {}
inline complex<double>::complex(const complex<float>& _X)
	: _Complex_base<double>((_Ty)_X.real(), (_Ty)_X.imag()) {}
inline complex<double>::complex(const complex<long double>& _X)
	: _Complex_base<double>((_Ty)_X.real(), (_Ty)_X.imag()) {}
inline complex<long double>::complex(const complex<float>& _X)
	: _Complex_base<long double>((_Ty)_X.real(), (_Ty)_X.imag()) {}
inline complex<long double>::complex(const complex<double>& _X)
	: _Complex_base<long double>((_Ty)_X.real(), (_Ty)_X.imag()) {}
		// TEMPLATE CLASS complex
template<class _Ty>
	class complex : public _Complex_base<_Ty> {
public:
	complex(const _Ty& _R = 0, const _Ty& _I = 0)
		: _Complex_base<_Ty>(_R, _I) {}
	typedef _Ty _U;
	complex(const complex<_U>& _X)
		: _Complex_base<_Ty>((_Ty)_X.real(), (_Ty)_X.imag()) {}
	complex<_Ty>& operator=(const complex<_U>& _X)
		{_Re = (_Ty)_X.real();
		_Im = (_Ty)_X.imag();
		return (*this); }
	};
		// TEMPLATE complex OPERATORS
template<class _Ty, class _U> inline
	complex<_Ty>& __cdecl operator+=(
	complex<_Ty>& _X,
	const complex<_U>& _Y)
	{_X.real(_X.real() + (_Ty)_Y.real());
	_X.imag(_X.imag() + (_Ty)_Y.imag());
	return (_X); }
template<class _Ty, class _U> inline
	 complex<_Ty>& __cdecl operator-=(
	complex<_Ty>& _X,
	const complex<_U>& _Y)
	{_X.real(_X.real() - (_Ty)_Y.real());
	_X.imag(_X.imag() - (_Ty)_Y.imag());
	return (_X); }
template<class _Ty, class _U> inline
	 complex<_Ty>& __cdecl operator*=(
	complex<_Ty>& _X,
	const complex<_U>& _Y)
	{_Ty _Yre = (_Ty)_Y.real();
	_Ty _Yim = (_Ty)_Y.imag();
	_Ty _W = _X.real() * _Yre - _X.imag() * _Yim;
	_X.imag(_X.real() * _Yim + _X.imag() * _Yre);
	_X.real(_W);
	return (_X); }
template<class _Ty, class _U> inline
	 complex<_Ty>& __cdecl operator/=(
	complex<_Ty>& _X,
	const complex<_U>& _Y)
	{typedef _Ctr<_Ty> _Myctr;
	_Ty _Yre = (_Ty)_Y.real();
	_Ty _Yim = (_Ty)_Y.imag();
	if (_Myctr::_Isnan(_Yre) || _Myctr::_Isnan(_Yim))
		_X.real(_Myctr::_Nanv(_Yre)), _X.imag(_X.real());
	else if ((_Yim < 0 ? -_Yim : +_Yim)
		< (_Yre < 0 ? -_Yre : +_Yre))
		{_Ty _Wr = _Yim / _Yre;
		_Ty _Wd = _Yre + _Wr * _Yim;
		if (_Myctr::_Isnan(_Wd) || _Wd == 0)
			_X.real(_Myctr::_Nanv(_Yre)), _X.imag(_X.real());
		else
			{_Ty _W = (_X.real() + _X.imag() * _Wr) / _Wd;
			_X.imag((_X.imag() - _X.real() * _Wr) / _Wd);
			_X.real(_W); }}
	else if (_Yim == 0)
		_X.real(_Myctr::_Nanv(_Yre)), _X.imag(_X.real());
	else
		{_Ty _Wr = _Yre / _Yim;
		_Ty _Wd = _Yim + _Wr * _Yre;
		if (_Myctr::_Isnan(_Wd) || _Wd == 0)
			_X.real(_Myctr::_Nanv(_Yre)), _X.imag(_X.real());
		else
			{_Ty _W = (_X.real() * _Wr + _X.imag()) / _Wd;
			_X.imag((_X.imag() * _Wr - _X.real()) / _Wd);
			_X.real(_W); }}
	return (_X); }
		// TEMPLATE FUNCTION operator>>
template<class _E, class _Tr, class _U> inline
	basic_istream<_E, _Tr>& __cdecl operator>>(
		basic_istream<_E, _Tr>& _I, complex<_U>& _X)
	{typedef complex<_U> _Myt;
	_E _Ch;
	long double _Re, _Im;
	if (_I >> _Ch && _Ch != '(')
		_I.putback(_Ch), _I >> _Re, _Im = 0;
	else if (_I >> _Re >> _Ch && _Ch != ',')
		if (_Ch == ')')
			_Im = 0;
		else
			_I.putback(_Ch), _I.setstate(ios_base::failbit);
	else if (_I >> _Im >> _Ch && _Ch != ')')
			_I.putback(_Ch), _I.setstate(ios_base::failbit);
	if (!_I.fail())
		_X = _Myt((_U)_Re, (_U)_Im);
	return (_I); }
		// TEMPLATE FUNCTION operator<<
template<class _E, class _Tr, class _U> inline
	basic_ostream<_E, _Tr>& __cdecl operator<<(
		basic_ostream<_E, _Tr>& _O, const complex<_U>& _X)
	{basic_ostringstream<_E, _Tr, allocator<_E> > _S;
	_S.flags(_O.flags());
	_S.imbue(_O.getloc());
	_S.precision(_O.precision());
	_S << '(' << real(_X) << ',' << imag(_X) << ')';
	return (_O << _S.str().c_str()); }
 #define _CMPLX(T)	complex<T >
 #define _CTR(T)	_Ctr<T >
 #define _TMPLT(T)	template<class T >
 #include <xcomplex>
_STD_END
#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif /* _COMPLEX_ */

/*
 * Copyright (c) 1994 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
