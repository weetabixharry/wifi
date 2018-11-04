#ifndef POLYPHASE_FILTER_LUT_H
#define	POLYPHASE_FILTER_LUT_H

#include "dsp/gaussian_sinc.h"

#include <vector>
#include <cassert>

namespace dsp
{
	template <typename T>
	class polyphase_filter_lut
	{
	
	public:
	
		polyphase_filter_lut(size_t Ntaps, size_t Nphases, size_t ctap, double bw, double alpha) :
			_ntaps(Ntaps),
			_nphases(Nphases),
			_ctap(ctap),
			_bw(bw),
			_alpha(alpha)
		{
			create_lut();
		}
		
		const T* get_filter(size_t phase) const
		{
			return &_lut[phase*_ntaps];
		}
		
		// Getters
		size_t get_ntaps() const { return _ntaps; }
		size_t get_nphases() const { return _nphases; }
		size_t get_ctap() const { return _ctap; }
		double get_bw() const { return _bw; }
		
	private:
		
		void create_lut()
		{
			assert(_ntaps % 2 == 0);
			_lut.resize(_ntaps*_nphases);
			for (size_t i=0; i<_nphases; i++)
			{
				double c = _ctap + 0.5 - i/double(_nphases);
				dsp::gaussian_sinc(_ntaps, c, _bw, _alpha, &_lut[i*_ntaps], true);
			}
		}
		std::vector<T> _lut;
		size_t _ntaps;
		size_t _nphases;
		size_t _ctap;
		double _bw;
		double _alpha;
	};

} // namespace dsp
#endif	/* POLYPHASE_FILTER_LUT_H */
