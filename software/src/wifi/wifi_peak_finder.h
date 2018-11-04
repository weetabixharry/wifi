#ifndef WIFI_PEAK_FINDER_H
#define	WIFI_PEAK_FINDER_H

#include <cstddef>
#include <vector>
#include <complex>

namespace wifi
{
	template<typename T>
	class peak_finder
	{
	public:
		peak_finder(typename T::complex_type threshold, size_t countdown_limit, size_t countup_limit)  :
			_threshold(threshold),
			_peakabs(threshold),
			_countdown_limit(countdown_limit),
			_countup_limit(countup_limit),
			_countdown(0),
			_countup(0)
		{}
		
		// The inputs should be complex-valued correlations
		void update(const T* x, size_t Nx, std::vector<T>& detections)
		{
			for (size_t i=0; i<Nx; i++)
			{
				// Check countup has elapsed since last accepted detection
				if (_countup > 0)
				{
					_countup--;
					continue;
				}
				
				typename T::complex_type absx = std::abs(x[i].data);
				if (absx > _peakabs)
				{
					// NEW MAX FOUND. Store value and start countdown.
					_peakabs = absx;
					_peakval = x[i];
					_countdown = _countdown_limit;
				}
				else if (_countdown > 0)
				{
					// Check whether countdown has elapsed since last max
					if(--_countdown == 0)
					{
						// ACCEPT PEAK. Reset maximum. Start countup.
						_peakabs = _threshold;
						_countup = _countup_limit;
						detections.push_back(_peakval);
					}
				}
				else
				{
					// Nothing interesting is happening...
				}
			}
		}
	
	private:
		
		// Config...
		typename T::complex_type _threshold; // Correlation threshold <= 1.0
		size_t _countdown_limit; // See _countdown
		size_t _countup_limit; // See _countup
		
		// Peak info...
		typename T::complex_type _peakabs;
		T _peakval;
		size_t _peak_index;
		
		// COUNTDOWN: After new peak before accepting it won't be beaten. This
		//            primarily rejects local peaks on the up-ramp. Note that
		//            this introduces genuine latency into the detection.
		size_t _countdown;
		// COUNTUP: After accepted peak before considering anything new. This
		//          primarily gives time to ramp back down below threshold. Note
		//          that this does NOT introduce any latency to the detection.
		size_t _countup; 
	};
}

#endif	/* WIFI_PEAK_FINDER_H */
