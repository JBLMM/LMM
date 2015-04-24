#include <LMM/Test/Test_CalibrationConfig.h>

#include <cassert>
#include <string.h>
#include <fstream>

#include <LMM/LmmModel/LmmGlobal_gCostFunction.h>
#include <LMM/LmmModel/LmmLocal_gCostFunction.h>
#include <LMM/LmmModel/LmmCascade_gCostFunction.h>

void Test_CalibrationConfig::printExacCalibResult(const std::string& filename, const QuantLib::Array& init_shift, const QuantLib::Array& perturb_shift, const QuantLib::Array& calib_shift)
{
	assert(init_shift.size() == perturb_shift.size() && perturb_shift.size() == calib_shift.size() );

	std::string path_OutPut = LMMPATH::get_output_path() + filename;
	std::ofstream result_stream;

	result_stream.open(path_OutPut.c_str());

	result_stream<< " -------COMPARISON INIT Shifted PARAM vs CALIBRATED Shifted PARAMS--------,"<<std::endl;
	result_stream<<std::endl<< "shift Init , shift Perturbed, shift Calibrated , , Diff Pert(%),  Diff Cali(%), "<<std::endl;

	double rel_linf_diff_calib=0,rel_linf_diff_perturb=0; // relative error

	for(size_t i=0;i<init_shift.size();++i)
	{

		const double rel_diff_perturbed =100* (perturb_shift[i] - init_shift[i])  / (init_shift[i]); // relative error
		const double rel_diff_calib     =100* (calib_shift[i]   - init_shift[i] ) / (init_shift[i]); // relative error

		result_stream<<init_shift[i]<<","<<perturb_shift[i]<<","<<calib_shift[i]<<", ,"<<rel_diff_perturbed<<","<<rel_diff_calib<<",";
		if(std::abs(rel_diff_perturbed) > std::abs(rel_diff_calib) )
			result_stream<<"Ok,";
		else
			result_stream<<"Bad,";

		result_stream<<std::endl;

		if(rel_linf_diff_calib< std::abs(rel_diff_calib)) rel_linf_diff_calib= std::abs(rel_diff_calib);
		if(rel_linf_diff_perturb< std::abs(rel_diff_perturbed)) rel_linf_diff_perturb = std::abs(rel_diff_perturbed);
	}

	result_stream<<std::endl;
	result_stream<< ",,,Diff (%), "<<rel_linf_diff_perturb<<","<<rel_linf_diff_calib <<std::endl;
	result_stream.close();
}
void Test_CalibrationConfig::printExacCalibResult(const std::string& filename, const LowerTriangularDoubleMatrix& init_g, const LowerTriangularDoubleMatrix& perturb_g, const LowerTriangularDoubleMatrix& calib_g) 
{
	assert(init_g.size1() == perturb_g.size1() && init_g.size1() == calib_g.size1() );
	assert(init_g.size2() == perturb_g.size2() && init_g.size2() == calib_g.size2() );
	LowerTriangularDoubleMatrix realv_error_matrix(init_g.size1(),init_g.size2());

	std::string path_OutPut = LMMPATH::get_output_path() + filename;
	std::ofstream result_stream;

	result_stream.open(path_OutPut.c_str());

	result_stream<< " -------COMPARISON INIT VOLPARAM vs CALIBRATED VOLPARAMS--------,"<<std::endl;
	result_stream<<std::endl<< "Init , Perturbed, Calibrated , , Diff Perturb(%),  Diff Calib(%), , , (i;j)"<<std::endl;

	double rel_linf_diff_calib=0,rel_linf_diff_perturb=0; // relative error
	for(size_t i=1;i<init_g.size1();++i)
	{
		for(size_t j=1;j<=i;++j)
		{
			const double rel_diff_perturbed = 100 * (perturb_g(i,j) - init_g(i,j))  / (init_g(i,j)); // relative error
			const double rel_diff_calib     = 100 * (calib_g(i,j)   - init_g(i,j) ) / (init_g(i,j)); // relative error
			realv_error_matrix(i,j)=rel_diff_calib;
			result_stream<<init_g(i,j)<<","<<perturb_g(i,j)<<","<<calib_g(i,j)<<", ,"<<rel_diff_perturbed<<","<<rel_diff_calib<<",";
			if(std::abs(rel_diff_perturbed) < std::abs(rel_diff_calib) )
				result_stream<<"Bad,";				
			else
				result_stream<<"Ok,";

			result_stream<<" ,("<<i<<";"<<j<<"),"<<std::endl;

			if(rel_linf_diff_calib< std::abs(rel_diff_calib)) rel_linf_diff_calib= std::abs(rel_diff_calib);
			if(rel_linf_diff_perturb<std::abs(rel_diff_perturbed)) rel_linf_diff_perturb = std::abs(rel_diff_perturbed);
		}	
	}

	result_stream<<std::endl;
	result_stream<< ",,,Diff (%), "<<rel_linf_diff_perturb<<","<<rel_linf_diff_calib <<std::endl;

	result_stream<<std::endl<<std::endl;
	result_stream.close();

	{ // print error  in matrix format

		std::ofstream result_file ;
		result_file.open(path_OutPut.c_str(), std::ios::app);
		result_file<<"Relative calibration Error(%) "<<std::endl;
		result_file.close();

		std::vector<PrintElement_PTR> elements_print;

		PrintElement_PTR realv_error_print = PrintElement_PTR(new MatrixPrintElement<LowerTriangularDoubleMatrix>("Relv Calib Error(%) ", realv_error_matrix));			

		elements_print.push_back(realv_error_print);

		Printer printer(path_OutPut, elements_print);
		printer.print(false); 
	}

}
void Test_CalibrationConfig::printDiagComparison(const std::string& filename, const LowerTriangularDoubleMatrix& init_g, const LowerTriangularDoubleMatrix& perturb_g, const LowerTriangularDoubleMatrix& calib_g) 
{
	assert(init_g.size1() == perturb_g.size1() && init_g.size1() == calib_g.size1() );
	assert(init_g.size2() == perturb_g.size2() && init_g.size2() == calib_g.size2() );
	assert(init_g.size1() == init_g.size2() );

	std::string path_OutPut = LMMPATH::get_output_path() + filename;
	std::ofstream result_stream;

	size_t horizon = init_g.size1()-1 ;
	size_t nb_diag = init_g.size1()-1 ;

	result_stream.open(path_OutPut.c_str()) ;

	for(size_t idiag=0;idiag<nb_diag;++idiag)
	{
		size_t nb_element_diag = horizon - idiag;

		std::stringstream diagInitVol     ; diagInitVol     <<" true_g diag."<<idiag<<",";
		std::stringstream diagPerturbVol  ; diagPerturbVol  <<"pertu_g diag."<<idiag<<",";
		std::stringstream diagRegulizedVol; diagRegulizedVol<<"calib_g diag."<<idiag<<",";

		for(size_t element_diag=1;element_diag<=nb_element_diag;++element_diag)
		{
			size_t jCol = element_diag ;
			size_t iRow = element_diag + idiag;
			double gInit = init_g(iRow,jCol);
			double gPert = perturb_g(iRow,jCol);
			double gRegu = calib_g(iRow,jCol);

			diagInitVol<<gInit<<",";
			diagPerturbVol<<gPert<<",";
			diagRegulizedVol<<gRegu<<",";			
		}

		//result_stream<<diagPerturbVol.str()<<std::endl;
		result_stream<<diagInitVol.str()<<std::endl;
		result_stream<<diagRegulizedVol.str()<<std::endl<<std::endl;		
	}

	result_stream.close();
}


void Test_CalibrationConfig::re_init_filenames()const 
{
	// setting base name files corresponding to specific test
	std::ostringstream base_file_name_stream;

	base_file_name_stream <<"test_"<<"_s"<<unique_seed<<"_";
	if(penlt_time_homo>0)
		base_file_name_stream<<"pnlt_time"<<penlt_time_homo<<"_";
	if(penlt_libor>0)
		base_file_name_stream<<"pnlt_libor"<<penlt_libor<<"_";

	base_file_name_stream <<calib_type.Name();

	base_file_name_stream <<nbYear<<"_YR";

	basefilename = base_file_name_stream.str();
}

void Test_CalibrationConfig::write_to_stream(std::ostream& os)const
{
	os<< "nbYear,"<<nbYear<<",,  unique_seed,"<<unique_seed
		<<",,  penalty time homogene,"<<penlt_time_homo <<", penalty libor,"<<penlt_libor <<"," <<std::endl<<std::endl;
	os<< "g_diag_noise,"<<g_diag_noise<<","<<std::endl<<"perturbe_noise,"<<perturbe_noise<<","<<std::endl<<std::endl;
	os<< "x_epsilon,"<<x_epsilon<<","<<std::endl<< "f_epsilon,"<<f_epsilon<<","<<std::endl<< "maxIter,"<<maxIter<<","<< "maxIterPenalty,"<<maxIterPenalty<<","<<std::endl<<std::endl;
	os<< "a,b,c,d,"<<std::endl<<a<<","<<b<<","<<c<<","<<d<<","<<std::endl;
	os<<"constant_rate,"<<constant_rate<<std::endl; 
	os<<"skew_strike_bump,"<<skew_strike_bump<<std::endl; 	
	os<<"const_shift,"<<const_shift<<std::endl<<std::endl; 	
	os<<"Base File Name,"<<basefilename<<std::endl;
	os<< "Fixed Tenor,"<<tenorfixedleg<<","<<std::endl
		<<"Float Tenor,"<<tenorfloatleg<<","<<std::endl
		<<"Fixed Float Step,"<<fixedfloatRatio<<","<<std::endl;

	os<<std::endl;
	os<<"Unactive Swaption Row :,";for(size_t i=0;i<unactive_swpm_rows.size();++i){  os<<unactive_swpm_rows[i]<<",";  }
	os<<std::endl;
	os<<"Unactive Swaption Col :,";for(size_t i=0;i<unactive_swpm_cols.size();++i){  os<<unactive_swpm_cols[i]<<",";  }
	os<<std::endl;
	os<<"Truncated Cell :,";for(size_t i=0;i<truncated_cells.size();++i){  os<<truncated_cells[i]<<",";  }
	os<<std::endl;
}

void Test_CalibrationConfig::print(const std::string& filename) const 
{
	std::string path_OutPut = LMMPATH::get_output_path() + filename;path_OutPut+="_config.csv";
	std::ofstream configOut;

	configOut.open(path_OutPut.c_str());
	this->write_to_stream(configOut);
	configOut<<std::endl;
	configOut.close();
}

void Test_CalibrationConfig::re_init_LMM_Components()
{
	pLmmPenalty.reset( new LmmPenalty(penlt_time_homo,penlt_libor) );

	fixedfloatRatio = tenorfixedleg.ratioTo(tenorfloatleg);	

	pLMMTenorStructure = create_LMMTenorStructure() ;
	pLiborQuotes       = LiborQuotes::create_LiborInit(pLMMTenorStructure, constant_rate);

	pCorrelation = create_Correlation();	
	pShifted_HGVolatilityParam   = create_Shifted_HGVolatilityParam()   ;
	pNoShifted_HGVolatilityParam = create_NoShifted_HGVolatilityParam() ;

	pLmmVanillaSwaptionApproxPricer_Rebonato = create_LmmVanillaSwaptionApproxPricer_Rebonato();

	pUpperTriangleVanillaSwaptionQuotes = create_UpperTriangleVanillaSwaptionQuotes();
	pUpperTriangleSkewQuotes = create_UpperTriangleTriangleSkewQuotes();

	pGMatrixMapping = create_GMatrixMapping();

	pLmmBaseCostFunction = create_LmmBaseCostFunction();
}

LMMTenorStructure_PTR Test_CalibrationConfig::create_LMMTenorStructure() const
{
	LMMTenorStructure_PTR new_pLMMTenorStructure (new LMMTenorStructure(tenorfloatleg,nbYear) );
	return new_pLMMTenorStructure;
}

Correlation_PTR Test_CalibrationConfig::create_Correlation() const
{
	//! Correlation 
	size_t nbFactor       = 3; // need to test nbFactor  = 3, and nbFactor = 
	size_t correlFullRank = pLMMTenorStructure->get_horizon()+1;
	size_t correlReducedRank = nbFactor;
	CorrelationReductionType::CorrelationReductionType correlReductionType = CorrelationReductionType::PCA;
	double correlAlpha = 0.02;
	double correlBeta  = 0.1;
	Correlation_PTR correlation_ptr(new XY_beta_Correlation(correlFullRank,correlReducedRank, correlReductionType,correlAlpha,correlBeta));
	correlation_ptr->calculate(); // for print.
	return correlation_ptr;
}


ConstShifted_HGVolatilityParam_PTR Test_CalibrationConfig::create_NoShifted_HGVolatilityParam() const
{
	Shifted_HGVolatilityParam::ABCDParameter abcdParam(a,b,c,d);
	ConstShifted_HGVolatilityParam_PTR new_pNoShifted_HGVolatilityParam( new ConstShifted_HGVolatilityParam(pLMMTenorStructure, abcdParam, 1., 0.));
	return new_pNoShifted_HGVolatilityParam;
}


ConstShifted_HGVolatilityParam_PTR Test_CalibrationConfig::create_Shifted_HGVolatilityParam() const
{
	Shifted_HGVolatilityParam::ABCDParameter abcdParam(a,b,c,d);
	ConstShifted_HGVolatilityParam_PTR new_pNoShifted_HGVolatilityParam( new ConstShifted_HGVolatilityParam(pLMMTenorStructure, abcdParam, 1., const_shift));

	const QuantLib::Array const_shift_vector = new_pNoShifted_HGVolatilityParam->get_ArrayFrom_Shift();

	// varie init shift vector with noise instead of keeping it constant
	QuantLib::Array copy_shift_vector(const_shift_vector);
	std::vector<double> multiplicative_noise = Multiplicative::UniformNoise(copy_shift_vector.size(),g_diag_noise, unique_seed);
	for(size_t i=0;i<copy_shift_vector.size();++i) copy_shift_vector[i]*= multiplicative_noise[i]; 
	new_pNoShifted_HGVolatilityParam->reset_Shift_FromArray(copy_shift_vector);

	return new_pNoShifted_HGVolatilityParam;
}


LmmVanillaSwaptionApproxPricer_Rebonato_PTR Test_CalibrationConfig::create_LmmVanillaSwaptionApproxPricer_Rebonato() const
{

	Shifted_HGVolatilityFunction_PTR pVolatilityFunction (new ConstShifted_HGVolatilityFunction(pLMMTenorStructure, pCorrelation, pNoShifted_HGVolatilityParam)); 

	//! Dispersion
	Dispersion dispersion(pVolatilityFunction);

	Lmm_PTR lmm_ptr(new Lmm(dispersion) );

	LmmVanillaSwaptionApproxPricer_Rebonato_PTR new_pLmmVanillaSwaptionApproxPricer_Rebonato(new LmmVanillaSwaptionApproxPricer_Rebonato(lmm_ptr));	

	return new_pLmmVanillaSwaptionApproxPricer_Rebonato;
}

UpperTriangleVanillaSwaptionQuotes_ConstPTR Test_CalibrationConfig::create_UpperTriangleVanillaSwaptionQuotes() const 
{
	UpperTriangleVanillaSwaptionQuotes_ConstPTR new_pUpperTriangleVanillaSwaptionQuotes = UpperTriangleVanillaSwaptionQuotes::create_ATMSwaptionImpliedVol
		(
		pLiborQuotes,
		tenorfixedleg,
		tenorfloatleg,
		pLmmVanillaSwaptionApproxPricer_Rebonato
		);
	return new_pUpperTriangleVanillaSwaptionQuotes;
}


UpperTriangleVanillaSwaptionQuotes_ConstPTR Test_CalibrationConfig::create_UpperTriangleTriangleSkewQuotes() const 
{
	// skew data is created with shifted volatility
	pLmmVanillaSwaptionApproxPricer_Rebonato->update_VolatilityParam(pShifted_HGVolatilityParam);

	UpperTriangleVanillaSwaptionQuotes_ConstPTR p_swaption_quote_ATMpp = UpperTriangleVanillaSwaptionQuotes::create_ATMSwaptionImpliedVol
		(
		pLiborQuotes,
		tenorfixedleg,
		tenorfloatleg,
		pLmmVanillaSwaptionApproxPricer_Rebonato,
		skew_strike_bump
		);

	UpperTriangleVanillaSwaptionQuotes_ConstPTR p_swaption_quote_ATMmm = UpperTriangleVanillaSwaptionQuotes::create_ATMSwaptionImpliedVol
		(
		pLiborQuotes,
		tenorfixedleg,
		tenorfloatleg,
		pLmmVanillaSwaptionApproxPricer_Rebonato,
		-skew_strike_bump
		);

	UpperTriangularDoubleMatrix vol_quote_pp =  p_swaption_quote_ATMpp->get_UpperTriangularQuoteValues() ;
	UpperTriangularDoubleMatrix vol_quote_mm =  p_swaption_quote_ATMmm->get_UpperTriangularQuoteValues() ;

	UpperTriangularDoubleMatrix strike_quote_pp =  p_swaption_quote_ATMpp->get_UpperTriangularStrike() ;
	UpperTriangularDoubleMatrix strike_quote_mm =  p_swaption_quote_ATMmm->get_UpperTriangularStrike() ;

	UpperTriangularDoubleMatrix skew_quote_pp = (vol_quote_pp - vol_quote_mm)/(2*skew_strike_bump);
	UpperTriangularDoubleMatrix atm_strke_quote_pp = (strike_quote_pp + strike_quote_mm)/2;

	UpperTriangleVanillaSwaptionQuotes_ConstPTR new_pUpperTriangleSkewQuotes ( new UpperTriangleVanillaSwaptionQuotes(
		p_swaption_quote_ATMpp->get_LMMTenorStructure_PTR(),
		p_swaption_quote_ATMpp->get_lastYear(),
		p_swaption_quote_ATMpp->get_fixedTenor(),
		p_swaption_quote_ATMpp->get_floatingTenor(),
		atm_strke_quote_pp,
		skew_quote_pp		
		) 
		);

	//check if created strikes are coherent with strike_bump
	for(size_t i=1;i<atm_strke_quote_pp.size1();++i)
	{
		for(size_t j=1;j<atm_strke_quote_pp.size2()-i;++j)
		{
			double strike_diff = strike_quote_pp(i,j)-strike_quote_mm(i,j);
			std::cout<<"("<<i<<","<<j<<")   strike_diff."<<strike_diff<<"	 2*strike_bump="<<2*skew_strike_bump<<std::endl;
			assert(std::abs(strike_diff - 2*skew_strike_bump) <1e-12  );//strike_diff == 2*skew_strike_bump
		}	
	}	

	// return the default No Shifted volatility to the setting
	pLmmVanillaSwaptionApproxPricer_Rebonato->update_VolatilityParam(pNoShifted_HGVolatilityParam);

	return new_pUpperTriangleSkewQuotes;
}

GMatrixMapping_PTR Test_CalibrationConfig::create_GMatrixMapping()
{
	size_t        g_matrix_size = GMatrixMapping::get_gSizeFromNbYear(nbYear,fixedfloatRatio );
	size_t delegate_matrix_size = GMatrixMapping::get_gDelegateSizeFromHorizon(pLMMTenorStructure->get_horizon() ,fixedfloatRatio );
	UpperTriangularDoubleMatrix empty_delegate_matrix(delegate_matrix_size,delegate_matrix_size);

	GMatrixMapping_PTR new_pGMatrixMapping( new GMatrixMapping(g_matrix_size, empty_delegate_matrix, pUpperTriangleVanillaSwaptionQuotes->get_UpperTriangularIndexPairMatrix())  );

	//if(unactive_swpm_rows.size() > 0 ) new_pGMatrixMapping->reset_Truncated_gDelegate_Rows(unactive_swpm_rows); 
	//if(unactive_swpm_cols.size() > 0 ) new_pGMatrixMapping->reset_Truncated_gDelegate_Cols(unactive_swpm_cols);

	for(size_t i=0;i<truncated_cells.size();++i)
	{
		new_pGMatrixMapping->add_Truncated_gDelegate_Cells(truncated_cells[i]);
	}

	initialize_gDelegate_with_noise(new_pGMatrixMapping);

	pNoShifted_HGVolatilityParam->reset_g_matrix( new_pGMatrixMapping->get_g_Ref() );

	pLmmVanillaSwaptionApproxPricer_Rebonato->update_VolatilityParam(pNoShifted_HGVolatilityParam);

	pUpperTriangleVanillaSwaptionQuotes = create_UpperTriangleVanillaSwaptionQuotes();

	return new_pGMatrixMapping;
}

LmmBaseCostFunction_PTR Test_CalibrationConfig::create_LmmBaseCostFunction() const 
{
	LmmBaseCostFunction_PTR new_pLmmBaseCostFunction;
	if( calib_type.isGlobal() )
	{
		new_pLmmBaseCostFunction.reset
			(
			new LmmGlobal_gCostFunction
			(
			pLmmVanillaSwaptionApproxPricer_Rebonato,
			pLiborQuotes,
			pUpperTriangleVanillaSwaptionQuotes,
			pGMatrixMapping,
			pNoShifted_HGVolatilityParam,
			pLmmPenalty			
			)
			);
	}
	else if( calib_type.isLocal() )
	{
		new_pLmmBaseCostFunction.reset
			(
			new LmmLocal_gCostFunction
			(
			pLmmVanillaSwaptionApproxPricer_Rebonato,
			pLiborQuotes,
			pUpperTriangleVanillaSwaptionQuotes,
			pGMatrixMapping,
			pNoShifted_HGVolatilityParam
			)
			);	
	}
	else if( calib_type.isCascade() )
	{
		new_pLmmBaseCostFunction.reset
			(
			new LmmCascade_gCostFunction
			(
			pLmmVanillaSwaptionApproxPricer_Rebonato,
			pLiborQuotes,
			pUpperTriangleVanillaSwaptionQuotes,
			pGMatrixMapping,
			pNoShifted_HGVolatilityParam
			)
			);	
	}

	return new_pLmmBaseCostFunction;
}

void Test_CalibrationConfig::initialize_gDelegate_with_noise(GMatrixMapping_PTR pGMatrixMapping ) 
{
	QuantLib::Array g_delegate_vector =  pGMatrixMapping->get_DelegateArray();

	size_t nbElement = g_delegate_vector.size();

	// initialize every g node to 1.
	for(size_t i=0;i<nbElement;++i) 
	{
		g_delegate_vector[i] = 1.;
	}

	// bump a small noise to every g node to 1.
	std::vector<double> mult_noise = Multiplicative::UniformNoise(nbElement,g_diag_noise,unique_seed+1000);// use the different seed of perturbation noises

	for(size_t i=0;i<nbElement;++i)
	{
		g_delegate_vector[i] *= mult_noise[i];		
	}

	pGMatrixMapping->reset_gDelegate(g_delegate_vector);

	if( calib_type.isCascade() ) // cascade for instance reset differently
	{
		//pGMatrixMapping->print( "Test_Calibration_ResetInitgDelegate_BeforeReset.csv" );

		const UpperTriangularDoubleMatrix& gDelegateMatrix = pGMatrixMapping->get_gDelegate_Ref();
		const std::vector<size_t> & truncataed_rows =pGMatrixMapping->get_truncated_gDelegate_Rows();
		const std::vector<size_t> & truncataed_cols =pGMatrixMapping->get_truncated_gDelegate_Cols();		
		for(size_t iDelegate=1;iDelegate<gDelegateMatrix.size1();++iDelegate)
		{
			if( std::find(truncataed_rows.begin(), truncataed_rows.end() , iDelegate)  == truncataed_rows.end())
			{
				for(size_t jDelegate=1;jDelegate<gDelegateMatrix.size2() -iDelegate  ;++jDelegate)
				{
					if( std::find(truncataed_cols.begin(), truncataed_cols.end() , jDelegate)  == truncataed_cols.end()	)
					{
						const double gDelegate_cell_value = gDelegateMatrix(iDelegate,jDelegate);
						std::pair<size_t,size_t> gDelegate_cell(iDelegate,jDelegate);
						pGMatrixMapping->reset_gDelegate(gDelegate_cell_value,gDelegate_cell);	

						//std::ostringstream local_gDelegate_file; 
						//local_gDelegate_file<< "Test_Calibration_ResetInitgDelegate_"<<iDelegate<<"row_"<< jDelegate<<"col"  <<".csv";
						//pGMatrixMapping->print( local_gDelegate_file.str() );	
					}
				}
			}
		}	
	}
}


QuantLib::Array Test_CalibrationConfig::get_Perturbed_Array(const QuantLib::Array& init_array )const
{
	size_t nbElement = init_array.size();

	// copy init to perturbed
	QuantLib::Array perturbed_gDelegateArray(init_array);

	// bump a small noise to every g node to 1.
	std::vector<double> mult_noise = Multiplicative::UniformNoise(nbElement,perturbe_noise,unique_seed);

	for(size_t i=0;i<nbElement;++i)
	{
		perturbed_gDelegateArray[i] *= mult_noise[i];		
	}
	return perturbed_gDelegateArray;
}

void Test_CalibrationConfig::update_gDelegateChange() const
{
	pNoShifted_HGVolatilityParam->reset_g_matrix( pGMatrixMapping->get_g_Ref() );
	pLmmVanillaSwaptionApproxPricer_Rebonato->update_VolatilityParam(pNoShifted_HGVolatilityParam);
}