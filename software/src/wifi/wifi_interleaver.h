#ifndef WIFI_INTERLEAVER_H
#define	WIFI_INTERLEAVER_H

#include <algorithm>
#include <vector>

namespace wifi
{
	class interleaver
	{
		public:
			interleaver() {};
			
			std::vector<uint8_t> interleave(std::vector<uint8_t> x, size_t Ncbps, size_t Nbpsc)
			{
				// Equation (17-17)
				const size_t s = std::max(Nbpsc/2, 1U);
				
				const size_t Nbits = 8 * x.size();
				const size_t Nsyms = Nbits / Ncbps;
				
				// Init to zeros so we can bitwise-or remapped bits in
				std::vector<uint8_t> y(x.size(), 0);
				for (size_t k=0; k<Ncbps; k++)
				{
					// First permuation
					const size_t i = (Ncbps/16) * (k % 16) + (k/16);
					
					// Second permutation
					const size_t j = s * (i/s) + ((i + Ncbps - (16 * i/Ncbps)) % s);
					
					// Apply this subcarrier's mapping for all Nsyms symbols
					for (size_t sym=0; sym<Nsyms; sym++)
					{
						// Bit index mapping: ybits[bit_k] := xbits[bit_j]
						size_t bit_j = sym*Ncbps + j;
						size_t bit_k = sym*Ncbps + k;
						
						// Extract the bit from x byte-array to write to y
						uint8_t xbit = (x[bit_k / 8] >> (bit_k % 8)) & 0x1;
						
						// Write into appropriate bit of y
						y[bit_j / 8] |= (xbit << (bit_j % 8));
					}
				}
				
				return y;
			}
			
			
		private:

	};
	
}

#endif	/* WIFI_INTERLEAVER_H */
