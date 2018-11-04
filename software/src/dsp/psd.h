#ifndef PSD_H
#define	PSD_H

#include "dsp/hanning.h"
#include "dsp/fft.h"

#include <complex>
#include <vector>

namespace dsp
{

	template <typename T>
	void psd(const std::complex<T>* x, size_t nx, T* P, size_t Nfft)
	{
		// Create the window
		const std::vector<T> w = hanning<T>(Nfft, true);
		
		// Clear output buffer to accumulate into
		for (size_t i=0; i<Nfft; i++)
			P[i] = T(0);
	
		size_t offset = 0;
		size_t avgs = 0;
		std::vector<std::complex<T> > X(Nfft);
		while (offset+Nfft <= nx)
		{
			// Apply window
			for (size_t i=0; i<Nfft; i++)
				X[i] = x[offset+i] * w[i];
		
			// Compute FFT
			dsp::fft(X, -1);
			
			// Accumulate spectrum
			for (size_t i=0; i<Nfft; i++)
				P[i] += std::norm(X[i]);
		
			// Step forward (half overlap)
			offset += Nfft / 2;
			avgs++;
		}
		
		// Normalise
		if (avgs != 0)
			for (size_t i=0; i<Nfft; i++)
				P[i] /= (T) (avgs * Nfft * Nfft);
	}

	template <typename T>
	std::vector<T> psd(const std::vector<std::complex<T> > x, size_t Nfft)
	{
		std::vector<T> P(Nfft);
		psd(&x[0], x.size(), &P[0], P.size());
		return P;
	}
	
} // namespace dsp
#endif	/* PSD_H */
