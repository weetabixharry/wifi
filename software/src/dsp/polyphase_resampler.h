#ifndef POLYPHASE_RESAMPLER_H
#define	POLYPHASE_RESAMPLER_H

#include <vector>
#include <cassert>
#include "dsp/polyphase_filter_lut.h"

namespace dsp
{
	
	template <typename Tdata, typename Tfilter>
	class polyphase_resampler
	{
	
	public:
	
		// Filter LUT can be externally provided.
		polyphase_resampler(dsp::polyphase_filter_lut<Tfilter>* lut, double ratio) :
			_ratio(ratio),
			_lut(lut),
			_lut_owner(false)
		{}
		
		// If LUT is not provided, create it.
		polyphase_resampler(size_t Ntaps, size_t Nphases, size_t ctap, double bw, double alpha, double ratio) :
			_ratio(ratio),
			_lut(new dsp::polyphase_filter_lut<Tfilter>(Ntaps, Nphases, ctap, bw, alpha)),
			_lut_owner(true)
		{}
		
		~polyphase_resampler()
		{
			if (_lut_owner)
				delete _lut;
		}
		
		double get_ratio() const { return _ratio; }
		void set_ratio(double r) { r = _ratio; }
		
		void resample(const Tdata* x, size_t Nx, std::vector<Tdata>& y)
		{
			const size_t Nphases = _lut->get_nphases();
			const size_t Ntaps = _lut->get_ntaps();
			assert(Nx >= Ntaps);
			
			// First output sample is at exactly t=npost. Last possible is at
			// (or before) t=L-npre+0.5. For now, allocate space for this worst
			// case and trim at the end if required.
			const double tmin = (double) npost();
			const double tmax = Nx - npre() + 0.5;
			const double tspan_max = tmax - tmin;
			const size_t nout_max = (size_t) ceil(tspan_max / _ratio);
			
			// Append zeros at the end of y for accumulating new values
			size_t offset = y.size();
			y.resize(offset+nout_max, (Tdata) 0);
			
			// Pass (reversed) filter across data at r increments
			double t = tmin;
			while (true)
			{
				// Determine the "closest" input sample and fractional offset (i.e. phase)
				size_t tint = (size_t) round(t);
				double p = t - tint; // N.B. [-0.5,0.5)
				size_t pfxp = round((p+0.5)*Nphases); // N.B. [0,Nphases] ==> correction:
				// N.B. We really want tint=round(t+0.5/Nphases), but there is no really
				//      safe shortcut to ensuring we have no issues with machine precision
				//      still rounding us out of bounds. Therefore, we coerce explicitly:
				if (pfxp >= Nphases)
				{
					tint = tint + 1;
					p = p - 1;
					pfxp = pfxp - Nphases;
				}

				// Exit when tint-npost()+Ntaps-1 runs off the end
				if (tint+npre() >= Nx)
					break;

				// Look up the appropriate filter
				const Tfilter* h = _lut->get_filter(pfxp);

				// Apply filter (N.B. Taps stored backwards for convenience)
				for (size_t i=0; i<Ntaps; i++)
					y[offset] += h[i] * x[tint-npost()+i];
				
				// Proceed to the next output sample time
				t = t + _ratio;
				offset++;
			}

			// Trim off the data
			y.resize(offset);
		}
		
		const dsp::polyphase_filter_lut<Tfilter>* get_lut() const { return _lut; }
		
	private:
	
		// Number of taps before and after the centre tap (npre+npost+1 = Ntaps)
		size_t npre() const { return _lut->get_ctap(); }
		size_t npost() const { return _lut->get_ntaps() - _lut->get_ctap() - 1; }
		
		double _ratio;
		const dsp::polyphase_filter_lut<Tfilter>* const _lut;
		const bool _lut_owner;
	};

} // namespace dsp
#endif	/* POLYPHASE_RESAMPLER_H */
