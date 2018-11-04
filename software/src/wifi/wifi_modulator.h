#ifndef WIFI_MODULATOR_H
#define	WIFI_MODULATOR_H

#include <vector>
#include "dsp/fft.h"
#include "comms/crc.h"
#include "wifi/wifi_stf.h"
#include "wifi/wifi_ltf.h"
#include "wifi/wifi_bcc_encoder.h"
#include "wifi/wifi_interleaver.h"
#include "wifi/wifi_mapper.h"
#include "wifi/wifi_scrambler.h"
#include "util/fileio_helper.h"
#include "wifi_sig.h"

namespace wifi
{
	class modulator
	{
		public:
			modulator() {}
			
			void modulate(const uint8_t* mac, size_t Nmac, const uint8_t* msg, size_t Nmsg, size_t mcs)
			{
				// Reset pilot polarity at start of every packet
				_p127_index = 0;
				
				// Reserve enough space for STF+LTF+SIG (+ 1-sample overlap)
				_data.reserve(160+160+81);
				
				// Write STF and LTF with overlap
				copy(wifi::stf_161.begin(), wifi::stf_161.end(), back_inserter(_data));
				_data[160] += wifi::ltf_161[0]; // (overlap)
				copy(wifi::ltf_161.begin()+1, wifi::ltf_161.end(), back_inserter(_data));
				
				// PSDU will comprise MAC heaeder, message and FCS
				const size_t Nfcs = 4;
				const size_t Npsdu = Nmac+Nmsg+Nfcs;
				
				// Generate SIG
				std::vector<std::complex<float> > sig_timedomain = generate_sig(mcs, Npsdu);
				
				// Write SIG data into data stream with 1-sample overlap
				_data[320] += sig_timedomain[0]; // (overlap)
				copy(sig_timedomain.begin()+1, sig_timedomain.end(), back_inserter(_data));
				
				// Generate DATA
				std::vector<std::complex<float> > data_timedomain = generate_data(mac, Nmac, msg, Nmsg, Npsdu);
				
				_data[_data.size()-1] += data_timedomain[0]; // (overlap)
				copy(data_timedomain.begin()+1, data_timedomain.end(), back_inserter(_data));
				
				util::save_data(_data, "whole_packet.cf", false);
			}
			
		private:
			
			void construct_psdu(const uint8_t* mac, size_t Nmac, const uint8_t* msg, size_t Nmsg, uint8_t* psdu)
			{
				// (1) MAC Header
				for (size_t i=0; i<Nmac; i++)
					psdu[i] = mac[i];
				
				// (2) Message
				for (size_t i=0; i<Nmsg; i++)
					psdu[Nmac+i] = msg[i];
				
				// Compute FCS
				comms::crc crc_generator;
				unsigned long crc32 = crc_generator.compute(&psdu[0], Nmac+Nmsg, comms::crc_algorithm_t::TABLE_FAST);
				
				// (3) FCS
				for (size_t i=0; i<4; i++)
					psdu[Nmac+Nmsg+i] = (uint8_t) (crc32 >> 8*i) & 0xFF;
			}
			
			std::vector<std::complex<float> > construct_ofdm_data(std::vector<std::complex<float> > x)
			{
				const size_t Nx = x.size();
				const size_t Nsyms = Nx / _data_subcarrs.size();
				
				// Construct OFDM symbols
				size_t ix = 0;
				size_t iy = 0;
				std::vector<std::complex<float> > y(Nsyms*80 + 1); // Note: +1 for overlap
				for (size_t sym=0; sym<Nsyms; sym++)
				{
					// Manage pilot polarity
					const int8_t pilot_polarity = _p127[_p127_index];
					if (_p127_index < 126)
						_p127_index++;
					else
						_p127_index = 0;
					
					std::complex<float> x64[64] = {0};
					
					// Insert data bins
					for (size_t k=0; k<_data_subcarrs.size(); k++)
						x64[32+_data_subcarrs[k]] = x[ix++] / 64.0f;
					
					// Insert pilot bins
					for (size_t k=0; k<_pilot_subcarrs.size(); k++)
						x64[32+_pilot_subcarrs[k]] = float(_pilot_values[k] * pilot_polarity) / 64.0f;
					
					// Do OFDM IFFT
					dsp::fft(&x64[0], 64, +1);
					
					// Twiddle sign to do fftshift *before* FFT
					for (size_t i=0; i<64/2; i++)
						x64[2*i+1] = -x64[2*i+1];
					
					// Add cyclic prefix and overlap sample
					std::complex<float> x81[81];
					for (size_t i=0; i<81; i++)
						x81[i] = x64[(i+2*64-80) % 64];
					
					// Apply overlap weighting
					x81[0] = x81[0] * 0.5f;
					x81[80] = x81[80] * 0.5f;
					
					// Superimpose overlapping first sample onto data stream
					y[iy] += x81[0];
					
					// Write all other samples into data stream
					for (size_t i=1; i<81; i++)
						y[iy+i] = x81[i];
					
					// Only increment index by 80, so we get 1-sample overlap
					iy += 80;
				}
				
				return y;
			}
			
			std::vector<std::complex<float> > generate_sig(size_t mcs, size_t psdu_length)
			{
				// Create SIG object
				_sig.init(mcs, psdu_length);
				
				// Retrieve raw SIG bytes
				std::vector<uint8_t> sig_bytes = _sig.get_bytes();
				
				// BCC encode SIG
				std::vector<uint8_t> sig_encoded = _bcc.encode(sig_bytes, rate_info_t::RATE_1_2);
				
				// Interleave SIG
				std::vector<uint8_t> sig_interleaved = _interleaver.interleave(sig_encoded, 48, 1);
				
				// Map SIG
				std::vector<std::complex<float> > sig_mapped = _mapper.map(sig_interleaved, 1);
				
				// Generate time-domain SIG data
				std::vector<std::complex<float> > sig_timedomain = construct_ofdm_data(sig_mapped);
				
				return sig_timedomain;
			}
			
			std::vector<std::complex<float> > generate_data(const uint8_t* mac, size_t mac_bytes,
															const uint8_t* msg, size_t msg_bytes,
															const size_t psdu_bytes)
			{
				// Get SIG info
				wifi::rate_info_t rate_info = _sig.get_rate_info();
				
				// Number of required BITS
				const size_t Nservice = 16;
				const size_t Npsdu = psdu_bytes*8;
				const size_t Ntail = 6;
				const size_t Nunpadded = Nservice + Npsdu + Ntail;

				// Round up to integer number of OFDM symbols
				const size_t Ndbps = rate_info.Ndbps;
				const size_t Nsyms = (Nunpadded + Ndbps - 1) / Ndbps;
				const size_t Npadded = Nsyms * Ndbps;
				const size_t Npad = Npadded - Nunpadded;
				
				size_t Nbytes = (Npadded+7) / 8;
				
				std::vector<uint8_t> data(Nbytes);
				data[0] = 0; // SERVICE byte 1 of 2
				data[1] = 0; // SERVICE byte 2 of 2
				// Construct PSDU (append 4-byte FCS)
				construct_psdu(mac, mac_bytes, msg, msg_bytes, &data[2]);
				// TAIL and PADDING
				for (size_t i=2+Npsdu; i<Nbytes; i++)
					data[i] = 0;
				
				util::save_data(data, "scrambler_in.u8", false);
				
				// Scramble
				_scrambler.scramble(data);
				
				// N.B. Zero the tail bits again before encoding (see I.1.5.2)
				data[(Nservice+Npsdu)/8] &= 0xC0;
				
				util::save_data(data, "data_scrambled.u8", false);
				
				std::vector<uint8_t> punctured_data = _bcc.encode(&data[0], data.size(), rate_info.r);
				
				util::save_data(punctured_data, "data_punctured.u8", false);
				
				std::vector<uint8_t> interleaved_data = _interleaver.interleave(punctured_data, rate_info.Ncbps, rate_info.Nbpsc);
				
				util::save_data(interleaved_data, "data_interleaved.u8", false);
				
				// Map DATA
				std::vector<std::complex<float> > mapped_data = _mapper.map(interleaved_data, rate_info.Nbpsc);
				
				util::save_data(mapped_data, "data_mapped.cf", false);
				
				// Generate time-domain DATA
				std::vector<std::complex<float> > timedomain_data = construct_ofdm_data(mapped_data);
				
				util::save_data(timedomain_data, "data_timedomain.cf", false);

				return timedomain_data;
			}
			
			sig _sig;
			bcc_encoder _bcc;
			interleaver _interleaver;
			mapper _mapper;
			scrambler _scrambler;
			std::vector<std::complex<float> > _data;
			
			size_t _p127_index;
			static const std::vector<int8_t> _p127; // Pilot polarities
			static const std::vector<int8_t> _pilot_values; // Pilot values
			static const std::vector<int8_t> _pilot_subcarrs; // Pilot subcarriers
			static const std::vector<int8_t> _data_subcarrs; // Data subcarriers
	};
	
const std::vector<int8_t> modulator::_p127 = {1,1,1,1,  -1,-1,-1,1,  -1,-1,-1,-1,  1,1,-1,1, -1,-1,1,1,  -1,1,1,-1,  1,1,1,1,  1,1,-1,1,
									1,1,-1,1,  1,-1,-1,1,   1,1,-1,1,  -1,-1,-1,1,  -1,1,-1,-1,  1,-1,-1,1,  1,1,1,1,  -1,-1,1,1,
									-1,-1,1,-1,  1,-1,1,1,  -1,-1,-1,1,  1,-1,-1,-1,  -1,1,-1,-1,  1,-1,1,1,  1,1,-1,1,  -1,1,-1,1,
									-1,-1,-1,-1,  -1,1,-1,1,  1,-1,1,-1,  1,1,1,-1,  -1,1,-1,-1,  -1,1,1,1,  -1,-1,-1,-1,  -1,-1,-1};

const std::vector<int8_t> modulator::_pilot_values = {1, 1, 1, -1};

const std::vector<int8_t> modulator::_pilot_subcarrs = {-21, -7, 7, 21};

const std::vector<int8_t> modulator::_data_subcarrs = {-26, -25, -24, -23, -22,
													   -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9 , -8 ,
													   -6 , -5 , -4 , -3 , -2 , -1,
													    1 ,  2 ,  3 ,  4 ,  5 ,  6,
													    8 ,  9 ,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,
														22,  23,  24,  25,  26};
}

#endif	/* WIFI_MODULATOR_H */
