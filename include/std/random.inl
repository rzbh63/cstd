// random standard header
#pragma once
#ifndef _RANDOM_
#define _RANDOM_
#ifndef RC_INVOKED
#include <istream>
#include <vector>

 #pragma pack(push,_CRT_PACKING)
 #pragma warning(push,3)
 #pragma push_macro("new")
 #undef new

 #pragma warning(disable: 4127 4244 4296 4389 4521 4723)

  #pragma warning(disable: 6294 6326)

 #ifndef _BITS_BYTE
  #define _BITS_BYTE	8
 #endif /* _BITS_BYTE */

 #if defined(_DEBUG) || defined(_RNG_CHECK)
  #define _RNG_ASSERT(ex, msg)	\
	((ex) ? (void)0 : _Rng_abort(__FILE__ "(" _STRINGIZE(__LINE__) "): " msg))

 #else /* defined(_DEBUG) || defined(_RNG_CHECK) */
  #define _RNG_ASSERT(ex, msg) ((void)0)
 #endif /* defined(_DEBUG) || defined(_RNG_CHECK) */

_STD_BEGIN
	// TEMPLATE STRUCT _Is_RealType
template<class _Ty>
	struct _Is_RealType
		: _Cat_base<is_same<_Ty, float>::value
		|| is_same<_Ty, double>::value
		|| is_same<_Ty, long double>::value>
	{	// determine whether _Ty satisfies <random>'s RealType requirements
	};

	// TEMPLATE STRUCT _Is_UIntType
template<class _Ty>
	struct _Is_UIntType
		: _Cat_base<is_same<_Ty, unsigned short>::value
		|| is_same<_Ty, unsigned int>::value
		|| is_same<_Ty, unsigned long>::value
		|| is_same<_Ty, unsigned long long>::value>
	{	// determine whether _Ty satisfies <random>'s UIntType requirements
	};

	// TEMPLATE STRUCT _Is_IntType
template<class _Ty>
	struct _Is_IntType
		: _Cat_base<is_same<_Ty, short>::value
		|| is_same<_Ty, int>::value
		|| is_same<_Ty, long>::value
		|| is_same<_Ty, long long>::value
		|| _Is_UIntType<_Ty>::value>
	{	// determine whether _Ty satisfies <random>'s IntType requirements
	};

	// TEMPLATE STRUCT _Enable_if_seed_seq
template<class _Seed_seq,
	class _Self,
	class _Engine = _Self>
	struct _Enable_if_seed_seq
		: enable_if<
			!is_convertible<typename decay<_Seed_seq>::type,
				typename _Self::result_type>::value
			&& !is_same<typename decay<_Seed_seq>::type,
				_Self>::value
			&& !is_same<typename decay<_Seed_seq>::type,
				_Engine>::value>
	{	// determine whether _Seed_seq qualifies as a seed sequence
	};

	// CONSTANTS
static constexpr long double _Pi = 3.14159265358979323846264338327950288L;
static constexpr long double _Exp1 = 2.71828182845904523536028747135266250L;
static constexpr long double _Two32 = 4294967296.0L;
static constexpr long double _Two31 = 2147483648.0L;

	// HELPER FUNCTIONS
[[noreturn]] _CRTIMP2_PURE void __CLRCALL_PURE_OR_CDECL _Rng_abort(_In_z_ const char *_Msg);
_CRTIMP2_PURE float __CLRCALL_PURE_OR_CDECL _XLgamma(float);
_CRTIMP2_PURE double __CLRCALL_PURE_OR_CDECL _XLgamma(double);
_CRTIMP2_PURE long double __CLRCALL_PURE_OR_CDECL _XLgamma(long double);

	// I/O HELPERS FOR FLOATING-POINT VALUES
static constexpr int _Nwords = 4;

template<class _Elem,
	class _Traits>
	basic_ostream<_Elem, _Traits>& _Write(
		basic_ostream<_Elem, _Traits>& _Os, long double _Dx)
	{	// write long double to stream
	int _Ex;
	long double _Frac = _CSTD frexpl(_Dx, &_Ex);
	for (int _Nw = 0; _Nw < _Nwords; ++_Nw)
		{	// break into 31-bit words
		_Frac *= _Two31;
		long _Digits = (long)_Frac;
		_Frac -= _Digits;
		_Os << ' ' << _Digits;
		}
	_Os << ' ' << _Ex;
	return (_Os);
	}

template<class _Elem,
	class _Traits>
	basic_istream<_Elem, _Traits>&
		_Read(basic_istream<_Elem, _Traits>& _Is, long double& _Dx)
	{	// read long double from stream
	long double _Frac = 0.0;
	long _Digits;
	for (int _Nw = 0; _Nw < _Nwords; ++_Nw)
		{	// accumulate 31-bit words
		_Is >> _Digits;
		long double _Temp = _Digits / _Two31;
		for (int _Idx = 0; _Idx < _Nw; ++_Idx)
			_Temp /= _Two31;
		_Frac += _Temp;
		}
	_Is >> _Digits;
	_Dx = _CSTD ldexpl(_Frac, _Digits);
	return (_Is);
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>&
		_In(basic_istream<_Elem, _Traits>& _Is, _Ty& _Dx)
	{	// read from stream
	long double _Vx;
	_Ty _Max = (numeric_limits<_Ty>::max)();
	_Read(_Is, _Vx);
	if (_CSTD fabsl(_Vx) <= _Max)
		_Dx = (_Ty)_Vx;
	else if (_Vx < 0)
		_Dx = -_Max;
	else
		_Dx = _Max;
	return (_Is);
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>&
		_Out(basic_ostream<_Elem, _Traits>& _Os, _Ty _Dx)
	{	// write to stream
	return (_Write(_Os, _Dx));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	class _Wrap_istream
	{	// wrap input stream as function object
public:
	_Wrap_istream(basic_istream<_Elem, _Traits>& _Is)
		: _Str(_Is)
		{	// construct
		}

	_Ty operator()()
		{	// read next value
		_Ty _Data;
		_Str >> _Data;
		if (!_Str)
			_Xinvalid_argument("input stream corrupted");
		return (_Data);
		}

	_Wrap_istream& operator=(const _Wrap_istream&) = delete;

private:
	basic_istream<_Elem, _Traits>& _Str;
	};

	// CLASS seed_seq
class seed_seq
	{	// standard sequence of seed values
public:
	typedef unsigned int result_type;

	seed_seq()
		{	// default constructor
		}

	template<class _Ty>
		seed_seq(_XSTD initializer_list<_Ty> _Ilist)
		{	// construct from [_First, _Last)
		_Construct(_Ilist.begin(), _Ilist.end());
		}

	template<class _InIt>
		seed_seq(_InIt _First, _InIt _Last)
		{	// construct from [_First, _Last)
		_Construct(_First, _Last);
		}

	template<class _RanIt>
		void generate(_RanIt _First, _RanIt _Last) const
		{	// generate randomized interval from seeds
		_DEBUG_RANGE(_First, _Last);
		_Generate_unchecked(_Unchecked(_First), _Unchecked(_Last));
		}

	template<class _OutIt>
		void param(_OutIt _Dest) const
		{	// copy seeds
		_STD copy(_Myvec.begin(), _Myvec.end(), _Dest);
		}

	size_t size() const _NOEXCEPT
		{	// get size of seed
		return (_Myvec.size());
		}

	seed_seq(const seed_seq&) = delete;
	void operator=(const seed_seq&) = delete;

private:
	template<class _InIt>
		void _Construct(_InIt _First, _InIt _Last)
		{	// construct from [_First, _Last)
		for (; _First != _Last; ++_First)
			_Myvec.push_back((unsigned int)*_First);
		}

	template<class _RanIt>
		void _Generate_unchecked(_RanIt _First, _RanIt _Last) const
		{	// generate randomized interval from seeds
		const size_t _Nx = _Last - _First;

		if (0 < _Nx)
			{	// finite interval, fill it
			const size_t _Sx = _Myvec.size();
			const size_t _Tx = 623 <= _Nx ? 11 : 68 <= _Nx ? 7
				: 39 <= _Nx ? 5 : 7 <= _Nx ? 3 : (_Nx - 1) / 2;
			const size_t _Px = (_Nx - _Tx) / 2;
			const size_t _Qx = _Px + _Tx;
			const size_t _Mx = _Nx <= _Sx ? _Sx + 1 : _Nx;
			size_t _Kx;

			_Iter_value_t<_RanIt> _Mask = _Iter_value_t<_RanIt>(1) << 31;
			_Mask <<= 1;	// build 32-bit mask safely
			_Mask -= 1;

			for (_Kx = 0; _Kx < _Nx; ++_Kx)
				_First[_Kx] = 0x8b8b8b8b;
			for (_Kx = 0; _Kx < _Mx; ++_Kx)
				{	// scramble and add any vector contributions
				result_type _R1 = 1664525 * _Xor27(_First[_Kx % _Nx]
					^ _First[(_Kx + _Px) % _Nx] ^ _First[(_Kx - 1) % _Nx]);
				result_type _R2 = (result_type)((_R1 + (_Kx == 0 ? _Sx
					: _Kx <= _Sx ? _Kx % _Nx + _Myvec[(_Kx - 1) % _Sx]
					: _Kx % _Nx)) & _Mask);

				_First[(_Kx + _Px) % _Nx] =
					(_First[(_Kx + _Px) % _Nx] + _R1) & _Mask;
				_First[(_Kx + _Qx) % _Nx] =
					(_First[(_Kx + _Qx) % _Nx] + _R2) & _Mask;
				_First[_Kx % _Nx] = _R2;
				}
			for (; _Kx < _Mx + _Nx; ++_Kx)
				{	// rescramble
				result_type _R3 = 1566083941 * _Xor27(_First[_Kx % _Nx]
					+ _First[(_Kx + _Px) % _Nx] + _First[(_Kx - 1) % _Nx]);
				result_type _R4 = (result_type)((_R3 - _Kx % _Nx) & _Mask);

				_First[(_Kx + _Px) % _Nx] =
					(_First[(_Kx + _Px) % _Nx] ^ _R3) & _Mask;
				_First[(_Kx + _Qx) % _Nx] =
					(_First[(_Kx + _Qx) % _Nx] ^ _R4) & _Mask;
				_First[_Kx % _Nx] = _R4;
				}
			}
		}

	result_type _Xor27(result_type _Val) const
		{	// shift and merge
		return (_Val ^ (_Val >> 27));
		}

	vector<result_type> _Myvec;
	};

	// TEMPLATE FUNCTION generate_canonical
template<class _Real,
	size_t _Bits,
	class _Gen>
	_Real generate_canonical(_Gen& _Gx)
	{	// build a floating-point value from random sequence
	static_assert(_Is_RealType<_Real>::value,
		"invalid template argument for generate_canonical");

	const size_t _Digits = static_cast<size_t>(numeric_limits<_Real>::digits);
	const size_t _Minbits = _Digits < _Bits ? _Digits : _Bits;

	const _Real _Gxmin = static_cast<_Real>((_Gx.min)());
	const _Real _Gxmax = static_cast<_Real>((_Gx.max)());
	const _Real _Rx = (_Gxmax - _Gxmin) + static_cast<_Real>(1);

	const int _Ceil = static_cast<int>(_STD ceil(
		static_cast<_Real>(_Minbits) / _STD log2(_Rx)));
	const int _Kx = _Ceil < 1 ? 1 : _Ceil;

	_Real _Ans = static_cast<_Real>(0);
	_Real _Factor = static_cast<_Real>(1);

	for (int _Idx = 0; _Idx < _Kx; ++_Idx)
		{	// add in another set of bits
		_Ans += (static_cast<_Real>(_Gx()) - _Gxmin) * _Factor;
		_Factor *= _Rx;
		}

	return (_Ans / _Factor);
	}

#define _NRAND(eng, resty) \
	(_STD generate_canonical<resty, static_cast<size_t>(-1)>(eng))


	// MULTIPLE PRECISION MATH FUNCTIONS
typedef _ULONGLONG _Max_type;

static constexpr int _MP_len = 5;
typedef _Max_type _MP_arr[_MP_len];

_CRTIMP2_PURE _Max_type __CLRCALL_PURE_OR_CDECL _MP_Get(_MP_arr);
_CRTIMP2_PURE void __CLRCALL_PURE_OR_CDECL _MP_Add(_MP_arr, _Max_type);
_CRTIMP2_PURE void __CLRCALL_PURE_OR_CDECL _MP_Mul(_MP_arr, _Max_type, _Max_type);
_CRTIMP2_PURE void __CLRCALL_PURE_OR_CDECL _MP_Rem(_MP_arr, _Max_type);

	// TEMPLATE CLASS linear_congruential
template<class _Ity,
	class _Ty,
	_Max_type _Ax,
	_Max_type _Cx,
	_Max_type _Mx>
	struct _Mul_mod
	{	// template class for linear congruential generator where
		// _Ty is large enough to hold intermediate result without overflow
	_Mul_mod(_Ty _Val = 0)
		: _Prev(_Val)
		{	// construct
		}

	_Ty operator()()
		{	// return next value
		_Ty _Divisor = (_Ty)_Mx;

		_Prev = _Mx ? ((_Ity)_Ax * _Prev + (_Ty)_Cx) % _Divisor
			: ((_Ity)_Ax * _Prev + (_Ty)_Cx);
		if (_Prev < static_cast<_Ty>(0))
			_Prev += (_Ty)_Mx;
		return (_Prev);
		}

	_Ty _Prev;
	};

template<class _Ty,
	_Max_type _Ax,
	_Max_type _Cx,
	_Max_type _Mx>
	class _Mult_prec
	{	// template class for linear congruential generator using
		// multiple precision arithmetic to avoid overflows
public:
	_Mult_prec(_Ty _Val = 0)
		: _Prev(_Val)
		{	// construct
		}

	_Ty operator()()
		{	// return next value
		_MP_arr _Wx;
		_MP_Mul(_Wx, _Prev, _Ax);
		_MP_Add(_Wx, _Cx);
		_MP_Rem(_Wx, _Mx);
		_Prev = _MP_Get(_Wx);
		return (_Prev);
		}

	_Ty _Prev;
	};

template<class _Ty,
	_Max_type _Ax,
	_Max_type _Cx,
	_Max_type _Mx,
	bool>
	struct _Select_ulonglong
	{	// unsigned long long too small, use multiple precision
	typedef _Mult_prec<_Ty, _Ax, _Cx, _Mx> type;
	};

template<class _Ty,
	_Max_type _Ax,
	_Max_type _Cx,
	_Max_type _Mx>
	struct _Select_ulonglong<_Ty, _Ax, _Cx, _Mx, true>
	{	// unsigned long long can hold intermediate result
	typedef _Mul_mod<_ULONGLONG, _Ty, _Ax, _Cx, _Mx> type;
	};

template<class _Ty,
	_Max_type _Ax,
	_Max_type _Cx,
	_Max_type _Mx,
	bool>
	struct _Select_ulong
	{	// unsigned long too small, try unsigned long long
	typedef typename _Select_ulonglong<_Ty, _Ax, _Cx, _Mx,
		_Cx < _ULLONG_MAX && _Mx <= (_ULLONG_MAX - _Cx) / _Ax>::type type;
	};

template<class _Ty,
	_Max_type _Ax,
	_Max_type _Cx,
	_Max_type _Mx>
	struct _Select_ulong<_Ty, _Ax, _Cx, _Mx, true>
	{	// unsigned long can hold intermediate result
	typedef _Mul_mod<unsigned long, _Ty, _Ax, _Cx, _Mx> type;
	};

template<class _Ty,
	_Max_type _Ax,
	_Max_type _Cx,
	_Max_type _Mx,
	bool>
	struct _Select_uint
	{	// unsigned int too small, try unsigned long
	typedef typename _Select_ulong<_Ty, _Ax, _Cx, _Mx,
		_Cx < ULONG_MAX && _Mx <= (ULONG_MAX - _Cx) / _Ax>::type type;
	};

template<class _Ty,
	_Max_type _Ax,
	_Max_type _Cx,
	_Max_type _Mx>
	struct _Select_uint<_Ty, _Ax, _Cx, _Mx, true>
	{	// unsigned int can hold intermediate result
	typedef _Mul_mod<unsigned int, _Ty, _Ax, _Cx, _Mx> type;
	};

template<class _Ty,
	_Max_type _Ax,
	_Max_type _Cx,
	_Max_type _Mx>
	struct _Select
	{	// select a type large enough to hold intermediate result
	typedef typename _Select_uint<_Ty, _Ax, _Cx, _Mx,
		_Cx < UINT_MAX && _Mx <= (UINT_MAX - _Cx) / _Ax>::type type;
	};

template<class _Uint,
	_Uint _Ax,
	_Uint _Cx,
	_Uint _Mx>
	class linear_congruential
	{	// template class to implement linear congruential generator
public:
	typedef linear_congruential<_Uint, _Ax, _Cx, _Mx> _Myt;
	typedef _Uint result_type;

	static constexpr _Uint multiplier = _Ax;
	static constexpr _Uint increment = _Cx;
	static constexpr _Uint modulus = _Mx;

	static constexpr _Uint default_seed = 1U;

	explicit linear_congruential(_Uint _X0 = default_seed)
		{	// construct from initial value
		seed(_X0);
		}

	template<class _Gen,
		class = typename _Enable_if_seed_seq<_Gen, _Myt>::type>
		linear_congruential(_Gen& _Gx)
		{	// construct from generator
		seed(_Gx);
		}

	void seed(_Uint _X0 = default_seed)
		{	// reset sequence from numeric value
		_Reset(_X0);
		}

	template<class _Gen>
		void seed(_Gen& _Gx, bool = false)
		{	// reset sequence from generator
		_Seed(_Gx, is_arithmetic<_Gen>());
		}

	_Uint (min)() const
		{	// return minimum possible generated value
		return (_Cx != 0 ? 0 : 1);
		}

	_Uint (max)() const
		{	// return maximum possible generated value
		return (_Mx != 0 ? _Mx - 1 : (numeric_limits<_Uint>::max)());
		}

	_Uint operator()()
		{	// return next value
		return (_Imp());
		}

	void discard(unsigned long long _Nskip)
		{	// discard _Nskip elements
		for (; 0 < _Nskip; --_Nskip)
			(*this)();
		}

	bool _Equals(const _Myt& _Right) const
		{	// return true if *this will generate same sequence as _Right
		return (_Imp._Prev == _Right._Imp._Prev);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>&
			_Write(basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		return (_Ostr << _Imp._Prev);
		}

protected:
	template<class _Gen>
		void _Seed(_Gen& _Gx, true_type)
		{	// reset sequence from numeric value
		_Reset((_Uint)_Gx);
		}

	template<class _Gen>
		void _Seed(_Gen& _Gx, false_type)
		{	// reset sequence from generator
		_Reset(_Gx());
		}

  #pragma warning(push)
  #pragma warning(disable: 4724) // potential mod by 0
	void _Reset(_Uint _X0)
		{	// reset sequence
		_Imp._Prev = _Mx != 0 ? _X0 % _Mx : _X0;
		if (_Imp._Prev == 0 && _Cx == 0)
			_Imp._Prev = 1;
		}
  #pragma warning(pop)

	typename _Select<_Uint, _Ax, _Cx, _Mx>::type _Imp;
	};

template<class _Uint,
	_Uint _Ax,
	_Uint _Cx,
	_Uint _Mx>
	bool operator==(
		const linear_congruential<_Uint, _Ax, _Cx, _Mx>& _Left,
		const linear_congruential<_Uint, _Ax, _Cx, _Mx>& _Right)
	{	// return true if _Left will generate same sequence as _Right
	return (_Left._Equals(_Right));
	}

template<class _Uint,
	_Uint _Ax,
	_Uint _Cx,
	_Uint _Mx>
	bool operator!=(
		const linear_congruential<_Uint, _Ax, _Cx, _Mx>& _Left,
		const linear_congruential<_Uint, _Ax, _Cx, _Mx>& _Right)
	{	// return true if *this will not generate same sequence as _Right
	return (!_Left._Equals(_Right));
	}

template<class _Elem,
	class _Traits,
	class _Uint,
	_Uint _Ax,
	_Uint _Cx,
	_Uint _Mx>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		linear_congruential<_Uint, _Ax, _Cx, _Mx>& _Eng)
	{	// read state from _Istr
	_Wrap_istream<_Elem, _Traits, _Uint> _In(_Istr);
	_Eng.seed(_In);
	return (_Istr);
	}

template<class _Elem,
	class _Traits,
	class _Uint,
	_Uint _Ax,
	_Uint _Cx,
	_Uint _Mx>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const linear_congruential<_Uint, _Ax, _Cx, _Mx>& _Eng)
	{	// write state to _Ostr
	return (_Eng._Write(_Ostr));
	}

		// TEMPLATE CLASS linear_congruential_engine
template<class _Uint,
	_Uint _Ax,
	_Uint _Cx,
	_Uint _Mx>
	class linear_congruential_engine
		: public linear_congruential<_Uint, _Ax, _Cx, _Mx>
	{	// template class to implement linear congruential generator
public:
	static_assert(_Is_UIntType<_Uint>::value
		&& (0 == _Mx || (_Ax < _Mx && _Cx < _Mx)),
		"invalid template argument for linear_congruential_engine");

	typedef linear_congruential_engine<_Uint, _Ax, _Cx, _Mx> _Myt;
	typedef linear_congruential<_Uint, _Ax, _Cx, _Mx> _Mybase;

	explicit linear_congruential_engine(_Uint _X0 = _Mybase::default_seed)
		: linear_congruential<_Uint, _Ax, _Cx, _Mx>(_X0)	// TRANSITION, VSO#184864
		{	// construct from initial value
		}

	template<class _Seed_seq,
		class = typename _Enable_if_seed_seq<_Seed_seq, _Myt>::type>
		explicit linear_congruential_engine(_Seed_seq& _Seq)
		: linear_congruential<_Uint, _Ax, _Cx, _Mx>()	// TRANSITION, VSO#184864
		{	// construct from seed sequence
		seed(_Seq);
		}

	void seed(_Uint _X0 = _Mybase::default_seed)
		{	// reset sequence from numeric value
		this->_Reset(_X0);
		}

	static constexpr int _Nw = (_BITS_BYTE * sizeof (_Uint) + 31) / 32;

	template<class _Seed_seq,
		class = typename _Enable_if_seed_seq<_Seed_seq, _Myt>::type>
		void seed(_Seed_seq& _Seq)
		{	// reset sequence from seed sequence
		_Uint _Arr[3 + _Nw];
		int _Lsh = _BITS_BYTE * sizeof (_Uint);	// to quiet diagnostics

		_Seq.generate(&_Arr[0], &_Arr[3 + _Nw]);
		for (int _Idx = _Nw; 0 < --_Idx; )
			_Arr[3 + _Idx - 1] |=
				_Arr[3 + _Idx] << _Lsh;
		this->_Reset(_Arr[3]);
		}

	static constexpr _Uint (min)()
		{	// return minimum possible generated value
		return (_Cx != 0 ? 0 : 1);
		}

	static constexpr _Uint (max)()
		{	// return maximum possible generated value
		return (_Mx != 0 ? _Mx - 1 : (numeric_limits<_Uint>::max)());
		}
	};

	// TEMPLATE CLASS _Circ_buf FOR subtract_with_carry,
	//	subtract_with_carry_01, and mersenne_twister
template<class _Ty,
	size_t _Nw>
	struct _Circ_buf
	{	// template class to hold historical values for generators
	_Ty _At(int _Ix) const
		{	// return value at current position plus _Ix
		return (_Ax[_Base(_Ix)]);
		}

	bool _Equals(const _Circ_buf& _Right) const
		{	// return true if *this holds same values as _Right
		const _Ty *_Last1 = _Ax + _Idx;
		const _Ty *_Last2 = _Right._Ax + _Right._Idx;
		const _Ty *_First, *_Last, *_Other;
		bool _Use2 = _Base() < _Right._Base();

		if (_Use2)
			{	// _Right's range is higher up in the array
				// than ours, so scan it first
			_First = _Right._Ax + _Right._Base();
			_Last = _Last2;
			_Other = _Ax + _Base();
			}
		else
			{	// our range is higher up in the array
				// than _Right's, so scan ours first
			_First = _Ax + _Base();
			_Last = _Last1;
			_Other = _Right._Ax + _Right._Base();
			}

		ptrdiff_t _N0 = _Nw;
		while (0 < _N0)
			{	// scan
				// note: may need up to three passes; each scan starts at the
				// current highest array position and ends at the end of the
				// array or the _Idx value, whichever comes first; the
				// equality test succeeds only by reaching the _Idx value.
			const _Ty *_Limit = _First < _Last ? _Last
				: _Use2 ? _Right._Ax + 2 * _Nw
				: _Ax + 2 * _Nw;
			_N0 -= _Limit - _First;
			while (_First != _Limit)
				if (*_First++ != *_Other++)
					return (false);
			_First = _Other;
			_Last = _Use2 ? _Last1 : _Last2;
			_Other = _Use2 ? _Right._Ax : _Ax;
			_Use2 = !_Use2;
			}
		return (true);
		}

	unsigned int _Base(int _Ix = 0) const
		{	// return _Ix'th historical element (loosely, (_Idx - _Nw) + _Ix)
		return ((_Ix += _Idx) < _Nw ? (_Ix + _Nw) : (_Ix - _Nw));
		}

	unsigned int _Idx;
	_Ty _Ax[2 * _Nw];
	};

	// TEMPLATE CLASS _Swc_base (subtract_with_carry, subtract_with_carry_01)
template<class _Ty,
	size_t _Sx,
	size_t _Rx,
	class _Swc_Traits>
	class _Swc_base
		: public _Circ_buf<_Ty, _Rx>
	{	// base template class for subtract_with_carry/_01
public:
	typedef _Ty result_type;
	typedef _Swc_Traits _Traits;
	typedef _Swc_base<_Ty, _Sx, _Rx, _Swc_Traits> _Myt;
	typedef _Circ_buf<_Ty, _Rx> _Mybase;
	typedef typename _Swc_Traits::_Seed_t _Seed_t;

	static constexpr size_t short_lag = _Sx;
	static constexpr size_t long_lag = _Rx;
	static constexpr _Seed_t default_seed = static_cast<_Seed_t>(19780503U);

	_Swc_base()
		{	// construct with default seed
		seed();
		}

	_Swc_base(_Seed_t _X0)
		{	// construct with specified seed
		seed(_X0);
		}

	template<class _Gen,
		class = typename _Enable_if_seed_seq<_Gen, _Myt>::type>
		_Swc_base(_Gen& _Gx)
		{	// construct with seed values from generator
		seed(_Gx);
		}

	void seed(_Seed_t _Value = default_seed)
		{	// set initial values from specified seed value
		_Seed(_Value, false, true_type());
		}

	template<class _Gen>
		void seed(_Gen& _Gx, bool _Readcy = false)
		{	// set initial values from range
		_Seed(_Gx, _Readcy, is_arithmetic<_Gen>());
		}

	result_type (min)() const
		{	// return minimum possible generated value
		return (0);
		}

	result_type (max)() const
		{	// return maximum possible generated value
		return (_Swc_Traits::_Max);
		}

	result_type operator()()
		{	// return next value
		int _Ix = 2 * _Rx <= this->_Idx ? 0 : this->_Idx;
		if (_Ix < _Sx)
			_Setx(_Ix, this->_Ax[_Ix + 2 * _Rx - _Sx],
				this->_Ax[_Ix + _Rx]);
		else if (_Ix < _Rx)
			_Setx(_Ix, this->_Ax[_Ix - _Sx], this->_Ax[_Ix + _Rx]);
		else
			_Setx(_Ix, this->_Ax[_Ix - _Sx], this->_Ax[_Ix - _Rx]);
		this->_Idx = _Ix + 1;
		return (this->_Ax[_Ix]);
		}

	void discard(unsigned long long _Nskip)
		{	// discard _Nskip elements
		for (; 0 < _Nskip; --_Nskip)
			(*this)();
		}

	bool _Equals(const _Swc_base& _Right) const
		{	// return true if *this will generate same sequence as _Right
		return (_Mybase::_Equals(_Right)
			&& _Carry == _Right._Carry);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>&
			_Write(basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Swc_Traits::_Write(_Ostr, *this, _Carry);
		return (_Ostr);
		}

protected:
	template<class _Gen>
		void _Seed(_Gen& _Gx, bool _Readcy, true_type)
		{	// reset sequence from numeric value
		linear_congruential<_Seed_t, 40014U, 0U, 2147483563U> _Lc(
			_Gx == 0U ? default_seed : _Gx);
		_Reset(_Lc, _Readcy);
		}

	template<class _Gen>
		void _Seed(_Gen& _Gx, bool _Readcy, false_type)
		{	// reset sequence from generator
		_Reset(_Gx, _Readcy);
		}

	template<class _Gen>
		void _Reset(_Gen& _Gx, bool _Readcy)
		{	// reset sequence
		_Carry = _Swc_Traits::_Reset(_Gx, this->_Ax, _Readcy);
		this->_Idx = _Rx;
		}

	void _Setx(int _Ix, _Ty _Xis, _Ty _Xir)
		{	// update _Ax[_Ix] and _Carry
		bool _Underflowed = false;
		_Ty _Newx = _Xis;
		if (_Newx < _Xir)
			_Underflowed = true;
		_Newx -= _Xir;
		if (_Newx < static_cast<typename _Swc_Traits::_UCy_t>(_Carry))
			_Underflowed = true;
		_Newx -= _Carry;
		if (_Underflowed)
			{	// underflowed, so add _Mod
			_Newx += _Swc_Traits::_Mod;
			_Carry = _Swc_Traits::_Cy;
			}
		else
			_Carry = 0;
		this->_Ax[_Ix] = _Newx;
		}

	typename _Swc_Traits::_Cy_t _Carry;
	};

template<class _Ty,
	size_t _Sx,
	size_t _Rx,
	class _Swc_Traits>
	bool operator==(const _Swc_base<_Ty, _Sx, _Rx, _Swc_Traits>& _Left,
		const _Swc_base<_Ty, _Sx, _Rx, _Swc_Traits>& _Right)
		{	// return true if _Left will generate same sequence as _Right
		return (_Left._Equals(_Right));
		}

template<class _Ty,
	size_t _Sx,
	size_t _Rx,
	class _Swc_Traits>
	bool operator!=(const _Swc_base<_Ty, _Sx, _Rx, _Swc_Traits>& _Left,
		const _Swc_base<_Ty, _Sx, _Rx, _Swc_Traits>& _Right)
		{	// return true if _Left will not generate same sequence as _Right
		return (!_Left._Equals(_Right));
		}

template<class _Elem,
	class _Traits,
	class _Ty,
	size_t _Sx,
	size_t _Rx,
	class _Swc_Traits>
	basic_istream<_Elem, _Traits>&
		operator>>(basic_istream<_Elem, _Traits>& _Istr,
		_Swc_base<_Ty, _Sx, _Rx, _Swc_Traits>& _Eng)
		{	// read state from _Istr
		_Wrap_istream<_Elem, _Traits, typename _Swc_Traits::_Seed_t>
			_Gen(_Istr);
		_Eng.seed(_Gen, true);
		return (_Istr);
		}

template<class _Elem,
	class _Traits,
	class _Ty,
	size_t _Sx,
	size_t _Rx,
	class _Swc_Traits>
	basic_ostream<_Elem, _Traits>&
		operator<<(basic_ostream<_Elem, _Traits>& _Ostr,
		const _Swc_base<_Ty, _Sx, _Rx, _Swc_Traits>& _Eng)
		{	// write state to _Ostr
		return (_Eng._Write(_Ostr));
		}

template<class _Ty,
	size_t _Sx,
	size_t _Rx,
	class _Swc_Traits>
	const size_t _Swc_base<_Ty, _Sx, _Rx, _Swc_Traits>::short_lag;

template<class _Ty,
	size_t _Sx,
	size_t _Rx,
	class _Swc_Traits>
	const size_t _Swc_base<_Ty, _Sx, _Rx, _Swc_Traits>::long_lag;

	// TEMPLATE CLASS _Swc_traits
template<class _Ty,
	_Ty _Mx,
	size_t _Nw>
	struct _Swc_traits
	{	// traits template for subtract_with_carry generator
	typedef int _Cy_t;
	typedef unsigned int _UCy_t;
	typedef _Ty _Mod_t;
	typedef _Ty _Seed_t;

	static constexpr _Cy_t _Cy = 1;
	static constexpr _Mod_t _Mod = _Mx;
	static constexpr _Ty _Max = _Mx - 1;

	static int _Get_wc()
		{	// compute number of 32-bit words per element
		int _Kx;

		if (_Mx == 0)
			_Kx = (_BITS_BYTE * sizeof (_Ty) + 31) / 32;
		else
			{	// compute number of 32-bit words required
			_ULonglong _Val = (_ULonglong)1 << 32;
			for (_Kx = 1; 0 < _Val && _Val < _Mx; ++_Kx)
				_Val = _Val << 32;
			}
		return (_Kx);
		}

	template<class _Gen>
		static _Cy_t _Reset(_Gen& _Gx, _Ty *_Ax, bool _Readcy)
		{	// set initial values of _Ax from generator _Gx
			// return value of _Cy from range if _Readcy is true,
			// otherwise compute from last value
		int _Kx = _Get_wc();

		for (int _Ix = 0; _Ix < _Nw; ++_Ix)
			{	// pack _Kx words
			_Ax[_Ix] = _Gx();
			for (int _Jx = 0; ++_Jx < _Kx; )
				_Ax[_Ix] |= (_Ty)_Gx() << (32 * _Jx);
			}

		_Cy_t _Ans = _Reduce(_Ax);
		if (!_Readcy)
			return (_Ans);
		else
			return (_Gx());
		}

  #pragma warning(push)
  #pragma warning(disable: 4724) // potential mod by 0
	static _Cy_t _Reduce(_Ty *_Ax)
		{	// reduce values to allowed range
		if (_Mx != 0)
			for (int _Ix = 0; _Ix < _Nw; ++_Ix)
				_Ax[_Ix] = _Ax[_Ix] % _Mx;
		return (_Ax[_Nw - 1] == 0);
		}
  #pragma warning(pop)

	template<class _Elem,
		class _Traits>
		static void _Write(basic_ostream<_Elem, _Traits>& _Ostr,
			const _Circ_buf<_Ty, _Nw>& _Buf, _Cy_t _Cy)
		{	// write state to _Ostr
		int _Kx = _Get_wc();

		for (int _Ix = 0; _Ix < _Nw; ++_Ix)
			for (int _Jx = 1; _Jx <= _Kx; ++_Jx)
				{	// unpack into _Kx words
				unsigned int _Word =
					(unsigned int)(_Buf._At(_Ix) >> ((_Kx - _Jx) * 32));
				_Ostr << _Word << ' ';
				}
		_Ostr << _Cy;
		}
	};

	// TEMPLATE CLASS subtract_with_carry
template<class _Ty,
	_Ty _Mx,
	size_t _Sx,
	size_t _Rx>
	class subtract_with_carry
		: public _Swc_base<_Ty, _Sx, _Rx, _Swc_traits<_Ty, _Mx, _Rx> >
	{	// template class for subtract_with_carry generator
public:
	typedef subtract_with_carry<_Ty, _Mx, _Sx, _Rx> _Myt;
	typedef _Swc_base<_Ty, _Sx, _Rx, _Swc_traits<_Ty, _Mx, _Rx> > _Mybase;

	static constexpr _Ty modulus = _Mx;

	using _Mybase::default_seed;

	explicit subtract_with_carry(_Ty _X0 = default_seed)
		: _Mybase(_X0)
		{	// construct with default seed
		}

	template<class _Gen,
		class = typename _Enable_if_seed_seq<_Gen, _Myt>::type>
		subtract_with_carry(_Gen& _Gx)
		: _Mybase(_Gx)
		{	// construct with seed values from generator
		}
	};

		// TEMPLATE CLASS subtract_with_carry_engine
template<class _Ty,
	size_t _Wx,
	size_t _Sx,
	size_t _Rx>
	class subtract_with_carry_engine
		: public subtract_with_carry<_Ty,
			(_Ty(1) << (_Wx - 1)) << 1, _Sx, _Rx>
	{	// template class for subtract_with_carry generator
public:
	static_assert(_Is_UIntType<_Ty>::value && 0U < _Sx && _Sx < _Rx
		&& 0 < _Wx && _Wx <= numeric_limits<_Ty>::digits,
		"invalid template argument for subtract_with_carry_engine");

	typedef subtract_with_carry_engine<_Ty, _Wx, _Sx, _Rx> _Myt;

	static constexpr _Ty _Mx = (_Ty(1) << (_Wx - 1)) << 1;
	static constexpr size_t word_size = _Wx;
	static constexpr size_t short_lag = _Sx;
	static constexpr size_t long_lag = _Rx;

	typedef subtract_with_carry<_Ty, _Mx, _Sx, _Rx> _Mybase;
	typedef typename _Mybase::_Traits _Traits;
	typedef _Ty result_type;

	using _Mybase::default_seed;

	explicit subtract_with_carry_engine(_Ty _X0 = default_seed)
		: subtract_with_carry<_Ty, _Mx, _Sx, _Rx>(_X0)	// TRANSITION, VSO#184864
		{	// construct with default seed
		}

	template<class _Seed_seq,
		class = typename _Enable_if_seed_seq<_Seed_seq, _Myt>::type>
		explicit subtract_with_carry_engine(_Seed_seq& _Seq)
		: subtract_with_carry<_Ty, _Mx, _Sx, _Rx>()	// TRANSITION, VSO#184864
		{	// construct from seed sequence
		seed(_Seq);
		}

	void seed(_Ty _Value = default_seed)
		{	// set initial values from specified seed value
		this->_Seed(_Value, false, true_type());
		}

	static constexpr int _Kx = (_BITS_BYTE * sizeof (_Ty) + 31) / 32;

	template<class _Seed_seq,
		class = typename _Enable_if_seed_seq<_Seed_seq, _Myt>::type>
		void seed(_Seed_seq& _Seq)
		{	// reset sequence from seed sequence
		unsigned long _Arr[_Kx * _Rx];
		_Seq.generate(&_Arr[0], &_Arr[_Kx * _Rx]);

		int _Idx0 = 0;
		for (int _Ix = 0; _Ix < _Rx; ++_Ix, _Idx0 += _Kx)
			{	// pack _Kx words
			this->_Ax[_Ix] = _Arr[_Idx0];
			for (int _Jx = 0; ++_Jx < _Kx; )
				this->_Ax[_Ix] |= (_Ty)_Arr[_Idx0 + _Jx] << (32 * _Jx);

			if (_Traits::_Mod != 0)
				this->_Ax[_Ix] %= _Traits::_Mod;
			}
		this->_Carry = _Traits::_Reduce(this->_Ax);
		this->_Idx = _Rx;
		}

	static constexpr _Ty (min)()
		{	// return minimum possible generated value
		return (0);
		}

	static constexpr _Ty (max)()
		{	// return maximum possible generated value
		return (_Mx - 1);
		}
	};

#if _HAS_TR1_NAMESPACE
	// TEMPLATE CLASS _Swc_01_traits
template<class _Ty,
	size_t _Wx,
	size_t _Rx>
	struct _Swc_01_traits
	{	// traits template for subtract_with_carry_01 generator
	typedef _Ty _Cy_t;
	typedef _Ty _UCy_t;
	typedef _Ty _Mod_t;
	typedef unsigned int _Seed_t;

	static const _Cy_t _Cy;
	static const _Mod_t _Mod;
	static const _Ty _Max;
	static constexpr int _Nwords = (_Wx + 31) / 32;

	template<class _Gen>
		static _Cy_t _Reset(_Gen& _Gx, _Ty *_Ax, bool _Readcy)
		{	// set initial values of _Ax from generator _Gx
			// return value of _Cy from range if _Readcy is true,
			// otherwise from last value
		for (int _Ix = 0; _Ix < _Rx; ++_Ix)
			{	// read values
			_Ty _Factor = 1;
			_Ty _Val = 0;
			for (int _Jx = 0; _Jx < _Nwords - 1; ++_Jx)
				{	// read components of value
				_Factor /= (_Ty)_Two32;
				_Val += _Gx() * _Factor;
				}
			_Ty _Temp = ((unsigned long)_Gx() & _Mask) / _Scale1;
			_Val += (_Temp - (unsigned long)_Temp) * _Factor;
			_Ax[_Ix] = _Val;
			}
		if (!_Readcy)
			return (_Ax[_Rx - 1] != 0 ? 0 : _Cy);
		else
			return (_Gx() == 0 ? 0 : _Cy);
		}

	template<class _Elem,
		class _Traits>
		static void _Write(basic_ostream<_Elem, _Traits>& _Ostr,
			const _Circ_buf<_Ty, _Rx>& _Buf, _Cy_t _Cy)
		{	// write state to _Ostr
		for (int _Ix = 0; _Ix < _Rx; ++_Ix)
			{	// write values
			_Ty _Val = _Buf._At(_Ix);
			unsigned long _Temp;
			for (int _Jx = 0; _Jx < _Nwords - 1; ++_Jx)
				{	// write components of value
				_Val *= (_Ty)_Two32;
				_Temp = (unsigned long)_Val;
				_Val -= _Temp;
				_Ostr << _Temp << ' ';
				}
			_Temp = (unsigned long)(_Val * _Scale1);
			_Ostr << _Temp << ' ';
			}
		_Ostr << (_Cy ? 1 : 0);
		}

private:
	static const _Ty _Scale1;
	static constexpr unsigned long _Mask = ~((~0UL) << (_Wx % 32));
	};

template<class _Ty,
	size_t _Wx,
	size_t _Rx>
	const typename _Swc_01_traits<_Ty, _Wx, _Rx>::_Cy_t
		_Swc_01_traits<_Ty, _Wx, _Rx>::_Cy =
			(typename _Swc_01_traits<_Ty, _Wx, _Rx>::_Cy_t)
				_CSTD ldexp(1.0, int(-ptrdiff_t(_Wx)));

template<class _Ty,
	size_t _Wx,
	size_t _Rx>
	const typename _Swc_01_traits<_Ty, _Wx, _Rx>::_Mod_t
		_Swc_01_traits<_Ty, _Wx, _Rx>::_Mod = 1;

template<class _Ty,
	size_t _Wx,
	size_t _Rx>
	const _Ty _Swc_01_traits<_Ty, _Wx, _Rx>::_Max = 1;

template<class _Ty,
	size_t _Wx,
	size_t _Rx>
	const _Ty _Swc_01_traits<_Ty, _Wx, _Rx>::_Scale1 =
		(_Ty)_CSTD ldexp(1.0, _Wx % 32);

	// TEMPLATE CLASS subtract_with_carry_01
template<class _Ty,
	size_t _Wx,
	size_t _Sx,
	size_t _Rx>
	class subtract_with_carry_01
		: public _Swc_base<_Ty, _Sx, _Rx, _Swc_01_traits<_Ty, _Wx, _Rx> >
	{	// template class for subtract_with_carry_01 generator
public:
	static constexpr size_t word_size = _Wx;
	typedef subtract_with_carry_01<_Ty, _Wx, _Sx, _Rx> _Myt;
	typedef _Swc_base<_Ty, _Sx, _Rx, _Swc_01_traits<_Ty, _Wx, _Rx> > _Mybase;

	subtract_with_carry_01()
		: _Mybase()
		{	// construct with default seed
		}

	explicit subtract_with_carry_01(typename _Mybase::_Seed_t _Value)
		: _Mybase(_Value)
		{	// construct with specified seed
		}

	template<class _Gen,
		class = typename _Enable_if_seed_seq<_Gen, _Myt>::type>
		subtract_with_carry_01(_Gen& _Gx)
			: _Mybase(_Gx)
		{	// construct with seed values from generator
		}
	};

template<class _Ty,
	size_t _Wx,
	size_t _Sx,
	size_t _Rx>
	const size_t subtract_with_carry_01<_Ty, _Wx, _Sx, _Rx>
		::word_size;
#endif /* _HAS_TR1_NAMESPACE */

	// TEMPLATE CLASS mersenne_twister
template<class _Ty,
	int _Wx,
	int _Nx,
	int _Mx,
	int _Rx,
	_Ty _Px,
	int _Ux,
	int _Sx,
	_Ty _Bx,
	int _Tx,
	_Ty _Cx,
	int _Lx>
	class mersenne_twister
		: public _Circ_buf<_Ty, _Nx>
	{	// template class for mersenne twister generator
public:
	typedef mersenne_twister<_Ty, _Wx, _Nx, _Mx, _Rx,
		_Px, _Ux, _Sx, _Bx, _Tx, _Cx, _Lx> _Myt;
	typedef _Ty result_type;

	static constexpr int word_size = _Wx;
	static constexpr int state_size = _Nx;
	static constexpr int shift_size = _Mx;
	static constexpr int mask_bits = _Rx;
	static constexpr _Ty parameter_a = _Px;
	static constexpr int output_u = _Ux;
	static constexpr int output_s = _Sx;
	static constexpr _Ty output_b = _Bx;
	static constexpr int output_t = _Tx;
	static constexpr _Ty output_c = _Cx;
	static constexpr int output_l = _Lx;

	static constexpr _Ty default_seed = 5489U;

	explicit mersenne_twister(_Ty _X0 = default_seed,
		_Ty _Dxarg = _WMSK,
		_Ty _Fxarg = (_Ty)1812433253)
		: _Dxval(_Dxarg)
		{	// construct with specified seed
		seed(_X0, _Fxarg);
		}

	template<class _Gen,
		class = typename _Enable_if_seed_seq<_Gen, _Myt>::type>
		explicit mersenne_twister(_Gen& _Gx)
		: _Dxval(_WMSK)
		{	// construct with seed values from generator
		seed(_Gx);
		}

	void seed(_Ty _X0 = default_seed,
		_Ty _Fx = (_Ty)1812433253)
		{	// set initial values from specified value
		_Ty _Prev = this->_Ax[0] = _X0 & _WMSK;
		for (int _Ix = 1; _Ix < _Nx; ++_Ix)
			_Prev = this->_Ax[_Ix] =
				(_Ix + _Fx * (_Prev ^ (_Prev >> (_Wx - 2)))) & _WMSK;
		this->_Idx = _Nx;
		}

	template<class _Gen,
		class = typename _Enable_if_seed_seq<_Gen, _Myt>::type>
		void seed(_Gen& _Gx, bool = false)
		{	// set initial values from range
		for (int _Ix = 0; _Ix < _Nx; ++_Ix)
			this->_Ax[_Ix] = _Gx() & _WMSK;
		this->_Idx = _Nx;
		}

	template<class _Elem,
		class _S_Traits>
		basic_ostream<_Elem, _S_Traits>&
			_Write(basic_ostream<_Elem, _S_Traits>& _Ostr) const
		{	// write state to _Ostr
		for (int _Ix = 0; _Ix < _Nx; ++_Ix)
			_Ostr << this->_At(_Ix) << ' ';
		return (_Ostr);
		}

	result_type (min)() const
		{	// return minimum possible generated value
		return (0);
		}

	result_type (max)() const
		{	// return maximum possible generated value
		return (_WMSK);
		}

	result_type operator()()
		{	// return next value
		if (this->_Idx == _Nx)
			_Refill_upper();
		else if (2 * _Nx <= this->_Idx)
			_Refill_lower();

		_Ty _Res = this->_Ax[this->_Idx++] & _WMSK;
		_Res ^= (_Res >> _Ux) & _Dxval;
		_Res ^= (_Res << _Sx) & _Bx;
		_Res ^= (_Res << _Tx) & _Cx;
		_Res ^= (_Res & _WMSK) >> _Lx;
		return (_Res);
		}

	void discard(unsigned long long _Nskip)
		{	// discard _Nskip elements
		for (; 0 < _Nskip; --_Nskip)
			(*this)();
		}

protected:

	_Post_satisfies_(this->_Idx == 0)

	void _Refill_lower()
		{	// compute values for the lower half of the history array
		int _Ix;
		for (_Ix = 0; _Ix < _Nx - _Mx; ++_Ix)
			{	// fill in lower region
			_Ty _Tmp = (this->_Ax[_Ix + _Nx] & _HMSK)
				| (this->_Ax[_Ix + _Nx + 1] & _LMSK);
			this->_Ax[_Ix] = (_Tmp >> 1)
				^ (_Tmp & 1 ? _Px : 0) ^ this->_Ax[_Ix + _Nx + _Mx];
			}

		for (; _Ix < _Nx - 1; ++_Ix)
			{	// fill in upper region (avoids modulus operation)
			_Ty _Tmp = (this->_Ax[_Ix +_Nx] & _HMSK)
				| (this->_Ax[_Ix + _Nx + 1] & _LMSK);
			this->_Ax[_Ix] = (_Tmp >> 1)
				^ (_Tmp & 1 ? _Px : 0) ^ this->_Ax[_Ix - _Nx + _Mx];
			}

		_Ty _Tmp = (this->_Ax[_Ix + _Nx] & _HMSK) | (this->_Ax[0] & _LMSK);
		this->_Ax[_Ix] = (_Tmp >> 1)
			^ (_Tmp & 1 ? _Px : 0) ^ this->_Ax[_Mx - 1];
		this->_Idx = 0;
		}

	void _Refill_upper()
		{	// compute values for the upper half of the history array
		int _Ix;
		for (_Ix = _Nx; _Ix < 2 * _Nx; ++_Ix)
			{	// fill in values
			_Ty _Tmp = (this->_Ax[_Ix - _Nx] & _HMSK)
				| (this->_Ax[_Ix - _Nx + 1] & _LMSK);
			this->_Ax[_Ix] = (_Tmp >> 1)
				^ (_Tmp & 1 ? _Px : 0) ^ this->_Ax[_Ix - _Nx + _Mx];
			}
		}

	_Ty _Dxval;

	static constexpr _Ty _WMSK = ~((~_Ty(0) << (_Wx - 1)) << 1);
	static constexpr _Ty _HMSK = (_WMSK << _Rx) & _WMSK;
	static constexpr _Ty _LMSK = ~_HMSK & _WMSK;
	};

template<class _Ty, int _Wx, int _Nx, int _Mx, int _Rx,
	_Ty _Px, int _Ux, int _Sx, _Ty _Bx, int _Tx, _Ty _Cx, int _Lx>
	bool operator==(
		const mersenne_twister<_Ty, _Wx, _Nx, _Mx, _Rx,
			_Px, _Ux, _Sx, _Bx, _Tx, _Cx, _Lx>& _Left,
		const mersenne_twister<_Ty, _Wx, _Nx, _Mx, _Rx,
			_Px, _Ux, _Sx, _Bx, _Tx, _Cx, _Lx>& _Right)
	{	// return true if _Left will generate same sequence as _Right
	return (_Left._Equals(_Right));
	}

template<class _Ty, int _Wx, int _Nx, int _Mx, int _Rx,
	_Ty _Px, int _Ux, int _Sx, _Ty _Bx, int _Tx, _Ty _Cx, int _Lx>
	bool operator!=(
		const mersenne_twister<_Ty, _Wx, _Nx, _Mx, _Rx,
			_Px, _Ux, _Sx, _Bx, _Tx, _Cx, _Lx>& _Left,
		const mersenne_twister<_Ty, _Wx, _Nx, _Mx, _Rx,
			_Px, _Ux, _Sx, _Bx, _Tx, _Cx, _Lx>& _Right)
	{	// return true if _Left will not generate same sequence as _Right
	return (!_Left._Equals(_Right));
	}

template<class _Elem,
	class _S_Traits,
	class _Ty, int _Wx, int _Nx, int _Mx, int _Rx,
	_Ty _Px, int _Ux, int _Sx, _Ty _Bx, int _Tx, _Ty _Cx, int _Lx>
	basic_istream<_Elem, _S_Traits>& operator>>(
		basic_istream<_Elem, _S_Traits>& _Istr,
		mersenne_twister<_Ty, _Wx, _Nx, _Mx, _Rx,
			_Px, _Ux, _Sx, _Bx, _Tx, _Cx, _Lx>& _Eng)
	{	// read state from _Istr
	_Wrap_istream<_Elem, _S_Traits, _Ty> _Gen(_Istr);
	_Eng.seed(_Gen);
	return (_Istr);
	}

template<class _Elem,
	class _S_Traits,
	class _Ty, int _Wx, int _Nx, int _Mx, int _Rx,
	_Ty _Px, int _Ux, int _Sx, _Ty _Bx, int _Tx, _Ty _Cx, int _Lx>
	basic_ostream<_Elem, _S_Traits>& operator<<(
		basic_ostream<_Elem, _S_Traits>& _Ostr,
		const mersenne_twister<_Ty, _Wx, _Nx, _Mx, _Rx,
			_Px, _Ux, _Sx, _Bx, _Tx, _Cx, _Lx>& _Eng)
	{	// write state to _Ostr
	return (_Eng._Write(_Ostr));
	}

	// TEMPLATE CLASS mersenne_twister_engine
template<class _Ty,
	size_t _Wx,
	size_t _Nx,
	size_t _Mx,
	size_t _Rx,
	_Ty _Px,
	size_t _Ux,
	_Ty _Dx,	// added
	size_t _Sx,
	_Ty _Bx,
	size_t _Tx,
	_Ty _Cx,
	size_t _Lx,
	_Ty _Fx>	// added
	class mersenne_twister_engine
		: public mersenne_twister<_Ty, _Wx, _Nx, _Mx, _Rx,
			_Px, _Ux, _Sx, _Bx, _Tx, _Cx, _Lx>
	{	// template class for mersenne twister generator
public:
	static constexpr unsigned long long
		_Max = (((1ULL << (_Wx - 1)) - 1) << 1) + 1;

	static_assert(_Is_UIntType<_Ty>::value && 0 < _Mx && _Mx <= _Nx
		&& 2U < _Wx && _Rx <= _Wx && _Ux <= _Wx && _Sx <= _Wx && _Tx <= _Wx
		&& _Lx <= _Wx && _Wx <= numeric_limits<_Ty>::digits && _Px <= _Max
		&& _Bx <= _Max && _Cx <= _Max && _Dx <= _Max && _Fx <= _Max,
		"invalid template argument for mersenne_twister_engine");

	typedef mersenne_twister_engine<_Ty, _Wx, _Nx, _Mx, _Rx,
		_Px, _Ux, _Dx, _Sx, _Bx, _Tx, _Cx, _Lx, _Fx> _Myt;
	typedef mersenne_twister<_Ty, _Wx, _Nx, _Mx, _Rx,
		_Px, _Ux, _Sx, _Bx, _Tx, _Cx, _Lx> _Mybase;
	typedef _Ty result_type;

	static constexpr size_t word_size = _Wx;
	static constexpr size_t state_size = _Nx;
	static constexpr size_t shift_size = _Mx;
	static constexpr size_t mask_bits = _Rx;
	static constexpr _Ty xor_mask = _Px;
	static constexpr size_t tempering_u = _Ux;
	static constexpr _Ty tempering_d = _Dx;
	static constexpr size_t tempering_s = _Sx;
	static constexpr _Ty tempering_b = _Bx;
	static constexpr size_t tempering_t = _Tx;
	static constexpr _Ty tempering_c = _Cx;
	static constexpr size_t tempering_l = _Lx;
	static constexpr _Ty initialization_multiplier = _Fx;

	static constexpr result_type default_seed = 5489U;

	explicit mersenne_twister_engine(result_type _X0 = default_seed)
		: mersenne_twister<_Ty, _Wx, _Nx, _Mx, _Rx, _Px, _Ux, _Sx, _Bx,
			_Tx, _Cx, _Lx>(_X0, _Dx, _Fx)	// TRANSITION, VSO#184864
		{	// construct with default seed
		}

	template<class _Seed_seq,
		class = typename _Enable_if_seed_seq<_Seed_seq, _Myt>::type>
		explicit mersenne_twister_engine(_Seed_seq& _Seq)
		: mersenne_twister<_Ty, _Wx, _Nx, _Mx, _Rx, _Px, _Ux, _Sx, _Bx,
			_Tx, _Cx, _Lx>(default_seed, _Dx, _Fx)	// TRANSITION, VSO#184864
		{	// construct from seed sequence
		seed(_Seq);
		}

	void seed(result_type _X0 = default_seed)
		{	// set initial values from specified value
		_Mybase::seed(_X0, _Fx);
		}

	template<class _Seed_seq,
		class = typename _Enable_if_seed_seq<_Seed_seq, _Myt>::type>
		void seed(_Seed_seq& _Seq)
		{	// reset sequence from seed sequence
		static constexpr int _Kx = (_Wx + 31) / 32;
		unsigned long _Arr[_Kx * _Nx];
		_Seq.generate(&_Arr[0], &_Arr[_Kx * _Nx]);

		int _Idx0 = 0;
		_Ty _Sum = 0;
		for (int _Ix = 0; _Ix < _Nx; ++_Ix, _Idx0 += _Kx)
			{	// pack _Kx words
			this->_Ax[_Ix] = _Arr[_Idx0];
			for (int _Jx = 0; ++_Jx < _Kx; )
				this->_Ax[_Ix] |= (_Ty)_Arr[_Idx0 + _Jx] << (32 * _Jx);
			this->_Ax[_Ix] &= this->_WMSK;

			if (_Ix == 0)
				_Sum = this->_Ax[_Ix] >> _Rx;
			else
				_Sum |= this->_Ax[_Ix];
			}
		if (_Sum == 0)
			this->_Ax[0] = this->_WMSK;

		this->_Idx = _Nx;
		}

	static constexpr result_type (min)()
		{	// return minimum possible generated value
		return (0);
		}

	static constexpr result_type (max)()
		{	// return maximum possible generated value
		return (_Mybase::_WMSK);
		}
	};

	// TEMPLATE CLASS discard_block
template<class _Engine,
	int _Px,
	int _Rx>
	class discard_block
	{	// template class for discard_block compound engine
public:
	typedef discard_block<_Engine, _Px, _Rx> _Myt;
	typedef _Engine base_type;
	typedef typename _Engine::result_type result_type;

	static constexpr int block_size = _Px;
	static constexpr int used_block = _Rx;

	discard_block()
		: _Eng(), _Nx(0)
		{	// construct
		}

	explicit discard_block(const _Engine& _Ex)
		: _Eng(_Ex), _Nx(0)
		{	// construct with engine initializer _Ex
		}

	explicit discard_block(result_type _Seed)
		: _Eng(_Seed), _Nx(0)
		{	// construct from specified seed value
		}


	template<class _Seed_seq,
		class = typename _Enable_if_seed_seq<_Seed_seq, _Myt, _Engine>::type>
		explicit discard_block(_Seed_seq& _Seq)
		: _Eng(_Seq), _Nx(0)
		{	// construct from seed sequence
		}


	void seed()
		{	// seed engine from default value
		_Eng.seed();
		_Nx = 0;
		}

	void seed(result_type _X0)
		{	// seed engine from specified value
		_Eng.seed(_X0);
		_Nx = 0;
		}


	template<class _Seed_seq,
		class = typename _Enable_if_seed_seq<_Seed_seq, _Myt>::type>
		void seed(_Seed_seq& _Seq)
		{	// seed engine from seed sequence
		_Eng.seed(_Seq);
		_Nx = 0;
		}


	const base_type& base() const _NOEXCEPT
		{	// return const reference to engine
		return (_Eng);
		}

	result_type (min)() const
		{	// return minimum possible generated value
		return ((_Eng.min)());
		}

	result_type (max)() const
		{	// return maximum possible generated value
		return ((_Eng.max)());
		}

	result_type operator()()
		{	// return next value
		if (_Rx <= _Nx)
			{	// discard values
			while (_Nx++ < _Px)
				_Eng();
			_Nx = 0;
			}
		++_Nx;
		return (_Eng());
		}

	void discard(unsigned long long _Nskip)
		{	// discard _Nskip elements
		for (; 0 < _Nskip; --_Nskip)
			(*this)();
		}

	bool _Equals(const _Myt& _Right) const
		{	// return true if *this will generate same sequence as _Right
		return (_Eng == _Right._Eng && _Nx == _Right._Nx);
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		return (_Istr >> _Eng >> _Nx);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		return (_Ostr << _Eng << ' ' << _Nx);
		}

private:
	base_type _Eng;
	int _Nx;
	};

template<class _Engine,
	int _Px,
	int _Rx>
	const int discard_block<_Engine, _Px, _Rx>::block_size;

template<class _Engine,
	int _Px,
	int _Rx>
	const int discard_block<_Engine, _Px, _Rx>::used_block;

template<class _Engine,
	int _Px,
	int _Rx>
	bool operator==(
		const discard_block<_Engine, _Px, _Rx>& _Left,
		const discard_block<_Engine, _Px, _Rx>& _Right)
	{	// return true if _Left will generate same sequence as _Right
	return (_Left._Equals(_Right));
	}

template<class _Engine,
	int _Px,
	int _Rx>
	bool operator!=(
		const discard_block<_Engine, _Px, _Rx>& _Left,
		const discard_block<_Engine, _Px, _Rx>& _Right)
	{	// return true if _Left will not generate same sequence as _Right
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Engine,
	int _Px,
	int _Rx>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		discard_block<_Engine, _Px, _Rx>& _Eng)
	{	// read state from _Istr
	return (_Eng._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Engine,
	int _Px,
	int _Rx>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const discard_block<_Engine, _Px, _Rx>& _Eng)
	{	// write state to _Ostr
	return (_Eng._Write(_Ostr));
	}

	// TEMPLATE CLASS discard_block_engine
template<class _Engine,
	size_t _Px,
	size_t _Rx>
	class discard_block_engine
		: public discard_block<_Engine, _Px, _Rx>
	{	// template class for discard_block_engine compound engine
public:
	static_assert(0 < _Rx && _Rx <= _Px,
		"invalid template argument for discard_block_engine");

	typedef discard_block_engine<_Engine, _Px, _Rx> _Myt;
	typedef discard_block<_Engine, _Px, _Rx> _Mybase;
	typedef typename _Engine::result_type result_type;

	discard_block_engine()
		: _Mybase()
		{	// default construct
		}

	explicit discard_block_engine(const _Engine& _Ex)
		: _Mybase(_Ex)
		{	// construct with engine initializer _Ex
		}

	explicit discard_block_engine(_Engine&& _Ex)
		: _Mybase(_STD forward<_Engine>(_Ex))
		{	// construct with engine initializer _Ex
		}

	explicit discard_block_engine(result_type _X0)
		: _Mybase(_X0)
		{	// construct from specified seed value
		}

	template<class _Seed_seq,
		class = typename _Enable_if_seed_seq<_Seed_seq, _Myt, _Engine>::type>
		explicit discard_block_engine(_Seed_seq& _Seq)
		: _Mybase(_Seq)
		{	// construct from seed sequence
		}

	static constexpr typename _Engine::result_type (min)()
		{	// return minimum possible generated value
		return ((_Engine::min)());
		}

	static constexpr typename _Engine::result_type (max)()
		{	// return maximum possible generated value
		return ((_Engine::max)());
		}
	};

	// TEMPLATE CLASS independent_bits_engine
template<class _Engine,
	size_t _Wx,
	class _UIntType>
	class independent_bits_engine
	{	// template class for independent_bits_engine compound engine
public:
	static_assert(_Is_UIntType<_UIntType>::value
		&& 0 < _Wx && _Wx <= numeric_limits<_UIntType>::digits,
		"invalid template argument for independent_bits_engine");

	typedef independent_bits_engine<_Engine, _Wx, _UIntType> _Myt;
	typedef _Engine base_type;
	typedef _UIntType result_type;
	typedef typename _Engine::result_type _Eres;

	independent_bits_engine()
		{	// construct
		_Init();
		}

	explicit independent_bits_engine(const _Engine& _Ex)
		: _Eng(_Ex)
		{	// construct with engine initializer _Ex
		_Init();
		}

	explicit independent_bits_engine(_Engine&& _Ex)
		: _Eng(_STD forward<_Engine>(_Ex))
		{	// construct with engine initializer _Ex
		_Init();
		}

	explicit independent_bits_engine(result_type _X0)
		: _Eng(_X0)
		{	// construct from specified seed value
		_Init();
		}

	template<class _Seed_seq,
		class = typename _Enable_if_seed_seq<_Seed_seq, _Myt, _Engine>::type>
		explicit independent_bits_engine(_Seed_seq& _Seq)
		: _Eng(_Seq)
		{	// construct from seed sequence
		_Init();
		}

	void seed()
		{	// seed engine from default value
		_Eng.seed();
		}

	void seed(result_type _X0)
		{	// seed engine from specified value
		_Eng.seed(_X0);
		}

	template<class _Seed_seq,
		class = typename _Enable_if_seed_seq<_Seed_seq, _Myt>::type>
		void seed(_Seed_seq& _Seq)
		{	// seed engine from seed sequence
		_Eng.seed(_Seq);
		}

	const _Engine& base() const _NOEXCEPT
		{	// return const reference to engine
		return (_Eng);
		}

	static constexpr result_type (min)()
		{	// return minimum possible generated value
		return (0);
		}

	static constexpr result_type (max)()
		{	// return maximum possible generated value
		return ((((result_type)1 << (_Wx - 1)) << 1) - 1);
		}

	result_type operator()()
		{	// return next value
		size_t _Idx = 0;
		result_type _Res = 0;
		result_type _Mask = (((result_type)1 << (_W0 - 1)) << 1) - 1;
		_Eres _Val;

		for (; _Idx < _N0; ++_Idx)
			{	// pack _W0-bit values
			for (; ; )
				{	// get a small enough value
				_Val = _Eng() - (_Engine::min)();
				if (_Val <= _Y0)
					break;
				}
			_Res = _Res << _W0 | static_cast<result_type>(_Val) & _Mask;
			}

		_Mask = _Mask << 1 | 1;
		for (; _Idx < _Nx; ++_Idx)
			{	// pack _W0+1-bit values
			for (; ; )
				{	// get a small enough value
				_Val = _Eng() - (_Engine::min)();
				if (_Val <= _Y1)
					break;
				}
			_Res = (_Res << (_W0 + 1)) | static_cast<result_type>(_Val) & _Mask;
			}
		return (_Res);
		}

	void discard(unsigned long long _Nskip)
		{	// discard _Nskip elements
		for (; 0 < _Nskip; --_Nskip)
			(*this)();
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		return (_Istr >> _Eng);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		return (_Ostr << _Eng);
		}

private:
	void _Init()
		{	// compute values for operator()
		size_t _Mx = 0;
		_Eres _Rx = (_Engine::max)() - (_Engine::min)() + 1;

		_Eres _Tmp = _Rx;
		if (_Tmp == 0)
			{	// all bits used, make _Rx finite
			_Mx = 1;
			--_Tmp;
			}
		for (; 1 < _Tmp; _Tmp >>= 1)
			++_Mx;	// compute _Mx = floor(log2(_Rx))

		for (size_t _Nfix = 0; ; ++_Nfix)
			{	// compute consistent set of parameters
			_Nx = (_Wx + _Mx - 1) / _Mx + _Nfix;	// trial _Nx
			_W0 = _Wx / _Nx;
			_N0 = _Nx - _Wx % _Nx;
			_Y0 = (_Rx >> _W0) << _W0;
			_Y1 = (((_Rx >> _W0) >> 1) << _W0) << 1;
			if (_Nfix == 1 || _Rx - _Y0 <= _Y0 / _Nx)
				break;	// also works if _Rx == 0 (_Mx == all bits)
			}
		--_Y0;
		--_Y1;
		}

	_Engine _Eng;	// the stored engine
	size_t _N0;		// number of smaller packing words
	size_t _Nx;		// total number of packing words
	size_t _W0;		// bits per smaller packing word
	_Eres _Y0;		// max value for smaller packing word
	_Eres _Y1;		// max value for larger packing word
	};

template<class _Engine,
	size_t _Wx,
	class _UIntType>
	bool operator==(
		const independent_bits_engine<_Engine, _Wx, _UIntType>& _Left,
		const independent_bits_engine<_Engine, _Wx, _UIntType>& _Right)
	{	// return true if _Left will generate same sequence as _Right
	return (_Left.base() == _Right.base());
	}

template<class _Engine,
	size_t _Wx,
	class _UIntType>
	bool operator!=(
		const independent_bits_engine<_Engine, _Wx, _UIntType>& _Left,
		const independent_bits_engine<_Engine, _Wx, _UIntType>& _Right)
	{	// return true if _Left will not generate same sequence as _Right
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Engine,
	size_t _Wx,
	class _UIntType>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		independent_bits_engine<_Engine, _Wx, _UIntType>& _Eng)
	{	// read state from _Istr
	return (_Eng._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Engine,
	size_t _Wx,
	class _UIntType>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const independent_bits_engine<_Engine, _Wx, _UIntType>& _Eng)
	{	// write state to _Ostr
	return (_Eng._Write(_Ostr));
	}

	// TEMPLATE CLASS shuffle_order_engine
template<class _Engine,
	size_t _Kx>
	class shuffle_order_engine
	{	// template class for shuffle_order_engine compound engine
public:
	static_assert(0 < _Kx,
		"invalid template argument for shuffle_order_engine");

	typedef shuffle_order_engine<_Engine, _Kx> _Myt;
	typedef _Engine base_type;
	typedef typename _Engine::result_type result_type;

	static constexpr size_t table_size = _Kx;

	shuffle_order_engine()
		{	// construct
		_Init();
		}

	explicit shuffle_order_engine(const _Engine& _Ex)
		: _Eng(_Ex)
		{	// construct with engine initializer _Ex
		_Init();
		}

	explicit shuffle_order_engine(_Engine&& _Ex)
		: _Eng(_STD forward<_Engine>(_Ex))
		{	// construct with engine initializer _Ex
		_Init();
		}

	explicit shuffle_order_engine(result_type _X0)
		: _Eng(_X0)
		{	// construct from specified seed value
		_Init();
		}

	template<class _Seed_seq,
		class = typename _Enable_if_seed_seq<_Seed_seq, _Myt, _Engine>::type>
		explicit shuffle_order_engine(_Seed_seq& _Seq)
		: _Eng(_Seq)
		{	// construct from seed sequence
		_Init();
		}

	void seed()
		{	// seed engine from default value
		_Eng.seed();
		_Init();
		}

	void seed(result_type _X0)
		{	// seed engine from specified value
		_Eng.seed(_X0);
		_Init();
		}

	template<class _Seed_seq,
		class = typename _Enable_if_seed_seq<_Seed_seq, _Myt>::type>
		void seed(_Seed_seq& _Seq)
		{	// seed engine from seed sequence
		_Eng.seed(_Seq);
		_Init();
		}

	const _Engine& base() const _NOEXCEPT
		{	// return const reference to engine
		return (_Eng);
		}

	static constexpr result_type (min)()
		{	// return minimum possible generated value
		return ((_Engine::min)());
		}

	static constexpr result_type (max)()
		{	// return maximum possible generated value
		return ((_Engine::max)());
		}

	result_type operator()()
		{	// return next value
		size_t _Idx = (size_t)((double)(_Yx - (_Eng.min)()) * _Scale);

		_Yx = _Arr[_Idx];
		_Arr[_Idx] = _Eng();
		return (_Yx);
		}

	void discard(unsigned long long _Nskip)
		{	// discard _Nskip elements
		for (; 0 < _Nskip; --_Nskip)
			(*this)();
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Istr >> _Eng;
		for (size_t _Idx = 0; _Idx < _Kx; ++_Idx)
			_Istr >> _Arr[_Idx];
		return (_Istr >> _Yx);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Ostr << _Eng;
		for (size_t _Idx = 0; _Idx < _Kx; ++_Idx)
			_Ostr << ' ' << _Arr[_Idx];
		return (_Ostr << ' ' << _Yx);
		}

private:
	void _Init()
		{	// compute values for operator()
		for (size_t _Idx = 0; _Idx < _Kx; ++_Idx)
			_Arr[_Idx] = _Eng();
		_Yx = _Eng();
		_Scale = (double)_Kx
			/ ((double)(_Eng.max)() - (double)(_Eng.min)() + 1.0);
		}

	_Engine _Eng;	// the stored engine
	result_type _Arr[_Kx];
	result_type _Yx;
	double _Scale;
	};

template<class _Engine,
	size_t _Kx>
	bool operator==(
		const shuffle_order_engine<_Engine, _Kx>& _Left,
		const shuffle_order_engine<_Engine, _Kx>& _Right)
	{	// return true if _Left will generate same sequence as _Right
	return (_Left.base() == _Right.base());
	}

template<class _Engine,
	size_t _Kx>
	bool operator!=(
		const shuffle_order_engine<_Engine, _Kx>& _Left,
		const shuffle_order_engine<_Engine, _Kx>& _Right)
	{	// return true if _Left will not generate same sequence as _Right
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Engine,
	size_t _Kx>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		shuffle_order_engine<_Engine, _Kx>& _Eng)
	{	// read state from _Istr
	return (_Eng._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Engine,
	size_t _Kx>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const shuffle_order_engine<_Engine, _Kx>& _Eng)
	{	// write state to _Ostr
	return (_Eng._Write(_Ostr));
	}

	// TEMPLATE CLASS uniform_int
template<class _Ty = int>
	class uniform_int
	{	// template class for uniform integer distribution
public:
	typedef uniform_int<_Ty> _Myt;
	typedef _Ty result_type;

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(result_type _Min0 = 0, result_type _Max0 = 9)
			{	// construct from parameters
			_Init(_Min0, _Max0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Min == _Right._Min && _Max == _Right._Max);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		result_type a() const
			{	// return a value
			return (_Min);
			}

		result_type b() const
			{	// return b value
			return (_Max);
			}

		void _Init(_Ty _Min0, _Ty _Max0)
			{	// set internal state
			_RNG_ASSERT(_Min0 <= _Max0,
				"invalid min and max arguments for uniform_int");
			_Min = _Min0;
			_Max = _Max0;
			}

		result_type _Min;
		result_type _Max;
		};

	explicit uniform_int(_Ty _Min0 = 0,
		_Ty _Max0 = 9)
		: _Par(_Min0, _Max0)
		{	// construct from parameters
		}

	explicit uniform_int(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	result_type a() const
		{	// return a value
		return (_Par.a());
		}

	result_type b() const
		{	// return b value
		return (_Par.b());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// return minimum possible generated value
		return (_Par._Min);
		}

	result_type (max)() const
		{	// return maximum possible generated value
		return (_Par._Max);
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par._Min, _Par._Max));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0._Min, _Par0._Max));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, result_type _Nx) const
		{	// return next value
		return (_Eval(_Eng, 0, _Nx - 1));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Ty _Min0;
		_Ty _Max0;
		_Istr >> _Min0 >> _Max0;
		_Par._Init(_Min0, _Max0);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		return (_Ostr << _Par._Min << ' ' << _Par._Max);
		}

private:
	typedef typename make_unsigned<_Ty>::type _Uty;

	template<class _Engine>
		result_type _Eval(_Engine& _Eng, _Ty _Min, _Ty _Max) const
		{	// compute next value in range [_Min, _Max]
		_Rng_from_urng<_Uty, _Engine> _Rng(_Eng);

		const _Uty _Umin = _Adjust(_Uty(_Min));
		const _Uty _Umax = _Adjust(_Uty(_Max));

		_Uty _Uret = 0;

		if (_Umax - _Umin == _Uty(-1))
			_Uret = static_cast<_Uty>(_Rng._Get_all_bits());
		else
			_Uret = static_cast<_Uty>(_Rng(_Umax - _Umin + 1));

		return (_Ty(_Adjust(_Uret + _Umin)));
		}

	static _Uty _Adjust(_Uty _Uval)
		{	// convert signed ranges to unsigned ranges and vice versa
		return (_Adjust(_Uval, is_signed<_Ty>()));
		}

	static _Uty _Adjust(_Uty _Uval, true_type)
		{	// convert signed ranges to unsigned ranges and vice versa
		const _Uty _Adjuster = (_Uty(-1) >> 1) + 1;	// 2^(N-1)

		if (_Uval < _Adjuster)
			return (_Uval + _Adjuster);
		else
			return (_Uval - _Adjuster);
		}

	static _Uty _Adjust(_Uty _Uval, false_type)
		{	// _Ty is already unsigned, do nothing
		return (_Uval);
		}

	param_type _Par;
	};

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		uniform_int<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const uniform_int<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}

	// TEMPLATE CLASS uniform_int_distribution
template<class _Ty = int>
	class uniform_int_distribution
		: public uniform_int<_Ty>
	{	// template class for uniform integer distribution
public:
	static_assert(_Is_IntType<_Ty>::value,
		"invalid template argument for uniform_int_distribution");

	typedef uniform_int_distribution<_Ty> _Myt;
	typedef uniform_int<_Ty> _Mybase;
	typedef typename _Mybase::param_type _Mypbase;
	typedef typename _Mybase::result_type result_type;

	struct param_type
		: public _Mypbase
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(result_type _Min0 = 0,
			result_type _Max0 = (numeric_limits<_Ty>::max)())
			: _Mypbase(_Min0, _Max0)
			{	// construct from parameters
			}

		param_type(const _Mypbase& _Right)
			: _Mypbase(_Right)
			{	// construct from base
			}
		};

	explicit uniform_int_distribution(_Ty _Min0 = 0,
		_Ty _Max0 = (numeric_limits<_Ty>::max)())
		: _Mybase(_Min0, _Max0)
		{	// construct from parameters
		}

	explicit uniform_int_distribution(const param_type& _Par0)
		: _Mybase(_Par0)
		{	// construct from parameter package
		}
	};

template<class _Ty>
	bool operator==(const uniform_int_distribution<_Ty>& _Left,
		const uniform_int_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const uniform_int_distribution<_Ty>& _Left,
		const uniform_int_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

	// CLASS bernoulli_distribution
class bernoulli_distribution
	{	// class for bernoulli distribution
public:
	typedef bernoulli_distribution _Myt;
	typedef bool result_type;

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(double _P0 = 0.5)
			{	// construct from parameters
			_Init(_P0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Px == _Right._Px);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		double p() const
			{	// return probability
			return (_Px);
			}

		void _Init(double _P0)
			{	// set internal state
			_RNG_ASSERT(0.0 <= _P0 && _P0 <= 1.0,
				"invalid probability argument for bernoulli_distribution");

			_Px = _P0;
			}

		double _Px;
		};

	explicit bernoulli_distribution(double _P0 = 0.5)
		: _Par(_P0)
		{	// construct
		}

	explicit bernoulli_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	double p() const
		{	// return probability
		return (_Par.p());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// get smallest possible result
		return (false);
		}

	result_type (max)() const
		{	// get largest possible result
		return (true);
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		double _P0;
		_In(_Istr, _P0);
		_Par._Init(_P0);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Out(_Ostr, _Par._Px);
		return (_Ostr);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value
		return (_NRAND(_Eng, double) < _Par0._Px);
		}

	param_type _Par;
	};

inline bool operator==(const bernoulli_distribution& _Left,
	const bernoulli_distribution& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

inline bool operator!=(const bernoulli_distribution& _Left,
	const bernoulli_distribution& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		bernoulli_distribution& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const bernoulli_distribution& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}

	// TEMPLATE CLASS geometric_distribution

template<class _Ty = int>
	class geometric_distribution
	{	// template class for geometric distribution
public:
	typedef geometric_distribution<_Ty> _Myt;
	typedef double _Ty1;


	typedef _Ty result_type;
	static_assert(_Is_IntType<_Ty>::value,
		"invalid template argument for geometric_distribution");

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(_Ty1 _P0 = _Ty1(0.5))
			{	// construct from parameters
			_Init(_P0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Px == _Right._Px);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		_Ty1 p() const
			{	// return probability
			return (_Px);
			}

		void _Init(_Ty1 _P0)
			{	// initialize
			_RNG_ASSERT(0.0 < _P0 && _P0 < 1.0,
				"invalid probability argument for geometric_distribution");
			_Px = _P0;
			_Log_1_p = _CSTD log(1 - _Px);
			}

		_Ty1 _Px;
		_Ty1 _Log_1_p;
		};

	explicit geometric_distribution(_Ty1 _P0 = _Ty1(0.5))
		: _Par(_P0)
		{	// construct
		}

	explicit geometric_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	_Ty1 p() const
		{	// return probability
		return (_Par.p());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// get smallest possible result
		return (0);
		}

	result_type (max)() const
		{	// get largest possible result
		return ((numeric_limits<result_type>::max)());
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Ty1 _P0;
		_In(_Istr, _P0);
		_Par._Init(_P0);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Out(_Ostr, _Par._Px);
		return (_Ostr);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value
		return ((_Ty)(_CSTD log(_NRAND(_Eng, _Ty1))
			/ _Par0._Log_1_p));
		}

	param_type _Par;
	};

template<class _Ty>
	bool operator==(const geometric_distribution<_Ty>& _Left,
		const geometric_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const geometric_distribution<_Ty>& _Left,
		const geometric_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		geometric_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const geometric_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}


	// TEMPLATE CLASS poisson_distribution AND HELPER

template<class _Ty = int>
	class _Small_poisson_distribution
	{	//template class for poisson distribution with small mean
public:
	typedef double _Ty1;


	template<class _Engine>
		_Ty operator()(_Engine& _Eng) const
		{	// return next value
		_Ty _Res;
		_Ty1 _Val;
		for (_Res = 0, _Val = 1.0; ; ++_Res)
			{	// count repetitions
			_Val *= _NRAND(_Eng, _Ty1);
			if (_Val <= _G0)
				break;
			}
		return (_Res);
		}

	void _Init(const _Ty1& _Mean0)
		{	// set internal state
		_G0 = _CSTD exp(-_Mean0);
		}

private:
	_Ty1 _G0;
	};

template<class _Ty = int>
	class poisson_distribution
	{	// template class for poisson distribution
public:
	typedef poisson_distribution<_Ty> _Myt;
	typedef double _Ty1;


	typedef _Ty result_type;
	static_assert(_Is_IntType<_Ty>::value,
		"invalid template argument for poisson_distribution");

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(_Ty1 _Mean0 = _Ty1(1))
			{	// construct from parameters
			_Init(_Mean0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Mean == _Right._Mean);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		_Ty1 mean() const
			{	// return mean value
			return (_Mean);
			}

		void _Init(_Ty1 _Mean0)
			{	// set internal state
			_RNG_ASSERT(0.0 < _Mean0,
				"invalid mean argument for poisson_distribution");
			_Mean = _Mean0;
			_Sqrt = _CSTD sqrt(2.0 * _Mean0);
			_Logm = _CSTD log(_Mean0);
			_G1 = _Mean0 * _Logm - _XLgamma(_Mean0 + 1.0);
			_Small._Init(_Mean0);
			}

		_Ty1 _Mean;
		_Ty1 _Sqrt;
		_Ty1 _Logm;
		_Ty1 _G1;

		_Small_poisson_distribution<_Ty> _Small;

		};

	explicit poisson_distribution(_Ty1 _Mean0 = _Ty1(1))
		: _Par(_Mean0)
		{	// construct
		}

	explicit poisson_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	_Ty1 mean() const
		{	// return mean value
		return (_Par.mean());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// get smallest possible result
		return (0);
		}

	result_type (max)() const
		{	// get largest possible result
		return ((numeric_limits<result_type>::max)());
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Ty1 _Mean0;
		_In(_Istr, _Mean0);
		_Par._Init(_Mean0);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Out(_Ostr, _Par._Mean);
		return (_Ostr);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value
		if (_Par0._Mean < 12.0)
			return (_Par0._Small(_Eng));
		for (; ; )
			{	// generate and reject
			_Ty _Res;
			_Ty1 _Yx;
			for (; ; )
				{	// generate a tentative value
				_Yx = (_Ty1)_CSTD tan(_Pi * _NRAND(_Eng, _Ty1));
				_Res = (_Ty)(_Par0._Sqrt * _Yx + _Par0._Mean);
				if (static_cast<_Ty>(0) <= _Res)
					break;
				}
			if (_NRAND(_Eng, _Ty1) <= 0.9 * (1.0 + _Yx * _Yx)
				* _CSTD exp(_Res * _Par0._Logm
					- _XLgamma(_Res + 1.0) - _Par0._G1))
				return (_Res);
			}
		}

	param_type _Par;
	};

template<class _Ty>
	bool operator==(const poisson_distribution<_Ty>& _Left,
		const poisson_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const poisson_distribution<_Ty>& _Left,
		const poisson_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		poisson_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const poisson_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}


	// TEMPLATE CLASS binomial_distribution

template<class _Ty = int>
	class binomial_distribution
	{	// template class for binomial distribution
public:
	typedef binomial_distribution<_Ty> _Myt;
	typedef double _Ty1;


	typedef _Ty result_type;
	static_assert(_Is_IntType<_Ty>::value,
		"invalid template argument for binomial_distribution");

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(_Ty _T0 = 1, _Ty1 _P0 = _Ty1(0.5))
			{	// construct from parameters
			_Init(_T0, _P0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Tx == _Right._Tx && _Px == _Right._Px);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		_Ty t() const
			{	// return max value
			return (_Tx);
			}

		_Ty1 p() const
			{	// return probability
			return (_Px);
			}

		void _Init(_Ty _T0, _Ty1 _P0)
			{	// initialize
			_RNG_ASSERT(0.0 <= _T0,
				"invalid max argument for binomial_distribution");
			_RNG_ASSERT(0.0 <= _P0 && _P0 <= 1.0,
				"invalid probability argument for binomial_distribution");
			_Tx = _T0;
			_Px = _P0;
			_Pp = _Px < 0.5 ? _Px : (1.0 - _Px);
			_Mean = _Tx * _Pp;
			_G1 = _XLgamma(_Tx + 1.0);
			_Sqrt = _CSTD sqrt(2 * _Mean * (1 - _Pp));
			_Logp = _CSTD log(_Pp);
			_Logp1 = _CSTD log(1.0 - _Pp);
			_Small._Init(_Mean);
			}

		_Ty _Tx;
		_Ty1 _Px;
		_Ty1 _Pp;
		_Ty1 _Mean;
		_Ty1 _G1;
		_Ty1 _Sqrt;
		_Ty1 _Logp;
		_Ty1 _Logp1;

		_Small_poisson_distribution<_Ty> _Small;

		};

	explicit binomial_distribution(_Ty _T0 = 1,
		_Ty1 _P0 = _Ty1(0.5))
		: _Par(_T0, _P0)
		{	// construct
		}

	explicit binomial_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	_Ty t() const
		{	// return max value
		return (_Par.t());
		}

	_Ty1 p() const
		{	// return probability
		return (_Par.p());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// get smallest possible result
		return (0);
		}

	result_type (max)() const
		{	// get largest possible result
		return (_Par.t());
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Ty _T0;
		_Ty1 _P0;
		_In(_Istr, _P0);
		_In(_Istr, _T0);
		_Par._Init(_T0, _P0);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Out(_Ostr, _Par._Px);
		_Out(_Ostr, _Par._Tx);
		return (_Ostr);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0) const
		{	// Press et al., Numerical Recipes in C, 2nd ed., pp 295-296.
		_Ty _Res;
		if (_Par0._Tx < 25)
			{	// generate directly
			_Res = 0;
			for (_Ty _Ix = 0; _Ix < _Par0._Tx; ++_Ix)
				if (_NRAND(_Eng, _Ty1) < _Par0._Px)
					++_Res;
			return (_Res);
			}
		else if (_Par0._Mean < 1.0)
			// events are rare, use Poisson distribution
			_Res = _Par0._Small(_Eng);
		else
			{	// no shortcuts
			for (; ; )
				{	// generate and reject
				_Ty1 _Yx;
				for (; ; )
					{	// generate a tentative value
					_Yx = (_Ty1)_CSTD tan(_Pi * _NRAND(_Eng, _Ty1));
					_Res = (_Ty)(_Par0._Sqrt * _Yx + _Par0._Mean);
					if (static_cast<_Ty>(0) <= _Res && _Res <= _Par0._Tx)
						break;
					}
				if (_NRAND(_Eng, _Ty1) <= 1.2 * _Par0._Sqrt
					* (1.0 + _Yx * _Yx)
					* _CSTD exp(_Par0._G1 - _XLgamma(_Res + 1.0)
						- _XLgamma(_Par0._Tx - _Res
						+ 1.0) + _Res * _Par0._Logp
						+ (_Par0._Tx - _Res) * _Par0._Logp1))
					break;
				}
			}
		return (_Par0._Px == _Par0._Pp ? _Res : (_Par0._Tx - _Res));
		}

	param_type _Par;
	};

template<class _Ty>
	bool operator==(const binomial_distribution<_Ty>& _Left,
		const binomial_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const binomial_distribution<_Ty>& _Left,
		const binomial_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		binomial_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const binomial_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}


	// TEMPLATE CLASS uniform_real
template<class _Ty = double>
	class uniform_real
	{	// template class for uniform real distribution
public:
	typedef uniform_real<_Ty> _Myt;
	typedef _Ty result_type;

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(_Ty _Min0 = _Ty(0),
			_Ty _Max0 = _Ty(1))
			{	// construct from parameters
			_Init(_Min0, _Max0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Min == _Right._Min && _Max == _Right._Max);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		result_type a() const
			{	// return a value
			return (_Min);
			}

		result_type b() const
			{	// return b value
			return (_Max);
			}

		void _Init(_Ty _Min0, _Ty _Max0)
			{	// set internal state
			_RNG_ASSERT(_Min0 <= _Max0 && (0 <= _Min0
				|| _Max0 <= _Min0 + (numeric_limits<_Ty>::max)()),
				"invalid min and max arguments for uniform_real");
			_Min = _Min0;
			_Max = _Max0;
			}

		result_type _Min;
		result_type _Max;
		};

	explicit uniform_real(_Ty _Min0 = _Ty(0),
		_Ty _Max0 = _Ty(1))
		: _Par(_Min0, _Max0)
		{	// construct
		}

	explicit uniform_real(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	result_type a() const
		{	// return a value
		return (_Par.a());
		}

	result_type b() const
		{	// return b value
		return (_Par.b());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// return minimum possible generated value
		return (_Par._Min);
		}

	result_type (max)() const
		{	// return maximum possible generated value
		return (_Par._Max);
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Ty _Min0;
		_Ty _Max0;
		_In(_Istr, _Min0);
		_In(_Istr, _Max0);
		_Par._Init(_Min0, _Max0);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Out(_Ostr, _Par._Min);
		_Out(_Ostr, _Par._Max);
		return (_Ostr);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value
		return (_NRAND(_Eng, _Ty) * (_Par0._Max - _Par0._Min) + _Par0._Min);
		}

	param_type _Par;
	};

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		uniform_real<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const uniform_real<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}

	// TEMPLATE CLASS uniform_real_distribution
template<class _Ty = double>
	class uniform_real_distribution
		: public uniform_real<_Ty>
	{	// template class for uniform integer distribution
public:
	static_assert(_Is_RealType<_Ty>::value,
		"invalid template argument for uniform_real_distribution");

	typedef uniform_real_distribution<_Ty> _Myt;
	typedef uniform_real<_Ty> _Mybase;
	typedef typename _Mybase::param_type _Mypbase;
	typedef typename _Mybase::result_type result_type;

	struct param_type
		: public _Mypbase
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(_Ty _Min0 = _Ty(0),
			_Ty _Max0 = _Ty(1))
			: _Mypbase(_Min0, _Max0)
			{	// construct from parameters
			}

		param_type(const _Mypbase& _Right)
			: _Mypbase(_Right)
			{	// construct from base
			}
		};

	explicit uniform_real_distribution(_Ty _Min0 = _Ty(0),
		_Ty _Max0 = _Ty(1))
		: _Mybase(_Min0, _Max0)
		{	// construct from parameters
		}

	explicit uniform_real_distribution(const param_type& _Par0)
		: _Mybase(_Par0)
		{	// construct from parameter package
		}
	};

template<class _Ty>
	bool operator==(const uniform_real_distribution<_Ty>& _Left,
		const uniform_real_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const uniform_real_distribution<_Ty>& _Left,
		const uniform_real_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

	// TEMPLATE CLASS exponential_distribution
template<class _Ty = double>
	class exponential_distribution
	{	// template class for exponential distribution
public:
	static_assert(_Is_RealType<_Ty>::value,
		"invalid template argument for exponential_distribution");

	typedef exponential_distribution<_Ty> _Myt;
	typedef _Ty result_type;

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(_Ty _Lambda0 = _Ty(1))
			{	// construct from parameters
			_Init(_Lambda0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Lambda == _Right._Lambda);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		_Ty lambda() const
			{	// return lambda value
			return (_Lambda);
			}

		void _Init(_Ty _Lambda0)
			{	// set internal state
			_RNG_ASSERT(0.0 < _Lambda0,
				"invalid lambda argument for exponential_distribution");
			_Lambda = _Lambda0;
			}

		_Ty _Lambda;
		};

	explicit exponential_distribution(_Ty _Lambda0 = _Ty(1))
		: _Par(_Lambda0)
		{	// construct
		}

	explicit exponential_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	_Ty lambda() const
		{	// return lambda value
		return (_Par.lambda());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// get smallest possible result
		return (0);
		}

	result_type (max)() const
		{	// get largest possible result
		return ((numeric_limits<result_type>::max)());
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Ty _Lambda0;
		_In(_Istr, _Lambda0);
		_Par._Init(_Lambda0);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Out(_Ostr, _Par._Lambda);
		return (_Ostr);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value
		return (-_CSTD log(_Ty(1) - _NRAND(_Eng, _Ty)) / _Par0._Lambda);
		}

	param_type _Par;
	};

template<class _Ty>
	bool operator==(const exponential_distribution<_Ty>& _Left,
		const exponential_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const exponential_distribution<_Ty>& _Left,
		const exponential_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		exponential_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const exponential_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}

	// TEMPLATE CLASS normal_distribution
template<class _Ty = double>
	class normal_distribution
	{	// template class for normal distribution
public:
	static_assert(_Is_RealType<_Ty>::value,
		"invalid template argument for normal_distribution");

	typedef normal_distribution<_Ty> _Myt;
	typedef _Ty result_type;

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(_Ty _Mean0 = 0.0, _Ty _Sigma0 = 1.0)
			{	// construct from parameters
			_Init(_Mean0, _Sigma0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Mean == _Right._Mean && _Sigma == _Right._Sigma);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		_Ty mean() const
			{	// return mean value
			return (_Mean);
			}

		_Ty sigma() const
			{	// return sigma value
			return (_Sigma);
			}

		_Ty stddev() const
			{	// return sigma value
			return (_Sigma);
			}

		void _Init(_Ty _Mean0, _Ty _Sigma0)
			{	// set internal state
			_RNG_ASSERT(0.0 < _Sigma0,
				"invalid sigma argument for normal_distribution");
			_Mean = _Mean0;
			_Sigma = _Sigma0;
			}

		_Ty _Mean;
		_Ty _Sigma;
		};

	explicit normal_distribution(_Ty _Mean0 = 0.0, _Ty _Sigma0 = 1.0)
		: _Par(_Mean0, _Sigma0), _Valid(false), _X2(0)
		{	// construct
		}

	explicit normal_distribution(const param_type& _Par0)
		: _Par(_Par0), _Valid(false), _X2(0)
		{	// construct from parameter package
		}

	_Ty mean() const
		{	// return mean value
		return (_Par.mean());
		}

	_Ty sigma() const
		{	// return sigma value
		return (_Par.sigma());
		}

	_Ty stddev() const
		{	// return sigma value
		return (_Par.sigma());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		reset();
		}

	result_type (min)() const
		{	// get smallest possible result
		return (numeric_limits<result_type>::denorm_min());
		}

	result_type (max)() const
		{	// get largest possible result
		return ((numeric_limits<result_type>::max)());
		}

	void reset()
		{	// clear internal state
		_Valid = false;
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng)
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0)
		{	// return next value, given parameter package
		reset();
		return (_Eval(_Eng, _Par0, false));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Ty _Mean0;
		_Ty _Sigma0;
		_In(_Istr, _Mean0);
		_In(_Istr, _Sigma0);
		_Par._Init(_Mean0, _Sigma0);

		_Istr >> _Valid;
		_In(_Istr, _X2);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Out(_Ostr, _Par._Mean);
		_Out(_Ostr, _Par._Sigma);

		_Ostr << ' ' << _Valid;
		_Out(_Ostr, _X2);
		return (_Ostr);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0,
			bool _Keep = true)
		{	// compute next value
			// Knuth, vol. 2, p. 122, alg. P
		_Ty _Res;
		if (_Keep && _Valid)
			{	// return stored value
			_Res = _X2;
			_Valid = false;
			}
		else
			{	// generate two values, store one, return one
			double _V1, _V2, _Sx;
			for (; ; )
				{	// reject bad values
				_V1 = 2 * _NRAND(_Eng, _Ty) - 1.0;
				_V2 = 2 * _NRAND(_Eng, _Ty) - 1.0;
				_Sx = _V1 * _V1 + _V2 * _V2;
				if (_Sx < 1.0)
					break;
				}
			double _Fx = _CSTD sqrt(-2.0 * _CSTD log(_Sx) / _Sx);
			if (_Keep)
				{	// save second value for next call
				_X2 = _Fx * _V2;
				_Valid = true;
				}
			_Res = _Fx * _V1;
			}
		return (_Res * _Par0._Sigma + _Par0._Mean);
		}

	param_type _Par;
	bool _Valid;
	_Ty _X2;
	};

template<class _Ty>
	bool operator==(const normal_distribution<_Ty>& _Left,
		const normal_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const normal_distribution<_Ty>& _Left,
		const normal_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		normal_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const normal_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}

	// TEMPLATE CLASS gamma_distribution
template<class _Ty = double>
	class gamma_distribution
	{	// template class for gamma distribution
public:
	static_assert(_Is_RealType<_Ty>::value,
		"invalid template argument for gamma_distribution");

	typedef gamma_distribution<_Ty> _Myt;
	typedef _Ty result_type;

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(_Ty _Alpha0 = _Ty(1), _Ty _Beta0 = _Ty(1))
			{	// construct from parameters
			_Init(_Alpha0, _Beta0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Px == _Right._Px);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		_Ty alpha() const
			{	// return alpha value
			return (_Alpha);
			}

		_Ty beta() const
			{	// return beta value
			return (_Beta);
			}

		void _Init(_Ty _Alpha0, _Ty _Beta0)
			{	// initialize
			_RNG_ASSERT(0.0 < _Alpha0,
				"invalid alpha argument for gamma_distribution");
			_RNG_ASSERT(0.0 < _Beta0,
				"invalid beta argument for gamma_distribution");
			_Alpha = _Alpha0;
			_Beta = _Beta0;
			_Px = (_Ty)(_Exp1 / (_Alpha + _Exp1));
			_Sqrt = _CSTD sqrt(2 * _Alpha - 1);
			}

		_Ty _Alpha;
		_Ty _Beta;
		_Ty _Px;
		_Ty _Sqrt;
		exponential_distribution<_Ty> _Exp;
		};

	explicit gamma_distribution(_Ty _Alpha0 = _Ty(1), _Ty _Beta0 = _Ty(1))
		: _Par(_Alpha0, _Beta0)
		{	// construct
		}

	explicit gamma_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	_Ty alpha() const
		{	// return alpha value
		return (_Par.alpha());
		}

	_Ty beta() const
		{	// return beta value
		return (_Par.beta());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// get smallest possible result
		return (numeric_limits<result_type>::denorm_min());
		}

	result_type (max)() const
		{	// get largest possible result
		return ((numeric_limits<result_type>::max)());
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Ty _Alpha0;
		_Ty _Beta0;
		_In(_Istr, _Alpha0);
		_In(_Istr, _Beta0);
		_Par._Init(_Alpha0, _Beta0);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Out(_Ostr, _Par._Alpha);
		_Out(_Ostr, _Par._Beta);
		return (_Ostr);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, param_type _Par0) const
		{	// return next value
		_Ty _Ux, _Vx;
		_Ty _Xx, _Yx, _Qx;
		int _Count;
		if (_Par0._Alpha < 1)
			{	// small values of alpha
				// from Knuth
			for (; ; )
				{	// generate and reject
				_Ux = _NRAND(_Eng, _Ty);
				while ((_Vx = _NRAND(_Eng, _Ty)) == 0)
					;
				if (_Ux < _Par0._Px)
					{	// small _Ux
					_Xx = _CSTD pow(_Vx, _Ty(1) / _Par0._Alpha);
					_Qx = _CSTD exp(-_Xx);
					}
				else
					{	// large _Ux
					_Xx = 1 - _CSTD log(_Vx);
					_Qx = _CSTD pow(_Xx, _Par0._Alpha - 1);
					}
				if (_NRAND(_Eng, _Ty) < _Qx)
					return (_Par0._Beta * _Xx);
				}
			}
		else if (_Par0._Alpha == 1)
			return (_Par0._Beta * _Par0._Exp(_Eng));
		else if ((_Count = (int)_Par0._Alpha) == _Par0._Alpha
			&& _Count < 20)
			{	// _Alpha is small integer, compute directly
			_Yx = _NRAND(_Eng, _Ty);
			while (--_Count)
				{	// adjust result
				while ((_Ux = _NRAND(_Eng, _Ty)) == 0)
					;
				_Yx *= _Ux;
				}
			return (_Par0._Beta * -_CSTD log(_Yx));
			}
		else
			{	// no shortcuts
			for (; ; )
				{	// generate and reject
				_Yx = (_Ty)_CSTD tan(_Pi * _NRAND(_Eng, _Ty));
				_Xx = _Par0._Sqrt * _Yx + _Par0._Alpha - 1;
				if (0 < _Xx &&
					_NRAND(_Eng, _Ty) <= (1 + _Yx * _Yx)
						* _CSTD exp((_Par0._Alpha - 1)
						* _CSTD log(_Xx / (_Par0._Alpha - 1))
							- _Par0._Sqrt * _Yx))
					return (_Par0._Beta * _Xx);
				}
			}
		}

	param_type _Par;
	};

template<class _Ty>
	bool operator==(const gamma_distribution<_Ty>& _Left,
		const gamma_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const gamma_distribution<_Ty>& _Left,
		const gamma_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		gamma_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const gamma_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}

	// TEMPLATE CLASS weibull_distribution
template<class _Ty = double>
	class weibull_distribution
	{	// template class for weibull distribution
public:
	static_assert(_Is_RealType<_Ty>::value,
		"invalid template argument for weibull_distribution");

	typedef weibull_distribution<_Ty> _Myt;
	typedef _Ty result_type;

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(_Ty _A0 = _Ty(1),
			_Ty _B0 = _Ty(1))
			{	// construct from parameters
			_Init(_A0, _B0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Ax == _Right._Ax && _Bx == _Right._Bx);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		_Ty a() const
			{	// return a value
			return (_Ax);
			}

		_Ty b() const
			{	// return b value
			return (_Bx);
			}

		void _Init(_Ty _A0, _Ty _B0)
			{	// initialize
			_RNG_ASSERT(0.0 < _A0,
				"invalid a argument for weibull_distribution");
			_RNG_ASSERT(0.0 < _B0,
				"invalid b argument for weibull_distribution");
			_Ax = _A0;
			_Bx = _B0;
			}

		_Ty _Ax;
		_Ty _Bx;
		};

	explicit weibull_distribution(_Ty _A0 = _Ty(1),
		_Ty _B0 = _Ty(1))
		: _Par(_A0, _B0)
		{	// construct
		}

	explicit weibull_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	_Ty a() const
		{	// return a value
		return (_Par.a());
		}

	_Ty b() const
		{	// return b value
		return (_Par.b());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// get smallest possible result
		return (0);
		}

	result_type (max)() const
		{	// get largest possible result
		return ((numeric_limits<result_type>::max)());
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Ty _A0;
		_Ty _B0;
		_In(_Istr, _A0);
		_In(_Istr, _B0);
		_Par._Init(_A0, _B0);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Out(_Ostr, _Par._Ax);
		_Out(_Ostr, _Par._Bx);
		return (_Ostr);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0) const
		{	// generate pseudo-random value
		_Ty _Px = (_Ty(1) - _NRAND(_Eng, _Ty));
		return (_Par0._Bx * _CSTD pow(-_CSTD log(_Px), _Ty(1) / _Par0._Ax));
		}

	param_type _Par;
	};

template<class _Ty>
	bool operator==(const weibull_distribution<_Ty>& _Left,
		const weibull_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const weibull_distribution<_Ty>& _Left,
		const weibull_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		weibull_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const weibull_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}

	// TEMPLATE CLASS extreme_value_distribution
template<class _Ty = double>
	class extreme_value_distribution
	{	// template class for extreme value distribution
public:
	static_assert(_Is_RealType<_Ty>::value,
		"invalid template argument for extreme_value_distribution");

	typedef extreme_value_distribution<_Ty> _Myt;
	typedef _Ty result_type;

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(_Ty _A0 = _Ty(0),
			_Ty _B0 = _Ty(1))
			{	// construct from parameters
			_Init(_A0, _B0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Ax == _Right._Ax && _Bx == _Right._Bx);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		_Ty a() const
			{	// return a value
			return (_Ax);
			}

		_Ty b() const
			{	// return b value
			return (_Bx);
			}

		void _Init(_Ty _A0, _Ty _B0)
			{	// initialize
			_RNG_ASSERT(0.0 < _B0,
				"invalid b argument for extreme_value_distribution");
			_Ax = _A0;
			_Bx = _B0;
			}

		_Ty _Ax;
		_Ty _Bx;
		};

	explicit extreme_value_distribution(_Ty _A0 = _Ty(0),
		_Ty _B0 = _Ty(1))
		: _Par(_A0, _B0)
		{	// construct
		}

	explicit extreme_value_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	_Ty a() const
		{	// return a value
		return (_Par.a());
		}

	_Ty b() const
		{	// return b value
		return (_Par.b());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// get smallest possible result
		return ((numeric_limits<result_type>::min)());
		}

	result_type (max)() const
		{	// get largest possible result
		return ((numeric_limits<result_type>::max)());
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Ty _A0;
		_Ty _B0;
		_In(_Istr, _A0);
		_In(_Istr, _B0);
		_Par._Init(_A0, _B0);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Out(_Ostr, _Par._Ax);
		_Out(_Ostr, _Par._Bx);
		return (_Ostr);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0) const
		{	// generate pseudo-random value
		_Ty _Px = _NRAND(_Eng, _Ty);
		return (_Par0._Ax - _Par0._Bx * _CSTD log(-_CSTD log(_Px)));
		}

	param_type _Par;
	};

template<class _Ty>
	bool operator==(const extreme_value_distribution<_Ty>& _Left,
		const extreme_value_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const extreme_value_distribution<_Ty>& _Left,
		const extreme_value_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		extreme_value_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const extreme_value_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}

	// TEMPLATE CLASS lognormal_distribution
template<class _Ty = double>
	class lognormal_distribution
	{	// template class for lognormal_distribution
public:
	static_assert(_Is_RealType<_Ty>::value,
		"invalid template argument for lognormal_distribution");

	typedef lognormal_distribution<_Ty> _Myt;
	typedef _Ty result_type;

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(_Ty _M0 = _Ty(0),
			_Ty _S0 = _Ty(1))
			{	// construct from parameters
			_Init(_M0, _S0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Mx == _Right._Mx && _Sx == _Right._Sx);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		_Ty m() const
			{	// return m value
			return (_Mx);
			}

		_Ty s() const
			{	// return s value
			return (_Sx);
			}

		void _Init(_Ty _M0, _Ty _S0)
			{	// initialize
			_RNG_ASSERT(0.0 < _S0,
				"invalid s argument for lognormal_distribution");
			_Mx = _M0;
			_Sx = _S0;
			}

		_Ty _Mx;
		_Ty _Sx;
		};

	explicit lognormal_distribution(_Ty _M0 = _Ty(0),
		_Ty _S0 = _Ty(1))
		: _Par(_M0, _S0)
		{	// construct
		}

	explicit lognormal_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	_Ty m() const
		{	// return m value
		return (_Par.m());
		}

	_Ty s() const
		{	// return s value
		return (_Par.s());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// get smallest possible result
		return (-(numeric_limits<result_type>::max)());
		}

	result_type (max)() const
		{	// get largest possible result
		return ((numeric_limits<result_type>::max)());
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Ty _M0;
		_Ty _S0;
		_In(_Istr, _M0);
		_In(_Istr, _S0);
		_Par._Init(_M0, _S0);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Out(_Ostr, _Par._Mx);
		_Out(_Ostr, _Par._Sx);
		return (_Ostr);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, param_type _Par0) const
		{	// generate pseudo-random value
		normal_distribution<_Ty> _Dist(_Par0._Mx, _Par0._Sx);
		return (_CSTD exp(_Dist(_Eng)));
		}

	param_type _Par;
	};

template<class _Ty>
	bool operator==(const lognormal_distribution<_Ty>& _Left,
		const lognormal_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const lognormal_distribution<_Ty>& _Left,
		const lognormal_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		lognormal_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const lognormal_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}

	// TEMPLATE CLASS chi_squared_distribution
template<class _Ty = double>
	class chi_squared_distribution
	{	// template class for chi squared distribution
public:
	static_assert(_Is_RealType<_Ty>::value,
		"invalid template argument for chi_squared_distribution");

	typedef chi_squared_distribution<_Ty> _Myt;
	typedef _Ty result_type;

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(_Ty _N0 = _Ty(1))
			{	// construct from parameters
			_Init(_N0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Nx == _Right._Nx);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		_Ty n() const
			{	// return n value
			return (_Nx);
			}

		void _Init(_Ty _N0)
			{	// initialize
			_RNG_ASSERT(0 < _N0,
				"invalid n argument for chi_squared_distribution");
			_Nx = _N0;
			}

		_Ty _Nx;
		};

	explicit chi_squared_distribution(_Ty _N0 = _Ty(1))
		: _Par(_N0)
		{	// construct
		}

	explicit chi_squared_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	_Ty n() const
		{	// return n value
		return (_Par.n());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// get smallest possible result
		return (numeric_limits<result_type>::denorm_min());
		}

	result_type (max)() const
		{	// get largest possible result
		return ((numeric_limits<result_type>::max)());
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Ty _N0;
		_Istr >> _N0;
		_Par._Init(_N0);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		return (_Ostr << ' ' << _Par._Nx);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0) const
		{	// return pseudo-random value
		gamma_distribution<_Ty> _Dist((_Ty)_Par0._Nx * _Ty(0.5), _Ty(2));
		return (_Dist(_Eng));
		}

	param_type _Par;
	};

template<class _Ty>
	bool operator==(const chi_squared_distribution<_Ty>& _Left,
		const chi_squared_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const chi_squared_distribution<_Ty>& _Left,
		const chi_squared_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		chi_squared_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const chi_squared_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}

	// TEMPLATE CLASS cauchy_distribution
template<class _Ty = double>
	class cauchy_distribution
	{	// template class for Cauchy distribution
public:
	static_assert(_Is_RealType<_Ty>::value,
		"invalid template argument for cauchy_distribution");

	typedef cauchy_distribution<_Ty> _Myt;
	typedef _Ty result_type;

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(_Ty _A0 = _Ty(0),
			_Ty _B0 = _Ty(1))
			{	// construct from parameters
			_Init(_A0, _B0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Ax == _Right._Ax && _Bx == _Right._Bx);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		_Ty a() const
			{	// return a value
			return (_Ax);
			}

		_Ty b() const
			{	// return b value
			return (_Bx);
			}

		void _Init(_Ty _A0, _Ty _B0)
			{	// initialize
			_RNG_ASSERT(0.0 < _B0,
				"invalid b argument for cauchy_distribution");
			_Ax = _A0;
			_Bx = _B0;
			}

		_Ty _Ax;
		_Ty _Bx;
		};

	explicit cauchy_distribution(_Ty _A0 = _Ty(0),
		_Ty _B0 = _Ty(1))
		: _Par(_A0, _B0)
		{	// construct
		}

	explicit cauchy_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	_Ty a() const
		{	// return a value
		return (_Par.a());
		}

	_Ty b() const
		{	// return b value
		return (_Par.b());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// get smallest possible result
		return (-(numeric_limits<result_type>::max)());
		}

	result_type (max)() const
		{	// get largest possible result
		return ((numeric_limits<result_type>::max)());
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Ty _A0;
		_Ty _B0;
		_In(_Istr, _A0);
		_In(_Istr, _B0);
		_Par._Init(_A0, _B0);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Out(_Ostr, _Par._Ax);
		_Out(_Ostr, _Par._Bx);
		return (_Ostr);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0) const
		{	// generate pseudo-random value
		_Ty Px = _NRAND(_Eng, _Ty);
		return ((_Ty)(_Par0._Ax
			+ _Par0._Bx * _CSTD tan(_Pi * (Px - _Ty(0.5)))));
		}

	param_type _Par;
	};

template<class _Ty>
	bool operator==(const cauchy_distribution<_Ty>& _Left,
		const cauchy_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const cauchy_distribution<_Ty>& _Left,
		const cauchy_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		cauchy_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const cauchy_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}

	// TEMPLATE CLASS _Beta_distribution
template<class _Ty = double>
	class _Beta_distribution
	{	// template class for beta distribution
public:
	typedef _Ty result_type;

	explicit _Beta_distribution(const _Ty& _A0 = _Ty(1),
		const _Ty& _B0 = _Ty(1))
		{	// construct
		_Init(_A0, _B0);
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		if (_Ax < _Ty(1) && _Bx < _Ty(1))
			{	// look for a good value
			_Ty _Wx, _P1, _P2;
			for (; ; )
				{	// reject large values
				_P1 = _NRAND(_Eng, _Ty);
				_P2 = _NRAND(_Eng, _Ty);
				_P1 = _CSTD pow(_P1, _Ty(1) / _Ax);
				_P2 = _CSTD pow(_P2, _Ty(1) / _Bx);
				_Wx = _P1 + _P2;
				if (_Wx <= _Ty(1))
					break;
				}
			return (_P1 / _Wx);
			}
		else
			{	// use gamma distributions instead
			_Ty _P1, _P2;
			gamma_distribution<_Ty> _Dist1(_Ax, 1);
			gamma_distribution<_Ty> _Dist2(_Bx, 1);
			_P1 = _Dist1(_Eng);
			_P2 = _Dist2(_Eng);
			return (_P1 / (_P1 + _P2));
			}
		}

private:
	void _Init(_Ty _A0, _Ty _B0)
		{	// initialize
		_RNG_ASSERT(0.0 < _A0,
			"invalid a argument for _Beta_distribution");
		_RNG_ASSERT(0.0 < _B0,
			"invalid b argument for _Beta_distribution");
		_Ax = _A0;
		_Bx = _B0;
		}

	_Ty _Ax;
	_Ty _Bx;
	};

	// TEMPLATE CLASS fisher_f_distribution
template<class _Ty = double>
	class fisher_f_distribution
	{	// template class for fisher_f distribution
public:
	static_assert(_Is_RealType<_Ty>::value,
		"invalid template argument for fisher_f_distribution");

	typedef fisher_f_distribution<_Ty> _Myt;
	typedef _Ty result_type;

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(_Ty _M0 = _Ty(1), _Ty _N0 = _Ty(1))
			{	// construct from parameters
			_Init(_M0, _N0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Mx == _Right._Mx && _Nx == _Right._Nx);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		_Ty m() const
			{	// return m value
			return (_Mx);
			}

		_Ty n() const
			{	// return n value
			return (_Nx);
			}

		void _Init(_Ty _M0, _Ty _N0)
			{	// initialize
			_RNG_ASSERT(0 < _M0,
				"invalid m argument for fisher_f_distribution");
			_RNG_ASSERT(0 < _N0,
				"invalid n argument for fisher_f_distribution");
			_Mx = _M0;
			_Nx = _N0;
			}

		_Ty _Mx;
		_Ty _Nx;
		};

	explicit fisher_f_distribution(_Ty _M0 = _Ty(1),
		_Ty _N0 = _Ty(1))
		: _Par(_M0, _N0)
		{	// construct
		}

	explicit fisher_f_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	_Ty m() const
		{	// return m value
		return (_Par.m());
		}

	_Ty n() const
		{	// return n value
		return (_Par.n());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// get smallest possible result
		return (result_type(0));
		}

	result_type (max)() const
		{	// get largest possible result
		return ((numeric_limits<result_type>::max)());
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Ty _M0;
		_Ty _N0;
		_Istr >> _M0 >> _N0;
		_Par._Init(_M0, _N0);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Ostr << ' ' << _Par._Mx << ' ' << _Par._Nx;
		return (_Ostr);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value
		_Ty _Px, _V1, _V2;
		_V1 = (_Ty)_Par0._Mx * _Ty(0.5);
		_V2 = (_Ty)_Par0._Nx * _Ty(0.5);
		_Beta_distribution<_Ty> _Dist(_V1, _V2);
		_Px = _Dist(_Eng);

		return ((_V2 / _V1) * (_Px / (_Ty(1) - _Px)));
		}

	param_type _Par;
	};

template<class _Ty>
	bool operator==(const fisher_f_distribution<_Ty>& _Left,
		const fisher_f_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const fisher_f_distribution<_Ty>& _Left,
		const fisher_f_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		fisher_f_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const fisher_f_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}

	// TEMPLATE CLASS student_t_distribution
template<class _Ty = double>
	class student_t_distribution
	{	// template class for student_t distribution
public:
	static_assert(_Is_RealType<_Ty>::value,
		"invalid template argument for student_t_distribution");

	typedef student_t_distribution<_Ty> _Myt;
	typedef _Ty result_type;

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(_Ty _N0 = _Ty(1))
			{	// construct from parameters
			_Init(_N0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Nx == _Right._Nx);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		_Ty n() const
			{	// return n value
			return (_Nx);
			}

		void _Init(_Ty _N0)
			{	// initialize
			_RNG_ASSERT(0 < _N0,
				"invalid n argument for student_t_distribution");
			_Nx = _N0;
			}

		_Ty _Nx;
		};

	explicit student_t_distribution(_Ty _N0 = _Ty(1))
		: _Par(_N0)
		{	// construct
		}

	explicit student_t_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	_Ty n() const
		{	// return n value
		return (_Par.n());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// get smallest possible result
		return (-(numeric_limits<result_type>::max)());
		}

	result_type (max)() const
		{	// get largest possible result
		return ((numeric_limits<result_type>::max)());
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Ty _N0;
		_Istr >> _N0;
		_Par._Init(_N0);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		return (_Ostr << ' ' << _Par._Nx);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value
		_Ty _V1, _V2, _R, _Rs;
		uniform_real<_Ty> _Dist(-1, 1);
		for (; ; )
			{	// get a point inside unit circle
			_V1 = _Dist(_Eng);
			_V2 = _Dist(_Eng);
			_Rs = _V1 * _V1 + _V2 * _V2;

			if (_Rs < _Ty(1))
				break;
			}
		_R = _CSTD sqrt(_Rs);

		return (_V1 * _CSTD sqrt(_Par0._Nx
			* (_CSTD pow(_R, -_Ty(4) / _Par0._Nx) - _Ty(1)) / _Rs));
		}

	param_type _Par;
	};

template<class _Ty>
	bool operator==(const student_t_distribution<_Ty>& _Left,
		const student_t_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const student_t_distribution<_Ty>& _Left,
		const student_t_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		student_t_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const student_t_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}

	// TEMPLATE CLASS negative_binomial_distribution
template<class _Ty = int>
	class negative_binomial_distribution
	{	// template class for negative binomial distribution
public:
	static_assert(_Is_IntType<_Ty>::value,
		"invalid template argument for negative_binomial_distribution");

	typedef negative_binomial_distribution<_Ty> _Myt;
	typedef _Ty result_type;

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;

		explicit param_type(_Ty _K0 = 1,
			double _P0 = 0.5)
			{	// construct from parameters
			_Init(_K0, _P0);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Kx == _Right._Kx && _Px == _Right._Px);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		_Ty k() const
			{	// return k value
			return (_Kx);
			}

		double p() const
			{	// return probability
			return (_Px);
			}

		void _Init(_Ty _K0, double _P0)
			{	// initialize
			_RNG_ASSERT(0.0 < _K0,
				"invalid max argument for "
					"negative_binomial_distribution");
			_RNG_ASSERT(0.0 < _P0 && _P0 <= 1.0,
				"invalid probability argument for "
					"negative_binomial_distribution");
			_Kx = _K0;
			_Px = _P0;
			}

		_Ty _Kx;
		double _Px;
		};

	explicit negative_binomial_distribution(_Ty _K0 = 1,
		double _P0 = 0.5)
		: _Par(_K0, _P0)
		{	// construct
		}

	explicit negative_binomial_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	_Ty k() const
		{	// return k value
		return (_Par.k());
		}

	double p() const
		{	// return probability
		return (_Par.p());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// get smallest possible result
		return (0);
		}

	result_type (max)() const
		{	// get largest possible result
		return ((numeric_limits<result_type>::max)());
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Ty _K0;
		double _P0;
		_In(_Istr, _P0);
		_In(_Istr, _K0);
		_Par._Init(_K0, _P0);
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Out(_Ostr, _Par._Px);
		_Out(_Ostr, _Par._Kx);
		return (_Ostr);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0) const
		{	// return pseudo-random value
		double _V1;
		gamma_distribution<double>
			dist1(_Par0._Kx, (1 - _Par0._Px) / (_Par0._Px));
		_V1 = dist1(_Eng);
		poisson_distribution<_Ty> dist2(_V1);

		return (dist2(_Eng));
		}

	param_type _Par;
	};

template<class _Ty>
	bool operator==(const negative_binomial_distribution<_Ty>& _Left,
		const negative_binomial_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const negative_binomial_distribution<_Ty>& _Left,
		const negative_binomial_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		negative_binomial_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const negative_binomial_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}

	// TEMPLATE CLASS discrete_distribution
template<class _Ty = int>
	class discrete_distribution
	{	// template class for discrete integer distribution
public:
	static_assert(_Is_IntType<_Ty>::value,
		"invalid template argument for discrete_distribution");

	typedef discrete_distribution<_Ty> _Myt;
	typedef vector<double> _Myvec;
	typedef _Ty result_type;

	struct param_type
		{	// parameter package
		typedef _Myt distribution_type;
		struct _Noinit
			{	// placeholder to suppress initialization
			};

		param_type(_Noinit)
			{	// do-nothing constructor for derived classes
			}

		param_type()
			{	// default constructor
			_Init();
			}

		template<class _InIt>
			param_type(_InIt _First, _InIt _Last)
			: _Pvec(_First, _Last)
			{	// construct from [_First, _Last)
			_Init();
			}

		param_type(_XSTD initializer_list<double> _Ilist)
			: _Pvec(_Ilist)
			{	// construct from initializer list
			_Init();
			}

		template<class _Fn>
			param_type(size_t _Count, double _Low, double _High,
				_Fn _Func)
			{	// construct from function
			double _Range = _High - _Low;
			_RNG_ASSERT(0.0 < _Range,
				"invalid range for discrete_distribution");
			if (_Count <= 0)
				_Count = 1;

			_Range /= (double)_Count;
			_Low += 0.5 * _Range;	// evaluate in center of each interval
			for (size_t _Idx = 0; _Idx < _Count; ++_Idx)
				_Pvec.push_back(_Func(_Low + _Idx * _Range));
			_Init();
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (_Pvec == _Right._Pvec);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		_Myvec probabilities() const
			{	// return probabilities
			return (_Pvec);
			}

		void _Init(bool _Renorm = true)
			{	// initialize
			size_t _Size = _Pvec.size();
			size_t _Idx;

			if (!_Renorm)
				;
			else if (_Pvec.empty())
				_Pvec.push_back(1.0);	// make empty vector degenerate
			else
				{	// normalize probabilities
				double _Sum = 0;

				for (_Idx = 0; _Idx < _Size; ++_Idx)
					{	// sum all probabilities
					_RNG_ASSERT(0.0 <= _Pvec[_Idx],
						"invalid probability for discrete_distribution");
					_Sum += _Pvec[_Idx];
					}

				_RNG_ASSERT(0.0 < _Sum,
					"invalid probability vector for discrete_distribution");
				if (_Sum != 1.0)
					for (_Idx = 0; _Idx < _Size; ++_Idx)
						_Pvec[_Idx] /= _Sum;
				}

			_Pcdf.assign(1, _Pvec[0]);
			for (_Idx = 1; _Idx < _Size; ++_Idx)
				_Pcdf.push_back(_Pvec[_Idx] + _Pcdf[_Idx - 1]);
			}

		_Myvec _Pvec;
		_Myvec _Pcdf;
		};

	discrete_distribution()
		{	// default constructor
		}

	template<class _InIt>
		discrete_distribution(_InIt _First, _InIt _Last)
		: _Par(_First, _Last)
		{	// construct from [_First, _Last)
		}

	discrete_distribution(_XSTD initializer_list<double> _Ilist)
		: _Par(_Ilist)
		{	// construct from initializer list
		}

	template<class _Fn>
		discrete_distribution(size_t _Count, double _Low, double _High,
			_Fn _Func)
		: _Par(_Count, _Low, _High, _Func)
		{	// construct from function
		}

	explicit discrete_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	_Myvec probabilities() const
		{	// return scaled probabilities vector
		return (_Par.probabilities());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// return minimum possible generated value
		return (0);
		}

	result_type (max)() const
		{	// return maximum possible generated value
		return ((result_type)(_Par._Pvec.size() - 1));
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr,
			param_type& _Par0)
		{	// read state from _Istr
		size_t _Nvals;
		_Istr >> _Nvals;
		_Par0._Pvec.clear();
		for (; 0 < _Nvals; --_Nvals)
			{	// get a value and add to vector
			double _Val;
			_In(_Istr, _Val);
			_Par0._Pvec.push_back(_Val);
			}
		_Par0._Init(false);	// don't renormalize, just compute CDF
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr,
			const param_type& _Par0) const
		{	// write state to _Ostr
		size_t _Nvals = _Par0._Pvec.size();
		_Ostr << ' ' << _Nvals;

		for (size_t _Idx = 0; _Idx < _Par0._Pvec.size(); ++_Idx)
			_Out(_Ostr, _Par0._Pvec[_Idx]);
		return (_Ostr);
		}

private:
	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value
		double _Px = _NRAND(_Eng, double);
		result_type _Count = (result_type)_Par0._Pcdf.size();
		result_type _First = 0;

		while (0 < _Count)
			{	// divide and conquer, find half that contains answer
			result_type _Count2 = _Count / 2;
			result_type _Mid = _First + _Count2;

			if (_Px <= _Par0._Pcdf[_Mid])
				_Count = _Count2;	// answer in first half, stay there
			else
				{	// answer in second half, move up
				_First = ++_Mid;
				_Count -= _Count2 + 1;
				}
			}
		return (_First);
		}

public:
	param_type _Par;
	};

template<class _Ty>
	bool operator==(const discrete_distribution<_Ty>& _Left,
		const discrete_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const discrete_distribution<_Ty>& _Left,
		const discrete_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		discrete_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr, _Dist._Par));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const discrete_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr, _Dist._Par));
	}

	// TEMPLATE CLASS piecewise_constant_distribution
template<class _Ty = double>
	class piecewise_constant_distribution
		: public discrete_distribution<size_t>
	{	// template class for piecewise constant floating-point distribution
public:
	static_assert(_Is_RealType<_Ty>::value,
		"invalid template argument for piecewise_constant_distribution");

	typedef piecewise_constant_distribution<_Ty> _Myt;
	typedef discrete_distribution<size_t> _Mybase;
	typedef typename _Mybase::param_type _Mypbase;
	typedef typename _Mypbase::_Noinit _Noinit;
	typedef _Ty result_type;

	struct param_type
		: public _Mypbase
		{	// parameter package
		typedef _Myt distribution_type;

		param_type()
			{	// default constructor
			_Bvec.push_back(_Ty(0));
			_Bvec.push_back(_Ty(1));
			}

		template<class _InIt1,
			class _InIt2>
			param_type(_InIt1 _First1, _InIt1 _Last1, _InIt2 _First2)
				: _Mypbase(_Noinit())
			{	// construct from [_First1, _Last1) and [_First2, ...)
			for (; _First1 != _Last1; ++_First1)
				_Bvec.push_back(*_First1);

			if (2 <= _Bvec.size())
				for (size_t _Idx = 0; _Idx < _Bvec.size() - 1; ++_Idx)
					this->_Pvec.push_back(*_First2++);
			_Init();
			}

		template<class _Fn>
			param_type(_XSTD initializer_list<_Ty> _Ilist,
				_Fn _Func)
				: _Mypbase(_Noinit())
			{	// construct from _Ilist and function
			if (2 <= _Ilist.size())
				_Bvec.assign(_Ilist);
			else
				{	// default construct
				_Bvec.push_back(_Ty(0));
				_Bvec.push_back(_Ty(1));
				}

			_Ty _Low = _Bvec.front();
			_Ty _Range = _Bvec.back() - _Low;
			size_t _Count = _Bvec.size() - 1;

			_Range /= (_Ty)_Count;
			for (size_t _Idx = 0; _Idx < _Count; ++_Idx)
				_Pvec.push_back(_Func(_Low + _Idx * _Range));
			_Init();
			}

		template<class _Fn>
			param_type(size_t _Count, _Ty _Low, _Ty _High, _Fn _Func)
				: _Mypbase(_Count, _Low, _High, _Func)
			{	// construct from value range and function
			_Ty _Range = _High - _Low;
			if (_Count <= 0)
				_Count = 1;

			_Range /= (_Ty)_Count;
			for (size_t _Idx = 0; _Idx <= _Count; ++_Idx)
				_Bvec.push_back(_Low + _Idx * _Range);
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (*(_Mypbase *)this == *(_Mypbase *)&_Right
				&& _Bvec == _Right._Bvec);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		vector<_Ty> intervals() const
			{	// return intervals vector
			return (_Bvec);
			}

		vector<_Ty> densities() const
			{	// return densities vector
			vector<_Ty> _Ans(this->_Pvec.begin(),
				this->_Pvec.end());

			for (size_t _Idx = 0; _Idx < _Ans.size(); ++_Idx)
				_Ans[_Idx] /= _Bvec[_Idx + 1] - _Bvec[_Idx];
			return (_Ans);
			}

		void _Init()
			{	// initialize
			((_Mypbase *)this)->_Init();
			}

		vector<_Ty> _Bvec;
		};

	piecewise_constant_distribution()
		{	// default constructor
		}

	template<class _InIt1,
		class _InIt2>
		piecewise_constant_distribution(_InIt1 _First1, _InIt1 _Last1,
			_InIt2 _First2)
			: _Par(_First1, _Last1, _First2)
		{	// construct from [_First1, _Last1) and [_First2, ...)
		}

	template<class _Fn>
		piecewise_constant_distribution(_XSTD initializer_list<_Ty> _Ilist,
			_Fn _Func)
			: _Par(_Ilist, _Func)
		{	// construct from _Ilist and function
		}

	template<class _Fn>
		piecewise_constant_distribution(size_t _Count,
			_Ty _Low, _Ty _High, _Fn _Func)
			: _Par(_Count, _Low, _High, _Func)
		{	// construct from value range and function
		}

	explicit piecewise_constant_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	vector<_Ty> intervals() const
		{	// return intervals vector
		return (_Par.intervals());
		}

	vector<_Ty> densities() const
		{	// return densities vector
		return (_Par.densities());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// return minimum possible generated value
		return (_Par._Bvec.front());
		}

	result_type (max)() const
		{	// return maximum possible generated value
		return (_Par._Bvec.back());
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr)
		{	// read state from _Istr
		_Mybase::_Read(_Istr, _Par);

		_Par._Bvec.clear();
		for (size_t _Idx = _Par._Pvec.size() + 1; 0 < _Idx; --_Idx)
			{	// get a value and add to intervals vector
			double _Val;
			_In(_Istr, _Val);
			_Par._Bvec.push_back(_Val);
			}
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Mybase::_Write(_Ostr, _Par);

		for (size_t _Idx = 0; _Idx < _Par._Bvec.size(); ++_Idx)
			_Out(_Ostr, _Par._Bvec[_Idx]);
		return (_Ostr);
		}

	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value
		size_t _Px = (*(_Mybase *)this)(_Eng, _Par0);
		uniform_real<_Ty> _Dist(_Par0._Bvec[_Px], _Par0._Bvec[_Px + 1]);
		return (_Dist(_Eng));
		}

	param_type _Par;
	};

template<class _Ty>
	bool operator==(const piecewise_constant_distribution<_Ty>& _Left,
		const piecewise_constant_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const piecewise_constant_distribution<_Ty>& _Left,
		const piecewise_constant_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		piecewise_constant_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const piecewise_constant_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}

	// TEMPLATE CLASS piecewise_linear_distribution
template<class _Ty = double>
	class piecewise_linear_distribution
		: public discrete_distribution<size_t>
	{	// template class for piecewise linear floating-point distribution
public:
	static_assert(_Is_RealType<_Ty>::value,
		"invalid template argument for piecewise_linear_distribution");

	typedef piecewise_linear_distribution<_Ty> _Myt;
	typedef discrete_distribution<size_t> _Mybase;
	typedef typename _Mybase::param_type _Mypbase;
	typedef typename _Mypbase::_Noinit _Noinit;
	typedef _Ty result_type;

	struct param_type
		: public _Mypbase
		{	// parameter package
		typedef _Myt distribution_type;

		param_type()
			{	// default constructor
			_Bvec.push_back(_Ty(0));
			_Bvec.push_back(_Ty(1));
			}

		template<class _InIt1,
			class _InIt2>
			param_type(_InIt1 _First1, _InIt1 _Last1, _InIt2 _First2)
				: _Mypbase(_Noinit())
			{	// construct from [_First1, _Last1) and [_First2, ...)
			for (; _First1 != _Last1; ++_First1)
				_Bvec.push_back(*_First1);

			if (2 <= _Bvec.size())
				for (size_t _Idx = 0; _Idx < _Bvec.size(); ++_Idx)
					this->_Pvec.push_back(*_First2++);
			_Init();
			}

		template<class _Fn>
			param_type(_XSTD initializer_list<_Ty> _Ilist,
				_Fn _Func)
				: _Mypbase(_Noinit())
			{	// construct from _Ilist and function
			if (2 <= _Ilist.size())
				_Bvec.assign(_Ilist);
			else
				{	// default construct
				_Bvec.push_back(_Ty(0));
				_Bvec.push_back(_Ty(1));
				}

			_Ty _Low = _Bvec.front();
			_Ty _Range = _Bvec.back() - _Low;
			size_t _Count = _Bvec.size();

			_Range /= (_Ty)_Count;
			for (size_t _Idx = 0; _Idx < _Count; ++_Idx)
				this->_Pvec.push_back(_Func(_Low + _Idx * _Range));
			_Init();
			}

		template<class _Fn>
			param_type(size_t _Count, _Ty _Low, _Ty _High, _Fn _Func)
				: _Mypbase(_Noinit())
			{	// construct from value range and function
			_Ty _Range = _High - _Low;
			_RNG_ASSERT(_Ty(0) < _Range,
				"invalid range for piecewise_linear_distribution");
			if (_Count < 2)
				_Count = 2;

			_Range /= (double)_Count;
			for (size_t _Idx = 0; _Idx < _Count; ++_Idx)
				{	// compute _Bvec and _Pvec
				_Ty _Bval = _Low + _Idx * _Range;
				_Bvec.push_back(_Bval);
				this->_Pvec.push_back(_Func(_Bval));
				}
			_Init();
			}

		bool operator==(const param_type& _Right) const
			{	// test for equality
			return (*(_Mypbase *)this == *(_Mypbase *)&_Right
				&& _Bvec == _Right._Bvec);
			}

		bool operator!=(const param_type& _Right) const
			{	// test for inequality
			return (!(*this == _Right));
			}

		vector<_Ty> intervals() const
			{	// return intervals vector
			return (_Bvec);
			}

		vector<_Ty> densities() const
			{	// return densities vector
			vector<_Ty> _Ans(this->_Pvec.begin(),
				this->_Pvec.end());
			return (_Ans);
			}

		void _Init(bool _Renorm = true)
			{	// initialize
			size_t _Size = this->_Pvec.size();
			size_t _Idx;

			if (!_Renorm)
				;
			else if (this->_Pvec.empty())
				this->_Pvec.push_back(1.0);	// make empty vector degenerate
			else
				{	// normalize probabilities
				double _Sum = 0;

				for (_Idx = 1; _Idx < _Size; ++_Idx)
					{	// sum all probabilities
					_RNG_ASSERT(0.0 <= this->_Pvec[_Idx],
						"invalid probability for "
							"piecewise_linear_distribution");
					_Sum += 0.5 * (this->_Pvec[_Idx - 1] + this->_Pvec[_Idx]);
					}

				_RNG_ASSERT(0.0 < _Sum,
					"invalid probability vector for "
						"piecewise_linear_distribution");
				if (_Sum != 1.0)
					for (_Idx = 0; _Idx < _Size; ++_Idx)
						this->_Pvec[_Idx] /= _Sum;
				}

			this->_Pcdf.assign(1, 0.5 * (this->_Pvec[0] + this->_Pvec[1]));
			for (_Idx = 2; _Idx < _Size; ++_Idx)
				this->_Pcdf.push_back(0.5
					* (this->_Pvec[_Idx - 1] + this->_Pvec[_Idx])
					+ this->_Pcdf[_Idx - 2]);
			}

		vector<_Ty> _Bvec;
		};

	piecewise_linear_distribution()
		{	// default constructor
		}

	template<class _InIt1,
		class _InIt2>
		piecewise_linear_distribution(_InIt1 _First1, _InIt1 _Last1,
			_InIt2 _First2)
			: _Par(_First1, _Last1, _First2)
		{	// construct from [_First1, _Last1) and [_First2, ...)
		}

	template<class _Fn>
		piecewise_linear_distribution(_XSTD initializer_list<_Ty> _Ilist,
			_Fn _Func)
			: _Par(_Ilist, _Func)
		{	// construct from _Ilist and function
		}

	template<class _Fn>
		piecewise_linear_distribution(size_t _Count,
			_Ty _Low, _Ty _High, _Fn _Func)
			: _Par(_Count, _Low, _High, _Func)
		{	// construct from value range and function
		}

	explicit piecewise_linear_distribution(const param_type& _Par0)
		: _Par(_Par0)
		{	// construct from parameter package
		}

	vector<_Ty> intervals() const
		{	// return intervals vector
		return (_Par.intervals());
		}

	vector<_Ty> densities() const
		{	// return densities vector
		return (_Par.densities());
		}

	param_type param() const
		{	// return parameter package
		return (_Par);
		}

	void param(const param_type& _Par0)
		{	// set parameter package
		_Par = _Par0;
		}

	result_type (min)() const
		{	// return minimum possible generated value
		return (_Par._Bvec.front());
		}

	result_type (max)() const
		{	// return maximum possible generated value
		return (_Par._Bvec.back());
		}

	void reset()
		{	// clear internal state
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng) const
		{	// return next value
		return (_Eval(_Eng, _Par));
		}

	template<class _Engine>
		result_type operator()(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value, given parameter package
		return (_Eval(_Eng, _Par0));
		}

	template<class _Elem,
		class _Traits>
		basic_istream<_Elem, _Traits>& _Read(
			basic_istream<_Elem, _Traits>& _Istr,
			param_type& _Par0)
		{	// read state from _Istr
		size_t _Nvals;
		_Istr >> _Nvals;
		_Par0._Pvec.clear();
		for (; 0 < _Nvals; --_Nvals)
			{	// get a value and add to vector
			double _Val;
			_In(_Istr, _Val);
			_Par0._Pvec.push_back(_Val);
			}
		_Par0._Init(false);	// don't renormalize, just compute CDF

		_Par._Bvec.clear();
		for (size_t _Idx = _Par._Pvec.size(); 0 < _Idx; --_Idx)
			{	// get a value and add to intervals vector
			double _Val;
			_In(_Istr, _Val);
			_Par._Bvec.push_back(_Val);
			}
		return (_Istr);
		}

	template<class _Elem,
		class _Traits>
		basic_ostream<_Elem, _Traits>& _Write(
			basic_ostream<_Elem, _Traits>& _Ostr) const
		{	// write state to _Ostr
		_Mybase::_Write(_Ostr, _Par);

		for (size_t _Idx = 0; _Idx < _Par._Bvec.size(); ++_Idx)
			_Out(_Ostr, _Par._Bvec[_Idx]);
		return (_Ostr);
		}

	template<class _Engine>
		result_type _Eval(_Engine& _Eng, const param_type& _Par0) const
		{	// return next value
		size_t _Px = (*(_Mybase *)this)(_Eng, _Par0);
		double _P0 = _Par0._Pvec[_Px];
		double _P1 = _Par0._Pvec[_Px + 1];
		uniform_real<_Ty> _Dist;
		result_type _X0 = _Dist(_Eng);

		if (_P0 != _P1)
			_X0 = (_STD sqrt(_P0 * _P0 * (1.0 - _X0)
				+ _P1 * _P1 * _X0) - _P0) / (_P1 - _P0);
		return (_Par0._Bvec[_Px]
			+ _X0 * (_Par0._Bvec[_Px + 1] - _Par0._Bvec[_Px]));
		}

	param_type _Par;
	};

template<class _Ty>
	bool operator==(const piecewise_linear_distribution<_Ty>& _Left,
		const piecewise_linear_distribution<_Ty>& _Right)
	{	// test for equality
	return (_Left.param() == _Right.param());
	}

template<class _Ty>
	bool operator!=(const piecewise_linear_distribution<_Ty>& _Left,
		const piecewise_linear_distribution<_Ty>& _Right)
	{	// test for inequality
	return (!(_Left == _Right));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_istream<_Elem, _Traits>& operator>>(
		basic_istream<_Elem, _Traits>& _Istr,
		piecewise_linear_distribution<_Ty>& _Dist)
	{	// read state from _Istr
	return (_Dist._Read(_Istr, _Dist._Par));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>& operator<<(
		basic_ostream<_Elem, _Traits>& _Ostr,
		const piecewise_linear_distribution<_Ty>& _Dist)
	{	// write state to _Ostr
	return (_Dist._Write(_Ostr));
	}

	// PREDEFINED GENERATORS
typedef linear_congruential_engine<unsigned int, 16807, 0, 2147483647>
	minstd_rand0;
typedef linear_congruential_engine<unsigned int, 48271, 0, 2147483647>
	minstd_rand;

typedef mersenne_twister_engine<unsigned int, 32, 624, 397, 31, 0x9908b0df,
	11, 0xffffffff, 7, 0x9d2c5680, 15, 0xefc60000, 18, 1812433253> mt19937;


typedef subtract_with_carry<unsigned int, 1 << 24, 10, 24> _Ranbase;
typedef discard_block<_Ranbase, 223, 24> ranlux3;
typedef discard_block<_Ranbase, 389, 24> ranlux4;

#if _HAS_TR1_NAMESPACE
typedef subtract_with_carry_01<float, 24, 10, 24> ranlux_base_01;
typedef subtract_with_carry_01<double, 48, 5, 12> ranlux64_base_01;
typedef discard_block<ranlux_base_01, 223, 24> ranlux3_01;
typedef discard_block<ranlux_base_01, 389, 24> ranlux4_01;
#endif /* _HAS_TR1_NAMESPACE */

typedef mersenne_twister_engine<_ULonglong, 64, 312, 156, 31,
	0xb5026f5aa96619e9ULL, 29,
	0x5555555555555555ULL, 17,
	0x71d67fffeda60000ULL, 37,
	0xfff7eee000000000ULL, 43,
	6364136223846793005ULL> mt19937_64;
typedef subtract_with_carry_engine<unsigned int, 24, 10, 24> ranlux24_base;
typedef subtract_with_carry_engine<_ULonglong, 48, 5, 12> ranlux48_base;
typedef discard_block_engine<ranlux24_base, 223, 23> ranlux24;
typedef discard_block_engine<ranlux48_base, 389, 11> ranlux48;
typedef shuffle_order_engine<minstd_rand0, 256> knuth_b;

typedef mt19937 default_random_engine;

	// CLASS random_device

_CRTIMP2_PURE unsigned int __CLRCALL_PURE_OR_CDECL _Random_device();

class random_device
	{	// class to generate random numbers (from hardware where available)
public:
	typedef unsigned int result_type;

	explicit random_device(const string& = "")
		{	// construct
		(*this)();	// force early failure if bad engine
		}

	static constexpr result_type (min)()
		{	// return minimum possible generated value
		return (0);
		}

	static constexpr result_type (max)()
		{	// return maximum possible generated value
		return ((result_type)-1);
		}

	double entropy() _NOEXCEPT
		{	// return entropy of random number source
		return (32.0);
		}

	result_type operator()()
		{	// return next value
		return (_Random_device());
		}

	random_device(const random_device&) = delete;
	random_device& operator=(const random_device&) = delete;
	};

#if _HAS_TR1_NAMESPACE
namespace tr1 {	// TR1 ADDITIONS
using _STD bernoulli_distribution;
using _STD binomial_distribution;
using _STD discard_block;
using _STD exponential_distribution;
using _STD gamma_distribution;
using _STD geometric_distribution;
using _STD linear_congruential;
using _STD mersenne_twister;
using _STD minstd_rand;
using _STD minstd_rand0;
using _STD mt19937;
using _STD normal_distribution;
using _STD poisson_distribution;
using _STD random_device;
using _STD ranlux3;
using _STD ranlux3_01;
using _STD ranlux4;
using _STD ranlux4_01;
using _STD ranlux64_base_01;
using _STD ranlux_base_01;
using _STD subtract_with_carry;
using _STD subtract_with_carry_01;
using _STD uniform_int;
using _STD uniform_real;
using _STD cauchy_distribution;
using _STD chi_squared_distribution;
using _STD default_random_engine;
using _STD discard_block_engine;
using _STD discrete_distribution;
using _STD extreme_value_distribution;
using _STD fisher_f_distribution;
using _STD generate_canonical;
using _STD independent_bits_engine;
using _STD knuth_b;
using _STD linear_congruential_engine;
using _STD lognormal_distribution;
using _STD mersenne_twister_engine;
using _STD mt19937_64;
using _STD negative_binomial_distribution;
using _STD piecewise_constant_distribution;
using _STD piecewise_linear_distribution;
using _STD ranlux24;
using _STD ranlux24_base;
using _STD ranlux48;
using _STD ranlux48_base;
using _STD seed_seq;
using _STD shuffle_order_engine;
using _STD student_t_distribution;
using _STD subtract_with_carry_engine;
using _STD uniform_int_distribution;
using _STD uniform_real_distribution;
using _STD weibull_distribution;
}	// namespace tr1
#endif /* _HAS_TR1_NAMESPACE */
_STD_END

#undef _NRAND

 #pragma pop_macro("new")
 #pragma warning(pop)
 #pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _RANDOM_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
