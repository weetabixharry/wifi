#ifndef WIFI_ANNEX_I1
#define	WIFI_ANNEX_I1

#include "wifi/wifi_modulator.h"
#include "util/fileio_helper.h"
#include <cstring>
#include <vector>
#include <iostream>

namespace wifi
{
	std::vector<uint8_t> construct_psdu(const uint8_t* mac, size_t Nmac, const uint8_t* msg, size_t Nmsg)
	{
		const size_t Nfcs = 4;
		std::vector<uint8_t> psdu(Nmac+Nmsg+Nfcs);
		
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
		for (size_t i=0; i<Nfcs; i++)
			psdu[Nmac+Nmsg+i] = (uint8_t) (crc32 >> 8*i) & 0xFF;
		
		return psdu;
	}
	
	void annex_i1()
	{
		const size_t mcs = 5;
		
		// ======================= %
		// == I.1.2 The Message == %
		// ======================= %

		// The 24-byte MAC header
		const uint8_t mac[24] = {4, 2, 0, 46, 0, 96, 8, 205, 55, 166, 0, 32, 214, 1, 60, 241, 0, 96, 8, 173, 59, 175, 0, 0};
		const size_t Nmac = sizeof(mac);
		
		// The 72-character message
		const char* msg = "Joy, bright spark of divinity,\nDaughter of Elysium,\nFire-insired we trea";
		const size_t Nmsg = strlen(msg);
		
		// Append 4-byte FCS
		std::vector<uint8_t> psdu = construct_psdu(&mac[0], Nmac, (const uint8_t*) &msg[0], Nmsg);
		
		// Save PSDU to file for MATLAB check
		util::save_data(psdu, "psdu.u8", false);
		
		// Save STF to file for MATLAB check
		util::save_data(wifi::stf_64, "stf_64.cf", false);
		util::save_data(wifi::stf_161, "stf_161.cf", false);
		
		// Save LTF to file for MATLAB check
		util::save_data(wifi::ltf_64, "ltf_64.cf", false);
		util::save_data(wifi::ltf_161, "ltf_161.cf", false);
		
		// Generate SIG
		wifi::sig sig_inst;
		sig_inst.init(mcs, psdu.size());
		
		// Retrieve raw bytes and rate info from SIG
		std::vector<uint8_t> sig_bytes = sig_inst.get_bytes();
		wifi::rate_info_t rate_info = sig_inst.get_rate_info();
		
		// Save SIG bits to file for MATLAB check
		util::save_data(sig_bytes, "sig_bits.u8", false);
		
		wifi::bcc_encoder bcc;
		std::vector<uint8_t> sig_encoded = bcc.encode(sig_bytes, rate_info_t::RATE_1_2);
		
		// Save encoded SIG bits to file for MATLAB check
		util::save_data(sig_encoded, "sig_encoded.u8", false);
		
		wifi::interleaver interleaver;
		std::vector<uint8_t> sig_interleaved = interleaver.interleave(sig_encoded, 48, 1);
		
		// Save interleaved SIG bits to file for MATLAB check
		util::save_data(sig_interleaved, "sig_interleaved.u8", false);
		
		wifi::mapper mapper;
		std::vector<std::complex<float> > sig_mapped = mapper.map(sig_interleaved, 1);
		
		// Save mapped SIG to file for MATLAB check
		util::save_data(sig_mapped, "sig_mapped.cf", false);
		
		wifi::modulator mod;
		mod.modulate(&mac[0], Nmac, (const uint8_t*) &msg[0], Nmsg, mcs);
		
		std::cout << "Done." << std::endl;
	}
	
}

#endif	/* WIFI_ANNEX_I1 */
