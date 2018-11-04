#ifndef WIFI_TYPES_H
#define	WIFI_TYPES_H

#include <cstddef>
#include <complex>

namespace wifi
{
	template<typename T>
	struct sample_t
	{
		typedef T complex_type;
		std::complex<T> data;
	};
	
	template<typename T>
	struct indexed_sample_t : public sample_t<T>
	{
		size_t index;
	};
}

#endif	/* WIFI_TYPES_H */
