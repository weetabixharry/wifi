#ifndef STF_DETECTOR_H
#define	STF_DETECTOR_H

#include "wifi_autocorrelator_sliding.h"
#include "wifi_peak_finder.h"
#include <cstddef>

// Debugging includes...
#include "util/fileio_helper.h"
#include <iostream>

namespace wifi
{
	template<typename T, size_t MAX_SAMPS, bool DEBUG = false>
	class stf_detector
	{
	public:
		stf_detector(typename T::complex_type threshold) : _peak_finder(threshold, 16, 160)
		{
			if (DEBUG)
				util::create_empty_file("autocorr_out.cf");
		}
	
		void update(T* x, size_t Nx)
		{
			// Update sliding window autocorrelation
			std::vector<T> rho(Nx);
			size_t Nrho;
			_autocorr.update(x, Nx, &rho[0], Nrho);
			
			// Update peak detector
			std::vector<T> detections;
			_peak_finder.update(&rho[0], Nrho, detections);
			
			//////////////////////////// DEBUGGING /////////////////////////////
			if (DEBUG)
			{
				std::vector<std::complex<float> > debug(Nrho);
				for (size_t i=0; i<Nrho; i++)
					debug[i] = rho[i].data;
				util::append_to_file(debug, "autocorr_out.cf");
				
				for (size_t i=0; i<detections.size(); i++)
				{
					std::cout << "STF detected!" << std::endl;
					std::cout << "    Index: " << detections[i].index << std::endl;
					std::cout << "    Rho: " << detections[i].data << std::endl;
				}
			}
		}
	
	private:
		autocorrelator_sliding<T, 16, 9*16, MAX_SAMPS> _autocorr;
		peak_finder<T> _peak_finder;
	};
}

#endif	/* STF_DETECTOR_H */
