// cwctype standard header

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _CWCTYPE_
#define _CWCTYPE_
#ifdef _STD_USING
 #undef _STD_USING
 #include <wctype.h>
 #define _STD_USING
#else
 #include <wctype.h>
#endif /* _STD_USING */
#endif /* _CWCTYPE_ */

/*
 * Copyright (c) 1994 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
