// xlocmon internal header (from <locale>)

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _XLOCMON_
#define _XLOCMON_
#include <xiosbase>

#ifdef  _MSC_VER
#pragma pack(push,8)
#endif  /* _MSC_VER */
_STD_BEGIN
		// STRUCT money_base
struct _CRTIMP money_base : public locale::facet {
	enum _Part {symbol = '$', sign = '+', space = ' ',
		value = 'v', none = 'x'};
	_BITMASK(_Part, part);
	struct pattern {
		char field[4];
		};
	money_base(size_t _R = 0)
		: locale::facet(_R) {}
	};
_BITMASK_OPS(money_base::_Part);
		// TEMPLATE CLASS _Mpunct
template<class _E>
	class _Mpunct : public money_base {
public:
	typedef _E char_type;
	typedef basic_string<_E, char_traits<_E>, allocator<_E> >
		string_type;
	_E decimal_point() const
		{return (do_decimal_point()); }
	_E thousands_sep() const
		{return (do_thousands_sep()); }
	string grouping() const
		{return (do_grouping()); }
	string_type curr_symbol() const
		{return (do_curr_symbol()); }
	string_type positive_sign() const
		{return (do_positive_sign()); }
	string_type negative_sign() const
		{return (do_negative_sign()); }
	int frac_digits() const
		{return (do_frac_digits()); }
	pattern pos_format() const
		{return (do_pos_format()); }
	pattern neg_format() const
		{return (do_neg_format()); }
	explicit _Mpunct(size_t _R, bool _Intl)
		: money_base(_R), _Ifl(_Intl) {_Init(_Locinfo()); }
	_Mpunct(const _Locinfo& _Lobj, size_t _R, bool _Intl)
		: money_base(_R), _Ifl(_Intl) {_Init(_Lobj); }
	static size_t __cdecl _Getcat()
		{return (_LC_NUMERIC); }
_PROTECTED:
	virtual ~_Mpunct()
		{delete[] _Mgr;
		delete[] _Mcs;
		delete[] _Mps;
		delete[] _Mns; }
protected:
	void _Init(const _Locinfo& _Lobj)
		{const lconv *_P = _Lobj._Getlconv();
		_Mdp = _WIDEN(_E, _P->mon_decimal_point[0]);
		_Mks = _WIDEN(_E, _P->mon_thousands_sep[0]);
		_Mgr = _MAKLOCSTR(char, _P->mon_grouping);
		_Mcs = _MAKLOCSTR(_E, _Ifl ? _P->int_curr_symbol
			: _P->currency_symbol);
		_Mps = _MAKLOCSTR(_E, _P->p_sign_posn < 0
			|| 4 < _P->p_sign_posn
				? "" : _P->positive_sign);
		_Mns = _MAKLOCSTR(_E, _P->n_sign_posn < 0
			|| 4 < _P->n_sign_posn
				? "-" : _P->negative_sign);
		_Mfd = _Ifl ? _P->int_frac_digits
			: _P->frac_digits;
		if (_Mfd < 0 || CHAR_MAX <= _Mfd)
			_Mfd = 0;
		_Makpat(_Mpf, _P->p_sep_by_space,
			_P->p_cs_precedes, _P->p_sign_posn);
		_Makpat(_Mnf, _P->n_sep_by_space,
			_P->n_cs_precedes, _P->n_sign_posn); }
	virtual _E do_decimal_point() const
		{return (_Mdp); }
	virtual _E do_thousands_sep() const
		{return (_Mks); }
	virtual string do_grouping() const
		{return (string(_Mgr)); }
	virtual string_type do_curr_symbol() const
		{return (string_type(_Mcs)); }
	virtual string_type do_positive_sign() const
		{return (string_type(_Mps)); }
	virtual string_type do_negative_sign() const
		{return (string_type(_Mns)); }
	virtual int do_frac_digits() const
		{return (_Mfd); }
	virtual pattern do_pos_format() const
		{return (_Mpf); }
	virtual pattern do_neg_format() const
		{return (_Mnf); }
private:
	void _Makpat(pattern& _Pat, char _Sep, char _Pre, char _Pos)
		{char *_S = _Ifl || (_Sep & ~1) != 0 || (_Pre & ~1) != 0
			|| _Pos < 0 || 4 < _Pos ? "$+vx"
			: "+v$x" "+v$x" "v$+x" "v+$x" "v$+x"
				"+$vx" "+$vx" "$v+x" "+$vx" "$+vx"
				"+v $" "+v $" "v $+" "v+ $" "v $+"
				"+$ v" "+$ v" "$ v+" "+$ v" "$ +v" + (_Pos
				+ (_Pre == 1 ? 20 : 0) + (_Sep == 1 ? 40 : 0));
		memcpy(_Pat.field, _S, 4); }
	char *_Mgr;
	_E _Mdp, _Mks, *_Mcs, *_Mps, *_Mns;
	int _Mfd;
	pattern _Mpf, _Mnf;
	bool _Ifl;
	};
		// TEMPLATE CLASS moneypunct
template<class _E, bool _Intl = false>
	class moneypunct : public _Mpunct<_E> {
public:
	static const bool intl;
	static locale::id id;
	explicit moneypunct(size_t _R = 0)
		: _Mpunct<_E>(_R, _Intl) {}
	moneypunct(const _Locinfo& _Lobj, size_t _R = 0)
		: _Mpunct<_E>(_Lobj, _R, _Intl) {}
	static size_t __cdecl _Getcat()
		{return (_LC_NUMERIC); }
	};
template<class _E, bool _Intl>
	const bool moneypunct<_E, _Intl>::intl = _Intl;
template<class _E, bool _Intl>
	locale::id moneypunct<_E, _Intl>::id;
		// TEMPLATE CLASS moneypunct_byname
template<class _E, bool _Intl = false>
	class moneypunct_byname : public moneypunct<_E, _Intl> {
public:
	explicit moneypunct_byname(const char *_S, size_t _R = 0)
		: moneypunct<_E, _Intl>(_Locinfo(_S), _R) {}
_PROTECTED:
	virtual ~moneypunct_byname()
		{}
	};
		// TEMPLATE CLASS money_get
template<class _E,
	class _II = istreambuf_iterator<_E, char_traits<_E> > >
	class money_get : public locale::facet {
	typedef moneypunct<_E, false> _Mypunct0;
	typedef moneypunct<_E, true> _Mypunct1;
public:
	typedef _E char_type;
	typedef _II iter_type;
	typedef basic_string<_E, char_traits<_E>, allocator<_E> >
		string_type;
	_II get(_II _F, _II _L, bool _Intl, ios_base& _X,
		ios_base::iostate& _St, long double& _V) const
		{return (do_get(_F, _L, _Intl, _X, _St, _V)); }
	_II get(_II _F, _II _L, bool _Intl, ios_base& _X,
		ios_base::iostate& _St, string_type& _D) const
		{return (do_get(_F, _L, _Intl, _X, _St, _D)); }
	static locale::id id;
	explicit money_get(size_t _R = 0)
		: locale::facet(_R) {_Init(_Locinfo()); }
	money_get(const _Locinfo& _Lobj, size_t _R = 0)
		: locale::facet(_R) {_Init(_Lobj); }
	static size_t __cdecl _Getcat()
		{return (_LC_MONETARY); }
_PROTECTED:
	virtual ~money_get()
		{}
protected:
	void _Init(const _Locinfo& _Lobj)
		{}
	virtual _II do_get(_II _F, _II _L, bool _Intl, ios_base& _X,
		ios_base::iostate& _St, long double& _V) const
		{string_type _Str = _Getmfld(_F, _L, _Intl, _X);
		if (_F == _L)
			_St |= ios_base::eofbit;
		if (_Str.size() == 0)
			_St |= ios_base::failbit;
		else
			{string _Str2;
			_Str2.reserve(_Str.size());
			for (size_t _I = 0; _I < _Str.size(); ++_I)
				_Str2 += (char)_NARROW(_E, _Str[_I]);
			const char *_Eb = _Str2.c_str();
			char *_Ep;
			errno = 0;
			const long double _Ans = _Stold(_Eb, &_Ep, 0);
			if (_Ep == _Eb || errno != 0)
				_St |= ios_base::failbit;
			else
				_V = _Ans; }
		return (_F); }
	virtual _II do_get(_II _F, _II _L, bool _Intl, ios_base& _X,
		ios_base::iostate& _St, string_type& _D) const
		{string_type _Str = _Getmfld(_F, _L, _Intl, _X);
		if (_F == _L)
			_St |= ios_base::eofbit;
		if (_Str.size() == 0)
			_St |= ios_base::failbit;
		else
			_D = _Str;
		return (_F); }
private:
	string_type _Getmfld(_II& _F, _II& _L, bool _Intl,
		ios_base& _X) const
		{const _Mpunct<_E> *_Pfac;
		if (_Intl)
			_Pfac = &_USE(_X.getloc(), _Mypunct1);
		else
			_Pfac = &_USE(_X.getloc(), _Mypunct0);
		const ctype<_E>& _Fac2 = _USE(_X.getloc(), ctype<_E>);
		bool _Bad = false, _Neg = false;
		string_type _Sg, _Str;
		const money_base::pattern _Pat = _Pfac->neg_format();
		for (size_t _N = 0; !_Bad && _N < 4; ++_N)
			switch (_Pat.field[_N])
		{case money_base::symbol:
			{string_type _Cs = _Pfac->curr_symbol();
			const _E *_S;
			if (_X.flags() & ios_base::showbase)
				;
			else if (_N == 3 && _Sg.size() == 0
				|| _F == _L || *_F != *_Cs.c_str())
				_Cs.erase();
			for (_S = _Cs.begin(); _F != _L && _S != _Cs.end()
				&& *_F == *_S; ++_S, ++_F)
				;
			if (_S != _Cs.end())
				_Bad = true; }
			break;
		case money_base::sign:
			{if (_F == _L)
				;
			else if (0 < (_Pfac->positive_sign()).size()
				&& _Pfac->positive_sign()[0] == *_F)
				++_F, _Sg = _Pfac->positive_sign();
			else if (0 < (_Pfac->negative_sign()).size()
				&& _Pfac->negative_sign()[0] == *_F)
				++_F, _Sg = _Pfac->negative_sign(), _Neg = true;
			if (_Sg.size() == 1)
				_Sg.erase(); }
			break;
		case money_base::value:
			{int _Nfd = 0;
			int _Fd = _Pfac->frac_digits();
			const string _Gr = _Pfac->grouping();
			if (*_Gr.c_str() <= '\0')
				while (_F != _L
					&& _Fac2.is(ctype_base::digit, *_F))
					_Str += *_F++;
			else
				{const _E _Ks = _Pfac->thousands_sep();
				string _Grin(1, '\0');
				size_t _I = 0;
				for (; _F != _L; ++_F)
					if (_Fac2.is(ctype_base::digit, *_F))
						{_Str += *_F;
						if (_Grin[_I] != CHAR_MAX)
							++_Grin[_I]; }
					else if (_Grin[_I] == '\0' || *_F != _Ks)
						break;
					else
						_Grin.append(1, '\0'), ++_I;
				if (_I == 0)
					;
				else if ('\0' < _Grin[_I])
					++_I;
				else
					_Bad = true;
				for (const char *_Pg = _Gr.c_str();
					!_Bad && 0 < _I; --_I)
					if (*_Pg == CHAR_MAX)
						break;
					else if (0 < --_I && *_Pg != _Grin[_I]
						|| 0 == _I && *_Pg < _Grin[_I])
						_Bad = true;
					else if ('\0' < _Pg[1])
						++_Pg;
				if (_Bad)
					break; }
			if (_F != _L && *_F == _Pfac->decimal_point())
				{while (++_F != _L
					&& _Fac2.is(ctype_base::digit, *_F))
					if (_Nfd < _Fd)
						_Str += *_F, ++_Nfd; }
			if (_Str.size() == 0)
				_Bad = true;
			else
				for (; _Nfd < _Fd; ++_Nfd)
					_Str += _WIDEN(_E, '0'); }
			break;
		default:
			{if (_N == 3)
				break;
			while (_F != _L && _Fac2.is(ctype_base::space, *_F))
				++_F; }}
		if (!_Bad && 0 < _Sg.size())
			{const _E *_S;
			for (_S = _Sg.begin(); _F != _L
				&& ++_S != _Sg.end() && *_F == *_S; ++_F)
				;
			if (_S != _Sg.end())
				_Bad = true; }
		if (_Bad)
			_Str.erase();
		else if (_Neg)
			_Str.insert((size_t)0, (size_t)1, _WIDEN(_E, '-'));
		return (_Str); }
	};
template<class _E, class _II>
	locale::id money_get<_E, _II>::id;
		// TEMPLATE CLASS money_put
template<class _E,
	class _OI = ostreambuf_iterator<_E, char_traits<_E> > >
	class money_put : public locale::facet {
	typedef moneypunct<_E, false> _Mypunct0;
	typedef moneypunct<_E, true> _Mypunct1;
public:
	typedef _E char_type;
	typedef _OI iter_type;
	typedef basic_string<_E, char_traits<_E>, allocator<_E> >
		string_type;
	_OI put(_OI _F, bool _Intl, ios_base& _X, _E _Fill,
		long double _V) const
		{return (do_put(_F, _Intl, _X, _Fill, _V)); }
	_OI put(_OI _F, bool _Intl, ios_base& _X, _E _Fill,
		const string_type& _D) const
		{return (do_put(_F, _Intl, _X, _Fill, _D)); }
	static locale::id id;
	explicit money_put(size_t _R = 0)
		: locale::facet(_R) {_Init(_Locinfo()); }
	money_put(const _Locinfo& _Lobj, size_t _R = 0)
		: locale::facet(_R) {_Init(_Lobj); }
	static size_t __cdecl _Getcat()
		{return (_LC_MONETARY); }
_PROTECTED:
	virtual ~money_put()
		{}
protected:
	void _Init(const _Locinfo& _Lobj)
		{}
	virtual _OI do_put(_OI _F, bool _Intl,
		ios_base& _X, _E _Fill, long double _V) const
		{bool _Neg = false;
		if (_V < 0)
			_Neg = true, _V = -_V;
		size_t _Exp;
		for (_Exp = 0; 1e35 <= _V && _Exp < 5000; _Exp += 10)
			_V /= 1e10;
		string_type _D2;
		char _Buf[40];
		int _N = sprintf(_Buf, "%.0Lf", _V);
		for (int _I = 0; _I < _N; ++_I)
			_D2.append(1, _WIDEN(_E, _Buf[_I]));
		_D2.append(_Exp, _WIDEN(_E, '0'));
		return (_Putmfld(_F, _Intl, _X, _Fill, _Neg, _D2)); }
	virtual _OI do_put(_OI _F, bool _Intl,
		ios_base& _X, _E _Fill, const string_type& _D) const
		{const ctype<_E>& _Fac = _USE(_X.getloc(), ctype<_E>);
		const _E *_S = _D.c_str();
		bool _Neg = false;
		if (*_S == _WIDEN(_E, '-'))
			_Neg = true, ++_S;
		size_t _N;
		for (_N = 0; _Fac.is(ctype_base::digit, _S[_N]); ++_N)
			;
		string_type _D2(_S, _N);
		if (_N == 0)
			_D2.append(1, _WIDEN(_E, '0'));
		return (_Putmfld(_F, _Intl, _X, _Fill, _Neg, _D2)); }
private:
	_OI _Putmfld(_OI _F, bool _Intl, ios_base& _X, _E _Fill,
		bool _Neg, string_type _D) const
		{const _Mpunct<_E> *_Pfac;
		if (_Intl)
			_Pfac = &_USE(_X.getloc(), _Mypunct1);
		else
			_Pfac = &_USE(_X.getloc(), _Mypunct0);
		size_t _Fd = _Pfac->frac_digits();
		const string _Gr = _Pfac->grouping();
		if (_Fd < _D.size() && '\0' < *_Gr.c_str())
			{const _E _Ks = _Pfac->thousands_sep();
			const char *_Pg = _Gr.c_str();
			size_t _I = _D.size() - _Fd;
			while (*_Pg != CHAR_MAX && '\0' < *_Pg && *_Pg < _I)
				{_D.insert(_I -= *_Pg, (size_t)1, _Ks);
				if ('\0' < _Pg[1])
					++_Pg; }}
		money_base::pattern _Pat;
		string_type _Sg;
		if (_Neg)
			{_Pat = _Pfac->neg_format();
			_Sg = _Pfac->negative_sign(); }
		else
			{_Pat = _Pfac->pos_format();
			_Sg = _Pfac->positive_sign(); }
		string_type _Cs;
		if (_X.flags() & ios_base::showbase)
			_Cs = _Pfac->curr_symbol();
		bool _Intern = false;
		size_t _M, _N;
		for (_M = 0, _N = 0; _N < 4; ++_N)
			switch (_Pat.field[_N])
		{case money_base::symbol:
			_M += _Cs.size();
			break;
		case money_base::sign:
			_M += _Sg.size();
			break;
		case money_base::value:
			_M += _D.size() + (0 < _Fd ? 1 : 0)
				+ (_D.size() <= _Fd ? _Fd - _D.size() + 1 : 0);
			break;
		case money_base::space:
			_Intern = true; }
		_M = _X.width() <= 0 || _X.width() <= _M
			? 0 : _X.width() - _M;
		ios_base::fmtflags _Afl =
			_X.flags() & ios_base::adjustfield;
		if (_Afl != ios_base::left
			&& (_Afl != ios_base::internal || !_Intern))
			_F = _Rep(_F, _Fill, _M), _M = 0;
		for (_N = 0; _N < 4; ++_N)
			switch (_Pat.field[_N])
		{case money_base::symbol:
			_F = _Put(_F, _Cs.begin(), _Cs.size());
			break;
		case money_base::sign:
			if (0 < _Sg.size())
				_F = _Put(_F, _Sg.begin(), 1);
			break;
		case money_base::value:
			if (_Fd == 0)
				_F = _Put(_F, _D.begin(), _D.size());
			else if (_D.size() <= _Fd)
				{*_F++ = _WIDEN(_E, '0');
				*_F++ = _Pfac->decimal_point();
				_F = _Rep(_F, _WIDEN(_E, '0'), _Fd - _D.size());
				_F = _Put(_F, _D.begin(), _D.size()); }
			else
				{_F = _Put(_F, _D.begin(), _D.size() - _Fd);
				*_F++ = _Pfac->decimal_point();
				_F = _Put(_F, _D.end() - _Fd, _Fd); }
			break;
		case money_base::space:
			if (_Afl == ios_base::internal)
				_F = _Rep(_F, _Fill, _M), _M = 0; }
		if (1 < _Sg.size())
			_F = _Put(_F, _Sg.begin() + 1, _Sg.size() - 1);
		_X.width(0);
		return (_Rep(_F, _Fill, _M)); }
	static _OI _Put(_OI _F, const _E *_S, size_t _N)
		{for (; 0 < _N; --_N, ++_F, ++_S)
			*_F = *_S;
		return (_F); }
	static _OI _Rep(_OI _F, _E _C, size_t _N)
		{for (; 0 < _N; --_N, ++_F)
			*_F = _C;
		return (_F); }
	};
template<class _E, class _OI>
	locale::id money_put<_E, _OI>::id;

#ifdef	_DLL

#pragma warning(disable:4231) /* the extern before template is a non-standard extension */

extern template class _CRTIMP _Mpunct<char>;
extern template class _CRTIMP _Mpunct<wchar_t>;

extern template class _CRTIMP moneypunct<char, true>;
extern template class _CRTIMP moneypunct<char, false>;
extern template class _CRTIMP moneypunct<wchar_t, true>;
extern template class _CRTIMP moneypunct<wchar_t, false>;

extern template class _CRTIMP money_get<char, 
                      istreambuf_iterator<char, char_traits<char> > >;
extern template class _CRTIMP money_get<wchar_t, 
                      istreambuf_iterator<wchar_t, char_traits<wchar_t> > >;
extern template class _CRTIMP money_put<char,
                      ostreambuf_iterator<char, char_traits<char> > >;
extern template class _CRTIMP money_put<wchar_t,
                      ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >;

#pragma warning(default:4231) /* restore previous warning */

#endif		// _DLL


_STD_END
#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif /* _XLOCMON_ */

/*
 * Copyright (c) 1995 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
