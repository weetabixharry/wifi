#ifndef WIFI_SCRAMBLER_H
#define	WIFI_SCRAMBLER_H

#include <bitset>

namespace wifi
{
	class scrambler
	{
		public:
		
			scrambler(std::bitset<7> init = std::bitset<7>("1011101"))
			{
				// Generate repeating period-127 pattern for this init state
				for (size_t i=0; i<127; i++)
				{
					_pattern[i] = init[3] ^ init[6];
					
					for (size_t i=6; i>0; i--)
						init[i] = init[i-1];
					init[0] = _pattern[i];
				}
			}
			
			void scramble(std::vector<uint8_t>& x)
			{
				size_t ip = 0;
				for (size_t i=0; i<x.size(); i++)
				{
					// Scramble each bit in byte
					uint8_t ybyte = 0;
					for (size_t bit=0; bit<8; bit++)
					{
						// Extract bit to scramble
						bool xbit = (x[i] >> bit) & 0x1;
						// Scramble
						ybyte |= (xbit ^ _pattern[ip]) << bit;
						// Update pattern index
						ip = ip < _pattern.size()-1 ? ip+1 : 0;
					}
					// Write scrambled byte (in-place)
					x[i] = ybyte;
				}
			}

		private:
			std::bitset<127> _pattern;
	};
	

	
}

#endif	/* WIFI_SCRAMBLER_H */
