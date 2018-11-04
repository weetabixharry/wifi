#ifndef WIFI_DELAY_LINE_H
#define	WIFI_DELAY_LINE_H

#include <cstddef>

namespace wifi
{
	template<typename T, size_t LENGTH>
	class delay_line
	{
	public:
		
		delay_line() : _warm_count(0), _addr(0) {}
		void update(const T* in, size_t Nin, T* out, size_t& Nout)
		{
			// Output the delayed data before overwriting buffer
			size_t in_idx = 0;
			Nout = 0;
			size_t rd_addr = _addr;
			for (size_t i=0; i<Nin; i++)
			{
				if (i<LENGTH) // Output buffered data
				{
					// Only take valid buffer data (warmup)
					if (rd_addr < _warm_count)
						out[Nout++] = _buffer[rd_addr];
					
					// Update buffer read address
					rd_addr++;
					if (rd_addr > LENGTH-1)
						rd_addr = 0;
				}
				else // Output incoming data
					out[Nout++] = in[in_idx++];
			}
			
			// Write new data into buffer
			while (in_idx < Nin)
			{				
				// Update buffer
				_buffer[_addr++] = in[in_idx++];
				if (_addr > LENGTH-1)
					_addr = 0;
				
				// Record how many samples in the buffer are now valid
				if (_warm_count < LENGTH)
					_warm_count++;
			}
		}
		
	private:
		T _buffer[LENGTH];
		size_t _warm_count;
		size_t _addr; // Next write addr
	};
}

#endif	/* WIFI_DELAY_LINE_H */
