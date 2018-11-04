#ifndef GAUSSIAN_SINC_H
#define	GAUSSIAN_SINC_H

#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>

namespace dsp
{

	template<typename T>
	void gaussian_sinc(size_t Ntaps, double centre, double bw, double alpha, T* h, bool reverse = false)
	{
		assert(centre>=0 && centre<Ntaps);
		assert(bw>=0 && bw <=1);

		for (int i=0; i<Ntaps; i++)
		{
			double t = i - centre;
			// Make Gaussian window
			double x = t * 2 / double(Ntaps);
			double g = exp(-alpha*x*x);
			// Store taps forwards or backwards
			int idx = reverse ? Ntaps-1-i : i;
			// Make sinc and apply window
			h[idx] = (T) bw * util::sinc(bw*t) * g;
		}
	}

	template<typename T>
	std::vector<T> gaussian_sinc(size_t Ntaps, double centre, double bw, double alpha, bool reverse = false)
	{
		std::vector<T> h(Ntaps);
		gaussian_sinc(Ntaps, centre, bw, alpha, &h[0], reverse);
		return h;
	}

} // namespace dsp
#endif	/* GAUSSIAN_SINC_H */
