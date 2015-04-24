#include <vector>

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>
#include <LMM/numeric/NumericalMethods.h>


void test_BlackGreek()
{
	double fwd     = 100;
	double K	   = 100;
	double vol     = 0.2;
	double T       = 1;  // r = 0;

	double callPrice = NumericalMethods::Black_Price(fwd, K, vol, T);
	double callVega  = NumericalMethods::Black_Vega(fwd, K, vol, T);
	double callVolga = NumericalMethods::Black_Volga(fwd, K, vol, T);


	double strike_begin = 0.01;
	double strike_end   = 3*K;
	size_t strike_size  = 100;
	double strike_pace  = (strike_end - strike_begin) / (strike_size -1); 
	std::vector<double> strikes(strike_size);
	std::vector<double> Vegas(strike_size);
	std::vector<double> Volgas(strike_size);

	for(size_t i=0; i<strike_size; ++i)
	{
		strikes[i]  = strike_begin + i* strike_pace;
		Vegas[i]    = NumericalMethods::Black_Vega(fwd, strikes[i], vol, T);
		Volgas[i]   = NumericalMethods::Black_Volga(fwd, strikes[i], vol, T);
	}


	//! print 
	std::string fileName = "test_BlackGreek.csv"; //+ MCSchemeType::mcSchemeType2String(mcSchemeType) +".csv";
	std::string path = LMMPATH::get_output_path() + fileName;

	PrintElement_PTR stirkes_print      = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("strike", strikes));
	PrintElement_PTR Black_Vegas_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Black_Vegas", Vegas));
	PrintElement_PTR Black_Volgas_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Black_Volgas", Volgas));

	std::vector<PrintElement_PTR> elements_print;
	elements_print.push_back(stirkes_print);
	elements_print.push_back(Black_Vegas_print);
	elements_print.push_back(Black_Volgas_print);

	Printer printer(path, elements_print);
	printer.print();



	//! impliedvol formula	
	double impliedvol = NumericalMethods::Black_impliedVolatility(callPrice, fwd, K, T);
	std::cout << "impliedvol =" << impliedvol << std::endl;
}

