#ifndef HANNING_H
#define	HANNING_H

#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>

namespace dsp
{

	template<typename T>
	void hanning(T* h, size_t N, bool fftshift = false, bool periodic = false)
	{
		if (N == 1)
		{
			*h = T(1);
			return;
		}
		
		// Set periodic or symmetric behaviour
		size_t n = periodic ? N : N-1;
		double sign = 1.0;
		for (size_t i=0; i<N; i++)
		{
			h[i] = (T) sign * 0.5 * (1.0 - cos(2.0*M_PI*double(i) / double(n)));
			sign = fftshift ? -sign : sign;
		}
	}

	template<typename T>
	std::vector<T> hanning(size_t N, bool fftshift = false, bool periodic = false)
	{
		std::vector<T> h(N);
		hanning(&h[0], N, fftshift, periodic);
		return h;
	}

} // namespace dsp
#endif	/* HANNING_H */
