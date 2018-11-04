#ifndef SINC_H
#define	SINC_H

#define _USE_MATH_DEFINES
#include <cmath>

namespace util
{
	// Note: We use the definition sinc(x) = sin(pi*x) / (pi*x)
	template<typename T>
	T sinc(T x)
	{
		return x==0 ? T(1) : (T) sin(M_PI * x) / (M_PI * x);
	}

} // namespace util
#endif	/* SINC_H */
