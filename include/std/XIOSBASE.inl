// xiosbase internal header (from <ios>)

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _XIOSBASE_
#define _XIOSBASE_
#include <xlocale>

#ifdef  _MSC_VER
#pragma pack(push,8)
#endif  /* _MSC_VER */
_STD_BEGIN
                // CLASS ios_base
class _CRTIMP ios_base {
public:
                        // CLASS failure
        class failure : public runtime_error {
        public:
                explicit failure(const string &_S)
                        : runtime_error(_S) {}
                virtual ~failure()
                        {}
        protected:
                virtual void _Doraise() const
                        {_RAISE(*this); }
        };
        enum _Fmtflags {skipws = 0x0001, unitbuf = 0x0002,
                uppercase = 0x0004, showbase = 0x0008,
                showpoint = 0x0010, showpos = 0x0020,
                left = 0x0040, right = 0x0080, internal = 0x0100,
                dec = 0x0200, oct = 0x0400, hex = 0x0800,
                scientific = 0x1000, fixed = 0x2000, boolalpha = 0x4000,
                adjustfield = 0x01c0, basefield = 0x0e00,
                floatfield = 0x3000, _Fmtmask = 0x7fff, _Fmtzero = 0};
        enum _Iostate {goodbit = 0x0, eofbit = 0x1,
                failbit = 0x2, badbit = 0x4, _Statmask = 0x7};
        enum _Openmode {in = 0x01, out = 0x02, ate = 0x04,
                app = 0x08, trunc = 0x10, binary = 0x20};
        enum seekdir {beg = 0, cur = 1, end = 2};
        enum event {erase_event, imbue_event, copyfmt_event};
        typedef void (__cdecl *event_callback)(event, ios_base&, int);
        _BITMASK(_Fmtflags, fmtflags);
        _BITMASK(_Iostate, iostate);
        _BITMASK(_Openmode, openmode);
        typedef short io_state, open_mode, seek_dir;
                        // CLASS Init
        class _CRTIMP Init {
        public:
                Init();
                ~Init();
        private:
                static int _Init_cnt;
                };
        ios_base& operator=(const ios_base& _R)
                {if (this != &_R)
                        {_State = _R._State;
                        copyfmt(_R); }
                return (*this); }
        operator void *() const
                {return (fail() ? 0 : (void *)this); }
        bool operator!() const
                {return (fail()); }
        void clear(iostate = goodbit, bool = false);
        void clear(io_state _St)
                {clear((iostate)_St); }
        iostate rdstate() const
                {return (_State); }
        void setstate(iostate _St, bool _Ex = false)
                {if (_St != goodbit)
                        clear((iostate)((int)rdstate() | (int)_St), _Ex); }
        void setstate(io_state _St)
                {setstate((iostate)_St); }
        bool good() const
                {return (rdstate() == goodbit); }
        bool eof() const
                {return ((int)rdstate() & (int)eofbit); }
        bool fail() const
                {return (((int)rdstate() & ((int)badbit | (int)failbit)) != 0); }
        bool bad() const
                {return (((int)rdstate() & (int)badbit) != 0); }
        iostate exceptions() const
                {return (_Except); }
        void exceptions(iostate _Ne)
                {_Except = _Ne & _Statmask;
                clear(_State); }
        void exceptions(io_state _St)
                {exceptions((iostate)_St); }
        fmtflags flags() const
                {return (_Fmtfl); }
        fmtflags flags(fmtflags _Nf)
                {fmtflags _Of = _Fmtfl;
                _Fmtfl = _Nf & _Fmtmask;
                return (_Of); }
        fmtflags setf(fmtflags _Nf)
                {ios_base::fmtflags _Of = _Fmtfl;
                _Fmtfl |= _Nf & _Fmtmask;
                return (_Of); }
        fmtflags setf(fmtflags _Nf, fmtflags _M)
                {ios_base::fmtflags _Of = _Fmtfl;
                _Fmtfl = (_Fmtfl & ~_M) | (_Nf & _M & _Fmtmask);
                return (_Of); }
        void unsetf(fmtflags _M)
                {_Fmtfl &= ~_M; }
        streamsize precision() const
                {return (_Prec); }
        streamsize precision(int _Np)
                {streamsize _Op = _Prec;
                _Prec = _Np;
                return (_Op); }
        streamsize width() const
                {return (_Wide); }
        streamsize width(streamsize _Nw)
                {streamsize _Ow = _Wide;
                _Wide = _Nw;
                return (_Ow); }
        locale getloc() const
                {return (_Loc); }
        locale imbue(const locale&);
        static int __cdecl xalloc()
                {_Lockit _Lk;
                return (_Index++); }
        long& iword(int _Idx)
                {return (_Findarr(_Idx)._Lo); }
        void *& pword(int _Idx)
                {return (_Findarr(_Idx)._Vp); }
        void register_callback(event_callback, int);
        ios_base& copyfmt(const ios_base&);
        virtual ~ios_base();
        static bool __cdecl sync_with_stdio(bool _Sfl = true)
                {_Lockit _Lk;
                const bool _Osfl = _Sync;
                _Sync = _Sfl;
                return (_Osfl); }
protected:
        ios_base()
                : _Loc(_Noinit), _Stdstr(0) {}
        void _Addstd();
        void _Init();
private:
                        // STRUCT _Iosarray
        struct _Iosarray {
        public:
                _Iosarray(int _Idx, _Iosarray *_Link)
                        : _Next(_Link), _Index(_Idx), _Lo(0), _Vp(0) {}
                _Iosarray *_Next;
                int _Index;
                long _Lo;
                void *_Vp;
                };
                        // STRUCT _Fnarray
        struct _Fnarray {
                _Fnarray(int _Idx, event_callback _P, _Fnarray *_Link)
                        : _Next(_Link), _Index(_Idx), _Pfn(_P) {}
                _Fnarray *_Next;
                int _Index;
                event_callback _Pfn;
                };
        void _Callfns(event);
        _Iosarray& _Findarr(int);
        void _Tidy();
        iostate _State, _Except;
        fmtflags _Fmtfl;
        int _Prec, _Wide;
        _Iosarray *_Arr;
        _Fnarray *_Calls;
        locale _Loc;
        size_t _Stdstr;
        static int _Index;
        static bool _Sync;
        };
_BITMASK_OPS(ios_base::_Fmtflags)
_BITMASK_OPS(ios_base::_Iostate)
_BITMASK_OPS(ios_base::_Openmode)
_STD_END
#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif /* _XIOSBASE_ */

/*
 * Copyright (c) 1995 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
