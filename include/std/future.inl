// future standard header
#pragma once
#ifndef _FUTURE_
#define _FUTURE_
#ifndef RC_INVOKED

 #ifdef _M_CEE
  #error <future> is not supported when compiling with /clr or /clr:pure.
 #endif /* _M_CEE */

 #ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
  #include <experimental/resumable>
 #endif /* _RESUMABLE_FUNCTIONS_SUPPORTED */

#include <functional>
#include <system_error>
#include <utility>
#include <chrono>
#include <mutex>
#include <condition_variable>

 #include <ppltasks.h>

#include <thread>

 #pragma pack(push,_CRT_PACKING)
 #pragma warning(push,3)
 #pragma push_macro("new")
 #undef new

 #pragma warning(disable: 4482 4521 4522)

_STD_BEGIN
		// ENUM future_errc

enum class future_errc {	// names for futures errors
	broken_promise = 1,
	future_already_retrieved,
	promise_already_satisfied,
	no_state
	};

typedef future_errc _Future_errc;

		// ENUM launch

enum class launch {	// names for launch options passed to async
	async = 0x1,
	deferred = 0x2
	};

inline constexpr launch operator&(launch _Left, launch _Right)
	{	/* return _Left&_Right */
	return (static_cast<launch>(static_cast<unsigned int>(_Left)
		& static_cast<unsigned int>(_Right)));
	}

inline constexpr launch operator|(launch _Left, launch _Right)
	{	/* return _Left|_Right */
	return (static_cast<launch>(static_cast<unsigned int>(_Left)
		| static_cast<unsigned int>(_Right)));
	}

inline constexpr launch operator^(launch _Left, launch _Right)
	{	/* return _Left^_Right */
	return (static_cast<launch>(static_cast<unsigned int>(_Left)
		^ static_cast<unsigned int>(_Right)));
	}

inline constexpr launch operator~(launch _Left)
	{	/* return ~_Left */
	return (static_cast<launch>(~static_cast<unsigned int>(_Left)));
	}

inline launch& operator&=(launch& _Left, launch _Right)
	{	/* return _Left&=_Right */
	_Left = _Left & _Right;
	return (_Left);
	}

inline launch& operator|=(launch& _Left, launch _Right)
	{	/* return _Left|=_Right */
	_Left = _Left | _Right;
	return (_Left);
	}

inline launch& operator^=(launch& _Left, launch _Right)
	{	/* return _Left^=_Right */
	_Left = _Left ^ _Right;
	return (_Left);
	}

typedef launch _Launch_type;

		// ENUM future_status

enum class future_status {	// names for timed wait function returns
	ready,
	timeout,
	deferred
	};

typedef future_status _Future_status;

		// HELPER FUNCTIONS
[[noreturn]] _CRTIMP2_PURE void __CLRCALL_PURE_OR_CDECL _Throw_future_error(
	const error_code& _Code);
[[noreturn]] _CRTIMP2_PURE void __CLRCALL_PURE_OR_CDECL _Rethrow_future_exception(
	_XSTD exception_ptr _Ptr);

		// TEMPLATE CLASS SPECIALIZATION is_error_code_enum
template<>
	struct is_error_code_enum<_Future_errc>
		: public true_type
	{	// tests for error_code enumeration
	};

const error_category& future_category() _NOEXCEPT;

inline error_code make_error_code(_Future_errc _Errno) _NOEXCEPT
	{	// make an error_code object
	return (error_code(static_cast<int>(_Errno), future_category()));
	}

inline error_condition make_error_condition(_Future_errc _Errno) _NOEXCEPT
	{	// make an error_condition object
	return (error_condition(static_cast<int>(_Errno), future_category()));
	}

		// CLASS future_error
inline const char *_Future_error_map(int _Errcode) _NOEXCEPT
	{	// convert to name of future error
	switch (static_cast<_Future_errc>(_Errcode))
		{	// switch on error code value
	case future_errc::broken_promise:
		return ("broken promise");

	case future_errc::future_already_retrieved:
		return ("future already retrieved");

	case future_errc::promise_already_satisfied:
		return ("promise already satisfied");

	case future_errc::no_state:
		return ("no state");

	default:
		return (0);
		}
	}

class future_error
	: public logic_error
	{	// future exception
public:
	explicit future_error(error_code _Errcode)
		: logic_error(""), _Mycode(_Errcode)
		{	// construct from error code
		}

	const error_code& code() const _NOEXCEPT
		{	// return stored error code
		return (_Mycode);
		}

	const char *__CLR_OR_THIS_CALL what() const _THROW0()
		{	// get message string
		return (_Future_error_map(_Mycode.value()));
		}

 #if _HAS_EXCEPTIONS

 #else /* _HAS_EXCEPTIONS */
protected:
	virtual void _Doraise() const
		{	// perform class-specific exception handling
		_RAISE(*this);
		}
 #endif /* _HAS_EXCEPTIONS */

private:
	error_code _Mycode;	// the stored error code
	};

		// CLASS _Future_error_category
class _Future_error_category
	: public _Generic_error_category
	{	// categorize a future error
public:
	_Future_error_category()
		{	// default constructor
		_Addr = _Future_addr;
		}

	virtual const char *name() const _NOEXCEPT
		{	// get name of category
		return ("future");
		}

	virtual string message(int _Errcode) const
		{	// convert to name of error
		const char *_Name = _Future_error_map(_Errcode);
		if (_Name != 0)
			return (_Name);
		else
			return (_Generic_error_category::message(_Errcode));
		}
	};

inline const error_category& future_category() _NOEXCEPT
	{	// return error_category object for future
	return (_Immortalize<_Future_error_category>());
	}

		// TEMPLATE STRUCT _State_deleter
template<class _Ty>
	class _Associated_state;

template<class _Ty>
	struct _Deleter_base
	{	// abstract base class for managing deletion of state objects
	virtual void _Delete(_Associated_state<_Ty> *) = 0;
	virtual ~_Deleter_base() _NOEXCEPT
		{	// destroy the object
		}
	};

template<class _Ty,
	class _Derived,
	class _Alloc>
	struct _State_deleter
	: _Deleter_base<_Ty>
	{	// manage allocator and deletion state objects
	_State_deleter(const _Alloc& _Al)
		: _My_alloc(_Al)
		{	// construct with allocator
		}

	_State_deleter(const _State_deleter&) = delete;
	_State_deleter& operator=(const _State_deleter&) = delete;

	inline void _Delete(_Associated_state<_Ty> *_State);

	_Alloc _My_alloc;
	};

		// TEMPLATE CLASS _Associated_state
template<class _Ty>
	class _Associated_state
	{	// class for managing associated synchronous state
public:
	typedef _Ty _State_type;
	typedef _Deleter_base<_Ty> _Mydel;

	_Associated_state(_Mydel *_Dp = 0)
		: _Exception(),
		_Retrieved(false),
		_Running(false),
		_Ready(false),
		_Ready_at_thread_exit(false),
		_Has_stored_result(false),
		_Deleter(_Dp)
		{	// construct
		// TODO: _Associated_state ctor assumes _Ty is default constructible
		_Init_refs();
		}

	virtual ~_Associated_state() _NOEXCEPT
		{	// destroy
		if (_Has_stored_result && !_Ready)
			{	// registered for release at thread exit
			_Cond._Unregister(_Mtx);
			}
		_Destroy_refs();
		}

	void _Retain()
		{	// increment reference count
		_MT_INCR(_Refs);
		}

	void _Release()
		{	// decrement reference count and destroy when zero
		if (_MT_DECR(_Refs) == 0)
			_Delete_this();
		}

private:
	_Atomic_counter_t _Refs;

public:
	void _Init_refs()
		{	// initialize reference count
		_Init_atomic_counter(_Refs, 1);
		}

	void _Destroy_refs()
		{	// destroy reference count
		}

	virtual void _Wait()
		{	// wait for signal
		unique_lock<mutex> _Lock(_Mtx);
		_Maybe_run_deferred_function(_Lock);
		while (!_Ready)
			_Cond.wait(_Lock);
		}

	struct _Test_ready
		{	// wraps _Associated_state
		_Test_ready(const _Associated_state *_St)
			: _State(_St)
			{	// construct from associated state
			}

		bool operator()() const
			{	// test state
			return (_State->_Ready != 0);
			}
		const _Associated_state *_State;
		};

	template<class _Rep,
		class _Per>
		_Future_status _Wait_for(
			const chrono::duration<_Rep, _Per>& _Rel_time)
		{	// wait for duration
		unique_lock<mutex> _Lock(_Mtx);
		if (_Has_deferred_function())
			return (_Future_status::deferred);
		if (_Cond.wait_for(_Lock, _Rel_time, _Test_ready(this)))
			return (_Future_status::ready);
		return (_Future_status::timeout);
		}

	template<class _Clock,
		class _Dur>
		_Future_status _Wait_until(
			const chrono::time_point<_Clock, _Dur>& _Abs_time)
		{	// wait until time point
		unique_lock<mutex> _Lock(_Mtx);
		if (_Has_deferred_function())
			return (_Future_status::deferred);
		if (_Cond.wait_until(_Lock, _Abs_time, _Test_ready(this)))
			return (_Future_status::ready);
		return (_Future_status::timeout);
		}

	virtual _Ty& _Get_value(bool _Get_only_once)
		{	// return the stored result or throw stored exception
		unique_lock<mutex> _Lock(_Mtx);
		if (_Get_only_once && _Retrieved)
			_Throw_future_error(
				make_error_code(future_errc::future_already_retrieved));
		if (_Exception)
			_Rethrow_future_exception(_Exception);
		_Retrieved = true;
		_Maybe_run_deferred_function(_Lock);
		while (!_Ready)
			_Cond.wait(_Lock);
		if (_Exception)
			_Rethrow_future_exception(_Exception);
		return (_Result);
		}

	void _Set_value(const _Ty& _Val, bool _At_thread_exit)
		{	// store a result
		unique_lock<mutex> _Lock(_Mtx);
		_Set_value_raw(_Val, &_Lock, _At_thread_exit);
		}

	void _Set_value_raw(const _Ty& _Val, unique_lock<mutex> *_Lock,
		bool _At_thread_exit)
		{	// store a result while inside a locked block
		if (_Has_stored_result)
			_Throw_future_error(
				make_error_code(future_errc::promise_already_satisfied));
		_Result = _Val;
		_Do_notify(_Lock, _At_thread_exit);
		}

	void _Set_value(_Ty&& _Val, bool _At_thread_exit)
		{	// store a result
		unique_lock<mutex> _Lock(_Mtx);
		_Set_value_raw(_STD forward<_Ty>(_Val), &_Lock, _At_thread_exit);
		}

	void _Set_value_raw(_Ty&& _Val, unique_lock<mutex> *_Lock,
		bool _At_thread_exit)
		{	// store a result while inside a locked block
		if (_Has_stored_result)
			_Throw_future_error(
				make_error_code(future_errc::promise_already_satisfied));
		_Result = _STD forward<_Ty>(_Val);
		_Do_notify(_Lock, _At_thread_exit);
		}

	void _Set_value(bool _At_thread_exit)
		{	// store a (void) result
		unique_lock<mutex> _Lock(_Mtx);
		_Set_value_raw(&_Lock, _At_thread_exit);
		}

	void _Set_value_raw(unique_lock<mutex> *_Lock, bool _At_thread_exit)
		{	// store a (void) result while inside a locked block
		if (_Has_stored_result)
			_Throw_future_error(
				make_error_code(future_errc::promise_already_satisfied));
		_Do_notify(_Lock, _At_thread_exit);
		}

	void _Set_exception(_XSTD exception_ptr _Exc, bool _At_thread_exit)
		{	// store a result
		unique_lock<mutex> _Lock(_Mtx);
		_Set_exception_raw(_Exc, &_Lock, _At_thread_exit);
		}

	struct _Anon
		{	// anonymous type
		};

	void _Set_exception_raw(_XSTD exception_ptr _Exc,
		unique_lock<mutex> *_Lock, bool _At_thread_exit)
		{	// store a result while inside a locked block
		if (_Has_stored_result)
			_Throw_future_error(
				make_error_code(future_errc::promise_already_satisfied));
		_Exception = _Exc;
		if (!_Exc)	// make a non-null exception_ptr

			_Exception = make_exception_ptr(_Anon());

		_Do_notify(_Lock, _At_thread_exit);
		}

	bool _Is_ready() const
		{	// return ready status
		return (_Ready != 0);
		}

	bool _Already_has_stored_result() const
		{	// return presence of stored result
		return (_Has_stored_result);
		}

	bool _Already_retrieved() const
		{	// return retrieved status
		return (_Retrieved);
		}

	void _Abandon()
		{	// abandon shared state
		unique_lock<mutex> _Lock(_Mtx);
		if (!_Has_stored_result)
			{	// queue exception
			future_error _Fut(make_error_code(future_errc::broken_promise));
			_Set_exception_raw(_XSTD make_exception_ptr(_Fut), &_Lock, false);
			}
		}

protected:
	void _Make_ready_at_thread_exit()
		{	// set ready status at thread exit
		if (_Ready_at_thread_exit)
			_Ready = true;
		}

	void _Maybe_run_deferred_function(unique_lock<mutex>& _Lock)
		{	// run a deferred function if not already done
		if (!_Running)
			{	// run the function
			_Running = true;
			_Run_deferred_function(_Lock);
			}
		}

public:
	_Ty _Result;
	_XSTD exception_ptr _Exception;
	mutex _Mtx;
	condition_variable _Cond;
	bool _Retrieved;
	int _Ready;
	bool _Ready_at_thread_exit;
	bool _Has_stored_result;
	bool _Running;

private:
	virtual bool _Has_deferred_function() const _NOEXCEPT
		{	// overridden by _Deferred_async_state
		return (false);
		}

	virtual void _Run_deferred_function(unique_lock<mutex>&)
		{	// do nothing
		}

	virtual void _Do_notify(unique_lock<mutex> *_Lock, bool _At_thread_exit)
		{	// notify waiting threads
		_Has_stored_result = true;
		if (_At_thread_exit)
			{	// notify at thread exit
			_Cond._Register(*_Lock, &_Ready);
			}
		else
			{	// notify immediately
			_Ready = true;
			_Cond.notify_all();
			}
		}

	void _Delete_this()
		{	// delete this object
		if (_Deleter)
			_Deleter->_Delete(this);
		else
			delete this;
		}

	_Mydel *_Deleter;

public:
	_Associated_state(const _Associated_state&) = delete;
	_Associated_state& operator=(const _Associated_state&) = delete;
	};

template<class _Ty,
	class _Derived,
	class _Alloc> inline
	void _State_deleter<_Ty, _Derived, _Alloc>::_Delete(
		_Associated_state<_Ty> *_State)
	{	// delete _State and this using stored allocator
	typedef _Wrap_alloc<_Alloc> _Alty0;

	typedef typename _Alty0::template rebind<_Derived>
		::other _State_allocator;
	_State_allocator _St_alloc(_My_alloc);

	typedef typename _Alty0::template rebind<_State_deleter>
		::other _Deleter_allocator;
	_Deleter_allocator _Del_alloc(_My_alloc);

	_Derived *_Ptr = static_cast<_Derived *>(_State);

	_St_alloc.destroy(_Ptr);
	_St_alloc.deallocate(_Ptr, 1);

	_Del_alloc.destroy(this);
	_Del_alloc.deallocate(this, 1);
	}

		// TEMPLATE CLASS _Packaged_state

template<class>
	class _Packaged_state;

template<class _Ret,
	class... _ArgTypes>
	class _Packaged_state<_Ret(_ArgTypes...)>
		: public _Associated_state<_Ret>
	{	// class for managing associated asynchronous state for packaged_task
public:
	typedef _Associated_state<_Ret> _Mybase;
	typedef typename _Mybase::_Mydel _Mydel;

	template<class _Fty2>
		_Packaged_state(const _Fty2& _Fnarg)
			: _Fn(_Fnarg)
		{	// construct from function object
		}

	template<class _Fty2,
		class _Alloc>
		_Packaged_state(const _Fty2& _Fnarg, const _Alloc& _Al, _Mydel *_Dp)
			: _Mybase(_Dp), _Fn(allocator_arg, _Al, _Fnarg)
		{	// construct from function object and allocator
		}

	template<class _Fty2>
		_Packaged_state(_Fty2&& _Fnarg)
			: _Fn(_STD forward<_Fty2>(_Fnarg))
		{	// construct from rvalue function object
		}

	template<class _Fty2,
		class _Alloc>
		_Packaged_state(_Fty2&& _Fnarg, const _Alloc& _Al, _Mydel *_Dp)
			: _Mybase(_Dp), _Fn(allocator_arg, _Al,
				_STD forward<_Fty2>(_Fnarg))
		{	// construct from rvalue function object and allocator
		}

	void _Call_deferred(_ArgTypes... _Args)
		{	// set deferred call
		_TRY_BEGIN
			// call function object and catch exceptions
			this->_Set_value(_Fn(_STD forward<_ArgTypes>(_Args)...),
				true);
		_CATCH_ALL
			// function object threw exception; record result
			this->_Set_exception(_XSTD current_exception(), true);
		_CATCH_END
		}

	void _Call_immediate(_ArgTypes... _Args)
		{	// call function object
		_TRY_BEGIN
			// call function object and catch exceptions
			this->_Set_value(_Fn(_STD forward<_ArgTypes>(_Args)...),
				false);
		_CATCH_ALL
			// function object threw exception; record result
			this->_Set_exception(_XSTD current_exception(), false);
		_CATCH_END
		}

	const function<_Ret(_ArgTypes...)>& _Get_fn()
		{	// return stored function object
		return (_Fn);
		}

private:
	function<_Ret(_ArgTypes...)> _Fn;
	};

template<class _Ret,
	class... _ArgTypes>
	class _Packaged_state<_Ret&(_ArgTypes...)>
		: public _Associated_state<_Ret *>
	{	// class for managing associated asynchronous state for packaged_task
public:
	typedef _Associated_state<_Ret *> _Mybase;
	typedef typename _Mybase::_Mydel _Mydel;

	template<class _Fty2>
		_Packaged_state(const _Fty2& _Fnarg)
			: _Fn(_Fnarg)
		{	// construct from function object
		}

	template<class _Fty2,
		class _Alloc>
		_Packaged_state(const _Fty2& _Fnarg, const _Alloc& _Al, _Mydel *_Dp)
			: _Mybase(_Dp), _Fn(allocator_arg, _Al, _Fnarg)
		{	// construct from function object and allocator
		}

	template<class _Fty2>
		_Packaged_state(_Fty2&& _Fnarg)
			: _Fn(_STD forward<_Fty2>(_Fnarg))
		{	// construct from rvalue function object
		}

	template<class _Fty2,
		class _Alloc>
		_Packaged_state(_Fty2&& _Fnarg, const _Alloc& _Al, _Mydel *_Dp)
			: _Mybase(_Dp), _Fn(allocator_arg, _Al,
				_STD forward<_Fty2>(_Fnarg))
		{	// construct from rvalue function object and allocator
		}

	void _Call_deferred(_ArgTypes... _Args)
		{	// set deferred call
		_TRY_BEGIN
			// call function object and catch exceptions
			this->_Set_value(
				_STD addressof(_Fn(_STD forward<_ArgTypes>(_Args)...)),
				true);
		_CATCH_ALL
			// function object threw exception; record result
			this->_Set_exception(_XSTD current_exception(), true);
		_CATCH_END
		}

	void _Call_immediate(_ArgTypes... _Args)
		{	// call function object
		_TRY_BEGIN
			// call function object and catch exceptions
			this->_Set_value(
				_STD addressof(_Fn(_STD forward<_ArgTypes>(_Args)...)),
				false);
		_CATCH_ALL
			// function object threw exception; record result
			this->_Set_exception(_XSTD current_exception(), false);
		_CATCH_END
		}

	const function<_Ret&(_ArgTypes...)>& _Get_fn()
		{	// return stored function object
		return (_Fn);
		}

private:
	function<_Ret&(_ArgTypes...)> _Fn;
	};

template<class... _ArgTypes>
	class _Packaged_state<void(_ArgTypes...)>
		: public _Associated_state<int>
	{	// class for managing associated asynchronous state for packaged_task
public:
	typedef _Associated_state<int> _Mybase;
	typedef typename _Mybase::_Mydel _Mydel;

	template<class _Fty2>
		_Packaged_state(const _Fty2& _Fnarg)
			: _Fn(_Fnarg)
		{	// construct from function object
		}

	template<class _Fty2,
		class _Alloc>
		_Packaged_state(const _Fty2& _Fnarg, const _Alloc& _Al, _Mydel *_Dp)
			: _Mybase(_Dp), _Fn(allocator_arg, _Al, _Fnarg)
		{	// construct from function object and allocator
		}

	template<class _Fty2>
		_Packaged_state(_Fty2&& _Fnarg)
			: _Fn(_STD forward<_Fty2>(_Fnarg))
		{	// construct from rvalue function object
		}

	template<class _Fty2,
		class _Alloc>
		_Packaged_state(_Fty2&& _Fnarg, const _Alloc& _Al, _Mydel *_Dp)
			: _Mybase(_Dp), _Fn(allocator_arg, _Al,
					_STD forward<_Fty2>(_Fnarg))
		{	// construct from rvalue function object and allocator
		}

	void _Call_deferred(_ArgTypes... _Args)
		{	// set deferred call
		_TRY_BEGIN
			// call function object and catch exceptions
			_Fn(_STD forward<_ArgTypes>(_Args)...);
			this->_Set_value(1, true);
		_CATCH_ALL
			// function object threw exception; record result
			this->_Set_exception(_XSTD current_exception(), true);
		_CATCH_END
		}

	void _Call_immediate(_ArgTypes... _Args)
		{	// call function object
		_TRY_BEGIN
			// call function object and catch exceptions
			_Fn(_STD forward<_ArgTypes>(_Args)...);
			this->_Set_value(1, false);
		_CATCH_ALL
			// function object threw exception; record result
			this->_Set_exception(_XSTD current_exception(), false);
		_CATCH_END
		}

	const function<void(_ArgTypes...)>& _Get_fn()
		{	// return stored function object
		return (_Fn);
		}

private:
	function<void(_ArgTypes...)> _Fn;
	};


template<class _Ty,
	class _Alloc> inline
	_Associated_state<_Ty> *_Make_associated_state(const _Alloc& _Al)
	{	// construct an _Associated_state object with an allocator
	typedef _State_deleter<_Ty, _Associated_state<_Ty>, _Alloc> _Delty;
	typedef _Wrap_alloc<_Alloc> _Alty0;

	typedef typename _Alty0::template rebind<_Delty>
		::other _Deleter_allocator;
	typedef typename _Alty0::template rebind<_Associated_state<_Ty> >
		::other _State_allocator;

	_Deleter_allocator _Del_alloc(_Al);
	_State_allocator _St_alloc(_Al);

	_Delty *_Del = 0;
	_Associated_state<_Ty> *_Res = 0;

	_Del = _Del_alloc.allocate(1);

	bool _Del_constructed = false;

	_TRY_BEGIN
		_Del_alloc.construct(_Del, _Al);
		_Del_constructed = true;

		_Res = _St_alloc.allocate(1);

		_St_alloc.construct(_Res, _Del);
	_CATCH_ALL
		if (_Res)
			_St_alloc.deallocate(_Res, 1);

		if (_Del_constructed)
			_Del_alloc.destroy(_Del);

		_Del_alloc.deallocate(_Del, 1);
	_RERAISE;
	_CATCH_END

	return (_Res);
	}

template<class _Pack_state,
	class _Fty2,
	class _Alloc> inline
	_Pack_state *_Make_packaged_state(_Fty2&& _Fnarg, const _Alloc& _Al)
	{	// construct a _Packaged_state object with an allocator from
		// an rvalue function object
	typedef _State_deleter<typename _Pack_state::_Mybase::_State_type,
		_Pack_state, _Alloc> _Delty;
	typedef _Wrap_alloc<_Alloc> _Alty0;

	typedef typename _Alty0::template rebind<_Delty>
		::other _Deleter_allocator;
	typedef typename _Alty0::template rebind<_Pack_state>
		::other _State_allocator;

	_Deleter_allocator _Del_alloc(_Al);
	_State_allocator _St_alloc(_Al);

	_Delty *_Del = 0;
	_Pack_state *_Res = 0;

	_Del = _Del_alloc.allocate(1);

	bool _Del_constructed = false;

	_TRY_BEGIN
		_Del_alloc.construct(_Del, _Al);
		_Del_constructed = true;

		_Res = _St_alloc.allocate(1);

		_St_alloc.construct(_Res, _STD forward<_Fty2>(_Fnarg), _Al, _Del);
	_CATCH_ALL
		if (_Res)
			_St_alloc.deallocate(_Res, 1);

		if (_Del_constructed)
			_Del_alloc.destroy(_Del);

		_Del_alloc.deallocate(_Del, 1);
	_RERAISE;
	_CATCH_END

	return (_Res);
	}

		// TEMPLATE CLASS _Deferred_async_state
template<class _Rx>
	class _Deferred_async_state
	: public _Packaged_state<_Rx()>
	{	// class for managing associated synchronous state for deferred
		// execution from async
public:
template<class _Fty2>
	_Deferred_async_state(const _Fty2& _Fnarg)
		: _Packaged_state<_Rx()>(_Fnarg)
		{	// construct from function object
		}

template<class _Fty2>
	_Deferred_async_state(_Fty2&& _Fnarg)
		: _Packaged_state<_Rx()>(_STD forward<_Fty2>(_Fnarg))
		{	// construct from rvalue function object
		}

private:
	virtual bool _Has_deferred_function() const _NOEXCEPT
		{	// this function is considered to be deferred until it's invoked
		return (!this->_Running);
		}

	virtual void _Run_deferred_function(unique_lock<mutex>& _Lock)
		{	// run the deferred function
		_Lock.unlock();
		_Packaged_state<_Rx()>::_Call_immediate();
		_Lock.lock();
		}
	};

		// TEMPLATE CLASS _Task_async_state

template<class _Rx, bool _Inline>
	class _Task_async_state
	: public _Packaged_state<_Rx()>
	{	// class for managing associated synchronous state for asynchronous
		// execution from async
public:
	typedef _Packaged_state<_Rx()> _Mybase;
	typedef typename _Mybase::_State_type _State_type;

	template<class _Fty2>
		_Task_async_state(_Fty2&& _Fnarg)
			: _Mybase(_STD forward<_Fty2>(_Fnarg))
		{	// construct from rvalue function object
		_Task = ::Concurrency::create_task([this]()
			{	// do it now
			this->_Call_immediate();
			});

		this->_Running = true;
		}

	virtual ~_Task_async_state() _NOEXCEPT
		{	// destroy
		if (!this->_Ready)
			_Wait();
		}

	virtual void _Wait()
		{	// wait for completion
		if (_Inline)
			_Task.wait();
		else
			_Mybase::_Wait();
		}

	virtual _State_type& _Get_value(bool _Get_only_once)
		{	// return the stored result or throw stored exception
		if (_Inline)
			_Task.wait();
		return (_Mybase::_Get_value(_Get_only_once));
		}

private:
	::Concurrency::task<void> _Task;
	};

		// TEMPLATE CLASS _State_manager
template<class _Ty>
	class _State_manager
	{	// class for managing possibly non-existent associated
		// asynchronous state object
public:
	_State_manager()
		: _Assoc_state(0)
		{	// construct with no associated asynchronous state object
		_Get_only_once = false;
		}

	_State_manager(_Associated_state<_Ty> *_New_state, bool _Get_once)
		: _Assoc_state(_New_state)
		{	// construct with _New_state
		_Get_only_once = _Get_once;
		}

	_State_manager(const _State_manager& _Other, bool _Get_once = false)
		: _Assoc_state(0)
		{	// construct from _Other
		_Copy_from(_Other);
		_Get_only_once = _Get_once;
		}

	_State_manager(_State_manager&& _Other, bool _Get_once = false)
		: _Assoc_state(0)
		{	// construct from rvalue _Other
		_Move_from(_Other);
		_Get_only_once = _Get_once;
		}

	~_State_manager() _NOEXCEPT
		{	// destroy
		if (_Assoc_state != 0)
			_Assoc_state->_Release();
		}

	_State_manager& operator=(const _State_manager& _Other)
		{	// assign from _Other
		_Copy_from(_Other);
		return (*this);
		}

	_State_manager& operator=(_State_manager&& _Other)
		{	// assign from rvalue _Other
		_Move_from(_Other);
		return (*this);
		}

	bool valid() const _NOEXCEPT
		{	// return status
		return (_Assoc_state != 0
			&& !(_Get_only_once && _Assoc_state->_Already_retrieved()));
		}

	void wait() const
		{	// wait for signal
		if (!valid())
			_Throw_future_error(make_error_code(future_errc::no_state));
		_Assoc_state->_Wait();
		}

	template<class _Rep,
		class _Per>
		_Future_status wait_for(
			const chrono::duration<_Rep, _Per>& _Rel_time) const
		{	// wait for duration
		if (!valid())
			_Throw_future_error(make_error_code(future_errc::no_state));
		return (_Assoc_state->_Wait_for(_Rel_time));
		}

	template<class _Clock,
		class _Dur>
		_Future_status wait_until(
			const chrono::time_point<_Clock, _Dur>& _Abs_time) const
		{	// wait until time point
		if (!valid())
			_Throw_future_error(make_error_code(future_errc::no_state));
		return (_Assoc_state->_Wait_until(_Abs_time));
		}

	_Ty& _Get_value() const
		{	// return the stored result or throw stored exception
		if (!valid())
			_Throw_future_error(
				make_error_code(future_errc::no_state));
		return (_Assoc_state->_Get_value(_Get_only_once));
		}

	void _Set_value(const _Ty& _Val, bool _Defer)
		{	// store a result
		if (!valid())
			_Throw_future_error(
				make_error_code(future_errc::no_state));
		_Assoc_state->_Set_value(_Val, _Defer);
		}

	void _Set_value(_Ty&& _Val, bool _Defer)
		{	// store a result
		if (!valid())
			_Throw_future_error(
				make_error_code(future_errc::no_state));
		_Assoc_state->_Set_value(_STD forward<_Ty>(_Val), _Defer);
		}

	void _Abandon()
		{	// abandon shared state
		if (_Assoc_state)
			_Assoc_state->_Abandon();
		}

	void _Set_exception(_XSTD exception_ptr _Exc, bool _Defer)
		{	// store a result
		if (!valid())
			_Throw_future_error(
				make_error_code(future_errc::no_state));
		_Assoc_state->_Set_exception(_Exc, _Defer);
		}

	void _Swap(_State_manager& _Other)
		{	// exchange with _Other
		_STD swap(_Assoc_state, _Other._Assoc_state);
		}

	_Associated_state<_Ty> *_Ptr() const
		{	// return pointer to stored associated asynchronous state object
		return (_Assoc_state);
		}

	void _Copy_from(const _State_manager& _Other)
		{	// copy stored associated asynchronous state object from _Other
		if (this != &_Other)
			{	// different, copy
			if (_Assoc_state)
				_Assoc_state->_Release();
			if (_Other._Assoc_state == 0)
				_Assoc_state = 0;
			else
				{	// do the copy
				_Other._Assoc_state->_Retain();
				_Assoc_state = _Other._Assoc_state;
				_Get_only_once = _Other._Get_only_once;
				}
			}
		}

	void _Move_from(_State_manager& _Other)
		{	// move stored associated asynchronous state object from _Other
		if (this != &_Other)
			{	// different, move
			if (_Assoc_state)
				_Assoc_state->_Release();
			_Assoc_state = _Other._Assoc_state;
			_Other._Assoc_state = 0;
			_Get_only_once = _Other._Get_only_once;
			}
		}

	bool _Is_ready() const
		{	// return status
		return (_Assoc_state && _Assoc_state->_Is_ready());
		}

private:
	_Associated_state<_Ty> *_Assoc_state;
	bool _Get_only_once;
	};

	// TEMPLATE CLASS future
template<class _Ty>
	class shared_future;

template<class _Ty>
	class future
		: public _State_manager<_Ty>
	{	// class that defines a non-copyable asynchronous return object
		// that holds a value
	typedef _State_manager<_Ty> _Mybase;
public:
	future() _NOEXCEPT
		{	// construct
		}

	future(future&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<future>(_Other), true)
		{	// construct from rvalue future object
		}

	future& operator=(future&& _Right) _NOEXCEPT
		{	// assign from rvalue future object
		_Mybase::operator=(_STD forward<future>(_Right));
		return (*this);
		}

	future(const _Mybase& _State, _Nil)
		: _Mybase(_State, true)
		{	// construct from associated asynchronous state object
		}

	~future() _NOEXCEPT
		{	// destroy
		}

	_Ty get()
		{	// block until ready then return the stored result or
			// throw the stored exception
		return (_STD move(this->_Get_value()));
		}

	shared_future<_Ty> share()
		{	// return state as shared_future
		return (shared_future<_Ty>(_STD move(*this)));
		}

	future(const future&) = delete;
	future& operator=(const future&) = delete;
	};

template<class _Ty>
	class future<_Ty&>
		: public _State_manager<_Ty *>
	{	// class that defines a non-copyable asynchronous return object
		// that holds a reference
	typedef _State_manager<_Ty *> _Mybase;
public:
	future() _NOEXCEPT
		{	// construct
		}

	future(future&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<future>(_Other), true)
		{	// construct from rvalue future object
		}

	future& operator=(future&& _Right) _NOEXCEPT
		{	// assign from rvalue future object
		_Mybase::operator=(_STD forward<future>(_Right));
		return (*this);
		}

	future(const _Mybase& _State, _Nil)
		: _Mybase(_State, true)
		{	// construct from associated asynchronous state object
		}

	~future() _NOEXCEPT
		{	// destroy
		}

	_Ty& get()
		{	// block until ready then return the stored result or
			// throw the stored exception
		return (*this->_Get_value());
		}

	shared_future<_Ty&> share()
		{	// return state as shared_future
		return (shared_future<_Ty&>(_STD move(*this)));
		}

	future(const future&) = delete;
	future& operator=(const future&) = delete;
	};

template<>
	class future<void>
		: public _State_manager<int>
	{	// class that defines a non-copyable asynchronous return object
		// that does not hold a value
	typedef _State_manager<int> _Mybase;
public:
	future() _NOEXCEPT
		{	// construct
		}

	future(future&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<future>(_Other), true)
		{	// construct from rvalue future object
		}

	future& operator=(future&& _Right) _NOEXCEPT
		{	// assign from rvalue future object
		_Mybase::operator=(_STD forward<future>(_Right));
		return (*this);
		}

	future(const _Mybase& _State, _Nil)
		: _Mybase(_State, true)
		{	// construct from associated asynchronous state object
		}

	~future() _NOEXCEPT
		{	// destroy
		}

	void get()
		{	// block until ready then return or
			// throw the stored exception
		this->_Get_value();
		}

	shared_future<void> share();

	future(const future&) = delete;
	future& operator=(const future&) = delete;
	};

	// TEMPLATE CLASS shared_future
template<class _Ty>
	class shared_future
	: public _State_manager<_Ty>
	{	// class that defines a copyable asynchronous return object
		// that holds a value
	typedef _State_manager<_Ty> _Mybase;

public:
	shared_future() _NOEXCEPT
		{	// construct
		}

	shared_future(const shared_future& _Other)
		: _Mybase(_Other)
		{	// construct from shared_future object
		}

	shared_future& operator=(const shared_future& _Right)
		{	// assign from shared_future object
		_Mybase::operator=(_Right);
		return (*this);
		}

	shared_future(future<_Ty>&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<_Mybase>(_Other))
		{	// construct from rvalue future object
		}

	shared_future(shared_future&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<shared_future>(_Other))
		{	// construct from rvalue shared_future object
		}

	shared_future& operator=(shared_future&& _Right) _NOEXCEPT
		{	// assign from shared_future rvalue object
		_Mybase::operator=(_STD forward<shared_future>(_Right));
		return (*this);
		}

	~shared_future() _NOEXCEPT
		{	// destroy
		}

	const _Ty& get() const
		{	// block until ready then return the stored result or
			// throw the stored exception
		return (this->_Get_value());
		}
	};

template<class _Ty>
	class shared_future<_Ty&>
		: public _State_manager<_Ty *>
	{	// class that defines a copyable asynchronous return object
		// that holds a reference
	typedef _State_manager<_Ty *> _Mybase;

public:
	shared_future() _NOEXCEPT
		{	// construct
		}

	shared_future(const shared_future& _Other)
		: _Mybase(_Other)
		{	// construct from shared_future object
		}

	shared_future& operator=(const shared_future& _Right)
		{	// assign from shared_future object
		_Mybase::operator=(_Right);
		return (*this);
		}

	shared_future(future<_Ty&>&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<_Mybase>(_Other))
		{	// construct from rvalue future object
		}

	shared_future(shared_future&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<shared_future>(_Other))
		{	// construct from rvalue shared_future object
		}

	shared_future& operator=(shared_future&& _Right) _NOEXCEPT
		{	// assign from rvalue shared_future object
		_Mybase::operator=(_STD forward<shared_future>(_Right));
		return (*this);
		}

	~shared_future() _NOEXCEPT
		{	// destroy
		}

	_Ty& get() const
		{	// block until ready then return the stored result or
			// throw the stored exception
		return (*this->_Get_value());
		}
	};

template<>
	class shared_future<void>
	: public _State_manager<int>
	{	// class that defines a copyable asynchronous return object
		// that does not hold a value
	typedef _State_manager<int> _Mybase;

public:
	shared_future() _NOEXCEPT
		{	// construct
		}

	shared_future(const shared_future& _Other)
		: _Mybase(_Other)
		{	// construct from shared_future object
		}

	shared_future& operator=(const shared_future& _Right)
		{	// assign from shared_future object
		_Mybase::operator=(_Right);
		return (*this);
		}

	shared_future(shared_future&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<shared_future>(_Other))
		{	// construct from rvalue shared_future object
		}

	shared_future(future<void>&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<_Mybase>(_Other))
		{	// construct from rvalue future object
		}

	shared_future& operator=(shared_future&& _Right)
		{	// assign from rvalue shared_future object
		_Mybase::operator=(_STD forward<shared_future>(_Right));
		return (*this);
		}

	~shared_future() _NOEXCEPT
		{	// destroy
		}

	void get() const
		{	// block until ready then return or
			// throw the stored exception
		this->_Get_value();
		}
	};

	// DEFINITION OF future<void>::share()
inline shared_future<void> future<void>::share()
	{	// return state as shared_future
	return (shared_future<void>(_STD move(*this)));
	}

	// TEMPLATE CLASS _Promise
template<class _Ty>
	class _Promise
	{	// class that implements core of promise
public:
	_Promise(_Associated_state<_Ty> *_State_ptr)
		: _State(_State_ptr, false),
			_Future_retrieved(false)
		{	// construct from associated asynchronous state object
		}

	_Promise(_Promise&& _Other)
		: _State(_STD forward<_State_manager<_Ty> >(_Other._State)),
			_Future_retrieved(_Other._Future_retrieved)
		{	// construct from rvalue _Promise object
		}

	_Promise& operator=(_Promise&& _Other)
		{	// assign from rvalue _Promise object
		_State = _STD move(_Other._State);
		_Future_retrieved = _Other._Future_retrieved;
		return (*this);
		}

	~_Promise() _NOEXCEPT
		{	// destroy
		}

	void _Swap(_Promise& _Other)
		{	// exchange with _Other
		_State._Swap(_Other._State);
		_STD swap(_Future_retrieved, _Other._Future_retrieved);
		}

	const _State_manager<_Ty>& _Get_state() const
		{	// return reference to associated asynchronous state object
		return (_State);
		}
	_State_manager<_Ty>& _Get_state()
		{	// return reference to associated asynchronous state object
		return (_State);
		}

	_State_manager<_Ty>& _Get_state_for_set()
		{	// return reference to associated asynchronous state object, or
			// throw exception if not valid for setting state
		if (!_State.valid())
			_Throw_future_error(
				make_error_code(future_errc::no_state));
		return (_State);
		}

	_State_manager<_Ty>& _Get_state_for_future()
		{	// return reference to associated asynchronous state object, or
			// throw exception if not valid for retrieving future
		if (!_State.valid())
			_Throw_future_error(
				make_error_code(future_errc::no_state));
		if (_Future_retrieved)
			_Throw_future_error(
				make_error_code(future_errc::future_already_retrieved));
		_Future_retrieved = true;
		return (_State);
		}

	bool _Is_valid() const _NOEXCEPT
		{	// return status
		return (_State.valid());
		}

	bool _Is_ready() const
		{	// return status
		return (_State._Is_ready());
		}

	_Promise(const _Promise&) = delete;
	_Promise& operator=(const _Promise&) = delete;

private:
	_State_manager<_Ty> _State;
	bool _Future_retrieved;
	};

	// TEMPLATE CLASS promise
template<class _Ty>
	class promise
	{	// class that defines an asynchronous provider that holds a value
public:
	promise()
		: _MyPromise(new _Associated_state<_Ty>)
		{	// construct
		}

	template<class _Alloc>
		promise(allocator_arg_t, const _Alloc& _Al)
		: _MyPromise(_Make_associated_state<_Ty>(_Al))
		{	// construct with allocator
		}

	promise(promise&& _Other) _NOEXCEPT
		: _MyPromise(_STD forward<_Promise<_Ty> >(_Other._MyPromise))
		{	// construct from rvalue promise object
		}

	promise& operator=(promise&& _Other) _NOEXCEPT
		{	// assign from rvalue promise object
		_MyPromise = _STD forward<_Promise<_Ty> >(_Other._MyPromise);
		return (*this);
		}

	~promise() _NOEXCEPT
		{	// destroy
		if (_MyPromise._Is_valid() && !_MyPromise._Is_ready())
			{	// exception if destroyed before function object returns
			future_error _Fut(make_error_code(future_errc::broken_promise));
			_MyPromise._Get_state()
				._Set_exception(_XSTD make_exception_ptr(_Fut), false);
			}
		}

	void swap(promise& _Other) _NOEXCEPT
		{	// exchange with _Other
		_MyPromise._Swap(_Other._MyPromise);
		}

	future<_Ty> get_future()
		{	// return a future object that shares the associated
			// asynchronous state
		return (future<_Ty>(_MyPromise._Get_state_for_future(), _Nil()));
		}

	void set_value(const _Ty& _Val)
		{	// store result
		_MyPromise._Get_state_for_set()._Set_value(_Val, false);
		}

	void set_value_at_thread_exit(const _Ty& _Val)
		{	// store result and block until thread exit
		_MyPromise._Get_state_for_set()._Set_value(_Val, true);
		}

	void set_value(_Ty&& _Val)
		{	// store result
		_MyPromise._Get_state_for_set()._Set_value(
			_STD forward<_Ty>(_Val), false);
		}

	void set_value_at_thread_exit(_Ty&& _Val)
		{	// store result and block until thread exit
		_MyPromise._Get_state_for_set()._Set_value(
			_STD forward<_Ty>(_Val), true);
		}

	void set_exception(_XSTD exception_ptr _Exc)
		{	// store result
		_MyPromise._Get_state_for_set()._Set_exception(_Exc, false);
		}

	void set_exception_at_thread_exit(_XSTD exception_ptr _Exc)
		{	// store result and block until thread exit
		_MyPromise._Get_state_for_set()._Set_exception(_Exc, true);
		}

	promise(const promise&) = delete;
	promise& operator=(const promise&) = delete;

private:
	_Promise<_Ty> _MyPromise;
	};

template<class _Ty>
	class promise<_Ty&>
	{	// class that defines an asynchronous provider that holds a reference
public:
	promise()
		: _MyPromise(new _Associated_state<_Ty *>)
		{	// construct
		}

	template<class _Alloc>
		promise(allocator_arg_t, const _Alloc& _Al)
		: _MyPromise(_Make_associated_state<_Ty *>(_Al))
		{	// construct with allocator
		}

	promise(promise&& _Other) _NOEXCEPT
		: _MyPromise(_STD forward<_Promise<_Ty *> >(_Other._MyPromise))
		{	// construct from rvalue promise object
		}

	promise& operator=(promise&& _Other) _NOEXCEPT
		{	// assign from rvalue promise object
		_MyPromise = _STD forward<_Promise<_Ty *> >(_Other._MyPromise);
		return (*this);
		}

	~promise() _NOEXCEPT
		{	// destroy
		if (_MyPromise._Is_valid() && !_MyPromise._Is_ready())
			{	// exception if destroyed before function object returns
			future_error _Fut(make_error_code(future_errc::broken_promise));
			_MyPromise._Get_state()
				._Set_exception(_XSTD make_exception_ptr(_Fut), false);
			}
		}

	void swap(promise& _Other) _NOEXCEPT
		{	// exchange with _Other
		_MyPromise._Swap(_Other._MyPromise);
		}

	future<_Ty&> get_future()
		{	// return a future object that shares the associated
			// asynchronous state
		return (future<_Ty&>(_MyPromise._Get_state_for_future(), _Nil()));
		}

	void set_value(_Ty& _Val)
		{	// store result
		_MyPromise._Get_state_for_set()._Set_value(&_Val, false);
		}

	void set_value_at_thread_exit(_Ty& _Val)
		{	// store result and block until thread exit
		_MyPromise._Get_state_for_set()._Set_value(&_Val, true);
		}

	void set_exception(_XSTD exception_ptr _Exc)
		{	// store result
		_MyPromise._Get_state_for_set()._Set_exception(_Exc, false);
		}

	void set_exception_at_thread_exit(_XSTD exception_ptr _Exc)
		{	// store result and block until thread exit
		_MyPromise._Get_state_for_set()._Set_exception(_Exc, true);
		}

	promise(const promise&) = delete;
	promise& operator=(const promise&) = delete;

private:
	_Promise<_Ty *> _MyPromise;
	};

template<>
	class promise<void>
	{	// defines an asynchronous provider that does not hold a value
public:
	promise()
		: _MyPromise(new _Associated_state<int>)
		{	// construct
		}

	template<class _Alloc>
		promise(allocator_arg_t, const _Alloc& _Al)
		: _MyPromise(_Make_associated_state<int>(_Al))
		{	// construct with allocator
		}

	promise(promise&& _Other) _NOEXCEPT
		: _MyPromise(_STD forward<_Promise<int> >(_Other._MyPromise))
		{	// construct from rvalue promise object
		}

	promise& operator=(promise&& _Other) _NOEXCEPT
		{	// assign from rvalue promise object
		_MyPromise = _STD forward<_Promise<int> >(_Other._MyPromise);
		return (*this);
		}

	~promise() _NOEXCEPT
		{	// destroy
		if (_MyPromise._Is_valid() && !_MyPromise._Is_ready())
			{	// exception if destroyed before function object returns
			future_error _Fut(make_error_code(future_errc::broken_promise));
			_MyPromise._Get_state()
				._Set_exception(_XSTD make_exception_ptr(_Fut), false);
			}
		}

	void swap(promise& _Other) _NOEXCEPT
		{	// exchange with _Other
		_MyPromise._Swap(_Other._MyPromise);
		}

	future<void> get_future()
		{	// return a future object that shares the associated
			// asynchronous state
		return (future<void>(_MyPromise._Get_state_for_future(), _Nil()));
		}

	void set_value()
		{	// store a (void) result
		_MyPromise._Get_state_for_set()._Set_value(1, false);
		}

	void set_value_at_thread_exit()
		{	// store result and block until thread exit
		_MyPromise._Get_state_for_set()._Set_value(1, true);
		}

	void set_exception(_XSTD exception_ptr _Exc)
		{	// store a result
		_MyPromise._Get_state_for_set()._Set_exception(_Exc, false);
		}

	void set_exception_at_thread_exit(_XSTD exception_ptr _Exc)
		{	// store result and block until thread exit
		_MyPromise._Get_state_for_set()._Set_exception(_Exc, true);
		}

	promise(const promise&) = delete;
	promise& operator=(const promise&) = delete;

private:
	_Promise<int> _MyPromise;
	};

template<class _Ty> inline
	void swap(promise<_Ty>& _Left, promise<_Ty>& _Right) _NOEXCEPT
	{	// exchange _Left and _Right
	_Left.swap(_Right);
	}

	// TEMPLATE CLASS packaged_task
template<class _Fret>
	struct _P_arg_type
	{	// type for functions returning T
	typedef _Fret type;
	};

template<class _Fret>
	struct _P_arg_type<_Fret&>
	{	// type for functions returning reference to T
	typedef _Fret *type;
	};

template<>
	struct _P_arg_type<void>
	{	// type for functions returning void
	typedef int type;
	};

template<class>
	class packaged_task;	// not defined

template<class _Ret,
	class... _ArgTypes>
	class packaged_task<_Ret(_ArgTypes...)>
	{	// class that defines an asynchronous provider that returns the
		// result of a call to a function object
public:
	typedef packaged_task<_Ret(_ArgTypes...)> _Myt;
	typedef typename _P_arg_type<_Ret>::type _Ptype;
	typedef _Promise<_Ptype> _MyPromiseType;
	typedef _State_manager<_Ptype> _MyStateManagerType;
	typedef _Packaged_state<_Ret(_ArgTypes...)> _MyStateType;

	packaged_task() _NOEXCEPT
		: _MyPromise(0)
		{	// construct
		}

	template<class _Fty2,
		class = typename enable_if<
			!is_same<typename decay<_Fty2>::type, _Myt>::value>::type>
		explicit packaged_task(_Fty2&& _Fnarg)
		: _MyPromise(new _MyStateType(_STD forward<_Fty2>(_Fnarg)))
		{	// construct from rvalue function object
		}

	packaged_task(packaged_task&& _Other) _NOEXCEPT
		: _MyPromise(_STD move(_Other._MyPromise))
		{	// construct from rvalue packaged_task object
		}

	packaged_task& operator=(packaged_task&& _Other) _NOEXCEPT
		{	// assign from rvalue packaged_task object
		_MyPromise = _STD move(_Other._MyPromise);
		return (*this);
		}

	template<class _Fty2,
		class _Alloc,
		class = typename enable_if<
			!is_same<typename decay<_Fty2>::type, _Myt>::value>::type>
		packaged_task(allocator_arg_t, const _Alloc& _Al, _Fty2&& _Fnarg)
		: _MyPromise(_Make_packaged_state<_MyStateType>(
			_STD forward<_Fty2>(_Fnarg), _Al))
		{	// construct from rvalue function object and allocator
		}

	~packaged_task() _NOEXCEPT
		{	// destroy
		_MyPromise._Get_state()._Abandon();
		}

	void swap(packaged_task& _Other) _NOEXCEPT
		{	// exchange with _Other
		_STD swap(_MyPromise, _Other._MyPromise);
		}

	bool valid() const _NOEXCEPT
		{	// return status
		return (_MyPromise._Is_valid());
		}

	future<_Ret> get_future()
		{	// return a future object that shares the associated
			// asynchronous state
		return (future<_Ret>(_MyPromise._Get_state_for_future(), _Nil()));
		}

	void operator()(_ArgTypes... _Args)
		{	// call the function object
		if (_MyPromise._Is_ready())
			_Throw_future_error(
				make_error_code(future_errc::promise_already_satisfied));
		_MyStateManagerType& _State = _MyPromise._Get_state_for_set();
		_MyStateType *_Ptr = static_cast<_MyStateType *>(_State._Ptr());
		_Ptr->_Call_immediate(_STD forward<_ArgTypes>(_Args)...);
		}

	void make_ready_at_thread_exit(_ArgTypes... _Args)
		{	// call the function object and block until thread exit
		if (_MyPromise._Is_ready())
			_Throw_future_error(
				make_error_code(future_errc::promise_already_satisfied));
		_MyStateManagerType& _State = _MyPromise._Get_state_for_set();
		if (_State._Ptr()->_Already_has_stored_result())
			_Throw_future_error(
				make_error_code(future_errc::promise_already_satisfied));
		_MyStateType *_Ptr = static_cast<_MyStateType *>(_State._Ptr());
		_Ptr->_Call_deferred(_STD forward<_ArgTypes>(_Args)...);
		}

	void reset()
		{	// reset to newly constructed state
		_MyStateManagerType& _State = _MyPromise._Get_state_for_set();
		_MyStateType *_MyState = static_cast<_MyStateType *>(_State._Ptr());
		function<_Ret(_ArgTypes...)> _Fnarg = _MyState->_Get_fn();
		_MyPromiseType _New_promise(new _MyStateType(_Fnarg));
		_MyPromise._Get_state()._Abandon();
		_MyPromise._Swap(_New_promise);
		}

	packaged_task(const packaged_task&) = delete;
	packaged_task& operator=(const packaged_task&) = delete;

private:
	_MyPromiseType _MyPromise;
	};


template<class _Ty> inline
	void swap(packaged_task<_Ty>& _Left,
		packaged_task<_Ty>& _Right) _NOEXCEPT
	{	// exchange _Left and _Right
	_Left.swap(_Right);
	}

		// TEMPLATE FUNCTION async
template<class... _Types>
	class _Fake_no_copy_callable_adapter
	{	// async() is built on packaged_task internals which incorrectly use std::function, which requires that things
		// be copyable. We can't fix this in an update, so this adapter turns copies into terminate(). When VSO#153581
		// is fixed, remove this adapter.
	typedef _Fake_no_copy_callable_adapter<_Types...> _Myt;
	typedef tuple<decay_t<_Types>...> _Storaget;

public:
	explicit _Fake_no_copy_callable_adapter(_Types&&... _Vals)
		: _Storage(_STD forward<_Types>(_Vals)...)
		{	// Initializes _Fake_no_copy_callable_adapter with a decayed callable object and arguments
		}

	_Fake_no_copy_callable_adapter(const _Myt& _Other)
		: _Storage(_STD move(_Other._Storage))
		{	// Very Bad Things^TM
		_STD terminate();
		}

	_Fake_no_copy_callable_adapter(_Myt&& _Other) = default;
	_Myt& operator=(const _Myt&) = delete;
	_Myt& operator=(_Myt&&) = delete;

	auto operator()() -> decltype(_Invoke_stored(_STD move(_STD declval<_Storaget&>())))
		{	// Invokes stored callable object
		return (_Invoke_stored(_STD move(_Storage)));
		}

private:
	mutable _Storaget _Storage;
	};

template<class _Ret,
	class _Fty> inline
	_Associated_state<typename _P_arg_type<_Ret>::type>
		*_Get_associated_state(_Launch_type _Psync, _Fty&& _Fnarg)
	{	// construct associated asynchronous state object for the launch type
	switch (_Psync)
		{	// select launch type
	case launch::async:
		return (new _Task_async_state<_Ret, false>(
			_STD forward<_Fty>(_Fnarg)));
	case launch::deferred:
		return (new _Deferred_async_state<_Ret>(
			_STD forward<_Fty>(_Fnarg)));
	default:
		return (new _Task_async_state<_Ret, true>(
			_STD forward<_Fty>(_Fnarg)));
		}
	}

template<class _Fty,
	class... _ArgTypes> inline
	future<result_of_t<decay_t<_Fty>(decay_t<_ArgTypes>...)>>
		async(launch _Policy, _Fty&& _Fnarg, _ArgTypes&&... _Args)
	{	// return a future object whose associated asynchronous state
		// manages a callable object launched with supplied policy
	typedef result_of_t<decay_t<_Fty>(decay_t<_ArgTypes>...)> _Ret;
	typedef typename _P_arg_type<_Ret>::type _Ptype;
	_Promise<_Ptype> _Pr(_Get_associated_state<_Ret>(_Policy,
		_Fake_no_copy_callable_adapter<_Fty, _ArgTypes...>(
			_STD forward<_Fty>(_Fnarg),
			_STD forward<_ArgTypes>(_Args)...
		)));

	return (future<_Ret>(_Pr._Get_state_for_future(), _Nil()));
	}

template<class _Fty,
	class... _ArgTypes> inline
	future<result_of_t<decay_t<_Fty>(decay_t<_ArgTypes>...)>>
		async(_Fty&& _Fnarg, _ArgTypes&&... _Args)
	{	// return a future object whose associated asynchronous state
		// manages a callable object launched with default policy
	return (_STD async(launch::async | launch::deferred,
		_STD forward<_Fty>(_Fnarg),
		_STD forward<_ArgTypes>(_Args)...
		));
	}

 #ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
namespace experimental {
template<class _Ty,
	class... _ArgTypes>
	struct coroutine_traits<future<_Ty>, _ArgTypes...>
	{	// defines resumable traits for functions returning future<_Ty>
	struct promise_type
		{
		promise<_Ty> _MyPromise;

		future<_Ty> get_return_object()
			{
			return (_MyPromise.get_future());
			}

		bool initial_suspend() const
			{
			return (false);
			}

		bool final_suspend() const
			{
			return (false);
			}

		template<class _Ut>
			void return_value(_Ut&& _Value)
			{
			_MyPromise.set_value(_STD forward<_Ut>(_Value));
			}

		void set_exception(exception_ptr _Exc)
			{
			_MyPromise.set_exception(_STD move(_Exc));
			}
		};
	};

template<class... _ArgTypes>
	struct coroutine_traits<future<void>, _ArgTypes...>
	{	// defines resumable traits for functions returning future<void>
	struct promise_type
		{
		promise<void> _MyPromise;

		future<void> get_return_object()
			{
			return (_MyPromise.get_future());
			}

		bool initial_suspend() const
			{
			return (false);
			}

		bool final_suspend() const
			{
			return (false);
			}

		void return_void()
			{
			_MyPromise.set_value();
			}

		void set_exception(exception_ptr _Exc)
			{
			_MyPromise.set_exception(_STD move(_Exc));
			}
		};
	};
}	// namespace experimental

template<class _Ty>
	bool await_ready(future<_Ty>& _Fut)
	{
	return (_Fut._Is_ready());
	}

template<class _Ty>
	void await_suspend(future<_Ty>& _Fut,
		experimental::coroutine_handle<> _ResumeCb)
	{	// change to .then when future gets .then
	thread _WaitingThread([&_Fut, _ResumeCb]{
		_Fut.wait();
		_ResumeCb();
	});
	_WaitingThread.detach();
	}

template<class _Ty>
	auto await_resume(future<_Ty>& _Fut)
	{
	return (_Fut.get());
	}
 #endif /* _RESUMABLE_FUNCTIONS_SUPPORTED */
_STD_END

namespace std {
template<class _Ty,
	class _Alloc>
	struct uses_allocator<promise<_Ty>, _Alloc>
		: true_type
	{	// asserts that promise<_Ty> can use an allocator
	};

template<class _Ty,
	class _Alloc>
	struct uses_allocator<packaged_task<_Ty>, _Alloc>
		: true_type
	{	// asserts that packaged_task<_Ty> can use an allocator
	};
}	// namespace std
 #pragma pop_macro("new")
 #pragma warning(pop)
 #pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _FUTURE_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
