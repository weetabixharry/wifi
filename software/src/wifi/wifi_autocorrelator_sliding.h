#ifndef WIFI_AUTOCORRELATOR_SLIDING_H
#define	WIFI_AUTOCORRELATOR_SLIDING_H

#include <cstddef>
#include <cassert>
#include <complex>
#include "wifi_delay_line.h"

namespace wifi
{
	// Templated type just needs to have a .data member of std::complex<> type
	template<typename T, size_t OFFSET, size_t SPAN, size_t MAX_SAMPS>
	class autocorrelator_sliding
	{
	public:
		autocorrelator_sliding() : _rho(), _power_x(0), _power_y(0) {}
		void update(const T* x, size_t Nx, T* rho, size_t& Nrho, typename T::complex_type* power_x = NULL, typename T::complex_type* power_y = NULL)
		{
			assert(Nx <= MAX_SAMPS);
			
			// Input --> SPAN delay line
			_span_delay_line.update(x, Nx, &_span_delay_data[0], _nspan);
			
			// Input --> OFFSET_NEW delay line
			_offset_new_delay_line.update(x, Nx, &_new_delay_data[0], _nnew);
			
			// SPAN out --> OFFSET_OLD delay line
			_offset_old_delay_line.update(&_span_delay_data[0], _nspan, &_old_delay_data[0], _nold);
			
			// Update sliding window
			Nrho = 0;
			for (size_t i=0; i<_nnew; i++)
			{				
				// Add new
				auto xnew = x[Nx-_nnew+i].data;
				auto ynew = _new_delay_data[i].data;
				_rho += xnew * std::conj(ynew);
				_power_x += std::norm(xnew);
				_power_y += std::norm(ynew);
				
				// Subtract old
				if (_nold+i >= _nnew)
				{
					auto xold = _span_delay_data[_nspan-_nnew+i].data;
					auto yold = _old_delay_data[_nold-_nnew+i].data;
					_rho -= xold * std::conj(yold);
					_power_x -= std::norm(xold);
					_power_y -= std::norm(yold);
				}
				
				// Output (includes the sample when warmup completes
				if (_nold+i+1 >= _nnew)
				{
					// Take metadata from newest sample and write outputs
					if (rho)
					{
						T r = x[Nx-_nnew+i];
						r.data = _rho / std::sqrt(_power_x * _power_y);
						rho[Nrho] = r;
					}
					
					if (power_x)
						power_x[Nrho] = _power_x;
					
					if (power_y)
						power_y[Nrho] = _power_y;
					
					Nrho++;
				}
			}
		}
	private:
		delay_line<T, OFFSET> _offset_old_delay_line;
		delay_line<T, OFFSET> _offset_new_delay_line;
		delay_line<T, SPAN> _span_delay_line;
		
		T _span_delay_data[MAX_SAMPS];
		size_t _nspan;
		
		T _new_delay_data[MAX_SAMPS];
		size_t _nnew;
		
		T _old_delay_data[MAX_SAMPS];
		size_t _nold;
		
		std::complex<typename T::complex_type> _rho;
		typename T::complex_type _power_x, _power_y;
	};
}

#endif	/* WIFI_AUTOCORRELATOR_SLIDING_H */
