// xfunctional internal header (pre-TR1 functional)
#pragma once
#ifndef _XFUNCTIONAL_
#define _XFUNCTIONAL_
#ifndef RC_INVOKED
#include <cstdlib>
#include <xstring>
#include <xstddef>

 #pragma pack(push,_CRT_PACKING)
 #pragma warning(push,3)
 #pragma push_macro("new")
 #undef new

 #pragma warning(disable: 4100 4180 4244)

 #ifndef _XSTD2
  #define _XSTD2
 #endif /* _XSTD2 */

_STD_BEGIN
		// TEMPLATE STRUCT plus
// defined in <xstddef>

		// TEMPLATE STRUCT minus
// defined in <xstddef>

		// TEMPLATE STRUCT multiplies
// defined in <xstddef>

		// TEMPLATE STRUCT divides
template<class _Ty = void>
	struct divides
	{	// functor for operator/
	typedef _Ty first_argument_type;
	typedef _Ty second_argument_type;
	typedef _Ty result_type;

	constexpr _Ty operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator/ to operands
		return (_Left / _Right);
		}
	};

		// TEMPLATE STRUCT modulus
template<class _Ty = void>
	struct modulus
	{	// functor for operator%
	typedef _Ty first_argument_type;
	typedef _Ty second_argument_type;
	typedef _Ty result_type;

	constexpr _Ty operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator% to operands
		return (_Left % _Right);
		}
	};

		// TEMPLATE STRUCT negate
template<class _Ty = void>
	struct negate
	{	// functor for unary operator-
	typedef _Ty argument_type;
	typedef _Ty result_type;

	constexpr _Ty operator()(const _Ty& _Left) const
		{	// apply operator- to operand
		return (-_Left);
		}
	};

		// TEMPLATE STRUCT equal_to
// defined in <xstddef>

		// TEMPLATE STRUCT not_equal_to
template<class _Ty = void>
	struct not_equal_to
	{	// functor for operator!=
	typedef _Ty first_argument_type;
	typedef _Ty second_argument_type;
	typedef bool result_type;

	constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator!= to operands
		return (_Left != _Right);
		}
	};

		// TEMPLATE STRUCT greater
template<class _Ty = void>
	struct greater
	{	// functor for operator>
	typedef _Ty first_argument_type;
	typedef _Ty second_argument_type;
	typedef bool result_type;

	constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator> to operands
		return (_Left > _Right);
		}
	};

		// TEMPLATE STRUCT less
// defined in <xstddef>

		// TEMPLATE STRUCT greater_equal
template<class _Ty = void>
	struct greater_equal
	{	// functor for operator>=
	typedef _Ty first_argument_type;
	typedef _Ty second_argument_type;
	typedef bool result_type;

	constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator>= to operands
		return (_Left >= _Right);
		}
	};

		// TEMPLATE STRUCT less_equal
template<class _Ty = void>
	struct less_equal
	{	// functor for operator<=
	typedef _Ty first_argument_type;
	typedef _Ty second_argument_type;
	typedef bool result_type;

	constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator<= to operands
		return (_Left <= _Right);
		}
	};

		// TEMPLATE STRUCT logical_and
template<class _Ty = void>
	struct logical_and
	{	// functor for operator&&
	typedef _Ty first_argument_type;
	typedef _Ty second_argument_type;
	typedef bool result_type;

	constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator&& to operands
		return (_Left && _Right);
		}
	};

		// TEMPLATE STRUCT logical_or
template<class _Ty = void>
	struct logical_or
	{	// functor for operator||
	typedef _Ty first_argument_type;
	typedef _Ty second_argument_type;
	typedef bool result_type;

	constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator|| to operands
		return (_Left || _Right);
		}
	};

		// TEMPLATE STRUCT logical_not
template<class _Ty = void>
	struct logical_not
	{	// functor for unary operator!
	typedef _Ty argument_type;
	typedef bool result_type;

	constexpr bool operator()(const _Ty& _Left) const
		{	// apply operator! to operand
		return (!_Left);
		}
	};

		// TEMPLATE STRUCT bit_and
template<class _Ty = void>
	struct bit_and
	{	// functor for operator&
	typedef _Ty first_argument_type;
	typedef _Ty second_argument_type;
	typedef _Ty result_type;

	constexpr _Ty operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator& to operands
		return (_Left & _Right);
		}
	};

		// TEMPLATE STRUCT bit_or
template<class _Ty = void>
	struct bit_or
	{	// functor for operator|
	typedef _Ty first_argument_type;
	typedef _Ty second_argument_type;
	typedef _Ty result_type;

	constexpr _Ty operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator| to operands
		return (_Left | _Right);
		}
	};

		// TEMPLATE STRUCT bit_xor
template<class _Ty = void>
	struct bit_xor
	{	// functor for operator^
	typedef _Ty first_argument_type;
	typedef _Ty second_argument_type;
	typedef _Ty result_type;

	constexpr _Ty operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator^ to operands
		return (_Left ^ _Right);
		}
	};

		// TEMPLATE STRUCT bit_not
template<class _Ty = void>
	struct bit_not
	{	// functor for unary operator~
	typedef _Ty argument_type;
	typedef _Ty result_type;

	constexpr _Ty operator()(const _Ty& _Left) const
		{	// apply operator~ to operand
		return (~_Left);
		}
	};

		// TEMPLATE STRUCT SPECIALIZATION plus
// defined in <xstddef>

		// TEMPLATE STRUCT SPECIALIZATION minus
// defined in <xstddef>

		// TEMPLATE STRUCT SPECIALIZATION multiplies
// defined in <xstddef>

		// TEMPLATE STRUCT SPECIALIZATION divides
template<>
	struct divides<void>
	{	// transparent functor for operator/
	typedef int is_transparent;

	template<class _Ty1,
		class _Ty2>
		constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
		-> decltype(_STD forward<_Ty1>(_Left) / _STD forward<_Ty2>(_Right))
		{	// transparently apply operator/ to operands
		return (_STD forward<_Ty1>(_Left) / _STD forward<_Ty2>(_Right));
		}
	};

		// TEMPLATE STRUCT SPECIALIZATION modulus
template<>
	struct modulus<void>
	{	// transparent functor for operator%
	typedef int is_transparent;

	template<class _Ty1,
		class _Ty2>
		constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
		-> decltype(_STD forward<_Ty1>(_Left) % _STD forward<_Ty2>(_Right))
		{	// transparently apply operator% to operands
		return (_STD forward<_Ty1>(_Left) % _STD forward<_Ty2>(_Right));
		}
	};

		// TEMPLATE STRUCT SPECIALIZATION negate
template<>
	struct negate<void>
	{	// transparent functor for unary operator-
	typedef int is_transparent;

	template<class _Ty>
		constexpr auto operator()(_Ty&& _Left) const
		-> decltype(-_STD forward<_Ty>(_Left))
		{	// transparently apply operator- to operand
		return (-_STD forward<_Ty>(_Left));
		}
	};

		// TEMPLATE STRUCT SPECIALIZATION equal_to
// defined in <xstddef>

		// TEMPLATE STRUCT SPECIALIZATION not_equal_to
template<>
	struct not_equal_to<void>
	{	// transparent functor for operator!=
	typedef int is_transparent;

	template<class _Ty1,
		class _Ty2>
		constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
		-> decltype(_STD forward<_Ty1>(_Left) != _STD forward<_Ty2>(_Right))
		{	// transparently apply operator!= to operands
		return (_STD forward<_Ty1>(_Left) != _STD forward<_Ty2>(_Right));
		}
	};

		// TEMPLATE STRUCT SPECIALIZATION greater
template<>
	struct greater<void>
	{	// transparent functor for operator>
	typedef int is_transparent;

	template<class _Ty1,
		class _Ty2>
		constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
		-> decltype(_STD forward<_Ty1>(_Left) > _STD forward<_Ty2>(_Right))
		{	// transparently apply operator> to operands
		return (_STD forward<_Ty1>(_Left) > _STD forward<_Ty2>(_Right));
		}
	};

template<class _Obj1,
	class _Obj2,
	class _FTy> inline
	_Lex_compare_check_element_types<greater<int>, _Obj1, _Obj2, _FTy>
		_Lex_compare_memcmp_classify(_Obj1 * const&, _Obj2 * const&, const greater<_FTy>&)
	{	// return lex_compare optimization category for pointer iterators and greater<_FTy>
	return {};
	}

		// TEMPLATE STRUCT SPECIALIZATION less
// defined in <xstddef>

		// TEMPLATE STRUCT SPECIALIZATION greater_equal
template<>
	struct greater_equal<void>
	{	// transparent functor for operator>=
	typedef int is_transparent;

	template<class _Ty1,
		class _Ty2>
		constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
		-> decltype(_STD forward<_Ty1>(_Left) >= _STD forward<_Ty2>(_Right))
		{	// transparently apply operator>= to operands
		return (_STD forward<_Ty1>(_Left) >= _STD forward<_Ty2>(_Right));
		}
	};

		// TEMPLATE STRUCT SPECIALIZATION less_equal
template<>
	struct less_equal<void>
	{	// transparent functor for operator<=
	typedef int is_transparent;

	template<class _Ty1,
		class _Ty2>
		constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
		-> decltype(_STD forward<_Ty1>(_Left) <= _STD forward<_Ty2>(_Right))
		{	// transparently apply operator<= to operands
		return (_STD forward<_Ty1>(_Left) <= _STD forward<_Ty2>(_Right));
		}
	};

		// TEMPLATE STRUCT SPECIALIZATION logical_and
template<>
	struct logical_and<void>
	{	// transparent functor for operator&&
	typedef int is_transparent;

	template<class _Ty1,
		class _Ty2>
		constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
		-> decltype(_STD forward<_Ty1>(_Left) && _STD forward<_Ty2>(_Right))
		{	// transparently apply operator&& to operands
		return (_STD forward<_Ty1>(_Left) && _STD forward<_Ty2>(_Right));
		}
	};

		// TEMPLATE STRUCT SPECIALIZATION logical_or
template<>
	struct logical_or<void>
	{	// transparent functor for operator||
	typedef int is_transparent;

	template<class _Ty1,
		class _Ty2>
		constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
		-> decltype(_STD forward<_Ty1>(_Left) || _STD forward<_Ty2>(_Right))
		{	// transparently apply operator|| to operands
		return (_STD forward<_Ty1>(_Left) || _STD forward<_Ty2>(_Right));
		}
	};

		// TEMPLATE STRUCT SPECIALIZATION logical_not
template<>
	struct logical_not<void>
	{	// transparent functor for unary operator!
	typedef int is_transparent;

	template<class _Ty>
		constexpr auto operator()(_Ty&& _Left) const
		-> decltype(!_STD forward<_Ty>(_Left))
		{	// transparently apply operator! to operand
		return (!_STD forward<_Ty>(_Left));
		}
	};

		// TEMPLATE STRUCT SPECIALIZATION bit_and
template<>
	struct bit_and<void>
	{	// transparent functor for operator&
	typedef int is_transparent;

	template<class _Ty1,
		class _Ty2>
		constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
		-> decltype(_STD forward<_Ty1>(_Left) & _STD forward<_Ty2>(_Right))
		{	// transparently apply operator& to operands
		return (_STD forward<_Ty1>(_Left) & _STD forward<_Ty2>(_Right));
		}
	};

		// TEMPLATE STRUCT SPECIALIZATION bit_or
template<>
	struct bit_or<void>
	{	// transparent functor for operator|
	typedef int is_transparent;

	template<class _Ty1,
		class _Ty2>
		constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
		-> decltype(_STD forward<_Ty1>(_Left) | _STD forward<_Ty2>(_Right))
		{	// transparently apply operator| to operands
		return (_STD forward<_Ty1>(_Left) | _STD forward<_Ty2>(_Right));
		}
	};

		// TEMPLATE STRUCT SPECIALIZATION bit_xor
template<>
	struct bit_xor<void>
	{	// transparent functor for operator^
	typedef int is_transparent;

	template<class _Ty1,
		class _Ty2>
		constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
		-> decltype(_STD forward<_Ty1>(_Left) ^ _STD forward<_Ty2>(_Right))
		{	// transparently apply operator^ to operands
		return (_STD forward<_Ty1>(_Left) ^ _STD forward<_Ty2>(_Right));
		}
	};

		// TEMPLATE STRUCT SPECIALIZATION bit_not
template<>
	struct bit_not<void>
	{	// transparent functor for unary operator~
	typedef int is_transparent;

	template<class _Ty>
		constexpr auto operator()(_Ty&& _Left) const
		-> decltype(~_STD forward<_Ty>(_Left))
		{	// transparently apply operator~ to operand
		return (~_STD forward<_Ty>(_Left));
		}
	};

		// TEMPLATE CLASS unary_negate
template<class _Fn1>
	class unary_negate
	{	// functor adapter !_Func(left)
public:
	typedef typename _Fn1::argument_type argument_type;
	typedef bool result_type;

	constexpr explicit unary_negate(const _Fn1& _Func)
		: _Functor(_Func)
		{	// construct from functor
		}

	constexpr bool operator()(const argument_type& _Left) const
		{	// apply functor to operand
		return (!_Functor(_Left));
		}

private:
	_Fn1 _Functor;	// the functor to apply
	};

		// TEMPLATE FUNCTION not1
template<class _Fn1> inline
	constexpr unary_negate<_Fn1> not1(const _Fn1& _Func)
	{	// return a unary_negate functor adapter
	return (unary_negate<_Fn1>(_Func));
	}

		// TEMPLATE CLASS binary_negate
template<class _Fn2>
	class binary_negate
	{	// functor adapter !_Func(left, right)
public:
	typedef typename _Fn2::first_argument_type first_argument_type;
	typedef typename _Fn2::second_argument_type second_argument_type;
	typedef bool result_type;

	constexpr explicit binary_negate(const _Fn2& _Func)
		: _Functor(_Func)
		{	// construct from functor
		}

	constexpr bool operator()(const first_argument_type& _Left,
		const second_argument_type& _Right) const
		{	// apply functor to operands
		return (!_Functor(_Left, _Right));
		}

private:
	_Fn2 _Functor;	// the functor to apply
	};

		// TEMPLATE FUNCTION not2
template<class _Fn2> inline
	constexpr binary_negate<_Fn2> not2(const _Fn2& _Func)
	{	// return a binary_negate functor adapter
	return (binary_negate<_Fn2>(_Func));
	}

 #if _HAS_AUTO_PTR_ETC
		// TEMPLATE CLASS binder1st
template<class _Fn2>
	class binder1st
		: public unary_function<typename _Fn2::second_argument_type,
			typename _Fn2::result_type>
	{	// functor adapter _Func(stored, right)
public:
	typedef unary_function<typename _Fn2::second_argument_type,
		typename _Fn2::result_type> _Base;
	typedef typename _Base::argument_type argument_type;
	typedef typename _Base::result_type result_type;

	binder1st(const _Fn2& _Func,
		const typename _Fn2::first_argument_type& _Left)
		: op(_Func), value(_Left)
		{	// construct from functor and left operand
		}

	result_type operator()(const argument_type& _Right) const
		{	// apply functor to operands
		return (op(value, _Right));
		}

	result_type operator()(argument_type& _Right) const
		{	// apply functor to operands
		return (op(value, _Right));
		}

protected:
	_Fn2 op;	// the functor to apply
	typename _Fn2::first_argument_type value;	// the left operand
	};

		// TEMPLATE FUNCTION bind1st
template<class _Fn2,
	class _Ty> inline
	binder1st<_Fn2> bind1st(const _Fn2& _Func, const _Ty& _Left)
	{	// return a binder1st functor adapter
	typename _Fn2::first_argument_type _Val(_Left);
	return (binder1st<_Fn2>(_Func, _Val));
	}

		// TEMPLATE CLASS binder2nd
template<class _Fn2>
	class binder2nd
		: public unary_function<typename _Fn2::first_argument_type,
			typename _Fn2::result_type>
	{	// functor adapter _Func(left, stored)
public:
	typedef unary_function<typename _Fn2::first_argument_type,
		typename _Fn2::result_type> _Base;
	typedef typename _Base::argument_type argument_type;
	typedef typename _Base::result_type result_type;

	binder2nd(const _Fn2& _Func,
		const typename _Fn2::second_argument_type& _Right)
		: op(_Func), value(_Right)
		{	// construct from functor and right operand
		}

	result_type operator()(const argument_type& _Left) const
		{	// apply functor to operands
		return (op(_Left, value));
		}

	result_type operator()(argument_type& _Left) const
		{	// apply functor to operands
		return (op(_Left, value));
		}

protected:
	_Fn2 op;	// the functor to apply
	typename _Fn2::second_argument_type value;	// the right operand
	};

		// TEMPLATE FUNCTION bind2nd
template<class _Fn2,
	class _Ty> inline
	binder2nd<_Fn2> bind2nd(const _Fn2& _Func, const _Ty& _Right)
	{	// return a binder2nd functor adapter
	typename _Fn2::second_argument_type _Val(_Right);
	return (binder2nd<_Fn2>(_Func, _Val));
	}

		// TEMPLATE CLASS pointer_to_unary_function
template<class _Arg,
	class _Result,
	class _Fn = _Result (*)(_Arg)>
	class pointer_to_unary_function
		: public unary_function<_Arg, _Result>
	{	// functor adapter (*pfunc)(left)
public:
	explicit pointer_to_unary_function(_Fn _Left)
		: _Pfun(_Left)
		{	// construct from pointer
		}

	_Result operator()(_Arg _Left) const
		{	// call function with operand
		return (_Pfun(_Left));
		}

protected:
	_Fn _Pfun;	// the function pointer
	};

		// TEMPLATE CLASS pointer_to_binary_function
template<class _Arg1,
	class _Arg2,
	class _Result,
	class _Fn = _Result (*)(_Arg1, _Arg2)>
	class pointer_to_binary_function
		: public binary_function<_Arg1, _Arg2, _Result>
	{	// functor adapter (*pfunc)(left, right)
public:
	explicit pointer_to_binary_function(_Fn _Left)
		: _Pfun(_Left)
		{	// construct from pointer
		}

	_Result operator()(_Arg1 _Left, _Arg2 _Right) const
		{	// call function with operands
		return (_Pfun(_Left, _Right));
		}

protected:
	_Fn _Pfun;	// the function pointer
	};

		// TEMPLATE FUNCTION ptr_fun
template<class _Arg,
	class _Result> inline
	pointer_to_unary_function<_Arg, _Result,
		_Result (__cdecl *)(_Arg)>
		ptr_fun(_Result (__cdecl *_Left)(_Arg))
	{	// return pointer_to_unary_function functor adapter
	return (pointer_to_unary_function<_Arg, _Result,
		_Result (__cdecl *)(_Arg)>(_Left));
	}

 #ifdef _M_IX86
template<class _Arg,
	class _Result> inline
	pointer_to_unary_function<_Arg, _Result,
		_Result (__stdcall *)(_Arg)>
			ptr_fun(_Result (__stdcall *_Left)(_Arg))
	{	// return pointer_to_unary_function functor adapter
	return (pointer_to_unary_function<_Arg, _Result,
		_Result (__stdcall *)(_Arg)>(_Left));
	}

  #ifndef _M_CEE
template<class _Arg,
	class _Result> inline
	pointer_to_unary_function<_Arg, _Result,
		_Result (__fastcall *)(_Arg)>
			ptr_fun(_Result (__fastcall *_Left)(_Arg))
	{	// return pointer_to_unary_function functor adapter
	return (pointer_to_unary_function<_Arg, _Result,
		_Result (__fastcall *)(_Arg)>(_Left));
	}
  #endif /* _M_CEE */
 #endif /* _M_IX86 */

 #ifdef _M_CEE
template<class _Arg,
	class _Result> inline
	pointer_to_unary_function<_Arg, _Result,
		_Result (__clrcall *)(_Arg)>
			ptr_fun(_Result (__clrcall *_Left)(_Arg))
	{	// return pointer_to_unary_function functor adapter
	return (pointer_to_unary_function<_Arg, _Result,
		_Result (__clrcall *)(_Arg)>(_Left));
	}
 #endif /* _M_CEE */

template<class _Arg1,
	class _Arg2,
	class _Result> inline
	pointer_to_binary_function<_Arg1, _Arg2, _Result,
		_Result (__cdecl *)(_Arg1, _Arg2)>
		ptr_fun(_Result (__cdecl *_Left)(_Arg1, _Arg2))
	{	// return pointer_to_binary_function functor adapter
	return (pointer_to_binary_function<_Arg1, _Arg2, _Result,
		_Result (__cdecl *)(_Arg1, _Arg2)>(_Left));
	}

 #ifdef _M_IX86
template<class _Arg1,
	class _Arg2,
	class _Result> inline
	pointer_to_binary_function<_Arg1, _Arg2, _Result,
		_Result(__stdcall *)(_Arg1, _Arg2)>
			ptr_fun(_Result (__stdcall *_Left)(_Arg1, _Arg2))
	{	// return pointer_to_binary_function functor adapter
	return (pointer_to_binary_function<_Arg1, _Arg2, _Result,
		_Result (__stdcall *)(_Arg1, _Arg2)>(_Left));
	}

  #ifndef _M_CEE
template<class _Arg1,
	class _Arg2,
	class _Result> inline
	pointer_to_binary_function<_Arg1, _Arg2, _Result,
		_Result(__fastcall *)(_Arg1, _Arg2)>
			ptr_fun(_Result (__fastcall *_Left)(_Arg1, _Arg2))
	{	// return pointer_to_binary_function functor adapter
	return (pointer_to_binary_function<_Arg1, _Arg2, _Result,
		_Result (__fastcall *)(_Arg1, _Arg2)>(_Left));
	}
  #endif /* _M_CEE */
 #endif /* _M_IX86 */

 #ifdef _M_CEE
template<class _Arg1,
	class _Arg2,
	class _Result> inline
	pointer_to_binary_function<_Arg1, _Arg2, _Result,
		_Result(__clrcall *)(_Arg1, _Arg2)>
			ptr_fun(_Result (__clrcall *_Left)(_Arg1, _Arg2))
	{	// return pointer_to_binary_function functor adapter
	return (pointer_to_binary_function<_Arg1, _Arg2, _Result,
		_Result (__clrcall *)(_Arg1, _Arg2)>(_Left));
	}
 #endif /* _M_CEE */

		// TEMPLATE CLASS mem_fun_t
template<class _Result,
	class _Ty>
	class mem_fun_t
		: public unary_function<_Ty *, _Result>
	{	// functor adapter (*p->*pfunc)(), non-const *pfunc
public:
	explicit mem_fun_t(_Result (_Ty::*_Pm)())
		: _Pmemfun(_Pm)
		{	// construct from pointer
		}

	_Result operator()(_Ty *_Pleft) const
		{	// call function
		return ((_Pleft->*_Pmemfun)());
		}

private:
	_Result (_Ty::*_Pmemfun)();	// the member function pointer
	};

		// TEMPLATE CLASS mem_fun1_t
template<class _Result,
	class _Ty,
	class _Arg>
	class mem_fun1_t
		: public binary_function<_Ty *, _Arg, _Result>
	{	// functor adapter (*p->*pfunc)(val), non-const *pfunc
public:
	explicit mem_fun1_t(_Result (_Ty::*_Pm)(_Arg))
		: _Pmemfun(_Pm)
		{	// construct from pointer
		}

	_Result operator()(_Ty *_Pleft, _Arg _Right) const
		{	// call function with operand
		return ((_Pleft->*_Pmemfun)(_Right));
		}

private:
	_Result (_Ty::*_Pmemfun)(_Arg);	// the member function pointer
	};

		// TEMPLATE CLASS const_mem_fun_t
template<class _Result,
	class _Ty>
	class const_mem_fun_t
		: public unary_function<const _Ty *, _Result>
	{	// functor adapter (*p->*pfunc)(), const *pfunc
public:
	explicit const_mem_fun_t(_Result (_Ty::*_Pm)() const)
		: _Pmemfun(_Pm)
		{	// construct from pointer
		}

	_Result operator()(const _Ty *_Pleft) const
		{	// call function
		return ((_Pleft->*_Pmemfun)());
		}

private:
	_Result (_Ty::*_Pmemfun)() const;	// the member function pointer
	};

		// TEMPLATE CLASS const_mem_fun1_t
template<class _Result,
	class _Ty,
	class _Arg>
	class const_mem_fun1_t
		: public binary_function<const _Ty *, _Arg, _Result>
	{	// functor adapter (*p->*pfunc)(val), const *pfunc
public:
	explicit const_mem_fun1_t(_Result (_Ty::*_Pm)(_Arg) const)
		: _Pmemfun(_Pm)
		{	// construct from pointer
		}

	_Result operator()(const _Ty *_Pleft, _Arg _Right) const
		{	// call function with operand
		return ((_Pleft->*_Pmemfun)(_Right));
		}

private:
	_Result (_Ty::*_Pmemfun)(_Arg) const;	// the member function pointer
	};

		// TEMPLATE FUNCTION mem_fun
template<class _Result,
	class _Ty> inline
	mem_fun_t<_Result, _Ty> mem_fun(_Result (_Ty::*_Pm)())
	{	// return a mem_fun_t functor adapter
	return (mem_fun_t<_Result, _Ty>(_Pm));
	}

template<class _Result,
	class _Ty,
	class _Arg> inline
	mem_fun1_t<_Result, _Ty, _Arg> mem_fun(_Result (_Ty::*_Pm)(_Arg))
	{	// return a mem_fun1_t functor adapter
	return (mem_fun1_t<_Result, _Ty, _Arg>(_Pm));
	}

template<class _Result,
	class _Ty> inline
	const_mem_fun_t<_Result, _Ty>
		mem_fun(_Result (_Ty::*_Pm)() const)
	{	// return a const_mem_fun_t functor adapter
	return (const_mem_fun_t<_Result, _Ty>(_Pm));
	}

template<class _Result,
	class _Ty,
	class _Arg> inline
	const_mem_fun1_t<_Result, _Ty, _Arg>
		mem_fun(_Result (_Ty::*_Pm)(_Arg) const)
	{	// return a const_mem_fun1_t functor adapter
	return (const_mem_fun1_t<_Result, _Ty, _Arg>(_Pm));
	}

		// TEMPLATE CLASS mem_fun_ref_t
template<class _Result,
	class _Ty>
	class mem_fun_ref_t
		: public unary_function<_Ty, _Result>
	{	// functor adapter (*left.*pfunc)(), non-const *pfunc
public:
	explicit mem_fun_ref_t(_Result (_Ty::*_Pm)())
		: _Pmemfun(_Pm)
		{	// construct from pointer
		}

	_Result operator()(_Ty& _Left) const
		{	// call function
		return ((_Left.*_Pmemfun)());
		}

private:
	_Result (_Ty::*_Pmemfun)();	// the member function pointer
	};

		// TEMPLATE CLASS mem_fun1_ref_t
template<class _Result,
	class _Ty,
	class _Arg>
	class mem_fun1_ref_t
		: public binary_function<_Ty, _Arg, _Result>
	{	// functor adapter (*left.*pfunc)(val), non-const *pfunc
public:
	explicit mem_fun1_ref_t(_Result (_Ty::*_Pm)(_Arg))
		: _Pmemfun(_Pm)
		{	// construct from pointer
		}

	_Result operator()(_Ty& _Left, _Arg _Right) const
		{	// call function with operand
		return ((_Left.*_Pmemfun)(_Right));
		}

private:
	_Result (_Ty::*_Pmemfun)(_Arg);	// the member function pointer
	};

		// TEMPLATE CLASS const_mem_fun_ref_t
template<class _Result,
	class _Ty>
	class const_mem_fun_ref_t
		: public unary_function<_Ty, _Result>
	{	// functor adapter (*left.*pfunc)(), const *pfunc
public:
	explicit const_mem_fun_ref_t(_Result (_Ty::*_Pm)() const)
		: _Pmemfun(_Pm)
		{	// construct from pointer
		}

	_Result operator()(const _Ty& _Left) const
		{	// call function
		return ((_Left.*_Pmemfun)());
		}

private:
	_Result (_Ty::*_Pmemfun)() const;	// the member function pointer
	};

		// TEMPLATE CLASS const_mem_fun1_ref_t
template<class _Result,
	class _Ty,
	class _Arg>
	class const_mem_fun1_ref_t
		: public binary_function<_Ty, _Arg, _Result>
	{	// functor adapter (*left.*pfunc)(val), const *pfunc
public:
	explicit const_mem_fun1_ref_t(_Result (_Ty::*_Pm)(_Arg) const)
		: _Pmemfun(_Pm)
		{	// construct from pointer
		}

	_Result operator()(const _Ty& _Left, _Arg _Right) const
		{	// call function with operand
		return ((_Left.*_Pmemfun)(_Right));
		}

private:
	_Result (_Ty::*_Pmemfun)(_Arg) const;	// the member function pointer
	};

		// TEMPLATE FUNCTION mem_fun_ref
template<class _Result,
	class _Ty> inline
	mem_fun_ref_t<_Result, _Ty> mem_fun_ref(_Result (_Ty::*_Pm)())
	{	// return a mem_fun_ref_t functor adapter
	return (mem_fun_ref_t<_Result, _Ty>(_Pm));
	}

template<class _Result,
	class _Ty,
	class _Arg> inline
	mem_fun1_ref_t<_Result, _Ty, _Arg>
		mem_fun_ref(_Result (_Ty::*_Pm)(_Arg))
	{	// return a mem_fun1_ref_t functor adapter
	return (mem_fun1_ref_t<_Result, _Ty, _Arg>(_Pm));
	}

template<class _Result,
	class _Ty> inline
	const_mem_fun_ref_t<_Result, _Ty>
		mem_fun_ref(_Result (_Ty::*_Pm)() const)
	{	// return a const_mem_fun_ref_t functor adapter
	return (const_mem_fun_ref_t<_Result, _Ty>(_Pm));
	}

template<class _Result,
	class _Ty,
	class _Arg> inline
	const_mem_fun1_ref_t<_Result, _Ty, _Arg>
		mem_fun_ref(_Result (_Ty::*_Pm)(_Arg) const)
	{	// return a const_mem_fun1_ref_t functor adapter
	return (const_mem_fun1_ref_t<_Result, _Ty, _Arg>(_Pm));
	}
 #endif /* _HAS_AUTO_PTR_ETC */
_STD_END

 #pragma pop_macro("new")
 #pragma warning(pop)
 #pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _XFUNCTIONAL_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
