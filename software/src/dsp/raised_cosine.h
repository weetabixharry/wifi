#ifndef RAISED_COSINE_H
#define	RAISED_COSINE_H

#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include "util/sinc.h"

namespace dsp
{

	template<typename T>
	std::vector<T> raised_cosine(size_t Nh, double Ts, double beta)
	{
		const double TOL = 1e-10;

		std::vector<T> h(Nh);
		for (size_t i=0; i<Nh; i++)
		{
			// Make sure t=0 always occurs, even if Ntaps is even.
			// For Ntaps odd:  t = -(N-1)/2 : (N-1)/2 [symmetric]
			// For Ntaps even: t = -N/2 : N/2-1		 [assymetric]
			int t = -int(Nh)/2 + int(i);
			double x = t / Ts;
			// Calculate the sinc part
			h[i] = util::sinc(x);
			// Fix 0 / 0 using L'Hopital's rule
			if (std::abs(2.0 * beta * std::abs(t) - Ts) < TOL)
				h[i] *= (T) M_PI / 4.0;
			else
			{
				double num = cos(M_PI * beta * t / Ts);
				double den = 1.0 - (2.0*beta*t / Ts)*(2.0*beta*t / Ts);
				h[i] *= (T) num / den;
			}
		}

		return h;
	}

} // namespace dsp
#endif	/* RAISED_COSINE_H */
