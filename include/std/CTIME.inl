
#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _CTIME_
#define _CTIME_
#ifdef _STD_USING
 #undef _STD_USING
 #include <time.h>
 #define _STD_USING
#else
 #include <time.h>
#endif /* _STD_USING */
#endif /* _CTIME_ */

/*
 * Copyright (c) 1994 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
