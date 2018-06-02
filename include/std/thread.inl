// thread standard header
#pragma once
#ifndef _THREAD_
#define _THREAD_
#ifndef RC_INVOKED

 #ifdef _M_CEE
  #error <thread> is not supported when compiling with /clr or /clr:pure.
 #endif /* _M_CEE */

#include <exception>
#include <iosfwd>
#include <functional>
#include <chrono>
#include <memory>
#include <tuple>

#include <thr/xthread>
#include <thr/xtimec.h>

 #pragma pack(push,_CRT_PACKING)
 #pragma warning(push,3)
 #pragma push_macro("new")
 #undef new

 #pragma warning(disable: 4521 4522 4800)

_STD_BEGIN
class thread
	{	// class for observing and managing threads
public:
	class id;

	typedef void *native_handle_type;

	thread() _NOEXCEPT
		{	// construct with no thread
		_Thr_set_null(_Thr);
		}


	template<class _Fn,
		class... _Args,
		class = typename enable_if<
			!is_same<typename decay<_Fn>::type, thread>::value>::type>
		explicit thread(_Fn&& _Fx, _Args&&... _Ax)
		{	// construct with _Fx(_Ax...)
		_Launch(&_Thr,
			_STD make_unique<tuple<decay_t<_Fn>, decay_t<_Args>...> >(
				_STD forward<_Fn>(_Fx), _STD forward<_Args>(_Ax)...));
		}


	~thread() _NOEXCEPT
		{	// clean up
		if (joinable())
			_XSTD terminate();
		}

	thread(thread&& _Other) _NOEXCEPT
		: _Thr(_Other._Thr)
		{	// move from _Other
		_Thr_set_null(_Other._Thr);
		}

	thread& operator=(thread&& _Other) _NOEXCEPT
		{	// move from _Other
		return (_Move_thread(_Other));
		}

	thread(const thread&) = delete;
	thread& operator=(const thread&) = delete;

	void swap(thread& _Other) _NOEXCEPT
		{	// swap with _Other
		_STD swap(_Thr, _Other._Thr);
		}

	bool joinable() const _NOEXCEPT
		{	// return true if this thread can be joined
		return (!_Thr_is_null(_Thr));
		}

	void join();

	void detach()
		{	// detach thread
		if (!joinable())
			_Throw_Cpp_error(_INVALID_ARGUMENT);
		_Thrd_detachX(_Thr);
		_Thr_set_null(_Thr);
		}

	id get_id() const _NOEXCEPT;

	static unsigned int hardware_concurrency() _NOEXCEPT
		{	// return number of hardware thread contexts
		return (_Thrd_hardware_concurrency());
		}

	native_handle_type native_handle()
		{	// return Win32 HANDLE as void *
		return (_Thr._Hnd);
		}

private:
	thread& _Move_thread(thread& _Other)
		{	// move from _Other
		if (joinable())
			_XSTD terminate();
		_Thr = _Other._Thr;
		_Thr_set_null(_Other._Thr);
		return (*this);
		}

	_Thrd_t _Thr;
	};

	namespace this_thread {
thread::id get_id() _NOEXCEPT;

inline void yield() _NOEXCEPT
	{	// give up balance of time slice
	_Thrd_yield();
	}

inline void sleep_until(const stdext::threads::xtime *_Abs_time)
	{	// sleep until _Abs_time
	_Thrd_sleep(_Abs_time);
	}

template<class _Rep,
	class _Period> inline
	void sleep_for(const chrono::duration<_Rep, _Period>& _Rel_time)
	{	// sleep for duration
	stdext::threads::xtime _Tgt = _To_xtime(_Rel_time);
	this_thread::sleep_until(&_Tgt);
	}

template<class _Clock,
	class _Duration> inline
	void sleep_until(
		const chrono::time_point<_Clock, _Duration>& _Abs_time)
	{	// sleep until time point
	this_thread::sleep_for(_Abs_time.time_since_epoch() - _Clock::now().time_since_epoch());
	}
	}	// namespace this_thread

class thread::id
	{	// thread id
public:
	id() _NOEXCEPT
		: _Id(0)
		{	// id for no thread
		}

	template<class _Ch,
		class _Tr>
		basic_ostream<_Ch, _Tr>& _To_text(
			basic_ostream<_Ch, _Tr>& _Str)
		{	// insert representation into stream
		return (_Str << _Id);
		}

private:
	id(_Thrd_id_t _Other_id)
		: _Id(_Other_id)
		{	// construct from unique id
		}

	size_t _Hash_id() const
		{	// hash bits to size_t value by pseudorandomizing transform
		return (hash<_Thrd_id_t>()(_Id));
		}

	_Thrd_id_t _Id;

	friend thread::id thread::get_id() const _NOEXCEPT;
	friend thread::id this_thread::get_id() _NOEXCEPT;
	friend bool operator==(thread::id _Left, thread::id _Right) _NOEXCEPT;
	friend bool operator<(thread::id _Left, thread::id _Right) _NOEXCEPT;
	friend struct hash<thread::id>;
	};

inline void thread::join()
	{	// join thread
	if (!joinable())
		_Throw_Cpp_error(_INVALID_ARGUMENT);
	const bool _Is_null = _Thr_is_null(_Thr);	// Avoid Clang -Wparentheses-equality
	if (_Is_null)
		_Throw_Cpp_error(_INVALID_ARGUMENT);
	if (get_id() == _STD this_thread::get_id())
		_Throw_Cpp_error(_RESOURCE_DEADLOCK_WOULD_OCCUR);
	if (_Thrd_join(_Thr, 0) != _Thrd_success)
		_Throw_Cpp_error(_NO_SUCH_PROCESS);
	_Thr_set_null(_Thr);
	}

inline thread::id thread::get_id() const _NOEXCEPT
	{	// return id for this thread
	return (_Thr_val(_Thr));
	}

inline thread::id this_thread::get_id() _NOEXCEPT
	{	// return id for current thread
	return (_Thrd_id());
	}

inline void swap(thread& _Left, thread& _Right) _NOEXCEPT
	{	// swap _Left with _Right
	_Left.swap(_Right);
	}

inline bool operator==(thread::id _Left, thread::id _Right) _NOEXCEPT
	{	// return true if _Left and _Right identify the same thread
	return (_Left._Id == _Right._Id);
	}

inline bool operator!=(thread::id _Left, thread::id _Right) _NOEXCEPT
	{	// return true if _Left and _Right do not identify the same thread
	return (!(_Left == _Right));
	}

inline bool operator<(thread::id _Left, thread::id _Right) _NOEXCEPT
	{	// return true if _Left precedes _Right
	return (_Left._Id < _Right._Id);
	}

inline bool operator<=(thread::id _Left, thread::id _Right) _NOEXCEPT
	{	// return true if _Left precedes or equals _Right
	return (!(_Right < _Left));
	}

inline bool operator>(thread::id _Left, thread::id _Right) _NOEXCEPT
	{	// return true if _Left follows _Right
	return (_Right < _Left);
	}

inline bool operator>=(thread::id _Left, thread::id _Right) _NOEXCEPT
	{	// return true if _Left follows or equals _Right
	return (!(_Left < _Right));
	}

template<class _Ch,
	class _Tr>
	basic_ostream<_Ch, _Tr>& operator<<(
		basic_ostream<_Ch, _Tr>& _Str,
		thread::id _Id)
	{	// insert id into stream
	return (_Id._To_text(_Str));
	}

	// TEMPLATE STRUCT SPECIALIZATION hash
template<>
	struct hash<thread::id>
	{	// hash functor for thread::id
	typedef thread::id argument_type;
	typedef size_t result_type;

	size_t operator()(const argument_type& _Keyval) const
		{	// hash _Keyval to size_t value by pseudorandomizing transform
		return (_Keyval._Hash_id());
		}
	};
_STD_END

 #pragma pop_macro("new")
 #pragma warning(pop)
 #pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _THREAD_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
