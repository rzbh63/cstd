// iostream standard header

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _IOSTREAM_
#define _IOSTREAM_
#include <istream>

#ifdef  _MSC_VER
#pragma pack(push,8)
#endif  /* _MSC_VER */
_STD_BEGIN
		// OBJECTS
static ios_base::Init _Ios_init;
extern _CRTIMP istream cin;
extern _CRTIMP ostream cout;
extern _CRTIMP ostream cerr, clog;
		// CLASS _Winit
class _CRTIMP _Winit {
public:
	_Winit();
	~_Winit();
private:
	static int _Init_cnt;
	};
		// WIDE OBJECTS
static _Winit _Wios_init;
extern _CRTIMP wistream wcin;
extern _CRTIMP wostream wcout, wcerr, wclog;
_STD_END
#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif /* _IOSTREAM_ */

/*
 * Copyright (c) 1994 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
