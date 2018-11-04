#ifndef FFT_H
#define	FFT_H
/*
 * This is a very slightly modified version of the FFT from Numerical Recipes.
 * Firstly, we -1 from all indexes to avoid their strange unused data[0].
 * Secondly, I/O data type is a more convenient C++ complex<vector<T> >&.
 * Note: Matlab's fft() <==> fft(x, -1). Matlab's ifft() <==> fft(x, +1) / Nfft.
 * Note: Nfft must be powers of 2 only.
*/
#include <complex>
#include <vector>
#include <cmath>
#include <cassert>

namespace dsp
{
	
	template <typename T>
	void fft(std::complex<T>* x, int nn, int isign)
	{
		// Cast to T* to fit with this old C-style code
		T* const data = (T* const) x;
		assert(nn && !(nn & (nn - 1))); // Check Nfft = 2^n
		
		int n, mmax, m, j, istep, i;
		T wtemp, wr, wpr, wpi, wi, theta;
		T tempr, tempi;
		
		n = nn << 1;
		j = 1;
		for (int i = 1; i < n; i += 2) {
		if (j > i)
		{
			assert(j >= 1 && i >= 1);
			tempr = data[j-1];     data[j-1] = data[i-1];     data[i-1] = tempr;
			tempr = data[j]; data[j] = data[i]; data[i] = tempr;
		}
		m = n >> 1;
		while (m >= 2 && j > m) {
			j -= m;
			m >>= 1;
		}
		j += m;
		}
		mmax = 2;
		while (n > mmax) {
		istep = 2*mmax;
		theta = 2.0 * M_PI / (isign*mmax);
		wtemp = sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi = sin(theta);
		wr = 1.0;
		wi = 0.0;
		for (m = 1; m < mmax; m += 2) {
			for (i = m; i <= n; i += istep) {
			j =i + mmax;
			assert(j >= 1 && i >= 1);
			tempr = wr*data[j-1]   - wi*data[j];
			tempi = wr*data[j] + wi*data[j-1];
			data[j-1]   = data[i-1]   - tempr;
			data[j] = data[i] - tempi;
			data[i-1] += tempr;
			data[i] += tempi;
			}
			wr = (wtemp = wr)*wpr - wi*wpi + wr;
			wi = wi*wpr + wtemp*wpi + wi;
		}
		mmax = istep;
		}
	}

	template <typename T>
	void fft(std::vector<std::complex<T> >& x, int isign)
	{
		fft(&x[0], x.size(), isign);
	}
	
} // namespace dsp
#endif	/* FFT_H */
