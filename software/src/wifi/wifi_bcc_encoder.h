#ifndef WIFI_BCC_ENCODER_H
#define	WIFI_BCC_ENCODER_H

#include <cassert>
#include "wifi/wifi_rate_info.h"

namespace wifi
{
	class bcc_encoder
	{
		public:
			bcc_encoder() {};
			
			std::vector<uint8_t> encode(const std::vector<uint8_t>& x, rate_info_t::coding_rate_t r)
			{
				return encode(&x[0], x.size(), r);
			}
			
			std::vector<uint8_t> encode(const uint8_t* x, size_t Nx, rate_info_t::coding_rate_t r)
			{
				// Output is twice length of input
				std::vector<uint8_t> y(2*Nx);
				
				// Initialise to zero state? Can't see where this is explicitly specified...
				std::bitset<7> reg("0000000");
				
				// For each data byte
				for (size_t i=0; i<Nx; i++)
				{
					// For each bit in byte
					std::bitset<8> ypair[2];
					for (size_t b=0; b<8; b++)
					{
						// Shift new bit into 7-bit reg
						for (size_t s=7; s>0; s--)
							reg[s] = reg[s-1];
						reg[0] = (x[i] >> b) & 0x1;
						
						// Implement xor connections as per Figure 17.8 of 802.11 standard
						bool s0 = false;
						bool s1 = false;
						for (size_t k=0; k<7; k++)
						{
							if (_g0[k])
								s0 ^= reg[k];
							
							if (_g1[k])
								s1 ^= reg[k];
						}
						// Stuff back into bytes
						ypair[b/4][2*(b % 4)+0] = s0;
						ypair[b/4][2*(b % 4)+1] = s1;
					}
					y[2*i+0] = (uint8_t) ypair[0].to_ulong();
					y[2*i+1] = (uint8_t) ypair[1].to_ulong();
				}
				
				size_t out_bits;
				std::vector<uint8_t> z = puncture(y, r, &out_bits);
				
				return z;
			}
			
		private:
			
			std::vector<uint8_t> puncture(const std::vector<uint8_t>& x, rate_info_t::coding_rate_t r, size_t* out_bits)
			{
				// Retrieve appropriate puncturing mask for this coding rate
				const std::vector<bool>& mask = _puncture_masks[r];
				
				// Determine output size
				uint8_t num, den;
				rate_info_t::get_rate_num_den(r, &num, &den);
				*out_bits = (x.size() * 8 * den) / size_t(2*num);
				const size_t out_bytes = (*out_bits + 7) / 8;
				
				std::vector<uint8_t> y(out_bytes);
				size_t mask_idx = 0;
				size_t out_bit_count = 0;
				uint8_t ybyte = 0;
				for (size_t i=0; i<x.size(); i++)
				{
					for (uint8_t bit=0; bit<8; bit++)
					{
						if (mask[mask_idx])
						{
							// Select input bit
							const uint8_t xbit = (x[i] >> bit) & 0x1;
							
							// Shorthand: output byte and bit-in-byte indices
							const size_t out_byte_idx = out_bit_count / 8;
							const uint8_t out_bit_idx = out_bit_count & 0x7;
							
							// Write input bit into output byte
							ybyte |= xbit << out_bit_idx;
							
							// Write completed byte (or incomplete final byte)
							if (out_bit_idx == 0x7 || out_bit_count == (*out_bits)-1)
							{
								y[out_byte_idx] = ybyte;
								ybyte = 0;
							}
							
							// Increment output bit count
							out_bit_count++;
						}
						
						// Loop over puncturing mask
						mask_idx = mask_idx<mask.size()-1 ? mask_idx+1 : 0;
					}
				}
				
				assert(out_bit_count == *out_bits);
				
				return y;
			}
			
			static const std::bitset<7> _g0;
			static const std::bitset<7> _g1;
			static const std::vector<std::vector<bool> > _puncture_masks;
	};
	
	namespace internal
	{
		std::vector<std::vector<bool> > init_puncture_masks()
		{
			// Note: These puncturing 'masks' are derived from [Figure 17-9]
			std::vector<std::vector<bool> > masks(3);
			masks[rate_info_t::RATE_1_2] = {true};                                 // r = 1/2
			masks[rate_info_t::RATE_2_3] = {true, true, true, false};              // r = 2/3
			masks[rate_info_t::RATE_3_4] = {true, true, true, false, false, true}; // r = 3/4
			return masks;
		}
	}
	
	// Industry-standard generator polynomials
	const std::bitset<7> bcc_encoder::_g0("1101101");
	const std::bitset<7> bcc_encoder::_g1("1001111");
	// Puncturing "masks"
	const std::vector<std::vector<bool> > bcc_encoder::_puncture_masks = internal::init_puncture_masks();
	
}

#endif	/* WIFI_BCC_ENCODER_H */
