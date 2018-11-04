#include "wifi/wifi_sig.h"
#include "wifi/wifi_peak_finder.h"
#include "wifi/wifi_autocorrelator_sliding.h"
#include "wifi/wifi_stf.h"
#include "wifi/wifi_ltf.h"
#include "wifi/wifi_stf_detector.h"
#include "wifi/wifi_annex_i1.h"
#include "util/fileio_helper.h"

#include <cstdlib>
#include <iostream>
#include <vector>

wifi::delay_line<std::complex<float>, 8> d;

void print(const std::vector<std::complex<float> >& x)
{
	for (size_t i=0; i<x.size(); i++)
		std::cout << x[i] << (i==x.size()-1 ? "" : ", ");
	std::cout << " (Length: " << x.size() << ")" << std::endl;
}

void do_delay_line_test(std::vector<int> x)
{
	// Cast to complex float
	std::vector<std::complex<float> > data(x.size());
	for (size_t i=0; i<x.size(); i++)
		data[i] = (float) x[i];
	
	std::vector<std::complex<float> > w;
	size_t Nw;
	w.resize(data.size());
	std::cout << "Updating with: "; print(data);
	d.update(&data[0], data.size(), &w[0], Nw);
	w.resize(Nw);
	std::cout << "Result: "; print(w);
	std::cout << std::endl;
}

struct data_t
{
	typedef float complex_type;
	std::complex<complex_type> data;
	size_t index;
};

void stf_tester()
{
	print(wifi::stf_64);
}

void ltf_tester()
{
	print(wifi::ltf_64);
}

void delay_line_tester()
{
	do_delay_line_test({0,1,2});
	do_delay_line_test({3,4,5,6});
	do_delay_line_test({7,8,9,10,11});
	do_delay_line_test({12,13,14,15,16,17,18,19,20,21,22,23});
}

void autocorrelator_tester()
{
	wifi::autocorrelator_sliding<data_t, 16, 9*16, 8192> autocorr;
	
	std::vector<data_t> x(200);
	for (size_t i=0; i<x.size(); i++)
	{
		x[i].data = std::complex<float>((float) i, (float)x.size()-1-i);
		x[i].index = i;
	}
	
	std::vector<data_t> rho(200);
	std::vector<typename data_t::complex_type> power_x(200);
	std::vector<typename data_t::complex_type> power_y(200);
	size_t Nrho;
	autocorr.update(&x[0], x.size(), &rho[0], Nrho, &power_x[0], &power_y[0]);
	rho.resize(Nrho);
	power_x.resize(Nrho);
	power_y.resize(Nrho);
	
	std::cout << "Nrho = " << Nrho << std::endl;
	for (size_t i=0; i<Nrho; i++)
		std::cout << rho[i].data << " [" << rho[i].index << "]" << std::endl;
}

void peak_finder_tester()
{
	wifi::peak_finder<data_t> p(0.7f, 10, 5);
	
	// Make a simple triangular peak
	std::vector<data_t> x(200);
	for (size_t i=0; i<x.size(); i++)
	{
		if (i<=x.size()/2)
			x[i].data = std::complex<float>(float(i) / float(x.size()/2), 0.0f);
		else
			x[i].data = std::complex<float>(float(x.size()-i) / float(x.size()/2), 0.0f);
		
		x[i].index = i;
	}
	
	// Shove something else in
	//x[110].data = std::complex<float>(0.0f, -1.01f);
	
	// Duplicate a few times
	const size_t Nreps = 4;
	x.resize(Nreps * x.size());
	for (size_t i=x.size()/Nreps; i<x.size(); i++)
	{
		x[i] = x[i % (x.size()/Nreps)];
		x[i].index = i;
	}
	
	// Run peak detect
	std::vector<data_t> detections;
	p.update(&x[0], x.size(), detections);
	
	std::cout << detections.size() << " detections: " << std::endl;
	for (size_t i=0; i<detections.size(); i++)
		std::cout << "[" << detections[i].index << "] : " << detections[i].data << std::endl;
	
}

void stf_detector_tester()
{
	// Load data from file
	std::vector<std::complex<int16_t> > x_i16 = util::load_file<std::complex<int16_t> >("C:/workspace/hmc/octave/wifi/snapshots/11n/wifi_11n_20M_MCS0_BCC_LGI_64Bytes.dat");
	
	// Convert to data_t
	std::vector<data_t> x(x_i16.size());
	for (size_t i=0; i<x.size(); i++)
	{
		x[i].data = std::complex<float>((float) x_i16[i].real(), (float) x_i16[i].imag());
		x[i].index = i;
	}
	
	const size_t MAX_SAMPS = 8192;
	wifi::stf_detector<data_t, MAX_SAMPS, true> stf_detector(0.7f);
	
	const size_t Nx = x.size();
	size_t offset = 0;
	while(offset < Nx)
	{
		size_t Nsamps = offset+MAX_SAMPS-1 < Nx ? MAX_SAMPS : Nx-offset;
		stf_detector.update(&x[offset], Nsamps);
		offset += MAX_SAMPS;
	}
}

void sig_tester()
{
	wifi::sig s;
	s.init(5, 100);
	
	std::cout << "SIG Length : " << s.get_length() << std::endl;
	std::cout << "SIG Mbps : " << s.get_rate_info().mbps << std::endl;
	std::cout << "SIG bits : " << s.get_bits() << std::endl;
}
void mapper_tester()
{
	const size_t Nbpsc = 4;
	
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
			map[gray2d[row][col]] = {float(offset + int(2*col)), float(offset + int(2*row))};
	
	// Print
	for (uint8_t i=0; i<M; i++)
		std::cout << map[i] << std::endl;

}


int main()
{
	//stf_tester();
//	ltf_tester();
	//delay_line_tester();
	//autocorrelator_tester();
	//peak_finder_tester();
	//stf_detector_tester();
	//sig_tester();
	//mapper_tester();
	
	wifi::annex_i1();
	
	return 0;
}

