// cstring standard header

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _CSTRING_
#define _CSTRING_
#ifdef _STD_USING
 #undef _STD_USING
 #include <string.h>
 #define _STD_USING
#else
 #include <string.h>
#endif /* _STD_USING */
#endif /* _CSTRING_ */

/*
 * Copyright (c) 1994 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
