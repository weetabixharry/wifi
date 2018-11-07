#ifndef WIFI_ANNEX_I1
#define	WIFI_ANNEX_I1

#include "wifi/wifi_modulator.h"
#include <cstring>

namespace wifi
{
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
		
		// Instantiate modulator
		wifi::modulator mod;
		mod.modulate((const uint8_t*) &msg[0], Nmsg, mcs, &mac[0], Nmac);
		
		std::cout << "Done." << std::endl;
	}
	
}

#endif	/* WIFI_ANNEX_I1 */
