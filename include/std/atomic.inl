// atomic standard header
#pragma once
#ifndef _ATOMIC_
#define _ATOMIC_
#ifndef RC_INVOKED
#include <yvals.h>

 #ifdef _M_CEE_PURE
  #error <atomic> is not supported when compiling with /clr:pure.
 #endif /* _M_CEE_PURE */

 #include <type_traits>


#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <xatomic.h>

 #pragma pack(push,_CRT_PACKING)
 #pragma warning(push,3)
 #pragma push_macro("new")
 #undef new

 #pragma warning(disable: 4522 4800)

		// LOCK-FREE PROPERTY
#define ATOMIC_CHAR_LOCK_FREE		_ATOMIC_CHAR_LOCK_FREE
#define ATOMIC_CHAR16_T_LOCK_FREE	_ATOMIC_CHAR16_T_LOCK_FREE
#define ATOMIC_CHAR32_T_LOCK_FREE	_ATOMIC_CHAR32_T_LOCK_FREE
#define ATOMIC_WCHAR_T_LOCK_FREE	_ATOMIC_WCHAR_T_LOCK_FREE
#define ATOMIC_SHORT_LOCK_FREE		_ATOMIC_SHORT_LOCK_FREE
#define ATOMIC_INT_LOCK_FREE		_ATOMIC_INT_LOCK_FREE
#define ATOMIC_LONG_LOCK_FREE		_ATOMIC_LONG_LOCK_FREE
#define ATOMIC_LLONG_LOCK_FREE		_ATOMIC_LLONG_LOCK_FREE

_STD_BEGIN
inline memory_order _Get_memory_order(memory_order _Order)
	{	// get second memory_order argument for cas functions
		// that take only one memory_order argument
	return (_Order == memory_order_acq_rel ? memory_order_acquire
		: _Order == memory_order_release ? memory_order_relaxed
		: _Order);
	}

		// TEMPLATE FUNCTION kill_dependency
template<class _Ty>
	_Ty kill_dependency(_Ty _Arg) _NOEXCEPT
	{	// magic template that kills dependency ordering when called
	return (_Arg);
	}

		// GENERAL OPERATIONS ON ATOMIC TYPES (FORWARD DECLARATIONS)
template <class _Ty>
	struct atomic;
template <class _Ty>
	bool atomic_is_lock_free(const volatile atomic<_Ty> *) _NOEXCEPT;
template <class _Ty>
	bool atomic_is_lock_free(const atomic<_Ty> *) _NOEXCEPT;
template <class _Ty>
	void atomic_init(volatile atomic<_Ty> *, _Ty) _NOEXCEPT;
template <class _Ty>
	void atomic_init(atomic<_Ty> *, _Ty) _NOEXCEPT;
template <class _Ty>
	void atomic_store(volatile atomic<_Ty> *, _Ty) _NOEXCEPT;
template <class _Ty>
	void atomic_store(atomic<_Ty> *, _Ty) _NOEXCEPT;
template <class _Ty>
	void atomic_store_explicit(volatile atomic<_Ty> *, _Ty,
		memory_order) _NOEXCEPT;
template <class _Ty>
	void atomic_store_explicit(atomic<_Ty> *, _Ty,
		memory_order) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_load(const volatile atomic<_Ty> *) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_load(const atomic<_Ty> *) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_load_explicit(const volatile atomic<_Ty> *,
		memory_order) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_load_explicit(const atomic<_Ty> *,
		memory_order) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_exchange(volatile atomic<_Ty> *, _Ty) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_exchange(atomic<_Ty> *, _Ty) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_exchange_explicit(volatile atomic<_Ty> *, _Ty,
		memory_order) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_exchange_explicit(atomic<_Ty> *, _Ty,
		memory_order) _NOEXCEPT;
template <class _Ty>
	bool atomic_compare_exchange_weak(volatile atomic<_Ty> *,
		_Ty *, _Ty) _NOEXCEPT;
template <class _Ty>
	bool atomic_compare_exchange_weak(atomic<_Ty> *,
		_Ty *, _Ty) _NOEXCEPT;
template <class _Ty>
	bool atomic_compare_exchange_weak_explicit(
		volatile atomic<_Ty> *, _Ty *, _Ty,
			memory_order, memory_order) _NOEXCEPT;
template <class _Ty>
	bool atomic_compare_exchange_weak_explicit(
		atomic<_Ty> *, _Ty *, _Ty,
			memory_order, memory_order) _NOEXCEPT;
template <class _Ty>
	bool atomic_compare_exchange_strong(volatile atomic<_Ty> *,
		_Ty *, _Ty) _NOEXCEPT;
template <class _Ty>
	bool atomic_compare_exchange_strong(atomic<_Ty> *,
		_Ty *, _Ty) _NOEXCEPT;
template <class _Ty>
	bool atomic_compare_exchange_strong_explicit(
		volatile atomic<_Ty> *, _Ty *, _Ty,
			memory_order, memory_order) _NOEXCEPT;
template <class _Ty>
	bool atomic_compare_exchange_strong_explicit(
		atomic<_Ty> *, _Ty *, _Ty,
			memory_order, memory_order) _NOEXCEPT;

		// TEMPLATED OPERATIONS ON ATOMIC TYPES (DECLARED BUT NOT DEFINED)
template <class _Ty>
	_Ty atomic_fetch_add(volatile atomic<_Ty>*, _Ty) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_add(atomic<_Ty>*, _Ty) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_add_explicit(volatile atomic<_Ty>*, _Ty,
		memory_order) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_add_explicit(atomic<_Ty>*, _Ty,
		memory_order) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_sub(volatile atomic<_Ty>*, _Ty) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_sub(atomic<_Ty>*, _Ty) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_sub_explicit(volatile atomic<_Ty>*, _Ty,
		memory_order) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_sub_explicit(atomic<_Ty>*, _Ty,
		memory_order) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_and(volatile atomic<_Ty>*, _Ty) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_and(atomic<_Ty>*, _Ty) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_and_explicit(volatile atomic<_Ty>*, _Ty,
		memory_order) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_and_explicit(atomic<_Ty>*, _Ty,
		memory_order) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_or(volatile atomic<_Ty>*, _Ty) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_or(atomic<_Ty>*, _Ty) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_or_explicit(volatile atomic<_Ty>*, _Ty,
		memory_order) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_or_explicit(atomic<_Ty>*, _Ty,
		memory_order) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_xor(volatile atomic<_Ty>*, _Ty) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_xor(atomic<_Ty>*, _Ty) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_xor_explicit(volatile atomic<_Ty>*, _Ty,
		memory_order) _NOEXCEPT;
template <class _Ty>
	_Ty atomic_fetch_xor_explicit(atomic<_Ty>*, _Ty,
		memory_order) _NOEXCEPT;

		// STRUCT atomic_flag
#define ATOMIC_FLAG_INIT	{0}
typedef struct atomic_flag
	{	// structure for managing flag with test-and-set semantics
	bool test_and_set(memory_order _Order = memory_order_seq_cst)
		volatile _NOEXCEPT;
	bool test_and_set(memory_order _Order = memory_order_seq_cst) _NOEXCEPT;
	void clear(memory_order _Order = memory_order_seq_cst)
		volatile _NOEXCEPT;
	void clear(memory_order _Order = memory_order_seq_cst) _NOEXCEPT;

	_Atomic_flag_t _My_flag;

	atomic_flag() _NOEXCEPT = default;
	atomic_flag(const atomic_flag&) = delete;
	atomic_flag& operator=(const atomic_flag&) = delete;
	atomic_flag& operator=(const atomic_flag&) volatile = delete;
	} atomic_flag;

inline bool atomic_flag::test_and_set(memory_order _Order)
		volatile _NOEXCEPT
	{	// atomically set *this to true and return previous value
	return (_ATOMIC_FLAG_TEST_AND_SET(&_My_flag, _Order));
	}

inline bool atomic_flag::test_and_set(memory_order _Order) _NOEXCEPT
	{	// atomically set *this to true and return previous value
	return (_ATOMIC_FLAG_TEST_AND_SET(&_My_flag, _Order));
	}

inline void atomic_flag::clear(memory_order _Order)
	volatile _NOEXCEPT
	{	// atomically clear *this
	_ATOMIC_FLAG_CLEAR(&_My_flag, _Order);
	}

inline void atomic_flag::clear(memory_order _Order) _NOEXCEPT
	{	// atomically clear *this
	_ATOMIC_FLAG_CLEAR(&_My_flag, _Order);
	}

inline bool atomic_flag_test_and_set(volatile atomic_flag *_Flag) _NOEXCEPT
	{	// atomically set *_Flag to true and return previous value
	return (_ATOMIC_FLAG_TEST_AND_SET(&_Flag->_My_flag, memory_order_seq_cst));
	}

inline bool atomic_flag_test_and_set(atomic_flag *_Flag) _NOEXCEPT
	{	// atomically set *_Flag to true and return previous value
	return (_ATOMIC_FLAG_TEST_AND_SET(&_Flag->_My_flag, memory_order_seq_cst));
	}

inline bool atomic_flag_test_and_set_explicit(
	volatile atomic_flag *_Flag, memory_order _Order) _NOEXCEPT
	{	// atomically set *_Flag to true and return previous value
	return (_ATOMIC_FLAG_TEST_AND_SET(&_Flag->_My_flag, _Order));
	}

inline bool atomic_flag_test_and_set_explicit(
	atomic_flag *_Flag, memory_order _Order) _NOEXCEPT
	{	// atomically set *_Flag to true and return previous value
	return (_ATOMIC_FLAG_TEST_AND_SET(&_Flag->_My_flag, _Order));
	}

inline void atomic_flag_clear(volatile atomic_flag *_Flag) _NOEXCEPT
	{	// atomically clear *_Flag
	_ATOMIC_FLAG_CLEAR(&_Flag->_My_flag, memory_order_seq_cst);
	}

inline void atomic_flag_clear(atomic_flag *_Flag) _NOEXCEPT
	{	// atomically clear *_Flag
	_ATOMIC_FLAG_CLEAR(&_Flag->_My_flag, memory_order_seq_cst);
	}

inline void atomic_flag_clear_explicit(
	volatile atomic_flag *_Flag, memory_order _Order) _NOEXCEPT
	{	// atomically clear *_Flag
	_ATOMIC_FLAG_CLEAR(&_Flag->_My_flag, _Order);
	}

inline void atomic_flag_clear_explicit(
	atomic_flag *_Flag, memory_order _Order) _NOEXCEPT
	{	// atomically clear *_Flag
	_ATOMIC_FLAG_CLEAR(&_Flag->_My_flag, _Order);
	}

		// TEMPLATE CLASS _Atomic_impl
template<unsigned _Bytes>
	struct _Atomic_impl
	{	// struct for managing locks around operations on atomic types
	typedef _Uint1_t _My_int;	// "1 byte" means "no alignment required"

	constexpr _Atomic_impl() _NOEXCEPT
		: _My_flag(0)
		{	// default constructor
		}

	bool _Is_lock_free() const volatile
		{	// operations that use locks are not lock-free
		return (false);
		}

	void _Store(void *_Tgt, const void *_Src, memory_order _Order) volatile
		{	// lock and store
		_Atomic_copy(&_My_flag, _Bytes, _Tgt, _Src, _Order);
		}

	void _Load(void *_Tgt, const void *_Src,
		memory_order _Order) const volatile
		{	// lock and load
		_Atomic_copy(&_My_flag, _Bytes, _Tgt, _Src, _Order);
		}

	void _Exchange(void *_Left, void *_Right, memory_order _Order) volatile
		{	// lock and exchange
		_Atomic_exchange(&_My_flag, _Bytes, _Left, _Right, _Order);
		}

	bool _Compare_exchange_weak(
		void *_Tgt, void *_Exp, const void *_Value,
		memory_order _Order1, memory_order _Order2) volatile
		{	// lock and compare/exchange
		return (_Atomic_compare_exchange_weak(
			&_My_flag, _Bytes, _Tgt, _Exp, _Value, _Order1, _Order2));
		}

	bool _Compare_exchange_strong(
		void *_Tgt, void *_Exp, const void *_Value,
		memory_order _Order1, memory_order _Order2) volatile
		{	// lock and compare/exchange
		return (_Atomic_compare_exchange_strong(
			&_My_flag, _Bytes, _Tgt, _Exp, _Value, _Order1, _Order2));
		}

private:
	mutable _Atomic_flag_t _My_flag;
	};

		// SPECIALIZATIONS OF _Atomic_impl

template<>
	struct _Atomic_impl<1U>
	{	// struct for managing lock-free operations on 1-byte atomic types
	typedef _Uint1_t _My_int;

	bool _Is_lock_free() const volatile
		{	// all operations are lock-free
		return (_Atomic_is_lock_free_1());
		}

	void _Store(void *_Tgt, const void *_Src, memory_order _Order) volatile
		{	// store
		_Atomic_store_1((_My_int*)_Tgt, *(_My_int*)_Src, _Order);
		}

	void _Load(void *_Tgt, const void *_Src,
		memory_order _Order) const volatile
		{	// load
		*(_My_int*)_Tgt = _Atomic_load_1((_My_int*)_Src, _Order);
		}

	void _Exchange(void *_Left, void *_Right, memory_order _Order) volatile
		{	// exchange
		*(_My_int*)_Right =
			_Atomic_exchange_1((_My_int*)_Left, *(_My_int*)_Right, _Order);
		}

	bool _Compare_exchange_weak(
		void *_Tgt, void *_Exp, const void *_Value,
		memory_order _Order1, memory_order _Order2) volatile
		{	// compare/exchange
		return (_Atomic_compare_exchange_weak_1(
			(_My_int*)_Tgt, (_My_int*)_Exp, *(_My_int*)_Value,
			_Order1, _Order2));
		}

	bool _Compare_exchange_strong(
		void *_Tgt, void *_Exp, const void *_Value,
		memory_order _Order1, memory_order _Order2) volatile
		{	// compare/exchange
		return (_Atomic_compare_exchange_strong_1(
			(_My_int*)_Tgt, (_My_int*)_Exp, *(_My_int*)_Value,
			_Order1, _Order2));
		}
	};

template<>
	struct _Atomic_impl<2U>
	{	// struct for managing lock-free operations on 2-byte atomic types
	typedef _Uint2_t _My_int;

	bool _Is_lock_free() const volatile
		{	// all operations are lock-free
		return (_Atomic_is_lock_free_2());
		}

	void _Store(void *_Tgt, const void *_Src, memory_order _Order) volatile
		{	// store
		_Atomic_store_2((_My_int*)_Tgt, *(_My_int*)_Src, _Order);
		}

	void _Load(void *_Tgt, const void *_Src,
		memory_order _Order) const volatile
		{	// load
		*(_My_int*)_Tgt = _Atomic_load_2((_My_int*)_Src, _Order);
		}

	void _Exchange(void *_Left, void *_Right, memory_order _Order) volatile
		{	// exchange
		*(_My_int*)_Right =
			_Atomic_exchange_2((_My_int*)_Left, *(_My_int*)_Right, _Order);
		}

	bool _Compare_exchange_weak(
		void *_Tgt, void *_Exp, const void *_Value,
		memory_order _Order1, memory_order _Order2) volatile
		{	// compare/exchange
		return (_Atomic_compare_exchange_weak_2(
			(_My_int*)_Tgt, (_My_int*)_Exp, *(_My_int*)_Value,
			_Order1, _Order2));
		}

	bool _Compare_exchange_strong(
		void *_Tgt, void *_Exp, const void *_Value,
		memory_order _Order1, memory_order _Order2) volatile
		{	// compare/exchange
		return (_Atomic_compare_exchange_strong_2(
			(_My_int*)_Tgt, (_My_int*)_Exp, *(_My_int*)_Value,
			_Order1, _Order2));
		}
	};

template<>
	struct _Atomic_impl<4U>
	{	// struct for managing lock-free operations on 4-byte atomic types
	typedef _Uint4_t _My_int;

	bool _Is_lock_free() const volatile
		{	// all operations are lock-free
		return (_Atomic_is_lock_free_4());
		}

	void _Store(void *_Tgt, const void *_Src, memory_order _Order) volatile
		{	// store
		_Atomic_store_4((_My_int*)_Tgt, *(_My_int*)_Src, _Order);
		}

	void _Load(void *_Tgt, const void *_Src,
		memory_order _Order) const volatile
		{	// load
		*(_My_int*)_Tgt = _Atomic_load_4((_My_int*)_Src, _Order);
		}

	void _Exchange(void *_Left, void *_Right, memory_order _Order) volatile
		{	// exchange
		*(_My_int*)_Right =
			_Atomic_exchange_4((_My_int*)_Left, *(_My_int*)_Right, _Order);
		}

	bool _Compare_exchange_weak(
		void *_Tgt, void *_Exp, const void *_Value,
		memory_order _Order1, memory_order _Order2) volatile
		{	// compare/exchange
		return (_Atomic_compare_exchange_weak_4(
			(_My_int*)_Tgt, (_My_int*)_Exp, *(_My_int*)_Value,
			_Order1, _Order2));
		}

	bool _Compare_exchange_strong(
		void *_Tgt, void *_Exp, const void *_Value,
		memory_order _Order1, memory_order _Order2) volatile
		{	// compare/exchange
		return (_Atomic_compare_exchange_strong_4(
			(_My_int*)_Tgt, (_My_int*)_Exp, *(_My_int*)_Value,
			_Order1, _Order2));
		}
	};

template<>
	struct _Atomic_impl<8U>
	{	// struct for managing lock-free operations on 8-byte atomic types
	typedef _Uint8_t _My_int;

	bool _Is_lock_free() const volatile
		{	// all operations are lock-free
		return (_Atomic_is_lock_free_8());
		}

	void _Store(void *_Tgt, const void *_Src, memory_order _Order) volatile
		{	// store
		_Atomic_store_8((_My_int*)_Tgt, *(_My_int*)_Src, _Order);
		}

	void _Load(void *_Tgt, const void *_Src,
		memory_order _Order) const volatile
		{	// load
		*(_My_int*)_Tgt = _Atomic_load_8((_My_int*)_Src, _Order);
		}

	void _Exchange(void *_Left, void *_Right, memory_order _Order) volatile
		{	// exchange
		*(_My_int*)_Right =
			_Atomic_exchange_8((_My_int*)_Left, *(_My_int*)_Right, _Order);
		}

	bool _Compare_exchange_weak(
		void *_Tgt, void *_Exp, const void *_Value,
		memory_order _Order1, memory_order _Order2) volatile
		{	// compare/exchange
		return (_Atomic_compare_exchange_weak_8(
			(_My_int*)_Tgt, (_My_int*)_Exp, *(_My_int*)_Value,
			_Order1, _Order2));
		}

	bool _Compare_exchange_strong(
		void *_Tgt, void *_Exp, const void *_Value,
		memory_order _Order1, memory_order _Order2) volatile
		{	// compare/exchange
		return (_Atomic_compare_exchange_strong_8(
			(_My_int*)_Tgt, (_My_int*)_Exp, *(_My_int*)_Value,
			_Order1, _Order2));
		}
	};

		// TEMPLATE CLASS _Atomic_base
template<class _Ty,
	unsigned _Bytes>
	struct _Atomic_base
		: _Atomic_impl<_Bytes>
	{	// struct that defines most member functions of std::atomic
	typedef _Atomic_impl<_Bytes> _Mybase;
	typedef typename _Mybase::_My_int _My_int;

	constexpr _Atomic_base(_Ty _Val) _NOEXCEPT
		: _My_val(_Val)
		{	// construct from _Val, initialization is not an atomic operation
		}

	_Atomic_base() _NOEXCEPT = default;
	_Atomic_base(const _Atomic_base&) = delete;
	_Atomic_base& operator=(const _Atomic_base&) = delete;
	_Atomic_base& operator=(const _Atomic_base&) volatile = delete;

	_Ty operator=(_Ty _Right) volatile _NOEXCEPT
		{	// assign from _Right
		this->_Store((void *)&_My_val, &_Right, memory_order_seq_cst);
		return (_Right);
		}

	_Ty operator=(_Ty _Right) _NOEXCEPT
		{	// assign from _Right
		this->_Store((void *)&_My_val, &_Right, memory_order_seq_cst);
		return (_Right);
		}

	bool is_lock_free() const volatile _NOEXCEPT
		{	// return true if operations are lock-free
		return (this->_Is_lock_free());
		}

	bool is_lock_free() const _NOEXCEPT
		{	// return true if operations are lock-free
		return (this->_Is_lock_free());
		}

	void store(_Ty _Value,
		memory_order _Order = memory_order_seq_cst) volatile _NOEXCEPT
		{	// store _Value into *this
		this->_Store((void *)&_My_val, &_Value, _Order);
		}

	void store(_Ty _Value,
		memory_order _Order = memory_order_seq_cst) _NOEXCEPT
		{	// store _Value into *this
		this->_Store((void *)&_My_val, &_Value, _Order);
		}

	_Ty load(memory_order _Order = memory_order_seq_cst)
		const volatile _NOEXCEPT
		{	// return value stored in *this
		_Ty _Result;
		this->_Load((void *)&_Result, (const void *)&_My_val, _Order);
		return (_Result);
		}

	_Ty load(memory_order _Order = memory_order_seq_cst)
		const _NOEXCEPT
		{	// return value stored in *this
		_Ty _Result;
		this->_Load((void *)&_Result, (const void *)&_My_val, _Order);
		return (_Result);
		}

	operator _Ty() const volatile _NOEXCEPT
		{	// return value stored in *this
		return (load());
		}

	operator _Ty() const _NOEXCEPT
		{	// return value stored in *this
		return (load());
		}

	_Ty exchange(_Ty _Value,
		memory_order _Order = memory_order_seq_cst) volatile _NOEXCEPT
		{	// exchange value stored in *this with _Value
		this->_Exchange((void *)&_My_val, (void *)&_Value, _Order);
		return (_Value);
		}

	_Ty exchange(_Ty _Value,
		memory_order _Order = memory_order_seq_cst) _NOEXCEPT
		{	// exchange value stored in *this with _Value
		this->_Exchange((void *)&_My_val, (void *)&_Value, _Order);
		return (_Value);
		}

	bool compare_exchange_weak(_Ty& _Exp, _Ty _Value,
		memory_order _Order1, memory_order _Order2) volatile _NOEXCEPT
		{	// compare and exchange value stored in *this with *_Exp, _Value
		return (this->_Compare_exchange_weak(
			(void *)&_My_val, (void *)&_Exp, (const void *)&_Value,
				_Order1, _Order2));
		}

	bool compare_exchange_weak(_Ty& _Exp, _Ty _Value,
		memory_order _Order1, memory_order _Order2) _NOEXCEPT
		{	// compare and exchange value stored in *this with *_Exp, _Value
		return (this->_Compare_exchange_weak(
			(void *)&_My_val, (void *)&_Exp, (const void *)&_Value,
				_Order1, _Order2));
		}

	bool compare_exchange_weak(_Ty& _Exp, _Ty _Value,
		memory_order _Order = memory_order_seq_cst) volatile _NOEXCEPT
		{	// compare and exchange value stored in *this with *_Exp, _Value
		return (compare_exchange_weak(
			_Exp, _Value, _Order, _Get_memory_order(_Order)));
		}

	bool compare_exchange_weak(_Ty& _Exp, _Ty _Value,
		memory_order _Order = memory_order_seq_cst) _NOEXCEPT
		{	// compare and exchange value stored in *this with *_Exp, _Value
		return (compare_exchange_weak(
			_Exp, _Value, _Order, _Get_memory_order(_Order)));
		}

	bool compare_exchange_strong(_Ty& _Exp, _Ty _Value,
		memory_order _Order1, memory_order _Order2) volatile _NOEXCEPT
		{	// compare and exchange value stored in *this with *_Exp, _Value
		return (this->_Compare_exchange_strong(
			(void *)&_My_val, (void *)&_Exp, (const void *)&_Value,
				_Order1, _Order2));
		}

	bool compare_exchange_strong(_Ty& _Exp, _Ty _Value,
		memory_order _Order1, memory_order _Order2) _NOEXCEPT
		{	// compare and exchange value stored in *this with *_Exp, _Value
		return (this->_Compare_exchange_strong(
			(void *)&_My_val, (void *)&_Exp, (void *)&_Value,
				_Order1, _Order2));
		}

	bool compare_exchange_strong(_Ty& _Exp, _Ty _Value,
		memory_order _Order = memory_order_seq_cst) volatile _NOEXCEPT
		{	// compare and exchange value stored in *this with *_Exp, _Value
		return (compare_exchange_strong(
			_Exp, _Value, _Order, _Get_memory_order(_Order)));
		}

	bool compare_exchange_strong(_Ty& _Exp, _Ty _Value,
		memory_order _Order = memory_order_seq_cst) _NOEXCEPT
		{	// compare and exchange value stored in *this with *_Exp, _Value
		return (compare_exchange_strong(
			_Exp, _Value, _Order, _Get_memory_order(_Order)));
		}

#ifndef _ENABLE_ATOMIC_ALIGNMENT_FIX
	static_assert(alignof(_Ty) >= sizeof(_My_int),
		"You've instantiated std::atomic<T> with sizeof(T) equal to 2/4/8 and alignof(T) < sizeof(T). "
		"Before VS 2015 Update 2, this would have misbehaved at runtime. "
		"VS 2015 Update 2 was fixed to handle this correctly, "
		"but the fix inherently changes layout and breaks binary compatibility. "
		"Please define _ENABLE_ATOMIC_ALIGNMENT_FIX to acknowledge that you understand this, "
		"and that everything you're linking has been compiled with VS 2015 Update 2 (or later).");
#endif /* _ENABLE_ATOMIC_ALIGNMENT_FIX */

	alignas(sizeof(_My_int)) alignas(_Ty) _Ty _My_val;
	};

		// TEMPLATE CLASS atomic
template<class _Ty>
	struct atomic
		: _Atomic_base<_Ty, sizeof (_Ty)>
	{	// template that manages values of _Ty atomically
	static_assert(is_trivially_copyable<_Ty>::value,
		"atomic<T> requires T to be trivially copyable.");

	typedef _Atomic_base<_Ty, sizeof (_Ty)> _My_base;

	atomic() _NOEXCEPT = default;
	atomic(const atomic&) = delete;
	atomic& operator=(const atomic&) = delete;
	atomic& operator=(const atomic&) volatile = delete;

	constexpr atomic(_Ty _Val) _NOEXCEPT
		: _My_base(_Val)
		{	// construct from _Val
		}

	_Ty operator=(_Ty _Right) volatile _NOEXCEPT
		{	// assign from _Right
		return (_My_base::operator=(_Right));
		}

	_Ty operator=(_Ty _Right) _NOEXCEPT
		{	// assign from _Right
		return (_My_base::operator=(_Right));
		}
	};
#define ATOMIC_VAR_INIT(_Val)	{ _Val }
#define _ATOMIC_ITYPE	_Atomic_bool
#define _ITYPE	bool
#define _ISIZE	1
#include <xxatomic>

#define _ATOMIC_ITYPE	_Atomic_char
#define _ITYPE	char
#define _ISIZE	1
#define _ATOMIC_HAS_ARITHMETIC_OPS
#include <xxatomic>

#define _ATOMIC_ITYPE	_Atomic_schar
#define _ITYPE signed char
#define _ISIZE 1
#define _ATOMIC_HAS_ARITHMETIC_OPS
#include <xxatomic>

#define _ATOMIC_ITYPE	_Atomic_uchar
#define _ITYPE unsigned char
#define _ISIZE 1
#define _ATOMIC_HAS_ARITHMETIC_OPS
#include <xxatomic>

#define _ATOMIC_ITYPE	_Atomic_char16_t
#define _ITYPE char16_t
#define _ISIZE 2
#define _ATOMIC_HAS_ARITHMETIC_OPS
#include <xxatomic>

#define _ATOMIC_ITYPE	_Atomic_char32_t
#define _ITYPE char32_t
#define _ISIZE 4
#define _ATOMIC_HAS_ARITHMETIC_OPS
#include <xxatomic>

#define _ATOMIC_ITYPE	_Atomic_wchar_t
#define _ITYPE wchar_t
#define _ISIZE _WCHAR_T_SIZE
#define _ATOMIC_HAS_ARITHMETIC_OPS

 #ifndef _NATIVE_WCHAR_T_DEFINED
 #define _ATOMIC_HAS_NO_SPECIALIZATION
 #endif /* _NATIVE_WCHAR_T_DEFINED */

#include <xxatomic>

#define _ATOMIC_ITYPE	_Atomic_short
#define _ITYPE short
#define _ISIZE _SHORT_SIZE
#define _ATOMIC_HAS_ARITHMETIC_OPS
#include <xxatomic>

#define _ATOMIC_ITYPE	_Atomic_ushort
#define _ITYPE unsigned short
#define _ISIZE _SHORT_SIZE
#define _ATOMIC_HAS_ARITHMETIC_OPS
#include <xxatomic>

#define _ATOMIC_ITYPE	_Atomic_int
#define _ITYPE int
#define _ISIZE _INT_SIZE
#define _ATOMIC_HAS_ARITHMETIC_OPS
#include <xxatomic>

#define _ATOMIC_ITYPE	_Atomic_uint
#define _ITYPE unsigned int
#define _ISIZE _INT_SIZE
#define _ATOMIC_HAS_ARITHMETIC_OPS
#include <xxatomic>

#define _ATOMIC_ITYPE	_Atomic_long
#define _ITYPE long
#define _ISIZE _LONG_SIZE
#define _ATOMIC_HAS_ARITHMETIC_OPS
#include <xxatomic>

#define _ATOMIC_ITYPE	_Atomic_ulong
#define _ITYPE unsigned long
#define _ISIZE _LONG_SIZE
#define _ATOMIC_HAS_ARITHMETIC_OPS
#include <xxatomic>

#define _ATOMIC_ITYPE	_Atomic_llong
#define _ITYPE long long
#define _ISIZE _LONGLONG_SIZE
#define _ATOMIC_HAS_ARITHMETIC_OPS
#include <xxatomic>

#define _ATOMIC_ITYPE	_Atomic_ullong
#define _ITYPE unsigned long long
#define _ISIZE _LONGLONG_SIZE
#define _ATOMIC_HAS_ARITHMETIC_OPS
#include <xxatomic>

#define _ATOMIC_ITYPE	_Atomic_address
#define _ITYPE void *
#define _ISIZE _ADDR_SIZE
#define _ATOMIC_IS_ADDRESS_TYPE
#define _ATOMIC_HAS_NO_SPECIALIZATION
#include <xxatomic>

		// GENERAL OPERATIONS ON ATOMIC TYPES
template <class _Ty>
	inline bool atomic_is_lock_free(
		const volatile atomic<_Ty> *_Atom) _NOEXCEPT
	{
	return (_Atom->is_lock_free());
	}

template <class _Ty>
	inline bool atomic_is_lock_free(
		const atomic<_Ty> *_Atom) _NOEXCEPT
	{
	return (_Atom->is_lock_free());
	}

template <class _Ty>
	inline void atomic_init(
		volatile atomic<_Ty> *_Atom, _Ty _Value) _NOEXCEPT
	{
	*_Atom = _Value;
	}

template <class _Ty>
	inline void atomic_init(
		atomic<_Ty> *_Atom, _Ty _Value) _NOEXCEPT
	{
	*_Atom = _Value;
	}

template <class _Ty>
	inline void atomic_store(volatile atomic<_Ty> *_Atom,
		_Ty _Value) _NOEXCEPT
	{
	_Atom->store(_Value);
	}

template <class _Ty>
	inline void atomic_store(atomic<_Ty> *_Atom,
		_Ty _Value) _NOEXCEPT
	{
	_Atom->store(_Value);
	}

template <class _Ty>
	inline void atomic_store_explicit(
		volatile atomic<_Ty> *_Atom, _Ty _Value,
			memory_order _Order) _NOEXCEPT
	{
	_Atom->store(_Value, _Order);
	}

template <class _Ty>
	inline void atomic_store_explicit(
		atomic<_Ty> *_Atom, _Ty _Value,
			memory_order _Order) _NOEXCEPT
	{
	_Atom->store(_Value, _Order);
	}

template <class _Ty>
	inline _Ty atomic_load(const volatile atomic<_Ty> *_Atom) _NOEXCEPT
	{
	return (_Atom->load());
	}

template <class _Ty>
	inline _Ty atomic_load(const atomic<_Ty> *_Atom) _NOEXCEPT
	{
	return (_Atom->load());
	}

template <class _Ty>
	inline _Ty atomic_load_explicit(
		const volatile atomic<_Ty> *_Atom,
			memory_order _Order) _NOEXCEPT
	{
	return (_Atom->load(_Order));
	}

template <class _Ty>
	inline _Ty atomic_load_explicit(
		const atomic<_Ty> *_Atom,
			memory_order _Order) _NOEXCEPT
	{
	return (_Atom->load(_Order));
	}

template <class _Ty>
	inline _Ty atomic_exchange(volatile atomic<_Ty> *_Atom,
		_Ty _Value) _NOEXCEPT
	{
	return (_Atom->exchange(_Value));
	}

template <class _Ty>
	inline _Ty atomic_exchange(atomic<_Ty> *_Atom,
		_Ty _Value) _NOEXCEPT
	{
	return (_Atom->exchange(_Value));
	}

template <class _Ty>
	inline _Ty atomic_exchange_explicit(volatile atomic<_Ty> *_Atom,
		_Ty _Value, memory_order _Order) _NOEXCEPT
	{
	return (_Atom->exchange(_Value, _Order));
	}

template <class _Ty>
	inline _Ty atomic_exchange_explicit(atomic<_Ty> *_Atom, _Ty _Value,
		memory_order _Order) _NOEXCEPT
	{
	return (_Atom->exchange(_Value, _Order));
	}

template <class _Ty>
	inline bool atomic_compare_exchange_weak(
		volatile atomic<_Ty> *_Atom, _Ty *_Exp, _Ty _Value) _NOEXCEPT
	{
	return (_Atom->compare_exchange_weak(*_Exp, _Value));
	}

template <class _Ty>
	inline bool atomic_compare_exchange_weak(
		atomic<_Ty> *_Atom, _Ty *_Exp, _Ty _Value) _NOEXCEPT
	{
	return (_Atom->compare_exchange_weak(*_Exp, _Value));
	}

template <class _Ty>
	inline bool atomic_compare_exchange_weak_explicit(
		volatile atomic<_Ty> *_Atom, _Ty *_Exp, _Ty _Value,
		memory_order _Order1, memory_order _Order2) _NOEXCEPT
	{
	return (_Atom->compare_exchange_weak(*_Exp, _Value, _Order1, _Order2));
	}

template <class _Ty>
	inline bool atomic_compare_exchange_weak_explicit(
		atomic<_Ty> *_Atom, _Ty *_Exp, _Ty _Value,
		memory_order _Order1, memory_order _Order2) _NOEXCEPT
	{
	return (_Atom->compare_exchange_weak(*_Exp, _Value, _Order1, _Order2));
	}

template <class _Ty>
	inline bool atomic_compare_exchange_strong(
		volatile atomic<_Ty> *_Atom, _Ty *_Exp, _Ty _Value) _NOEXCEPT
	{
	return (_Atom->compare_exchange_strong(*_Exp, _Value));
	}

template <class _Ty>
	inline bool atomic_compare_exchange_strong(
		atomic<_Ty> *_Atom, _Ty *_Exp, _Ty _Value) _NOEXCEPT
	{
	return (_Atom->compare_exchange_strong(*_Exp, _Value));
	}

template <class _Ty>
	inline bool atomic_compare_exchange_strong_explicit(
		volatile atomic<_Ty> *_Atom, _Ty *_Exp, _Ty _Value,
		memory_order _Order1, memory_order _Order2) _NOEXCEPT
	{
	return (_Atom->compare_exchange_strong(*_Exp, _Value, _Order1, _Order2));
	}

template <class _Ty>
	inline bool atomic_compare_exchange_strong_explicit(
		atomic<_Ty> *_Atom, _Ty *_Exp, _Ty _Value,
		memory_order _Order1, memory_order _Order2) _NOEXCEPT
	{
	return (_Atom->compare_exchange_strong(*_Exp, _Value, _Order1, _Order2));
	}

		// OVERLOADS FOR POINTERS
template <class _Ty>
	inline _Ty *atomic_fetch_add(volatile atomic<_Ty*> *_Atom,
		ptrdiff_t _Value) _NOEXCEPT
	{
	return (_Atom->fetch_add(_Value));
	}

template <class _Ty>
	inline _Ty *atomic_fetch_add(atomic<_Ty*> *_Atom,
		ptrdiff_t _Value) _NOEXCEPT
	{
	return (_Atom->fetch_add(_Value));
	}

template <class _Ty>
	inline _Ty *atomic_fetch_add_explicit(
		volatile atomic<_Ty*> *_Atom, ptrdiff_t _Value,
			memory_order _Order) _NOEXCEPT
	{
	return (_Atom->fetch_add(_Value, _Order));
	}

template <class _Ty>
	inline _Ty *atomic_fetch_add_explicit(
		atomic<_Ty*> *_Atom, ptrdiff_t _Value,
			memory_order _Order) _NOEXCEPT
	{
	return (_Atom->fetch_add(_Value, _Order));
	}

template <class _Ty>
	inline _Ty *atomic_fetch_sub(volatile atomic<_Ty*> *_Atom,
		ptrdiff_t _Value) _NOEXCEPT
	{
	return (_Atom->fetch_sub(_Value));
	}

template <class _Ty>
	inline _Ty *atomic_fetch_sub(atomic<_Ty*> *_Atom,
		ptrdiff_t _Value) _NOEXCEPT
	{
	return (_Atom->fetch_sub(_Value));
	}

template <class _Ty>
	inline _Ty *atomic_fetch_sub_explicit(
		volatile atomic<_Ty*> *_Atom, ptrdiff_t _Value,
			memory_order _Order) _NOEXCEPT
	{
	return (_Atom->fetch_sub(_Value, _Order));
	}

template <class _Ty>
	inline _Ty *atomic_fetch_sub_explicit(
		atomic<_Ty*> *_Atom, ptrdiff_t _Value,
			memory_order _Order) _NOEXCEPT
	{
	return (_Atom->fetch_sub(_Value, _Order));
	}

		// FENCES
extern "C" inline void atomic_thread_fence(memory_order _Order) _NOEXCEPT
	{	// force memory visibility and inhibit compiler reordering
	return (_ATOMIC_THREAD_FENCE(_Order));
	}

extern "C" inline void atomic_signal_fence(memory_order _Order) _NOEXCEPT
	{	// force memory visibility and inhibit compiler reordering
	return (_ATOMIC_SIGNAL_FENCE(_Order));
	}

		// ATOMIC TYPEDEFS
typedef atomic<bool> atomic_bool;

typedef atomic<char> atomic_char;
typedef atomic<signed char> atomic_schar;
typedef atomic<unsigned char> atomic_uchar;
typedef atomic<short> atomic_short;
typedef atomic<unsigned short> atomic_ushort;
typedef atomic<int> atomic_int;
typedef atomic<unsigned int> atomic_uint;
typedef atomic<long> atomic_long;
typedef atomic<unsigned long> atomic_ulong;
typedef atomic<long long> atomic_llong;
typedef atomic<unsigned long long> atomic_ullong;

typedef atomic<char16_t> atomic_char16_t;
typedef atomic<char32_t> atomic_char32_t;

typedef atomic<wchar_t> atomic_wchar_t;

typedef atomic<int8_t> atomic_int8_t;
typedef atomic<uint8_t> atomic_uint8_t;
typedef atomic<int16_t> atomic_int16_t;
typedef atomic<uint16_t> atomic_uint16_t;
typedef atomic<int32_t> atomic_int32_t;
typedef atomic<uint32_t> atomic_uint32_t;
typedef atomic<int64_t> atomic_int64_t;
typedef atomic<uint64_t> atomic_uint64_t;

typedef atomic<int_least8_t> atomic_int_least8_t;
typedef atomic<uint_least8_t> atomic_uint_least8_t;
typedef atomic<int_least16_t> atomic_int_least16_t;
typedef atomic<uint_least16_t> atomic_uint_least16_t;
typedef atomic<int_least32_t> atomic_int_least32_t;
typedef atomic<uint_least32_t> atomic_uint_least32_t;
typedef atomic<int_least64_t> atomic_int_least64_t;
typedef atomic<uint_least64_t> atomic_uint_least64_t;

typedef atomic<int_fast8_t> atomic_int_fast8_t;
typedef atomic<uint_fast8_t> atomic_uint_fast8_t;
typedef atomic<int_fast16_t> atomic_int_fast16_t;
typedef atomic<uint_fast16_t> atomic_uint_fast16_t;
typedef atomic<int_fast32_t> atomic_int_fast32_t;
typedef atomic<uint_fast32_t> atomic_uint_fast32_t;
typedef atomic<int_fast64_t> atomic_int_fast64_t;
typedef atomic<uint_fast64_t> atomic_uint_fast64_t;

typedef atomic<intptr_t> atomic_intptr_t;
typedef atomic<uintptr_t> atomic_uintptr_t;
typedef atomic<size_t> atomic_size_t;
typedef atomic<ptrdiff_t> atomic_ptrdiff_t;
typedef atomic<intmax_t> atomic_intmax_t;
typedef atomic<uintmax_t> atomic_uintmax_t;
_STD_END
 #pragma pop_macro("new")
 #pragma warning(pop)
 #pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _ATOMIC_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
