#include <LMM/LmmModel/LmmSkewCostFunction.h>

LmmSkewCostFunction::LmmSkewCostFunction( LmmVanillaSwaptionApproxPricer_Rebonato_PTR lmmRobonato_ptr                // pricer  
										 , LiborQuotes_ConstPTR liborQuotes_ptr
										 , const double& quoted_strike_bump
										 , UpperTriangleVanillaSwaptionQuotes_ConstPTR pUpperTriangleVanillaSkewQuotes // instrument to calibrate 
										 , Shifted_HGVolatilityParam_PTR pShifted_HGVolatilityParam )
										 : pLmmVanillaSwaptionApproxPricer_Rebonato_(lmmRobonato_ptr)
										 , pLiborQuotes_(liborQuotes_ptr)
										 , quoted_strike_bump_(quoted_strike_bump)
										 , pUpperTriangleVanillaSkewQuotes_(pUpperTriangleVanillaSkewQuotes)
										 , mdl_swaption_skew(pUpperTriangleVanillaSkewQuotes_->size1(), pUpperTriangleVanillaSkewQuotes_->size2() )
										 , buffer_Shifted_HGVolatilityParam_(pShifted_HGVolatilityParam)
										 , nbCalled(0)			
{}

UpperTriangularDoubleMatrix LmmSkewCostFunction::get_MarketQuotes() const 
{
	return pUpperTriangleVanillaSkewQuotes_->get_UpperTriangularQuoteValues();
}

UpperTriangularDoubleMatrix LmmSkewCostFunction::get_CurrentModelQuotes() const 
{
	return mdl_swaption_skew;
}

void LmmSkewCostFunction::reset_reference_calib(const QuantLib::Array & true_param)
{
	const size_t nbParam = true_param.size();
	buffer_calib_reference.resize(nbParam);
	for(size_t i=0;i<nbParam;++i)
	{
		buffer_calib_reference[i]=true_param[i];
	}
}

Shifted_HGVolatilityParam_PTR LmmSkewCostFunction::get_Shifted_HGVolatilityParam()
{
	return buffer_Shifted_HGVolatilityParam_; 
}

Real LmmSkewCostFunction::value(const Array & x) const
{
	Array diff_cost = values(x);

	Real res = 0;
	for (size_t i = 0; i < diff_cost.size(); ++i)
	{
		res += diff_cost[i]*diff_cost[i];
	}

	res = sqrt(res);
	return res;
}

//const Array& param_array
Disposable<Array> LmmSkewCostFunction::values(const Array& param_array) const
{
	buffer_Shifted_HGVolatilityParam_->reset_Shift_FromArray( param_array );
	pLmmVanillaSwaptionApproxPricer_Rebonato_->update_VolatilityParam( buffer_Shifted_HGVolatilityParam_ );

	size_t nbSwaption = calc_nbSwaptions() ;

	//! diff upperTriangleVanillaSwaptionMktQuotes_ - mdl_swaption_skew
	Array diff_cost( nbSwaption );
	size_t vector_counter=0;
	update_SwaptionMdlSkewValues();
	
	const UpperTriangularVanillaSwaptionQuotes & mkt_swaption_quote = pUpperTriangleVanillaSkewQuotes_->get_UpperTriangularVanillaSwaptionQuotes() ;
	
	for(size_t iExperity=1;iExperity<pUpperTriangleVanillaSkewQuotes_->size1()-1;++iExperity)
	{
		size_t nb_jSwaption = pUpperTriangleVanillaSkewQuotes_->size2()- iExperity;
		for(size_t jTenor = 1;jTenor< nb_jSwaption; ++jTenor)
		{
			const double diff = mkt_swaption_quote(iExperity,jTenor).second - mdl_swaption_skew(iExperity,jTenor);
			diff_cost[vector_counter]= diff;//*diff; // To check with quantLib. --> checked
			++vector_counter;
		}
	}

	// check if penalties fully fill the cost vector
	assert(vector_counter == diff_cost.size() ); 

	if(LMM::DEUBGLMM())
	{
		if( breakForPrintOut(nbCalled) ) 
		{
			std::cout<<std::endl<<LMM::NOTIF_MSG<<" iter."<<nbCalled<<std::endl;

			if(buffer_calib_reference.empty() )
			{
				std::cout <<"      x = "<<param_array<<std::endl<<std::endl;
			}
			else
			{
				QuantLib::Array error = error_calib(param_array);
				std::cout <<"diff   = "<<error<<std::endl<<std::endl;
			}
			std::cout <<"f(x,p) = "<<diff_cost<<std::endl;			
		}
	}

	++nbCalled;
	return diff_cost;
}

void LmmSkewCostFunction::update_SwaptionMdlSkewValues() const // pricing
{
	//! upperTriangle:

	const UpperTriangularVanillaSwaptionQuotes & mkt_swaption_quotes = pUpperTriangleVanillaSkewQuotes_->get_UpperTriangularVanillaSwaptionQuotes() ;
	const std::vector<double>& libor_init = pLiborQuotes_->get_InitLibor();

	for(size_t iExperity = 1; iExperity<mdl_swaption_skew.size1()-1; ++iExperity) // row
	{
		size_t nb_jSwaption = mdl_swaption_skew.size2()- iExperity;
		for(size_t jTenor = 1;jTenor< nb_jSwaption; ++jTenor)
		{
			VanillaSwaption copied_swaption ( mkt_swaption_quotes(iExperity,jTenor).first );

			const double blackVol_ij = pLmmVanillaSwaptionApproxPricer_Rebonato_->skew(copied_swaption,libor_init,quoted_strike_bump_);

			mdl_swaption_skew(iExperity,jTenor)=blackVol_ij;
		}
	}
}

QuantLib::Array LmmSkewCostFunction::error_calib(const QuantLib::Array & actual_param) const
{
	size_t nbParam = actual_param.size();
	QuantLib::Array error_calib(nbParam,0.);
	for(size_t i=0;i<nbParam;++i)
	{
		error_calib[i]  = actual_param[i] - buffer_calib_reference[i] ;
	}
	return error_calib;
}

size_t LmmSkewCostFunction::calc_nbSwaptions() const 
{
	return pUpperTriangleVanillaSkewQuotes_->get_nbSwaption(); 
}