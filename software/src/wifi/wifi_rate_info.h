#ifndef WIFI_RATE_INFO_H
#define	WIFI_RATE_INFO_H

#include <bitset>
#include <vector>
#include <iostream>

namespace wifi
{
	
	struct rate_info_t
	{
		enum coding_rate_t {RATE_1_2, RATE_2_3, RATE_3_4};
		
		// Default xtor (so we can make vectors)
		rate_info_t() {}
		// Fully specified xtor (so we can generate LUT)
		rate_info_t(std::bitset<4> R, coding_rate_t rr, size_t nbpsc, size_t ncbps, size_t ndbps, size_t m) :
			RATE(R),
			r(rr),
			Nbpsc(nbpsc),
			Ncbps(ncbps),
			Ndbps(ndbps),
			mbps(m)
			{}
		// Index-specified xtor (so we can retrieve full spec from LUT)
		rate_info_t(size_t lut_index);
		
		static void get_rate_num_den(coding_rate_t rr, uint8_t* num, uint8_t* den)
		{
			switch(rr)
			{
				case RATE_1_2:
					*num = 1;
					*den = 2;
					return;
				case RATE_2_3:
					*num = 2;
					*den = 3;
					return;
				case RATE_3_4:
					*num = 3;
					*den = 4;
					return;
				default:
					std::cout << "ERROR! Invalid/unsupported rate" << std::endl;
			}
		}
		
		std::bitset<4> RATE; // See Table 17-5
		coding_rate_t r;
		size_t Nbpsc;
		size_t Ncbps;
		size_t Ndbps;
		size_t mbps;
	};
	
	namespace internal
	{
		// Note: We have to reverse MCS-code bits to get right-MSB order
		template<size_t N>
		std::bitset<N> reverse(std::bitset<N> x)
		{
			for (size_t i=0; i<N/2; i++)
			{
				bool b = x[i];
				x[i] = x[N-1-i];
				x[N-1-i] = b;
			}
			return x;
		}
		
		std::vector<rate_info_t> init_rate_info_lut()
		{
			std::vector<rate_info_t> mcs(8);
			
			mcs[0] = rate_info_t(reverse(std::bitset<4>("1101")), rate_info_t::RATE_1_2, 1,  48,  24, 6);
			mcs[1] = rate_info_t(reverse(std::bitset<4>("1111")), rate_info_t::RATE_3_4, 1,  48,  36, 9);
			mcs[2] = rate_info_t(reverse(std::bitset<4>("0101")), rate_info_t::RATE_1_2, 2,  96,  48, 12);
			mcs[3] = rate_info_t(reverse(std::bitset<4>("0111")), rate_info_t::RATE_3_4, 2,  96,  72, 18);
			mcs[4] = rate_info_t(reverse(std::bitset<4>("1001")), rate_info_t::RATE_1_2, 4, 192,  96, 24);
			mcs[5] = rate_info_t(reverse(std::bitset<4>("1011")), rate_info_t::RATE_3_4, 4, 192, 144, 36);
			mcs[6] = rate_info_t(reverse(std::bitset<4>("0001")), rate_info_t::RATE_2_3, 6, 288, 192, 48);
			mcs[7] = rate_info_t(reverse(std::bitset<4>("0011")), rate_info_t::RATE_3_4, 6, 288, 216, 54);
			
			return mcs;
		}
		
		const std::vector<rate_info_t> rate_info_lut = init_rate_info_lut();
	}
	
	
	rate_info_t::rate_info_t(size_t mcs)
	{
		if (mcs < wifi::internal::rate_info_lut.size())
		{
			*this = wifi::internal::rate_info_lut[mcs];
			return;
		}
		std::cout << "ERROR! Invalid/unsupported RATE" << std::endl;
		*this = wifi::internal::rate_info_lut[0];
	}
}

#endif	/* WIFI_RATE_INFO_H */
