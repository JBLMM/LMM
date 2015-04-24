#include <LMM/LmmModel/LmmCorrelationCostFunction.h>

LmmCorrelationCostFunction::LmmCorrelationCostFunction( LmmVanillaSwaptionApproxPricer_Rebonato_PTR lmmRobonato_ptr                // pricer  
													   , LiborQuotes_ConstPTR liborQuotes_ptr
													   , UpperTriangleVanillaSwaptionQuotes_ConstPTR pUpperTriangleVanillaSwaptionQuotes // instrument to calibrate 
													   , Correlation_PTR pCorrelation )
													   : LmmBaseCostFunction(liborQuotes_ptr, pUpperTriangleVanillaSwaptionQuotes )
													   , pLmmVanillaSwaptionApproxPricer_Rebonato_(lmmRobonato_ptr) 
													   ,pCorrelation_(pCorrelation)													  
{

}

void LmmCorrelationCostFunction::reset_CalibrationParams(const QuantLib::Array & params) const 
{
	pCorrelation_->reset_FromArray(params);
	pLmmVanillaSwaptionApproxPricer_Rebonato_->update_Correlation();
}

//const Array& param_array
Disposable<Array> LmmCorrelationCostFunction::values(const Array& param_array) const
{
	// NOTE : 
	// Costfunction::values has to compute only values, not absolute values, nor squared values
	// see Introduction to Selected Classes of QuantLib II _ Dimitri Reiswich 2010 p.57/148

	reset_CalibrationParams(param_array);
	fully_UpdateSwaptionMdlValues();

	size_t nbSwaption = calc_nbSwaptions();

	Array diff_cost( nbSwaption);
	size_t vector_counter=0;	
	size_t swaption_matrix_size = upperTriangleVanillaSwaptionMdlValues_.size1();
	const UpperTriangularVanillaSwaptionQuotes & mkt_swaption_quote = pUpperTriangleVanillaSwaptionQuotes_->get_UpperTriangularVanillaSwaptionQuotes() ;

	for(size_t iExperity=1; iExperity<swaption_matrix_size-1 ; ++ iExperity)
	{
		const size_t maxTenorBound =  swaption_matrix_size - iExperity;

		for(size_t jTenor=1;jTenor<maxTenorBound;++jTenor)
		{
			const double diff = upperTriangleVanillaSwaptionMdlValues_(iExperity,jTenor) - mkt_swaption_quote(iExperity,jTenor).second ;
			diff_cost[vector_counter]= upperTriangleVanillaSwaptionWeights_(iExperity,jTenor) * diff;//*diff; // To check with quantLib. --> checked
			++vector_counter;
		}
	}

	// check if penalties fully fill the cost vector
	assert(vector_counter == diff_cost.size() ); 

	if(LMM::DEUBGLMM())
	{
		if( breakForPrintOut() ) 
		{
			std::cout<<std::endl<<LMM::NOTIF_MSG<<" iter."<<nbCalled;

			std::cout <<"      x = "<<param_array;

			if(!buffer_calib_reference.empty() )
			{
				QuantLib::Array error = error_calib(param_array);
				std::cout <<"	diff = "<<error<<std::endl<<std::endl;
			}
			else
			{
				std::cout <<std::endl;
			}

			std::cout <<"	f(x,p) = "<<diff_cost<<std::endl;			
		}
	}

	++nbCalled;
	return diff_cost;
}

void LmmCorrelationCostFunction::fully_UpdateSwaptionMdlValues() const // pricing
{
	//! upperTriangle:

	const UpperTriangularVanillaSwaptionQuotes & mkt_swaption_quotes = pUpperTriangleVanillaSwaptionQuotes_->get_UpperTriangularVanillaSwaptionQuotes() ;
	const std::vector<double>& libor_init = pLiborQuotes_->get_InitLibor();

	size_t swaption_matrix_size = upperTriangleVanillaSwaptionMdlValues_.size1();
	for(size_t iExperity=1; iExperity<swaption_matrix_size-1 ; ++ iExperity)
	{
		const size_t maxTenorBound =  swaption_matrix_size - iExperity;

		for(size_t jTenor=1;jTenor<maxTenorBound;++jTenor)
		{
			const VanillaSwaption & swaption = mkt_swaption_quotes(iExperity,jTenor).first;

			const double blackVol_ij = pLmmVanillaSwaptionApproxPricer_Rebonato_->volBlack(swaption,libor_init);

			upperTriangleVanillaSwaptionMdlValues_(iExperity,jTenor)=blackVol_ij;
		}
	}
}

QuantLib::Array LmmCorrelationCostFunction::error_calib(const QuantLib::Array & actual_param) const
{
	size_t nbParam = actual_param.size();
	assert(nbParam == buffer_calib_reference.size() );
	QuantLib::Array error_calib(nbParam,0.);
	for(size_t i=0;i<nbParam;++i)
	{
		error_calib[i]  = actual_param[i] - buffer_calib_reference[i] ;
	}
	return error_calib;
}

size_t LmmCorrelationCostFunction::calc_nbSwaptions() const 
{
	return (upperTriangleVanillaSwaptionMdlValues_.size1()-2)*(upperTriangleVanillaSwaptionMdlValues_.size1()-1)/2 ;
}


void LmmCorrelationCostFunction::print(const std::string& filename) const
{
	std::string path_OutPut = LMMPATH::get_output_path() + filename;

	{
		std::vector<PrintElement_PTR> elements_print;
		PrintElement_PTR swpm_weight_print = PrintElement_PTR(new MatrixPrintElement<UpperTriangularDoubleMatrix>("Swaption Weights",  upperTriangleVanillaSwaptionWeights_));

		elements_print.push_back(swpm_weight_print);

		Printer printer(path_OutPut, elements_print);
		printer.print(); 
	}
}


void LmmCorrelationCostFunction::reset_TrueParam(const QuantLib::Array & true_param)
{
	const size_t nbParam = true_param.size();
	buffer_calib_reference.resize(nbParam);
	for(size_t i=0;i<nbParam;++i)
	{
		buffer_calib_reference[i]=true_param[i];
	}
}