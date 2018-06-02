// xmemory internal header (from <memory>)

#ifndef _XMEMORY_
#define _XMEMORY_
//#include "cstdlib.inl"
//#include "new.inl"
//#include "utility.inl"
#if 0

#ifndef _FARQ	/* specify standard memory model */
 #define _FARQ
 #define _PDFT	ptrdiff_t
 #define _SIZT	size_t
#endif
 #define _POINTER_X(T, A)	T _FARQ *
 #define _REFERENCE_X(T, A)	T _FARQ &
_STD_BEGIN
		// TEMPLATE FUNCTION _Allocate
template<class _Ty> inline
	_Ty _FARQ *_Allocate(_PDFT _N, _Ty _FARQ *)
	{if (_N < 0)
		_N = 0;
	return ((_Ty _FARQ *)operator new(
		(_SIZT)_N * sizeof (_Ty))); }
		// TEMPLATE FUNCTION _Construct
template<class _T1, class _T2> inline
	void _Construct(_T1 _FARQ *_P, const _T2& _V)
	{new ((void _FARQ *)_P) _T1(_V); }
		// TEMPLATE FUNCTION _Destroy
template<class _Ty> inline
	void _Destroy(_Ty _FARQ *_P)
	{_DESTRUCTOR(_Ty, _P); }
inline void _Destroy(char _FARQ *_P)
	{}
inline void _Destroy(wchar_t _FARQ *_P)
	{}
		// TEMPLATE CLASS allocator
template<class _Ty>
	class allocator {
public:
	typedef _SIZT size_type;
	typedef _PDFT difference_type;
	typedef _Ty _FARQ *pointer;
	typedef const _Ty _FARQ *const_pointer;
	typedef _Ty _FARQ& reference;
	typedef const _Ty _FARQ& const_reference;
	typedef _Ty value_type;
	pointer address(reference _X) const
		{return (&_X); }
	const_pointer address(const_reference _X) const
		{return (&_X); }
	pointer allocate(size_type _N, const void *)
		{return (_Allocate((difference_type)_N, (pointer)0)); }
	char _FARQ *_Charalloc(size_type _N)
		{return (_Allocate((difference_type)_N,
			(char _FARQ *)0)); }
	void deallocate(void _FARQ *_P, size_type)
		{operator delete(_P); }
	void construct(pointer _P, const _Ty& _V)
		{_Construct(_P, _V); }
	void destroy(pointer _P)
		{_Destroy(_P); }
	_SIZT max_size() const
		{_SIZT _N = (_SIZT)(-1) / sizeof (_Ty);
		return (0 < _N ? _N : 1); }
	};
template<class _Ty, class _U> inline
	bool operator==(const allocator<_Ty>&, const allocator<_U>&)
	{return (true); }
template<class _Ty, class _U> inline
	bool operator!=(const allocator<_Ty>&, const allocator<_U>&)
	{return (false); }
		// CLASS allocator<void>
template<> class _CRTIMP allocator<void> {
public:
	typedef void _Ty;
	typedef _Ty _FARQ *pointer;
	typedef const _Ty _FARQ *const_pointer;
	typedef _Ty value_type;
	};

#endif
#endif /* _XMEMORY_ */

