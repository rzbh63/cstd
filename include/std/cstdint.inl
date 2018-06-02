// cstdint standard header
#pragma once
#ifndef _CSTDINT_
#define _CSTDINT_
#include <yvals.h>

#ifdef _STD_USING
 #undef _STD_USING
  #include <stdint.h>
 #define _STD_USING

#else /* _STD_USING */
 #include <stdint.h>
#endif /* _STD_USING */

 #if _GLOBAL_USING && !defined(RC_INVOKED)
_STD_BEGIN
using _CSTD int8_t; using _CSTD int16_t;
using _CSTD int32_t; using _CSTD int64_t;
using _CSTD uint8_t; using _CSTD uint16_t;
using _CSTD uint32_t; using _CSTD uint64_t;

using _CSTD int_least8_t; using _CSTD int_least16_t;
using _CSTD int_least32_t;  using _CSTD int_least64_t;
using _CSTD uint_least8_t; using _CSTD uint_least16_t;
using _CSTD uint_least32_t; using _CSTD uint_least64_t;

using _CSTD int_fast8_t; using _CSTD int_fast16_t;
using _CSTD int_fast32_t;  using _CSTD int_fast64_t;
using _CSTD uint_fast8_t; using _CSTD uint_fast16_t;
using _CSTD uint_fast32_t; using _CSTD uint_fast64_t;

using _CSTD intmax_t; using _CSTD intptr_t;
using _CSTD uintmax_t; using _CSTD uintptr_t;

#if _HAS_TR1_NAMESPACE
	namespace tr1 {
using _CSTD int8_t; using _CSTD int16_t;
using _CSTD int32_t; using _CSTD int64_t;
using _CSTD uint8_t; using _CSTD uint16_t;
using _CSTD uint32_t; using _CSTD uint64_t;

using _CSTD int_least8_t; using _CSTD int_least16_t;
using _CSTD int_least32_t;  using _CSTD int_least64_t;
using _CSTD uint_least8_t; using _CSTD uint_least16_t;
using _CSTD uint_least32_t; using _CSTD uint_least64_t;

using _CSTD int_fast8_t; using _CSTD int_fast16_t;
using _CSTD int_fast32_t;  using _CSTD int_fast64_t;
using _CSTD uint_fast8_t; using _CSTD uint_fast16_t;
using _CSTD uint_fast32_t; using _CSTD uint_fast64_t;

using _CSTD intmax_t; using _CSTD intptr_t;
using _CSTD uintmax_t; using _CSTD uintptr_t;
	}	// namespace tr1
#endif /* _HAS_TR1_NAMESPACE */
_STD_END
 #endif /* _GLOBAL_USING */

#endif /* _CSTDINT_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
