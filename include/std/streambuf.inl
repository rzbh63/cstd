// streambuf standard header

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _STREAMBUF_
#define _STREAMBUF_
#include <xlocnum>

#ifdef  _MSC_VER
#pragma pack(push,8)
#endif  /* _MSC_VER */
_STD_BEGIN
		// TEMPLATE CLASS basic_streambuf
template<class _E, class _Tr = char_traits<_E> >
	class basic_streambuf {
protected:
	basic_streambuf()
		: _Loc() {_Init(); }
	basic_streambuf(_Uninitialized)
		: _Loc(_Noinit) {}
public:
	typedef basic_streambuf<_E, _Tr> _Myt;
	typedef _E char_type;
	typedef _Tr traits_type;
	virtual ~basic_streambuf()
		{}
	typedef _Tr::int_type int_type;
	typedef _Tr::pos_type pos_type;
	typedef _Tr::off_type off_type;
	pos_type pubseekoff(off_type _O, ios_base::seekdir _W,
		ios_base::openmode _M = ios_base::in | ios_base::out)
		{return (seekoff(_O, _W, _M)); }
	pos_type pubseekoff(off_type _O, ios_base::seek_dir _W,
		ios_base::open_mode _M)
		{return (pubseekoff(_O, (ios_base::seekdir)_W,
			(ios_base::openmode)_M)); }
	pos_type pubseekpos(pos_type _P,
		ios_base::openmode _M = ios_base::in | ios_base::out)
		{return (seekpos(_P, _M)); }
	pos_type pubseekpos(pos_type _P, ios_base::open_mode _M)
		{return (seekpos(_P, (ios_base::openmode)_M)); }
	_Myt *pubsetbuf(_E *_S, streamsize _N)
		{return (setbuf(_S, _N)); }
	locale pubimbue(const locale &_Ln)
		{locale _Lo = _Loc;
		imbue(_Ln);
		_Loc = _Ln;
		return (_Lo); }
	locale getloc()
		{return (_Loc); }
	streamsize in_avail()
		{return (gptr() != 0 && gptr() < egptr()
			? egptr() - gptr() : showmanyc()); }
	int pubsync()
		{return (sync()); }
	int_type sbumpc()
		{return (gptr() != 0 && gptr() < egptr()
			? _Tr::to_int_type(*_Gninc()) : uflow()); }
	int_type sgetc()
		{return (gptr() != 0 && gptr() < egptr()
			? _Tr::to_int_type(*gptr()) : underflow()); }
	streamsize sgetn(_E *_S, streamsize _N)
		{return (xsgetn(_S, _N)); }
	int_type snextc()
		{return (_Tr::eq_int_type(_Tr::eof(), sbumpc())
			? _Tr::eof() : sgetc()); }
	int_type sputbackc(_E _C)
		{return (gptr() != 0 && eback() < gptr()
			&& _Tr::eq(_C, gptr()[-1])
			? _Tr::to_int_type(*_Gndec())
			: pbackfail(_Tr::to_int_type(_C))); }
	void stossc()
		{if (gptr() != 0 && gptr() < egptr())
			_Gninc();
		else
			uflow(); }
	int_type sungetc()
		{return (gptr() != 0 && eback() < gptr()
			? _Tr::to_int_type(*_Gndec()) : pbackfail()); }
	int_type sputc(_E _C)
		{return (pptr() != 0 && pptr() < epptr()
			? _Tr::to_int_type(*_Pninc() = _C)
			: overflow(_Tr::to_int_type(_C))); }
	streamsize sputn(const _E *_S, streamsize _N)
		{return (xsputn(_S, _N)); }
protected:
	_E *eback() const
		{return (*_IGbeg); }
	_E *gptr() const
		{return (*_IGnext); }
	_E *pbase() const
		{return (*_IPbeg); }
	_E *pptr() const
		{return (*_IPnext); }
	_E *egptr() const
		{return (*_IGnext + *_IGcnt); }
	void gbump(int _N)
		{*_IGcnt -= _N;
		*_IGnext += _N; }
	void setg(_E *_B, _E *_N, _E *_L)
		{*_IGbeg = _B, *_IGnext = _N, *_IGcnt = _L - _N; }
	_E *epptr() const
		{return (*_IPnext + *_IPcnt); }
	_E *_Gndec()
		{++*_IGcnt;
		return (--*_IGnext); }
	_E *_Gninc()
		{--*_IGcnt;
		return ((*_IGnext)++); }
	void pbump(int _N)
		{*_IPcnt -= _N;
		*_IPnext += _N; }
	void setp(_E *_B, _E *_L)
		{*_IPbeg = _B, *_IPnext = _B, *_IPcnt = _L - _B; }
	void setp(_E *_B, _E *_N, _E *_L)
		{*_IPbeg = _B, *_IPnext = _N, *_IPcnt = _L - _N; }
	_E *_Pninc()
		{--*_IPcnt;
		return ((*_IPnext)++); }
	void _Init()
		{_IGbeg = &_Gbeg, _IPbeg = &_Pbeg;
		_IGnext = &_Gnext, _IPnext = &_Pnext;
		_IGcnt = &_Gcnt, _IPcnt = &_Pcnt;
		setp(0, 0), setg(0, 0, 0); }
	void _Init(_E **_Gb, _E **_Gn, int *_Gc,
		_E **_Pb, _E **_Pn, int *_Pc)
		{_IGbeg = _Gb, _IPbeg = _Pb;
		_IGnext = _Gn, _IPnext = _Pn;
		_IGcnt = _Gc, _IPcnt = _Pc; }
	virtual int_type overflow(int_type = _Tr::eof())
		{return (_Tr::eof()); }
	virtual int_type pbackfail(int_type = _Tr::eof())
		{return (_Tr::eof()); }
	virtual int showmanyc()
		{return (0); }
	virtual int_type underflow()
		{return (_Tr::eof()); }
	virtual int_type uflow()
		{return (_Tr::eq_int_type(_Tr::eof(), underflow())
			? _Tr::eof() : _Tr::to_int_type(*_Gninc())); }
	virtual streamsize xsgetn(_E * _S, streamsize _N)
		{int_type _C;
		streamsize _M, _Ns;
		for (_Ns = 0; 0 < _N; )
			if (gptr() != 0 && 0 < (_M = egptr() - gptr()))
				{if (_N < _M)
					_M = _N;
				_Tr::copy(_S, gptr(), _M);
				_S += _M, _Ns += _M, _N -= _M, gbump(_M); }
			else if (_Tr::eq_int_type(_Tr::eof(), _C = uflow()))
				break;
			else
				*_S++ = _Tr::to_char_type(_C), ++_Ns, --_N;
		return (_Ns); }
	virtual streamsize xsputn(const _E *_S, streamsize _N)
		{streamsize _M, _Ns;
		for (_Ns = 0; 0 < _N; )
			if (pptr() != 0 && 0 < (_M = epptr() - pptr()))
				{if (_N < _M)
					_M = _N;
				_Tr::copy(pptr(), _S, _M);
				_S += _M, _Ns += _M, _N -= _M, pbump(_M); }
			else if (_Tr::eq_int_type(_Tr::eof(),
				overflow(_Tr::to_int_type(*_S))))
				break;
			else
				++_S, ++_Ns, --_N;
		return (_Ns); }
	virtual pos_type seekoff(off_type, ios_base::seekdir,
		ios_base::openmode = ios_base::in | ios_base::out)
		{return (streampos(_BADOFF)); }
	virtual pos_type seekpos(pos_type,
		ios_base::openmode = ios_base::in | ios_base::out)
		{return (streampos(_BADOFF)); }
	virtual _Myt *setbuf(_E *, streamsize)
		{return (this); }
	virtual int sync()
		{return (0); }
	virtual void imbue(const locale& _Loc)
		{}
private:
	_E *_Gbeg, *_Pbeg;
	_E **_IGbeg, **_IPbeg;
	_E *_Gnext, *_Pnext;
	_E **_IGnext, **_IPnext;
	int _Gcnt, _Pcnt;
	int *_IGcnt, *_IPcnt;
	locale _Loc;
	};

#ifdef _DLL
#pragma warning(disable:4231) /* the extern before template is a non-standard extension */
extern template class _CRTIMP basic_streambuf<char, char_traits<char> >;
extern template class _CRTIMP basic_streambuf<wchar_t, char_traits<wchar_t> >;
#pragma warning(default:4231) /* restore previous warning */
#endif		// _DLL

_STD_END
#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif /* _STREAMBUF_ */

/*
 * Copyright (c) 1994 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
