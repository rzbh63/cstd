// cstddef standard header

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _CSTDDEF_
#define _CSTDDEF_
#ifdef _STD_USING
 #undef _STD_USING
 #include <stddef.h>
 #define _STD_USING
#else
 #include <stddef.h>
#endif /* _STD_USING */
#endif /* _CSTDDEF_ */

/*
 * Copyright (c) 1994 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
