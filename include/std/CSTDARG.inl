// cstdarg standard header

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _CSTDARG_
#define _CSTDARG_
#ifdef _STD_USING
 #undef _STD_USING
 #include <stdarg.h>
 #define _STD_USING
#else
 #include <stdarg.h>
#endif /* _STD_USING */
#endif /* _CSTDARG_ */

/*
 * Copyright (c) 1994 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
