#ifndef CRC_H
#define	CRC_H

#include <iostream>

namespace comms
{
	enum crc_algorithm_t{TABLE = 0, TABLE_FAST, BITBYBIT, BITBYBIT_FAST};
	
	// Note: This is a C++ class version of CRC tester v1.3 by Sven Reifegerste.
	class crc
	{
	public:
		
		crc(int order = 32, unsigned long polynom = 0x4c11db7,
			bool direct = true, unsigned long reginit = 0xffffffff,
			unsigned long xorr = 0xffffffff, bool in_reflect = true,
			bool out_reflect = true) :
				_order(order),
				_polynom(polynom),
				_direct(direct),
				_init(reginit),
				_xor(xorr),
				_in_reflect(in_reflect),
				_out_reflect(out_reflect)
		{
			init();
		}
		
		// Note: Init should be re-called as required after any setters
		void init()
		{
			// At first, compute constant bit masks for whole CRC and CRC high bit
			_mask = ((((unsigned long)1<<(_order-1))-1)<<1)|1;
			_highbit = (unsigned long)1<<(_order-1);
			
			// Check parameters...

			if (_order < 1 || _order > 32)
				std::cout << "ERROR! Invalid _order. It must be in [1,32]." << std::endl;

			if (_polynom != (_polynom & _mask))
				std::cout << "ERROR! Invalid _polynom." << std::endl;

			if (_init != (_init & _mask))
				std::cout << "ERROR! Invalid _init." << std::endl;

			if (_xor != (_xor & _mask))
				std::cout << "ERROR! Invalid _xor." << std::endl;
			
			// Generate lookup table
			generate_crc_table();
			
			// Compute missing initial CRC value
			if (!_direct)
			{
				_init_nondirect = _init;
				unsigned long crc = _init;
				for (int i=0; i<_order; i++)
				{
					unsigned long bit = crc & _highbit;
					crc <<= 1;
					if (bit)
						crc ^= _polynom;
				}
				crc &= _mask;
				_init_direct = crc;
			}
			else
			{
				_init_direct = _init;
				unsigned long crc = _init;
				for (int i=0; i<_order; i++)
				{
					unsigned long bit = crc & 1;
					if (bit)
						crc ^= _polynom;
					crc >>= 1;
					if (bit)
						crc |= _highbit;
				}	
				_init_nondirect = crc;
			}
		}
				
		unsigned long compute(const unsigned char* p, unsigned long len,
					crc_algorithm_t algorithm = crc_algorithm_t::BITBYBIT_FAST)
		{
			switch(algorithm)
			{
				case crc_algorithm_t::TABLE :
					return crctable(p, len);
				case crc_algorithm_t::TABLE_FAST :
					return crctablefast(p, len);
				case crc_algorithm_t::BITBYBIT :
					return crcbitbybit(p, len);
				case crc_algorithm_t::BITBYBIT_FAST :
					return crcbitbybitfast(p, len);
				default :
					std::cout << "ERROR! Invalid CRC algorithm." << std::endl;
					return 0;
			}
		}
		
		// Getters and setters...
		void set_order(int order) { _order = order; }
		int get_order() const { return _order; }
		
		void set_polynom(unsigned long polynom) { _polynom = polynom; }
		unsigned long get_polynom() const { return _polynom; }
		
		void set_direct(bool direct) { _direct = direct; }
		bool get_direct() const { return _direct; }
		
		void set_init(unsigned long init) { _init = init; }
		unsigned long get_init() const { return _init; }
		
		void set_xor(unsigned long x) { _xor = x; }
		unsigned long get_xor() const { return _xor; }
		
		void set_in_reflect(bool r) { _in_reflect = r; }
		bool get_in_reflect() const { return _in_reflect; }
		
		void set_out_reflect(bool r) { _out_reflect = r; }
		bool get_out_reflect() const { return _out_reflect; }
		
	private:
		
		// HELPER: Reflects the lower 'bitnum' bits of 'crc'
		unsigned long reflect(unsigned long crc, int bitnum)
		{
			unsigned long j = 1;
			unsigned long crcout = 0;

			for (unsigned long i=(unsigned long)1<<(bitnum-1); i; i>>=1)
			{
				if (crc & i)
					crcout |= j;
				j <<= 1;
			}
			return crcout;
		}
		
		// HELPER: Make CRC lookup table used by table algorithms
		void generate_crc_table()
		{
			for (int i=0; i<256; i++)
			{
				unsigned long crc = (unsigned long) i;
				if (_in_reflect)
					crc = reflect(crc, 8);
				crc <<= _order-8;

				for (int j=0; j<8; j++)
				{
					unsigned long bit = crc & _highbit;
					crc <<= 1;
					if (bit)
						crc ^= _polynom;
				}			

				if (_in_reflect)
					crc = reflect(crc, _order);
				crc &= _mask;
				_tab[i]= crc;
			}
		}
		
		// Fast lookup table algorithm without augmented zero bytes, e.g. used 
		// in pkzip. Only usable with polynom orders of 8, 16, 24 or 32.
		unsigned long crctablefast(const unsigned char* p, unsigned long len)
		{
			if (_order&7)
			{
				std::cout << "ERROR! LUT-based CRC must take integer number of bytes" << std::endl;
				return 0;
			}
			
			unsigned long crc = _init_direct;

			if (_in_reflect)
				crc = reflect(crc, _order);

			if (!_in_reflect)
				while (len--)
					crc = (crc << 8) ^ _tab[ ((crc >> (_order-8)) & 0xff) ^ *p++];
			else
				while (len--)
					crc = (crc >> 8) ^ _tab[ (crc & 0xff) ^ *p++];

			if (_out_reflect^_in_reflect)
				crc = reflect(crc, _order);
			crc ^= _xor;
			crc &= _mask;

			return crc;
		}
		
		// Normal lookup table algorithm with augmented zero bytes.
		// Only usable with polynom orders of 8, 16, 24 or 32.
		unsigned long crctable(const unsigned char* p, unsigned long len)
		{
			if (_order&7)
			{
				std::cout << "ERROR! LUT-based CRC must take integer number of bytes" << std::endl;
				return 0;
			}
			
			unsigned long crc = _init_nondirect;

			if (_in_reflect)
				crc = reflect(crc, _order);

			if (!_in_reflect)
				while (len--)
					crc = ((crc << 8) | *p++) ^ _tab[ (crc >> (_order-8))  & 0xff];
			else
				while (len--)
					crc = ((crc >> 8) | (*p++ << (_order-8))) ^ _tab[ crc & 0xff];

			if (!_in_reflect)
				while (++len < _order/8)
					crc = (crc << 8) ^ _tab[ (crc >> (_order-8))  & 0xff];
			else
				while (++len < _order/8)
					crc = (crc >> 8) ^ _tab[crc & 0xff];

			if (_out_reflect^_in_reflect)
				crc = reflect(crc, _order);
			crc ^= _xor;
			crc &= _mask;

			return crc;
		}
		
		// Bit by bit algorithm with augmented zero bytes.
		// Does not use lookup table, suited for polynom orders between 1...32.
		unsigned long crcbitbybit(const unsigned char* p, unsigned long len)
		{
			unsigned long crc = _init_nondirect;

			for (unsigned long i=0; i<len; i++)
			{
				unsigned long c = (unsigned long) p[i];
				if (_in_reflect)
					c = reflect(c, 8);

				for (unsigned long j=0x80; j; j>>=1)
				{
					unsigned long bit = crc & _highbit;
					crc <<= 1;
					if (c & j)
						crc |= 1;
					if (bit)
						crc ^= _polynom;
				}
			}	

			for (unsigned long i=0; i<_order; i++)
			{
				unsigned long bit = crc & _highbit;
				crc <<= 1;
				if (bit)
					crc ^= _polynom;
			}

			if (_out_reflect)
				crc = reflect(crc, _order);
			crc ^= _xor;
			crc &= _mask;

			return crc;
		}
		
		// Fast bit by bit algorithm without augmented zero bytes.
		// Does not use lookup table, suited for polynom orders between 1...32.
		unsigned long crcbitbybitfast(const unsigned char* p, unsigned long len)
		{
			unsigned long crc = _init_direct;

			for (unsigned long i=0; i<len; i++)
			{
				unsigned long c = (unsigned long) p[i];
				if (_in_reflect)
					c = reflect(c, 8);

				for (unsigned long j=0x80; j; j>>=1)
				{
					unsigned long bit = crc & _highbit;
					crc <<= 1;
					if (c & j)
						bit ^= _highbit;
					if (bit)
						crc ^= _polynom;
				}
			}	

			if (_out_reflect)
				crc = reflect(crc, _order);
			crc ^= _xor;
			crc &= _mask;

			return crc;
		}
		
		// CRC parameters
		int _order;                 // [1..32] is the CRC polynom order, counted without the leading '1' bit
		unsigned long _polynom;     // is the CRC polynom *WITHOUT* leading '1' bit
		bool _direct;               // [0,1] specifies the kind of algorithm: 1=direct, no augmented zero bits
		unsigned long _init;        // is the initial CRC value belonging to that algorithm
		unsigned long _xor;         // is the final XOR value
		bool _in_reflect;           // [0,1] specifies if a data byte is reflected before processing (UART) or not
		bool _out_reflect;          // [0,1] specifies if the CRC will be reflected before XOR
		
		// Internal variables
		unsigned long _mask;
		unsigned long _highbit;
		unsigned long _init_direct;
		unsigned long _init_nondirect;
		unsigned long _tab[256];
	};
	
} // namespace comms
#endif /* CRC_H */
