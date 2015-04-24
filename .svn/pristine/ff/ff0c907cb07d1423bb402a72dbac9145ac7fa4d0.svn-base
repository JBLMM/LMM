#include <LMM/LmmModel/UpperTriangleVanillaSwaptionQuotes.h>
#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>
#include <LMM/helper/BuildVariable.h>
#include <LMM/pricer/LmmVanillaSwapPricer.h>

#include <fstream>

// ====   LIBOR QUOTE   ====  LIBOR QUOTE   ====  LIBOR QUOTE   ====  LIBOR QUOTE   ====  //
LiborQuotes::LiborQuotes(LMMTenorStructure_PTR lmmTenorStructure_ptr, const double constant_rate)
	: lmmTenorStructure_(lmmTenorStructure_ptr) 
{
	generate_DiscountFactor(constant_rate) ;
	initiate_Libors_from_DiscountFactor()  ;
}

LiborQuotes::LiborQuotes(LMMTenorStructure_PTR lmmTenorStructure_ptr, const std::vector<double>& discountfactor)
	: lmmTenorStructure_(lmmTenorStructure_ptr) 
	, discountFactor_(discountfactor)
{
	assert(discountFactor_.size() == lmmTenorStructure_->get_nbLIBOR()+1 );
	initiate_Libors_from_DiscountFactor()  ;
}

void LiborQuotes::reset_Libor(const std::vector<double>& libor_quotes)
{
	size_t nbLibor = lmmTenorStructure_->get_nbLIBOR();
	assert(libor_quotes.size() >= nbLibor) ;
	assert(liborInit_.size() == nbLibor) ;

	for(size_t iLibor=0;iLibor<liborInit_.size();++iLibor)
	{
		liborInit_[iLibor]=libor_quotes[iLibor];	
	}

	 initiate_DiscountFactor_From_Libors();
}

LiborQuotes_ConstPTR LiborQuotes::create_LiborInit(LMMTenorStructure_PTR lmmTenorStructure, const double constant_rate)
{
	LiborQuotes_ConstPTR libor_quote_ptr(new LiborQuotes(lmmTenorStructure,constant_rate) );
	return libor_quote_ptr;
}

void LiborQuotes::generate_DiscountFactor(const double constant_rate)
{
	size_t nbDF = lmmTenorStructure_->get_nbLIBOR() +1 ;
	assert(nbDF>2);

	discountFactor_.resize( nbDF );
	for(size_t i=0;i<nbDF;++i)
	{
		const double date_T_i = lmmTenorStructure_->get_tenorDate(i);
		discountFactor_[i]= std::exp( - (constant_rate * date_T_i) );
	}
}


void LiborQuotes::initiate_Libors_from_DiscountFactor()
{
	size_t nbLibor = discountFactor_.size() - 1;
	assert( nbLibor > 0 );//number of discounfactor >=3 in order to generate the Libors
	assert(nbLibor == lmmTenorStructure_->get_nbLIBOR() );

	liborInit_.resize(nbLibor);
	for(size_t iLibor=0;iLibor<nbLibor;++iLibor)
	{
		const double tau_i = lmmTenorStructure_->get_deltaT(iLibor);
		liborInit_[iLibor] = 1./tau_i *( discountFactor_[iLibor]/discountFactor_[iLibor+1] - 1. );
	}
}

void LiborQuotes::initiate_DiscountFactor_From_Libors()
{
	size_t nbLibor = discountFactor_.size() - 1;
	assert( nbLibor > 0 );//number of discounfactor >=3 
	assert(nbLibor == lmmTenorStructure_->get_nbLIBOR() );

	for(size_t iLibor=0;iLibor<nbLibor;++iLibor)
	{
		const double tau_i = lmmTenorStructure_->get_deltaT(iLibor);
		discountFactor_[iLibor+1] = discountFactor_[iLibor] / (1. + tau_i* liborInit_[iLibor]);		
	}
}


bool LiborQuotes::check_data_consistency() const
{
	bool data_consistency = true;

	//! check data format
	size_t nbLibor=lmmTenorStructure_->get_nbLIBOR();
	if(nbLibor != liborInit_.size() ) return false;
	if(discountFactor_.size() != nbLibor+1 ) return false;
	if(discountFactor_[0]!=1.) return false;

	for(size_t iLibor=0;iLibor<nbLibor;++iLibor)
	{
		const double tau_i = lmmTenorStructure_->get_deltaT(iLibor);
		const double libor_calculated_from_df = 1./tau_i *( discountFactor_[iLibor]/discountFactor_[iLibor+1] - 1. );
		if(liborInit_[iLibor] != libor_calculated_from_df) 
		{
			data_consistency = false;
			break;
		}
	}
	return data_consistency;
}

void LiborQuotes::print(const std::string& filename, const bool erase_file) const
{
	std::string path_OutPut = LMMPATH::get_output_path() + filename;
	std::vector<PrintElement_PTR> elements_print;

	PrintElement_PTR date_print_ptr  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Dates", lmmTenorStructure_->get_tenorDate() ));
	PrintElement_PTR df_print_ptr    = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Discount Rate", discountFactor_ ));
	PrintElement_PTR libor_print_ptr = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("LIBOR", liborInit_ ));

	elements_print.push_back(date_print_ptr);
	elements_print.push_back(df_print_ptr);
	elements_print.push_back(libor_print_ptr);

	{
		Printer printer(path_OutPut, elements_print);
		printer.print(erase_file);
	}
}
// ====    END LIBOR QUOTE   ====    END LIBOR QUOTE   ====    END LIBOR QUOTE   ====    END LIBOR QUOTE   ====  //


UpperTriangleVanillaSwaptionQuotes::UpperTriangleVanillaSwaptionQuotes(
	LMMTenorStructure_PTR lmmTenorStructure,
	int   lastYear, 	
	const Tenor& fixedTenor,
	const Tenor& floatingTenor,
	const UpperTriangularDoubleMatrix& strikes, // 1st row and column not used like g!
	const UpperTriangularDoubleMatrix& quotes)  // 1st row and column not used like g!
	: lmmTenorStructure_(lmmTenorStructure)
	, lastYear_(lastYear)
	, fixedTenor_(fixedTenor)
	, floatingTenor_(floatingTenor)
	, indexRatio_(fixedTenor.NbOfMonth()/floatingTenor.NbOfMonth())
	, upperTriangleVanillaSwaptionQuotes_(lastYear+1, lastYear+1)
	, indexMapping_gDelegate_gTransformed_(lastYear+1, lastYear+1)
{
	assert(lmmTenorStructure_->get_horizon() == lastYear*indexRatio_); 
	assert(lastYear>1);
	assert(strikes.size1() == strikes.size2() && strikes.size1() == lastYear_+1); 
	assert(quotes.size1()  == quotes.size2()  && quotes.size1()  == lastYear_+1);
	assert(fixedTenor.NbOfMonth() == 12 && fixedTenor.NbOfMonth()%floatingTenor.NbOfMonth() ==0);
	assert(lmmTenorStructure->get_tenorType() == floatingTenor);

	//! upperTriangle:
	for(size_t iExpirity = 1; iExpirity<quotes.size1(); ++iExpirity) // row
	{
		for(size_t jTenor = 1; jTenor<quotes.size2()-iExpirity; ++jTenor) // col
		{
			double strike = strikes(iExpirity,jTenor);
			//size_t indexStart = computeStartIndex(iExpirity);
			//size_t indexEnd   = computeIndexEnd(iExpirity,jTenor);
			size_t indexStart = iExpirity*indexRatio_;
			size_t indexEnd   = indexStart + jTenor*indexRatio_;
			VanillaSwap     vanillaSwap(strike, indexStart, indexEnd, floatingTenor_, fixedTenor_, lmmTenorStructure);
			VanillaSwaption vanillaSwaption(vanillaSwap, OptionType::OptionType::CALL);

			double quote = quotes(iExpirity,jTenor);

			std::pair<VanillaSwaption, double> p(vanillaSwaption, quote);

			upperTriangleVanillaSwaptionQuotes_(iExpirity,jTenor) = p;
		}
	}

	initialize_gDelegate_IndexIngTransformed();
}



void UpperTriangleVanillaSwaptionQuotes::initialize_gDelegate_IndexIngTransformed() // 1st row and column not used! as g!  // UTM: upperTriangleMatrix
{
	//! 1st column and 1st row initialized to absurd value
	for(size_t k=0; k<indexMapping_gDelegate_gTransformed_.size1(); ++k)
	{
		indexMapping_gDelegate_gTransformed_(0,k) = std::pair<LMM::Index,LMM::Index>(-1000, -1000);
		indexMapping_gDelegate_gTransformed_(k,0) = std::pair<LMM::Index,LMM::Index>(-1000, -1000);
	}
	//! upperTriangle:
	for(size_t iDelegated = 1; iDelegated<indexMapping_gDelegate_gTransformed_.size1(); ++iDelegated) // row
	{
		for(size_t jDelegated = 1; jDelegated<indexMapping_gDelegate_gTransformed_.size2()-iDelegated; ++jDelegated) // col
		{
			// ok for any indexRatio_ value
			size_t iRow_gMatrix = iDelegated*indexRatio_;	
			size_t jCol_gMatrix = jDelegated*indexRatio_;
			indexMapping_gDelegate_gTransformed_(iDelegated,jDelegated) = std::pair<LMM::Index,LMM::Index>( iRow_gMatrix, jCol_gMatrix); 
		}
	}
}

void UpperTriangleVanillaSwaptionQuotes::print(const std::string& filename, const bool erase_file) const
{
	std::string path_OutPut = LMMPATH::get_output_path() + filename;

	{
		std::vector<PrintElement_PTR> elements_print;
		UpperTriangularIndexPairMatrix swap_indices_matrix = get_UpperTriangularSwaptionIndexMatrix();	
		PrintElement_PTR swapMatrix_print = PrintElement_PTR(new MatrixPrintElement<UpperTriangularIndexPairMatrix>("swaps",  swap_indices_matrix));
		PrintElement_PTR mapMatrix_print = PrintElement_PTR(new MatrixPrintElement<UpperTriangularIndexPairMatrix>("mapping Indices",  indexMapping_gDelegate_gTransformed_));

		elements_print.push_back(swapMatrix_print);
		elements_print.push_back(mapMatrix_print);

		Printer printer(path_OutPut, elements_print);
		printer.print(erase_file);
	}


	{
		std::ofstream data_stream ;
		data_stream.open(path_OutPut.c_str(), std::ios::app);
		data_stream<<std::endl<<"--------"<<std::endl;
		data_stream.close();
	}

	{
		std::vector<PrintElement_PTR> elements_print;

		UpperTriangularDoubleMatrix quote_matrix  = get_UpperTriangularQuoteValues() ;
		UpperTriangularDoubleMatrix strike_matrix = get_UpperTriangularStrike() ;
		PrintElement_PTR quoteMatrix_print = PrintElement_PTR(new MatrixPrintElement<UpperTriangularDoubleMatrix>("Quote",  quote_matrix));
		PrintElement_PTR strikeMatrix_print = PrintElement_PTR(new MatrixPrintElement<UpperTriangularDoubleMatrix>("Strike",  strike_matrix));

		elements_print.push_back(quoteMatrix_print);
		elements_print.push_back(strikeMatrix_print);
		Printer printer(path_OutPut, elements_print);
		printer.print(false);
	}
}


UpperTriangleVanillaSwaptionQuotes_ConstPTR UpperTriangleVanillaSwaptionQuotes::create_ATMSwaptionImpliedVol
	(
	LiborQuotes_ConstPTR libor_quotes_ptr,
	const Tenor&  fixedTenor,
	const Tenor&  floatingTenor,
	LmmVanillaSwaptionApproxPricer_Rebonato_PTR black_vol_approx_ptr,
	const double&  strike_bump
	)
{
	LMMTenorStructure_PTR pLMMTenorStructure = libor_quotes_ptr->get_LMMTenorStructure_PTR() ;
	size_t fix_float_ratio = fixedTenor.ratioTo(floatingTenor);
	size_t lastLiborIndex  = pLMMTenorStructure->get_nbLIBOR()-1;
	assert(lastLiborIndex%fix_float_ratio == 0);
	size_t last_year = lastLiborIndex / fix_float_ratio ;

	size_t matrix_size = last_year + 1 ;
	UpperTriangularDoubleMatrix swap_rate_matrix(matrix_size,matrix_size);
	UpperTriangularDoubleMatrix black_vol_matrix(matrix_size,matrix_size);

	//first row and first column never used
	for(size_t k=0;k<matrix_size;++k)
	{
		swap_rate_matrix(k,0) = -1000000; swap_rate_matrix(0,k) = -1000000;
		black_vol_matrix(k,0) = -1000000; black_vol_matrix(0,k) = -1000000;
	}


	const std::vector<double> & init_libor = libor_quotes_ptr->get_InitLibor();
	LmmVanillaSwapPricer swap_pricer(pLMMTenorStructure);
	for(size_t iMaturity=1;iMaturity<matrix_size;++iMaturity)
	{
		for(size_t jTenor=1;jTenor<matrix_size - iMaturity ;++jTenor)
		{
			size_t start_swap_index = iMaturity * fix_float_ratio;
			size_t end_swap_index   = (iMaturity+jTenor) * fix_float_ratio;	

			double empty_strike = -100000000;
			VanillaSwap swap_ij(empty_strike,start_swap_index,end_swap_index,floatingTenor,fixedTenor,pLMMTenorStructure);
			double swap_rate_ij = strike_bump+swap_pricer.swapRate_Analytical(swap_ij,init_libor);

			swap_ij.set_strike(swap_rate_ij);
			swap_rate_matrix(iMaturity,jTenor) = swap_rate_ij;

			VanillaSwaption swaption_ij(swap_ij, OptionType::OptionType::CALL);
			double black_vol_ij = black_vol_approx_ptr->volBlack(swaption_ij,init_libor);

			black_vol_matrix(iMaturity,jTenor)=black_vol_ij;
		}
	}

	UpperTriangleVanillaSwaptionQuotes_PTR atm_swaption_implied_vol 
		(new UpperTriangleVanillaSwaptionQuotes(
		pLMMTenorStructure,
		last_year,
		fixedTenor,
		floatingTenor,
		swap_rate_matrix,
		black_vol_matrix
		) 
		);

	atm_swaption_implied_vol->set_Data_FileName("VirtuallyCreatedData");

	return atm_swaption_implied_vol;
}


double UpperTriangleVanillaSwaptionQuotes::get_MinQuote() const 
{
	double min_quote(1000000.);

	size_t nbRow = upperTriangleVanillaSwaptionQuotes_.size1();
	size_t nbCol = upperTriangleVanillaSwaptionQuotes_.size2();
	assert(nbRow == nbCol);

	for(size_t iExpirity = 1; iExpirity<nbRow; ++iExpirity) // row
	{
		for(size_t jTenor = 1; jTenor<nbCol-iExpirity; ++jTenor) // col
		{
			const double & quote = upperTriangleVanillaSwaptionQuotes_(iExpirity,jTenor).second;
			assert(quote > 0) ; // vol quote has to be positive , skew quote to do after
			if( min_quote > quote )  min_quote = quote ;
		}
	}

	return min_quote;
}

double UpperTriangleVanillaSwaptionQuotes::get_MaxQuote() const 
{
	double max_quote(-1000000.);

	size_t nbRow = upperTriangleVanillaSwaptionQuotes_.size1();
	size_t nbCol = upperTriangleVanillaSwaptionQuotes_.size2();
	assert(nbRow == nbCol);

	for(size_t iExpirity = 1; iExpirity<nbRow; ++iExpirity) // row
	{
		for(size_t jTenor = 1; jTenor<nbCol-iExpirity; ++jTenor) // col
		{
			const double & quote = upperTriangleVanillaSwaptionQuotes_(iExpirity,jTenor).second;
			assert(quote > 0) ; // vol quote has to be positive , skew quote to do after
			if( max_quote < quote )  max_quote = quote ;
		}
	}

	return max_quote ;
}


UpperTriangularDoubleMatrix UpperTriangleVanillaSwaptionQuotes::get_UpperTriangularQuoteValues() const 
{
	if(LMM::WARNLMM ())
		std::cout<<LMM::WARN_MSG << "UpperTriangleVanillaSwaptionQuotes::get_UpperTriangularQuoteValues() generate matrix copy, DO NOT USE IN A LOOP" << std::endl;

	size_t nbRow = upperTriangleVanillaSwaptionQuotes_.size1();
	size_t nbCol = upperTriangleVanillaSwaptionQuotes_.size2();
	assert(nbRow == nbCol);
	UpperTriangularDoubleMatrix quote_matrix(nbRow,nbCol );

	for(size_t k=0;k<nbRow;++k)//not use first row firt column
	{
		quote_matrix(0,k)=-10000;
		quote_matrix(k,0)=-10000;
	}

	for(size_t iExpirity = 1; iExpirity<nbRow; ++iExpirity) // row
	{
		for(size_t jTenor = 1; jTenor<nbCol-iExpirity; ++jTenor) // col
		{
			const double & quote = upperTriangleVanillaSwaptionQuotes_(iExpirity,jTenor).second;
			quote_matrix(iExpirity,jTenor) = quote;
		}
	}
	return quote_matrix;
}

UpperTriangularDoubleMatrix UpperTriangleVanillaSwaptionQuotes::get_UpperTriangularStrike() const
{
	if(LMM::WARNLMM ())
		std::cout<<LMM::WARN_MSG << "UpperTriangleVanillaSwaptionQuotes::get_UpperTriangularStrike() generate matrix copy, DO NOT USE IN A LOOP" << std::endl;

	size_t nbRow = upperTriangleVanillaSwaptionQuotes_.size1();
	size_t nbCol = upperTriangleVanillaSwaptionQuotes_.size2();
	assert(nbRow == nbCol);
	UpperTriangularDoubleMatrix strike_matrix(nbRow,nbCol );

	for(size_t k=0;k<nbRow;++k)//not use first row firt column
	{
		strike_matrix(0,k)=-10000;
		strike_matrix(k,0)=-10000;
	}

	for(size_t iExpirity = 1; iExpirity<nbRow; ++iExpirity) // row
	{
		for(size_t jTenor = 1; jTenor<nbCol-iExpirity; ++jTenor) // col
		{
			const double strike = upperTriangleVanillaSwaptionQuotes_(iExpirity,jTenor).first.get_strike();
			strike_matrix(iExpirity,jTenor) = strike;
		}
	}
	return strike_matrix;
}

UpperTriangularIndexPairMatrix UpperTriangleVanillaSwaptionQuotes::get_UpperTriangularSwaptionIndexMatrix() const
{
	if(LMM::WARNLMM ())
		std::cout<<LMM::WARN_MSG << "UpperTriangleVanillaSwaptionQuotes::get_UpperTriangularSwaptionIndexMatrix() generate matrix copy, DO NOT USE IN A LOOP" << std::endl;

	size_t nbRow = upperTriangleVanillaSwaptionQuotes_.size1();
	size_t nbCol = upperTriangleVanillaSwaptionQuotes_.size2();
	assert(nbRow == nbCol);
	UpperTriangularIndexPairMatrix swap_indices_matrix(nbRow,nbCol );

	for(size_t k=0;k<nbRow;++k)//not use first row firt column
	{
		swap_indices_matrix(0,k)=std::pair<size_t,size_t>(-1,-1);
		swap_indices_matrix(k,0)=std::pair<size_t,size_t>(-1,-1);
	}

	for(size_t iExpirity = 1; iExpirity<nbRow; ++iExpirity) // row
	{
		for(size_t jTenor = 1; jTenor<nbCol-iExpirity; ++jTenor) // col
		{
			std::pair<size_t,size_t> swap_pair_indice;
			const VanillaSwap swap = upperTriangleVanillaSwaptionQuotes_(iExpirity,jTenor).first.getUnderlyingSwap();
			swap_pair_indice.first = swap.get_indexStart();
			swap_pair_indice.second = swap.get_indexEnd();
			swap_indices_matrix(iExpirity,jTenor) = swap_pair_indice;
		}
	}
	return swap_indices_matrix;
}
