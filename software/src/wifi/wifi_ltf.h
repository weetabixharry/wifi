#ifndef WIFI_LTF_H
#define	WIFI_LTF_H

#include <vector>
#include <complex>
#include <cassert>
#include "../dsp/fft.h"

namespace wifi
{
	namespace internal
	{
		static std::vector< std::complex<float> > init_ltf_64()
		{
			// Subcarriers -26 to 26 as specified by the standard
			int8_t ltf_int[53] = {
				1,  1, -1, -1, 1,  1, -1,  1, -1,  1,  1,  1,  1,  1, 1, -1, -1,  1, 1, -1, 1, -1, 1, 1, 1, 1, 0,
				1, -1, -1,  1, 1, -1,  1, -1,  1, -1, -1, -1, -1, -1, 1,  1, -1, -1, 1, -1, 1, -1, 1, 1, 1, 1};
			
			// Populate FFT buffer
			std::vector< std::complex<float> > x(64);
			for (int i=-32; i<32; i++)
			{
				// Zero-pad at edges
				if (i < -26 || i > 26)
					x[i+32] = {0.0f, 0.0f};
				else
					x[i+32] = {float(ltf_int[i+26]) / 64.0f, 0.0f};
			}
			
			// Do OFDM IFFT
			dsp::fft(x, +1);

			// Twiddle sign to do fftshift *before* FFT
			for (size_t i=0; i<x.size()/2; i++)
				x[2*i+1] = -x[2*i+1];
			
			return x;
		}
	}
	
	const std::vector< std::complex<float> > ltf_64 = internal::init_ltf_64();
	
	namespace internal
	{
		static std::vector< std::complex<float> > init_ltf_161()
		{
			// 
			std::vector< std::complex<float> > x(161);
			for (size_t i=0; i<161; i++)
				x[i] = wifi::ltf_64[(i+3*64-160) % 64];
			
			// Apply overlap window weights
			x[0] = 0.5f * x[0];
			x[160] = 0.5f * x[160];
			
			return x;
		}
	}
	
	static const std::vector< std::complex<float> > ltf_161 = internal::init_ltf_161();
}

#endif	/* WIFI_LTF_H */
