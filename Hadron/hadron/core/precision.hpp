#ifndef HADRON_PRECISION_HPP
#define HADRON_PRECISION_HPP

#include <float.h>

namespace Hadron {
// Change this 0 to a 1 to enable float precision instead of double
#if 0
	typedef float real;
	#define real_pow powf
	#define real_sin sinf
	#define real_cos cosf
	#define real_tan tanf
	#define real_abs(n) fabs((real)n);
	#define REAL_MAX FLT_MAX;
#else
	typedef double real;
	#define real_pow pow
	#define real_sin sin
	#define real_cos cos
	#define real_tan tan
	#define real_abs(n) abs((real)n);
	#define REAL_MAX DBL_MAX;
#endif
};

#endif // HADRON_PRECISION_HPP