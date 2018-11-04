#ifndef WIFI_MAPPER_H
#define	WIFI_MAPPER_H

#include <vector>

namespace wifi
{
	class mapper
	{
		public:
			mapper() {}
			
			std::vector<std::complex<float> > map(std::vector<uint8_t> x, size_t Nbpsc)
			{
				// Get reference to appropriate map
				const std::vector<std::complex<float> >& map = _maps[Nbpsc/2];
				
				const size_t Nx_bits = 8 * x.size();
				const size_t Ny = Nx_bits / Nbpsc;
				
				size_t bit_offset = 0;
				size_t ix = 0;
				std::vector<std::complex<float> > y(Ny);
				for (size_t iy=0; iy<Ny; iy++)
				{
					// Extract symbols bits from byte stream
					size_t sym = 0;
					for (size_t bit=0; bit<Nbpsc; bit++)
					{
						if (bit_offset < 8)
							sym |= ((x[ix] >> bit_offset++) & 0x1) << Nbpsc-1-bit;
						else
						{
							sym |= (x[++ix] & 0x1) << Nbpsc-1-bit;
							bit_offset = 1;
						}
					}
					
					// Look up mapped value
					y[iy] = map[sym];
				}
				
				return y;
			}
			
		private:
			static const std::vector<std::vector<std::complex<float> > > _maps;

	};
	
	namespace internal
	{
		std::vector<std::complex<float> > init_map_lut(const size_t Nbpsc, float Kmod)
		{
			// BPSK is a special case (non-QAM)
			if (Nbpsc == 1)
			{
				std::vector<std::complex<float> > bpsk_map = {{-Kmod, 0.0f}, {Kmod, 0.0f}};
				return bpsk_map;
			}
			
			// Derive QAM parameters
			const size_t log2M = Nbpsc;
			const size_t log2sqrtM = (log2M >> 1U);
			const size_t M = (1U << log2M);
			const size_t sqrtM = (1U << log2sqrtM);
			
			// Create 1D gray code LUT
			uint8_t gray1d[sqrtM];
			for (uint8_t i=0; i<sqrtM; i++)
				gray1d[i] = i ^ (i >> 1);
			
			// Create 2D (complex) gray code LUT
			uint8_t gray2d[sqrtM][sqrtM];
			for (uint8_t row=0; row<sqrtM; row++)
				for (uint8_t col=0; col<sqrtM; col++)
					gray2d[row][col] = gray1d[row] + (gray1d[col] << log2sqrtM);
			
			// Construct mapped complex values constellation
			const int s = std::max(Nbpsc/2, 1U);
			const int offset = 1 - (1 << s);
			std::vector<std::complex<float> > map(M);
			for (uint8_t row=0; row<sqrtM; row++)
				for (uint8_t col=0; col<sqrtM; col++)
					map[gray2d[row][col]] = {Kmod*float(offset + int(2*col)), Kmod*float(offset + int(2*row))};
			
			return map;
		}
		
		std::vector<std::vector<std::complex<float> > > init_map_luts()
		{
			// List of all supported Nbpsc values
			const std::vector<size_t> Nbpscs = {1U, 2U, 4U, 6U};
			// [Table 17-6] Modulation-dependent normalization factor Kmod
			const std::vector<float> Kmods = {1.0f, 1.0f / sqrt(2.0f), 1.0f / sqrt(10.0f), 1.0f / sqrt(42.0f)};
			
			// Generate maps
			std::vector<std::vector<std::complex<float> > > maps(Nbpscs.size());
			for (size_t i=0; i<Nbpscs.size(); i++)
				maps[i] = init_map_lut(Nbpscs[i], Kmods[i]);
			
			return maps;
		}
	}
	
	const std::vector<std::vector<std::complex<float> > > mapper::_maps = internal::init_map_luts();
	
}

#endif	/* WIFI_MAPPER_H */
