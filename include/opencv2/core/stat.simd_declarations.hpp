#undef CV_CPU_SIMD_FILENAME
#define CV_CPU_SIMD_FILENAME "opencv2/core/stat.simd.hpp"

#undef CV_CPU_DISPATCH_MODE
#include "opencv2/core/private/cv_cpu_include_simd_declarations.hpp"

#define CV_CPU_DISPATCH_MODE SSE4_2
#include "opencv2/core/private/cv_cpu_include_simd_declarations.hpp"

#define CV_CPU_DISPATCH_MODE AVX2
#include "opencv2/core/private/cv_cpu_include_simd_declarations.hpp"

#undef CV_CPU_DISPATCH_MODES_ALL
#define CV_CPU_DISPATCH_MODES_ALL AVX2, SSE4_2, BASELINE
