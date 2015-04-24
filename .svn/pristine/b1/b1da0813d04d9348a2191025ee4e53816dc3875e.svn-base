#include <LMM/LmmModel/LmmCascade_gCostFunction.h>

LmmCascade_gCostFunction::LmmCascade_gCostFunction( LmmVanillaSwaptionApproxPricer_Rebonato_PTR lmmRobonato_ptr                // pricer  
						                          , LiborQuotes_ConstPTR liborQuotes_ptr
						                          , UpperTriangleVanillaSwaptionQuotes_ConstPTR pUpperTriangleVanillaSwaptionQuotes // instrument to calibrate 
				                                  , GMatrixMapping_PTR gMatrixMapping_ptr
						                          , Shifted_HGVolatilityParam_PTR pShifted_HGVolatilityParam
						                          )
							                      : LmmBaseCostFunction(liborQuotes_ptr, pUpperTriangleVanillaSwaptionQuotes )
											      , pLmmVanillaSwaptionApproxPricer_Rebonato_(lmmRobonato_ptr)
												  , pGMatrixMapping_(gMatrixMapping_ptr)
												  , buffer_Shifted_HGVolatilityParam_(pShifted_HGVolatilityParam)											   
{
	reset_SwaptionCell(1,1);	
}


void LmmCascade_gCostFunction::reset_SwaptionCell(const size_t iDelegate, const size_t jDelegate) 
{
	assert(iDelegate>0 && jDelegate>0);
	assert(iDelegate + jDelegate < upperTriangleVanillaSwaptionWeights_.size1() );
	current_cell_.first=iDelegate;
	current_cell_.second=jDelegate;
	nbCalled=0;
	buffer_calib_reference.clear();
}

void LmmCascade_gCostFunction::reset_CalibrationParams(const QuantLib::Array & param_array) const 
{
	assert(param_array.size() == 1); // cascade calibration calibrate one param at time

	pGMatrixMapping_->reset_gDelegate(param_array[0],current_cell_);
	buffer_Shifted_HGVolatilityParam_->reset_g_matrix( pGMatrixMapping_->get_g_Ref() );
	pLmmVanillaSwaptionApproxPricer_Rebonato_->update_VolatilityParam( buffer_Shifted_HGVolatilityParam_ );
}

Disposable<Array> LmmCascade_gCostFunction::values(const Array& param_array) const
{
	reset_CalibrationParams(param_array);
	partially_UpdateSwaptionMdlValues();

	Array diff_cost( 1 );
	size_t iExperity = current_cell_.first;
	size_t jTenor = current_cell_.second;
	
	const double& mkt_quote = pUpperTriangleVanillaSwaptionQuotes_->get_UpperTriangularVanillaSwaptionQuotes()(iExperity,jTenor).second ;
	const double& mdl_quote = upperTriangleVanillaSwaptionMdlValues_(iExperity,jTenor);

	const double diff = mdl_quote - mkt_quote;

	diff_cost[0] = upperTriangleVanillaSwaptionWeights_(iExperity,jTenor) * diff;

	if( breakForPrintOut() ) 
			std::cout<<LMM::NOTIF_MSG<<"   ------ Swaption("<<iExperity<<","<<jTenor<<")" <<"	--iter."<<nbCalled;

	if(LMM::DEUBGLMM())
	{
		if( breakForPrintOut() ) 
		{
			if(buffer_calib_reference.size()>0)
			{
				const double & true_x = buffer_calib_reference[0];
				const double diff_x  = param_array[0] - true_x;
				const double x_error = std::abs(diff_x) / true_x;
				std::cout<<"    true_x="<<true_x 
					<<"    x="<<param_array  
					<<"    f(x,p)="<<diff_cost[0]   
				<<"    diff_x="<<diff_x<<"	x_error(%)="<<x_error;
			}
			else
			{
				std::cout <<"	f(x,p)="<<diff_cost[0];			
				std::cout <<"	x="<<param_array;
				//ctndebug_calib_todelete
				//std::cout <<"			mkt_quote "<<pUpperTriangleVanillaSwaptionQuotes_->get_UpperTriangularVanillaSwaptionQuotes()(iExperity,jTenor).second;					
				//std::cout <<"    mdl_quote "<<upperTriangleVanillaSwaptionMdlValues_(iExperity,jTenor);

			}
			std::cout<<std::endl;
		}
	}

	++nbCalled;
	return diff_cost;
}

void LmmCascade_gCostFunction::partially_UpdateSwaptionMdlValues() const // pricing
{
	//! upperTriangle:

	const UpperTriangularVanillaSwaptionQuotes & mkt_swaption_quotes = pUpperTriangleVanillaSwaptionQuotes_->get_UpperTriangularVanillaSwaptionQuotes() ;
	const std::vector<double>& libor_init = pLiborQuotes_->get_InitLibor();

	size_t iExperity = current_cell_.first;
	size_t jTenor = current_cell_.second;

	const VanillaSwaption & swaption = mkt_swaption_quotes(iExperity,jTenor).first;

	const double blackVol_ij = pLmmVanillaSwaptionApproxPricer_Rebonato_->volBlack(swaption,libor_init);

	upperTriangleVanillaSwaptionMdlValues_(iExperity,jTenor)=blackVol_ij;
}

void LmmCascade_gCostFunction::fully_UpdateSwaptionMdlValues() const // pricing
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

void LmmCascade_gCostFunction::print(const std::string& filename) const
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

void LmmCascade_gCostFunction::reset_TrueParam(const QuantLib::Array & true_param) 
{
	assert( true_param.size() == 1 );
	buffer_calib_reference.resize(1);
	buffer_calib_reference[0] = true_param[0];
}