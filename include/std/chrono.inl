// chrono standard header
#pragma once
#ifndef _CHRONO_
#define _CHRONO_
#ifndef RC_INVOKED
#include <limits>
#include <ratio>
#include <utility>
#include <time.h>
#include <thr/xtimec.h>

 #pragma pack(push,_CRT_PACKING)
 #pragma warning(push,3)
 #pragma push_macro("new")
 #undef new

 #pragma warning(disable: 4455)
 #pragma warning(disable: 6326)

_STD_BEGIN
namespace chrono {
	// CLASS TEMPLATE treat_as_floating_point
template<class _Rep>
	struct treat_as_floating_point
		: is_floating_point<_Rep>
	{	// tests for floating-point type
	};

 #if _HAS_VARIABLE_TEMPLATES
template<class _Rep>
	constexpr bool treat_as_floating_point_v = treat_as_floating_point<_Rep>::value;
 #endif /* _HAS_VARIABLE_TEMPLATES */

	// CLASS TEMPLATE duration_values
template<class _Rep>
	struct duration_values
	{	// gets arithmetic properties of a type
	static constexpr _Rep zero()
		{	// get zero value
		return (_Rep(0));
		}

	static constexpr _Rep (min)()
		{	// get smallest value
		return (numeric_limits<_Rep>::lowest());
		}

	static constexpr _Rep (max)()
		{	// get largest value
		return ((numeric_limits<_Rep>::max)());
		}
	};

	// CLASS TEMPLATE _Is_ratio
template<class _Ty>
	struct _Is_ratio
	{	// test for ratio type
	static constexpr bool value = false;
	};

template<intmax_t _R1,
	intmax_t _R2>
	struct _Is_ratio<ratio<_R1, _R2> >
	{	// test for ratio type
	static constexpr bool value = true;
	};

	// CLASS TEMPLATE duration
template<class _Rep,
	class _Period = ratio<1> >
	class duration;

template<class _Ty>
	struct _Is_duration
		: false_type
	{	// tests for duration
	};

template<class _Rep,
	class _Period>
	struct _Is_duration<duration<_Rep, _Period> >
		: true_type
	{	// tests for duration
	};

template<class _To,
	class _Rep,
	class _Period> inline
	constexpr typename enable_if<_Is_duration<_To>::value, _To>::type
		duration_cast(const duration<_Rep, _Period>&);

template<class _Rep,
	class _Period>
	class duration
	{	// represents a time duration
public:
	typedef duration<_Rep, _Period> _Myt;
	typedef _Rep rep;
	typedef _Period period;

	static_assert(!_Is_duration<_Rep>::value,
		"duration can't have duration as first template argument");
	static_assert(_Is_ratio<_Period>::value,
		"period not an instance of std::ratio");
	static_assert(0 < _Period::num,
		"period negative or zero");

	constexpr duration() = default;

	template<class _Rep2,
		class = typename enable_if<is_convertible<_Rep2, _Rep>::value
			&& (treat_as_floating_point<_Rep>::value
				|| !treat_as_floating_point<_Rep2>::value),
			void>::type>
		constexpr explicit duration(const _Rep2& _Val)
			: _MyRep(static_cast<_Rep>(_Val))
		{	// construct from representation
		}

	template<class _Rep2,
		class _Period2,
		class = typename enable_if<treat_as_floating_point<_Rep>::value

			|| (_Ratio_divide_sfinae<_Period2, _Period>::den == 1

				&& !treat_as_floating_point<_Rep2>::value),
			void>::type>
		constexpr duration(const duration<_Rep2, _Period2>& _Dur)
			: _MyRep(chrono::duration_cast<_Myt>(_Dur).count())
		{	// construct from a duration
		}

	constexpr _Rep count() const
		{	// get stored rep
		return (_MyRep);
		}

	constexpr _Myt operator+() const
		{	// get value
		return (*this);
		}

	constexpr _Myt operator-() const
		{	// get negated value
		return (_Myt(0 - _MyRep));
		}

	_Myt& operator++()
		{	// increment rep
		++_MyRep;
		return (*this);
		}

	_Myt operator++(int)
		{	// postincrement rep
		return (_Myt(_MyRep++));
		}

	_Myt& operator--()
		{	// decrement rep
		--_MyRep;
		return (*this);
		}

	_Myt operator--(int)
		{	// postdecrement rep
		return (_Myt(_MyRep--));
		}

	_Myt& operator+=(const _Myt& _Right)
		{	// add _Right to rep
		_MyRep += _Right._MyRep;
		return (*this);
		}

	_Myt& operator-=(const _Myt& _Right)
		{	// subtract _Right from rep
		_MyRep -= _Right._MyRep;
		return (*this);
		}

	_Myt& operator*=(const _Rep& _Right)
		{	// multiply rep by _Right
		_MyRep *= _Right;
		return (*this);
		}

	_Myt& operator/=(const _Rep& _Right)
		{	// divide rep by _Right
		_MyRep /= _Right;
		return (*this);
		}

	_Myt& operator%=(const _Rep& _Right)
		{	// modulus rep by _Right
		_MyRep %= _Right;
		return (*this);
		}

	_Myt& operator%=(const _Myt& _Right)
		{	// modulus rep by _Right
		_MyRep %= _Right.count();
		return (*this);
		}

	static constexpr _Myt zero()
		{	// get zero value
		return (_Myt(duration_values<_Rep>::zero()));
		}

	static constexpr _Myt (min)()
		{	// get minimum value
		return (_Myt((duration_values<_Rep>::min)()));
		}
	static constexpr _Myt (max)()
		{	// get maximum value
		return (_Myt((duration_values<_Rep>::max)()));
		}

private:
	_Rep _MyRep;	// the stored rep
	};

template<class _Clock,
	class _Duration = typename _Clock::duration>
	class time_point
	{	// represents a point in time
public:
	typedef _Clock clock;
	typedef _Duration duration;
	typedef typename _Duration::rep rep;
	typedef typename _Duration::period period;

	static_assert(_Is_duration<_Duration>::value,
		"duration must be an instance of std::duration");

	constexpr time_point()
		: _MyDur(_Duration::zero())
		{	// construct with value epoch
		}

	constexpr explicit time_point(const _Duration& _Other)
		: _MyDur(_Other)
		{	// construct from a duration
		}

	template<class _Duration2,
		class = typename enable_if<is_convertible<_Duration2,
			_Duration>::value,
			void>::type>
		constexpr time_point(const time_point<_Clock, _Duration2>& _Tp)
		: _MyDur(_Tp.time_since_epoch())
		{	// construct from another duration
		}

	constexpr _Duration time_since_epoch() const
		{	// get duration from epoch
		return (_MyDur);
		}

	time_point& operator+=(const _Duration& _Dur)
		{	// increment by duration
		_MyDur += _Dur;
		return (*this);
		}

	time_point& operator-=(const _Duration& _Dur)
		{	// decrement by duration
		_MyDur -= _Dur;
		return (*this);
		}

	static constexpr time_point (min)()
		{	// get minimum time point
		return (time_point((_Duration::min)()));
		}
	static constexpr time_point (max)()
		{	// get maximum time point
		return (time_point((_Duration::max)()));
		}

private:
	_Duration _MyDur;	// duration since the epoch
	};
}	// namespace chrono

	// CLASS TEMPLATE _Lcm (LEAST COMMON MULTIPLE)
template<intmax_t _Ax,
	intmax_t _Bx>
	struct _Lcm
	{	/* compute least common multiple of _Ax and _Bx */
	static constexpr intmax_t _Gx = _Gcd<_Ax, _Bx>::value;
	static constexpr intmax_t value = (_Ax / _Gx) * _Bx;
	};

	// CLASS TEMPLATE common_type SPECIALIZATIONS
template<class _Rep1,
	class _Period1,
	class _Rep2,
	class _Period2>
	struct common_type<
		chrono::duration<_Rep1, _Period1>,
		chrono::duration<_Rep2, _Period2> >
	{	// common type of two durations
	typedef chrono::duration<typename common_type<_Rep1, _Rep2>::type,
		ratio<_Gcd<_Period1::num, _Period2::num>::value,
			_Lcm<_Period1::den, _Period2::den>::value> > type;
	};

template<class _Clock,
	class _Duration1,
	class _Duration2>
	struct common_type<
		chrono::time_point<_Clock, _Duration1>,
		chrono::time_point<_Clock, _Duration2> >
	{	// common type of two time points
	typedef chrono::time_point<
		_Clock, typename common_type<_Duration1, _Duration2>::type> type;
	};

namespace chrono {
	// duration ARITHMETIC
template<class _Rep1,
	class _Period1,
	class _Rep2,
	class _Period2> inline
	constexpr typename common_type<
		duration<_Rep1, _Period1>,
		duration<_Rep2, _Period2> >::type
		operator+(
			const duration<_Rep1, _Period1>& _Left,
			const duration<_Rep2, _Period2>& _Right)
	{	// add two durations
	typedef typename common_type<
		duration<_Rep1, _Period1>,
		duration<_Rep2, _Period2> >::type _CD;
	return (_CD(_CD(_Left).count() + _CD(_Right).count()));
	}

template<class _Rep1,
	class _Period1,
	class _Rep2,
	class _Period2> inline
	constexpr typename common_type<
		duration<_Rep1, _Period1>,
		duration<_Rep2, _Period2> >::type
		operator-(
			const duration<_Rep1, _Period1>& _Left,
			const duration<_Rep2, _Period2>& _Right)
	{	// subtract two durations
	typedef typename common_type<
		duration<_Rep1, _Period1>, duration<_Rep2, _Period2> >::type _CD;
	return (_CD(_CD(_Left).count() - _CD(_Right).count()));
	}

template<class _Rep1,
	class _Period1,
	class _Rep2> inline
	constexpr typename enable_if<is_convertible<_Rep2,
		typename common_type<_Rep1, _Rep2>::type>::value,
		duration<typename common_type<_Rep1, _Rep2>::type, _Period1> >::type
		operator*(
			const duration<_Rep1, _Period1>& _Left,
			const _Rep2& _Right)
	{	// multiply duration by rep
	typedef typename common_type<_Rep1, _Rep2>::type _CR;
	typedef duration<_CR, _Period1> _CD;
	return (_CD(_CD(_Left).count() * _Right));
	}

template<class _Rep1,
	class _Rep2,
	class _Period2> inline
	constexpr typename enable_if<is_convertible<_Rep1,
		typename common_type<_Rep1, _Rep2>::type>::value,
		duration<typename common_type<_Rep1, _Rep2>::type, _Period2> >::type
		operator*(
			const _Rep1& _Left,
			const duration<_Rep2, _Period2>& _Right)
	{	// multiply rep by duration
	return (_Right * _Left);
	}

template<class _CR,
	class _Period1,
	class _Rep2,
	bool = is_convertible<_Rep2, _CR>::value>
	struct _Duration_div_mod1
	{	// return type for duration / rep and duration % rep
	typedef duration<_CR, _Period1> type;
	};

template<class _CR,
	class _Period1,
	class _Rep2>
	struct _Duration_div_mod1<_CR, _Period1, _Rep2, false>
	{	// no return type
	};

template<class _Rep1,
	class _Period1,
	class _Rep2,
	bool = _Is_duration<_Rep2>::value>
	struct _Duration_div_mod
	{	// no return type
	};

template<class _Rep1,
	class _Period1,
	class _Rep2>
	struct _Duration_div_mod<_Rep1, _Period1, _Rep2, false>
		: _Duration_div_mod1<typename common_type<_Rep1, _Rep2>::type,
			_Period1, _Rep2>
	{	// return type for duration / rep and duration % rep
	};

template<class _Rep1,
	class _Period1,
	class _Rep2> inline
	typename _Duration_div_mod<_Rep1, _Period1, _Rep2>::type
		constexpr operator/(
			const duration<_Rep1, _Period1>& _Left,
			const _Rep2& _Right)
	{	// divide duration by rep
	typedef typename common_type<_Rep1, _Rep2>::type _CR;
	typedef duration<_CR, _Period1> _CD;
	return (_CD(_CD(_Left).count() / _Right));
	}

template<class _Rep1,
	class _Period1,
	class _Rep2,
	class _Period2> inline
	constexpr typename common_type<_Rep1, _Rep2>::type
		operator/(
			const duration<_Rep1, _Period1>& _Left,
			const duration<_Rep2, _Period2>& _Right)
	{	// divide duration by duration
	typedef typename common_type<
		duration<_Rep1, _Period1>, duration<_Rep2, _Period2> >::type _CD;
	return (_CD(_Left).count() / _CD(_Right).count());
	}

template<class _Rep1,
	class _Period1,
	class _Rep2> inline
	constexpr typename _Duration_div_mod<_Rep1, _Period1, _Rep2>::type
		operator%(
			const duration<_Rep1, _Period1>& _Left,
			const _Rep2& _Right)
	{	// divide duration by rep
	typedef typename common_type<_Rep1, _Rep2>::type _CR;
	typedef duration<_CR, _Period1> _CD;
	return (_CD(_CD(_Left).count() % _Right));
	}

template<class _Rep1,
	class _Period1,
	class _Rep2,
	class _Period2> inline
	constexpr typename common_type<
		duration<_Rep1, _Period1>, duration<_Rep2, _Period2> >::type
		operator%(
			const duration<_Rep1, _Period1>& _Left,
			const duration<_Rep2, _Period2>& _Right)
	{	// divide duration by duration
	typedef typename common_type<
		duration<_Rep1, _Period1>, duration<_Rep2, _Period2> >::type _CD;
	return (_CD(_CD(_Left).count() % _CD(_Right).count()));
	}

	// duration COMPARISONS
template<class _Rep1,
	class _Period1,
	class _Rep2,
	class _Period2> inline
	constexpr bool operator==(
		const duration<_Rep1, _Period1>& _Left,
		const duration<_Rep2, _Period2>& _Right)
	{	// test if duration == duration
	typedef typename common_type<
		duration<_Rep1, _Period1>,
		duration<_Rep2, _Period2> >::type _CT;
	return (_CT(_Left).count() == _CT(_Right).count());
	}

template<class _Rep1,
	class _Period1,
	class _Rep2,
	class _Period2> inline
	constexpr bool operator!=(
		const duration<_Rep1, _Period1>& _Left,
		const duration<_Rep2, _Period2>& _Right)
	{	// test if duration != duration
	return (!(_Left == _Right));
	}

template<class _Rep1,
	class _Period1,
	class _Rep2,
	class _Period2> inline
	constexpr bool operator<(
		const duration<_Rep1, _Period1>& _Left,
		const duration<_Rep2, _Period2>& _Right)
	{	// test if duration < duration
	typedef typename common_type<
		duration<_Rep1, _Period1>,
		duration<_Rep2, _Period2> >::type _CT;
	return (_CT(_Left).count() < _CT(_Right).count());
	}

template<class _Rep1,
	class _Period1,
	class _Rep2,
	class _Period2> inline
	constexpr bool operator<=(
		const duration<_Rep1, _Period1>& _Left,
		const duration<_Rep2, _Period2>& _Right)
	{	// test if duration <= duration
	return (!(_Right < _Left));
	}

template<class _Rep1,
	class _Period1,
	class _Rep2,
	class _Period2> inline
	constexpr bool operator>(
		const duration<_Rep1, _Period1>& _Left,
		const duration<_Rep2, _Period2>& _Right)
	{	// test if duration > duration
	return (_Right < _Left);
	}

template<class _Rep1,
	class _Period1,
	class _Rep2,
	class _Period2> inline
	constexpr bool operator>=(
		const duration<_Rep1, _Period1>& _Left,
		const duration<_Rep2, _Period2>& _Right)
	{	// test if duration >= duration
	return (!(_Left < _Right));
	}

	// FUNCTION TEMPLATE duration_cast
template<class _To,
	class _Rep,
	class _Period> inline
	constexpr typename enable_if<_Is_duration<_To>::value, _To>::type
		duration_cast(const duration<_Rep, _Period>& _Dur)
	{	// convert duration to another duration; truncate
	typedef ratio_divide<_Period, typename _To::period> _CF;

	typedef typename _To::rep _ToRep;
	typedef typename common_type<_ToRep, _Rep, intmax_t>::type _CR;
	return (_CF::num == 1 && _CF::den == 1
			? static_cast<_To>(static_cast<_ToRep>(_Dur.count()))
		: _CF::num != 1 && _CF::den == 1
			? static_cast<_To>(static_cast<_ToRep>(
				static_cast<_CR>(
					_Dur.count()) * static_cast<_CR>(_CF::num)))
		: _CF::num == 1 && _CF::den != 1
			? static_cast<_To>(static_cast<_ToRep>(
				static_cast<_CR>(_Dur.count())
					/ static_cast<_CR>(_CF::den)))
		: static_cast<_To>(static_cast<_ToRep>(
			static_cast<_CR>(_Dur.count()) * static_cast<_CR>(_CF::num)
				/ static_cast<_CR>(_CF::den))));
	}

	// ALIAS TEMPLATE _As_integral
template<class _Dur>
	using _As_integral = duration<
		typename conditional<treat_as_floating_point<typename _Dur::rep>::value,
			intmax_t,
			typename _Dur::rep>::type,
		typename _Dur::period>;

	// FUNCTION TEMPLATE floor
template<class _Rep,
	class _Period,
	class _To_integral> inline
	constexpr _To_integral _Floor(const duration<_Rep, _Period>& _Dur,
		const _To_integral& _Dur_integral)
	{
	return (_Dur_integral <= _Dur
		? _Dur_integral
		: _Dur_integral - static_cast<_To_integral>(1));
	}

template<class _To,
	class _Rep,
	class _Period> inline
	constexpr typename enable_if<_Is_duration<_To>::value, _To>::type
		floor(const duration<_Rep, _Period>& _Dur)
	{	// convert duration to another duration; round towards negative infinity
		// i.e. the greatest integral result such that the result <= _Dur
	return (_Floor(_Dur, chrono::duration_cast<_As_integral<_To> >(_Dur)));
	}

	// FUNCTION TEMPLATE ceil
template<class _Rep,
	class _Period,
	class _To_integral> inline
	constexpr _To_integral _Ceil(const duration<_Rep, _Period>& _Dur,
		const _To_integral& _Dur_integral)
	{
	return (_Dur <= _Dur_integral
		? _Dur_integral
		: _Dur_integral + static_cast<_To_integral>(1));
	}

template<class _To,
	class _Rep,
	class _Period> inline
	constexpr typename enable_if<_Is_duration<_To>::value, _To>::type
		ceil(const duration<_Rep, _Period>& _Dur)
	{	// convert duration to another duration; round towards positive infinity
		// i.e. the least integral result such that _Dur <= the result
	return (_Ceil(_Dur, chrono::duration_cast<_As_integral<_To> >(_Dur)));
	}

	// FUNCTION TEMPLATE _Make_unsigned
template<class _Rep> inline
	constexpr typename make_unsigned<_Rep>::type _Make_unsigned(_Rep _Val)
	{	// Makes _Val unsigned
	return (static_cast<typename make_unsigned<_Rep>::type>(_Val));
	}

	// FUNCTION TEMPLATE _Is_even
template<class _Rep> inline
	constexpr bool _Is_even(_Rep _Val)
	{	// Tests whether _Val is even
	return (_Val % 2 == 0);
	}

	// FUNCTION TEMPLATE _Bankers_divide
template<class _Rep> inline
	constexpr _Rep _Bankers_divide_impl(_Rep _Truncated, _Rep _Mod,
		_Rep _Half_divisor, bool _Half_divisor_is_exact)
	{	// Implement _Bankers_divide for unsigned numbers
	return (_Truncated + static_cast<_Rep>(
		// strictly closer to positive infinity
		_Mod > _Half_divisor
		// equidistant, ties to even
			|| (_Mod == _Half_divisor
			// The next condition detects _Half_divisor itself being truncated
			&& _Half_divisor_is_exact
			&& !_Is_even(_Truncated)
		)));
	}

template<class _Rep> inline
	constexpr _Rep _Bankers_divide_tag(_Rep _Dividend, _Rep _Divisor, false_type)
	{	// _Bankers_divide for unsigned numbers
	return (_Bankers_divide_impl(
		_Dividend / _Divisor,
		_Dividend % _Divisor,
		_Divisor / static_cast<_Rep>(2),
		_Is_even(_Divisor)));
	}

template<class _Rep> inline
	constexpr _Rep _Bankers_divide_tag(_Rep _Dividend, _Rep _Divisor, true_type)
	{	// Transform to unsigned then transform back
	return (static_cast<_Rep>(duration_values<_Rep>::zero() <= _Dividend
		? _Bankers_divide_tag(_Make_unsigned(_Dividend),
			_Make_unsigned(_Divisor), false_type())
		: duration_values<_Rep>::zero() - _Bankers_divide_tag(
			duration_values<_Rep>::zero() - _Make_unsigned(_Dividend),
			_Make_unsigned(_Divisor), false_type())));
	}

template<class _Rep> inline
	constexpr _Rep _Bankers_divide(_Rep _Dividend, _Rep _Divisor)
	{	// Integer divide _Dividend by _Divisor, round to nearest, ties to even
		// Assumes _Divisor > 0 because all the callers of this put
		// ratio::den as _Divisor, and ratio::den can't be negative or zero by
		// N4527 20.11.3 [ratio.ratio]/2.2
	return (_Bankers_divide_tag(_Dividend, _Divisor, is_signed<_Rep>()));
	}

	// FUNCTION TEMPLATE _Round_fp
template<class _Rep> inline
	constexpr bool _Round_should_truncate(_Rep _Val, intmax_t _Val_trunc)
	{	// Tests if floating point _Val is closer to _Val_trunc, or should tie to even
	return (_Val - static_cast<_Rep>(_Val_trunc) < static_cast<_Rep>(_Val_trunc + 1) - _Val
		|| (_Val - static_cast<_Rep>(_Val_trunc) == static_cast<_Rep>(_Val_trunc + 1) - _Val
			&& _Is_even(_Val_trunc)));
	}

template<class _Rep> inline
	constexpr intmax_t _Round_fp_non_negative(_Rep _Val, intmax_t _Val_trunc)
	{	// Rounds non-negative floating point _Val to nearest integer, ties to even
	return (_Round_should_truncate(_Val, _Val_trunc) ? _Val_trunc : _Val_trunc + 1);
	}

template<class _Rep> inline
	constexpr intmax_t _Round_fp_non_negative(_Rep _Val)
	{	// Rounds non-negative floating point _Val to nearest integer, ties to even
	return (_Round_fp_non_negative(_Val, static_cast<intmax_t>(_Val)));
	}

template<class _Rep> inline
	constexpr _Rep _Round_fp(_Rep _Val)
	{	// Rounds floating point _Val to nearest, ties to even
		// Transform to non-negative, then transform back.
	static_assert(treat_as_floating_point<_Rep>::value,
		"_Round_fp needs floating input");
	return (static_cast<_Rep>(duration_values<_Rep>::zero() <= _Val
		? _Round_fp_non_negative(_Val)
		: (duration_values<_Rep>::zero()
			- _Round_fp_non_negative(duration_values<_Rep>::zero() - _Val))
		));
	}

	// FUNCTION TEMPLATE round
template<class _To,
	class _Rep,
	class _Period> inline
	constexpr _To _Round(const duration<_Rep, _Period>& _Dur,
		false_type, false_type)
	{	// Implement round() when everything is integral
	typedef ratio_divide<_Period, typename _To::period> _CF;

	typedef typename _To::rep _ToRep;
	typedef typename common_type<_ToRep, _Rep, intmax_t>::type _CR;
	return (_CF::num == 1 && _CF::den == 1
			? static_cast<_To>(static_cast<_ToRep>(_Dur.count()))
		: _CF::num != 1 && _CF::den == 1
			? static_cast<_To>(static_cast<_ToRep>(
				static_cast<_CR>(
					_Dur.count()) * static_cast<_CR>(_CF::num)))
		: _CF::num == 1 && _CF::den != 1
			? static_cast<_To>(static_cast<_ToRep>(_Bankers_divide(
				static_cast<_CR>(_Dur.count()), static_cast<_CR>(_CF::den))))
		: static_cast<_To>(static_cast<_ToRep>(
			_Bankers_divide(
				static_cast<_CR>(_Dur.count()) * static_cast<_CR>(_CF::num),
				static_cast<_CR>(_CF::den)))));
	}

template<class _To,
	class _Rep,
	class _Period> inline
	constexpr _To _Round(const duration<_Rep, _Period>& _Dur,
		false_type, true_type)
	{	// Implement round() when the target is integral, source is floating
		// Do any period adjustments in floating point values
		// (in duration_cast), then round to nearest integer, ties to even
	typedef duration<_Rep, typename _To::period> _FloatTo;
	return (static_cast<_To>(static_cast<typename _To::rep>(
		_Round_fp(chrono::duration_cast<_FloatTo>(_Dur).count()))));
	}

template<class _To,
	class _Rep,
	class _Period,
	class _Source_integral> inline
	constexpr _To _Round(const duration<_Rep, _Period>& _Dur,
		true_type, _Source_integral)
	{	// Implement round() when the target is floating, source is anything
		// Convert to floating point, then round to nearest integer, ties to even
	return (static_cast<_To>(_Round_fp(chrono::duration_cast<_To>(_Dur).count())));
	}

template<class _To,
	class _Rep,
	class _Period> inline
	constexpr typename enable_if<_Is_duration<_To>::value, _To>::type
		round(const duration<_Rep, _Period>& _Dur)
	{	// convert duration to another duration, round to nearest, ties to even
	return (_Round<_To>(_Dur,
		treat_as_floating_point<typename _To::rep>(),
		treat_as_floating_point<_Rep>()));
	}

	// FUNCTION TEMPLATE abs
template<class _Rep,
	class _Period> inline
	constexpr typename enable_if<numeric_limits<_Rep>::is_signed,
		duration<_Rep, _Period> >::type
		abs(const duration<_Rep, _Period> _Dur)
	{	// create a duration with count() the absolute value of _Dur.count()
	return (_Dur < duration<_Rep, _Period>::zero()
		? duration<_Rep, _Period>::zero() - _Dur
		: _Dur);
	}

	// duration TYPEDEFS
typedef duration<long long, nano> nanoseconds;
typedef duration<long long, micro> microseconds;
typedef duration<long long, milli> milliseconds;
typedef duration<long long> seconds;
typedef duration<int, ratio<60> > minutes;
typedef duration<int, ratio<3600> > hours;

	// time_point ARITHMETIC
template<class _Clock,
	class _Duration,
	class _Rep,
	class _Period> inline
	constexpr time_point<_Clock,
		typename common_type<_Duration, duration<_Rep, _Period> >::type>
		operator+(
			const time_point<_Clock, _Duration>& _Left,
			const duration<_Rep, _Period>& _Right)
	{	// add duration to time_point
	typedef time_point<_Clock, typename common_type<
		_Duration, duration<_Rep, _Period> >::type> _RT;
	return (_RT(_Left.time_since_epoch() + _Right));
	}

template<class _Rep,
	class _Period,
	class _Clock,
	class _Duration> inline
	constexpr time_point<_Clock,
		typename common_type<duration<_Rep, _Period>, _Duration>::type>
		operator+(
			const duration<_Rep, _Period>& _Left,
			const time_point<_Clock, _Duration>& _Right)
	{	// add time_point to duration
	return (_Right + _Left);
	}

template<class _Clock,
	class _Duration,
	class _Rep,
	class _Period> inline
	constexpr time_point<_Clock,
		typename common_type<_Duration, duration<_Rep, _Period> >::type>
		operator-(
			const time_point<_Clock, _Duration>& _Left,
			const duration<_Rep, _Period>& _Right)
	{	// subtract duration from time_point
	return (_Left + (-_Right));
	}

template<class _Clock,
	class _Duration1,
	class _Duration2> inline
	constexpr typename common_type<_Duration1, _Duration2>::type
		operator-(
			const time_point<_Clock, _Duration1>& _Left,
			const time_point<_Clock, _Duration2>& _Right)
	{	// add time_point to time_point
	return (_Left.time_since_epoch() - _Right.time_since_epoch());
	}

	// time_point COMPARISONS
template<class _Clock,
	class _Duration1,
	class _Duration2> inline
	constexpr bool operator==(
		const time_point<_Clock, _Duration1>& _Left,
		const time_point<_Clock, _Duration2>& _Right)
	{	// test for time_point == time_point
	return (_Left.time_since_epoch() == _Right.time_since_epoch());
	}

template<class _Clock,
	class _Duration1,
	class _Duration2> inline
	constexpr bool operator!=(
		const time_point<_Clock, _Duration1>& _Left,
		const time_point<_Clock, _Duration2>& _Right)
	{	// test for time_point != time_point
	return (!(_Left == _Right));
	}

template<class _Clock,
	class _Duration1,
	class _Duration2> inline
	constexpr bool operator<(
		const time_point<_Clock, _Duration1>& _Left,
		const time_point<_Clock, _Duration2>& _Right)
	{	// test for time_point < time_point
	return (_Left.time_since_epoch() < _Right.time_since_epoch());
	}

template<class _Clock,
	class _Duration1,
	class _Duration2> inline
	constexpr bool operator<=(
		const time_point<_Clock, _Duration1>& _Left,
		const time_point<_Clock, _Duration2>& _Right)
	{	// test for time_point <= time_point
	return (!(_Right < _Left));
	}

template<class _Clock,
	class _Duration1,
	class _Duration2> inline
	constexpr bool operator>(
		const time_point<_Clock, _Duration1>& _Left,
		const time_point<_Clock, _Duration2>& _Right)
	{	// test for time_point > time_point
	return (_Right < _Left);
	}

template<class _Clock,
	class _Duration1,
	class _Duration2> inline
	constexpr bool operator>=(
		const time_point<_Clock, _Duration1>& _Left,
		const time_point<_Clock, _Duration2>& _Right)
	{	// test for time_point >= time_point
	return (!(_Left < _Right));
	}

	// FUNCTION TEMPLATE time_point_cast
template<class _To,
	class _Clock,
	class _Duration> inline
	constexpr typename enable_if<_Is_duration<_To>::value,
		time_point<_Clock, _To> >::type
		time_point_cast(const time_point<_Clock, _Duration>& _Time)
	{	// change the duration type of a time_point; truncate
	return (time_point<_Clock, _To>(
		chrono::duration_cast<_To>(_Time.time_since_epoch())));
	}

	// FUNCTION TEMPLATE floor (for time_point instances)
template<class _To,
	class _Clock,
	class _Duration> inline
	constexpr typename enable_if<_Is_duration<_To>::value,
		time_point<_Clock, _To> >::type
		floor(const time_point<_Clock, _Duration>& _Time)
	{	// change the duration type of a time_point; round towards negative infinity
	return (time_point<_Clock, _To>(
		chrono::floor<_To>(_Time.time_since_epoch())));
	}

	// FUNCTION TEMPLATE ceil (for time_point instances)
template<class _To,
	class _Clock,
	class _Duration> inline
	constexpr typename enable_if<_Is_duration<_To>::value,
		time_point<_Clock, _To> >::type
		ceil(const time_point<_Clock, _Duration>& _Time)
	{	// change the duration type of a time_point; round towards positive infinity
	return (time_point<_Clock, _To>(
		chrono::ceil<_To>(_Time.time_since_epoch())));
	}

	// FUNCTION TEMPLATE round (for time_point instances)
template<class _To,
	class _Clock,
	class _Duration> inline
	constexpr typename enable_if<_Is_duration<_To>::value,
		time_point<_Clock, _To> >::type
		round(const time_point<_Clock, _Duration>& _Time)
	{	// change the duration type of a time_point; round to nearest, ties to even
	return (time_point<_Clock, _To>(
		chrono::round<_To>(_Time.time_since_epoch())));
	}

	// CLOCKS
struct system_clock

	{	// wraps GetSystemTimePreciseAsFileTime/GetSystemTimeAsFileTime
	typedef long long rep;

	typedef ratio_multiply<ratio<_XTIME_NSECS_PER_TICK, 1>, nano> period;

	typedef chrono::duration<rep, period> duration;
	typedef chrono::time_point<system_clock> time_point;
	static constexpr bool is_steady = false;

	static time_point now() _NOEXCEPT
		{	// get current time
		return (time_point(duration(_Xtime_get_ticks())));
		}

	static __time64_t to_time_t(const time_point& _Time) _NOEXCEPT
		{	// convert to __time64_t
		return ((__time64_t)(_Time.time_since_epoch().count()
			/ _XTIME_TICKS_PER_TIME_T));
		}

	static time_point from_time_t(__time64_t _Tm) _NOEXCEPT
		{	// convert from __time64_t
		return (time_point(duration(_Tm * _XTIME_TICKS_PER_TIME_T)));
		}
	};

struct steady_clock
	{	// wraps QueryPerformanceCounter
	typedef long long rep;
	typedef nano period;
	typedef nanoseconds duration;
	typedef chrono::time_point<steady_clock> time_point;
	static constexpr bool is_steady = true;

	static time_point now() _NOEXCEPT
		{	// get current time
		const long long _Freq = _Query_perf_frequency();	// doesn't change after system boot
		const long long _Ctr = _Query_perf_counter();
		static_assert(period::num == 1, "This assumes period::num == 1.");
		const long long _Whole = (_Ctr / _Freq) * period::den;
		const long long _Part = (_Ctr % _Freq) * period::den / _Freq;
		return (time_point(duration(_Whole + _Part)));
		}
	};

typedef steady_clock high_resolution_clock;
}	// namespace chrono

	// HELPERS
template<class _Rep,
	class _Period> inline
	xtime _To_xtime(const chrono::duration<_Rep, _Period>& _Rel_time)
	{	// convert duration to xtime
	xtime _Xt;
	if (_Rel_time <= chrono::duration<_Rep, _Period>::zero())
		{	// negative or zero relative time, return zero
		_Xt.sec = 0;
		_Xt.nsec = 0;
		}
	else
		{	// positive relative time, convert
		chrono::nanoseconds _T0 =
			chrono::system_clock::now().time_since_epoch();
		_T0 += chrono::duration_cast<chrono::nanoseconds>(_Rel_time);
		_Xt.sec = chrono::duration_cast<chrono::seconds>(_T0).count();
		_T0 -= chrono::seconds(_Xt.sec);
		_Xt.nsec = (long)_T0.count();
		}
	return (_Xt);
	}

	// duration LITERALS
#ifdef __clang__
 #pragma clang diagnostic push
 #pragma clang diagnostic ignored "-Wuser-defined-literals"
#endif /* __clang__ */

inline namespace literals {
inline namespace chrono_literals {
inline constexpr chrono::hours operator "" h(unsigned long long _Val)
	{	// return integral hours
	return (chrono::hours(_Val));
	}

inline constexpr chrono::duration<double, ratio<3600> > operator "" h(
	long double _Val)
	{	// return floating-point hours
	return (chrono::duration<double, ratio<3600> >(_Val));
	}

inline constexpr chrono::minutes (operator "" min)(unsigned long long _Val)
	{	// return integral minutes
	return (chrono::minutes(_Val));
	}

inline constexpr chrono::duration<double, ratio<60> > (operator "" min)(
	long double _Val)
	{	// return floating-point minutes
	return (chrono::duration<double, ratio<60> >(_Val));
	}

inline constexpr chrono::seconds operator "" s(unsigned long long _Val)
	{	// return integral seconds
	return (chrono::seconds(_Val));
	}

inline constexpr chrono::duration<double> operator "" s(long double _Val)
	{	// return floating-point seconds
	return (chrono::duration<double>(_Val));
	}

inline constexpr chrono::milliseconds operator "" ms(unsigned long long _Val)
	{	// return integral milliseconds
	return (chrono::milliseconds(_Val));
	}

inline constexpr chrono::duration<double, milli> operator "" ms(
	long double _Val)
	{	// return floating-point milliseconds
	return (chrono::duration<double, milli>(_Val));
	}

inline constexpr chrono::microseconds operator "" us(unsigned long long _Val)
	{	// return integral microseconds
	return (chrono::microseconds(_Val));
	}

inline constexpr chrono::duration<double, micro> operator "" us(
	long double _Val)
	{	// return floating-point microseconds
	return (chrono::duration<double, micro>(_Val));
	}

inline constexpr chrono::nanoseconds operator "" ns(unsigned long long _Val)
	{	// return integral nanoseconds
	return (chrono::nanoseconds(_Val));
	}

inline constexpr chrono::duration<double, nano> operator "" ns(
	long double _Val)
	{	// return floating-point nanoseconds
	return (chrono::duration<double, nano>(_Val));
	}
}	// inline namespace chrono_literals
}	// inline namespace literals

namespace chrono {
	using namespace literals::chrono_literals;
}	// namespace chrono

#ifdef __clang__
 #pragma clang diagnostic pop
#endif /* __clang__ */

_STD_END
 #pragma pop_macro("new")
 #pragma warning(pop)
 #pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _CHRONO_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
