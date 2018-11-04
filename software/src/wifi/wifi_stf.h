#ifndef WIFI_STF_H
#define	WIFI_STF_H

#include <cstddef>
#include <vector>
#include <complex>
#include <cassert>
#include "../dsp/fft.h"

namespace wifi
{
	namespace internal
	{
		static std::vector< std::complex<float> > init_stf_64()
		{
			// Unnormalised STF
			std::vector< std::complex<float> > x =
				 // Subcarriers -32 to -27
				{{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
				 // Subcarriers -26 to 26, as specified by the standard
				 {0,0}, {0,0}, {1,1},   {0,0}, {0,0}, {0,0}, {-1,-1}, {0,0},
				 {0,0}, {0,0}, {1,1},   {0,0}, {0,0}, {0,0}, {-1,-1}, {0,0},
				 {0,0}, {0,0}, {-1,-1}, {0,0}, {0,0}, {0,0}, {1,1},   {0,0},
				 {0,0}, {0,0}, {0,0},   {0,0}, {0,0}, {0,0}, {-1,-1}, {0,0},
				 {0,0}, {0,0}, {-1,-1}, {0,0}, {0,0}, {0,0}, {1,1},   {0,0},
				 {0,0}, {0,0}, {1,1},   {0,0}, {0,0}, {0,0}, {1,1},   {0,0},
				 {0,0}, {0,0}, {1,1},   {0,0}, {0,0},
				 // Subcarriers 27 to 31
				 {0,0}, {0,0}, {0,0}, {0,0}, {0,0}};
			
			// Sanity check
			assert(x.size() == 64);
			
			// Normalise
			for (size_t i=0; i<x.size(); i++)
				x[i] *= sqrt(13.0f/6.0f) / 64.0f;
			
			// Do OFDM IFFT
			dsp::fft(x, +1);

			// Twiddle sign to do fftshift *before* FFT
			for (size_t i=0; i<x.size()/2; i++)
				x[2*i+1] = -x[2*i+1];
			
			return x;
		}
	}
	
	static const std::vector< std::complex<float> > stf_64 = internal::init_stf_64();
	
	namespace internal
	{
		static std::vector< std::complex<float> > init_stf_161()
		{
			// Repeat periodically up to 161 samples. Note: We don't need to do
			// anything more clever due to 16-sample periodicity of STF.
			std::vector< std::complex<float> > x(161);
			for (size_t i=0; i<161; i++)
				x[i] = wifi::stf_64[i % 64];
			
			// Apply overlap window weights
			x[0] = 0.5f * x[0];
			x[160] = 0.5f * x[160];
			
			return x;
		}
	}
	
	static const std::vector< std::complex<float> > stf_161 = internal::init_stf_161();
	
}

#endif	/* WIFI_STF_H */
