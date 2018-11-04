#ifndef WIFI_SIG_H
#define	WIFI_SIG_H

#include <bitset>
#include <string>
#include <vector>
#include "wifi/wifi_rate_info.h"

namespace wifi
{
	class sig
	{
	public:
		sig() {}
		init(size_t mcs, size_t length)
		{
			_rate_info = rate_info_t(mcs);
			_length = length;
			
			_parity  = false;
				
			// Rate (4 bits)
			for (size_t i=0; i<4; i++)
			{
				_bits[i] = _rate_info.RATE[i];
				_parity ^= _bits[i];
			}
			
			// Reserved (1 bit)
			_bits[4] = false;
			
			// Length (12 bits)
			for (size_t i=0; i<12; i++)
			{
				_bits[5+i] = (_length >> i) & 0x1;
				_parity ^= _bits[5+i];
			}
			
			// Parity (1 bit)
			_bits[17] = _parity;
			
			// Tail (6 bits)
			for (size_t i=0; i<6; i++)
				_bits[18+i] = false;
		}
		
		rate_info_t get_rate_info() const { return _rate_info; }
		size_t get_length() const { return _length; }
		bool get_parity() const { return _parity; }
		const std::bitset<24>& get_bits() const { return _bits; }
		const std::vector<uint8_t> get_bytes()
		{
			unsigned long sig_bits_ul = this->get_bits().to_ulong();
			std::vector<uint8_t> sig_bytes(3);
			for (size_t i=0; i<3; i++)
				sig_bytes[i] = (sig_bits_ul >> i*8) & 0xFF;
			
			return sig_bytes;
		}
		
	private:
		rate_info_t _rate_info;
		size_t _length;
		bool _parity;
		std::bitset<24> _bits;
	};
}

#endif	/* WIFI_SIG_H */
