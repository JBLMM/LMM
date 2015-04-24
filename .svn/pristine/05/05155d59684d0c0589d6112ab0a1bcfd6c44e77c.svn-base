#include <iostream>
#include <fstream>

#include <LMM/Test/Tests.h>
#include <LMM/helper/TenorType.h>
#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/LmmModel/Shifted_HGVolatilityFunction.h>

#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
//#include <LMM/calibration/TestHGVolatilityParam.h>

void test_HGVolatility_getter_by_indices();
void test_HGVolatility_setter_by_column_indices();
std::vector<Shifted_HGVolatilityParam::VectorOfMatrixIndices> createIndexMatrix(size_t horizon, size_t sparse_step);
std::vector<QuantLib::Array > createSparseMatrix(const std::vector<Shifted_HGVolatilityParam::VectorOfMatrixIndices>& );
void test_HGVolatility()
{
	test_HGVolatility_getter_by_indices();
	test_HGVolatility_setter_by_column_indices();
}

void test_HGVolatility_setter_by_column_indices()
{
	std::cout<<"test_HGVolatility_setter_by_column_indices() Called"<<std::endl;
	Tenor tenorType = Tenor::_6M;
	size_t nbYear = 10; size_t horizon = nbYear*2;
	LMMTenorStructure_PTR pLMMTenorStructure(new LMMTenorStructure(tenorType, nbYear));
	size_t nbLIBOR = pLMMTenorStructure->get_nbLIBOR();

	double a = 0.0;
	double b = 0.0;
	double c = 0.0;
	double d = 1.;

	double g_constParam = 1.0;
	double shift_constParam = 0.0;
	ConstShifted_HGVolatilityParam_PTR pHGVolatilityParam(new TestHGVolatilityParam(pLMMTenorStructure, a,b,c,d, g_constParam, shift_constParam) );
	//HGVolatilityFunction_PTR pHGVolatilityFunction (new HGVolatilityFunction(*pHGVolatilityParam ,  pLMMTenorStructure) ); 

	TestHGVolatilityParam::VectorOfMatrixIndices empty_indices;

	QuantLib::Array full_vol_g_param = pHGVolatilityParam->get_ArrayFrom_G(empty_indices);

	size_t nbFullParam = full_vol_g_param.size();
	for(size_t i=0;i<nbFullParam;++i)
	{
		full_vol_g_param[i] = 0.01 * (i+1);
	}

	std::cout<<"Vol Param before full reset : "<<std::endl<<*pHGVolatilityParam<<std::endl;
	pHGVolatilityParam->reset_G_FromArray(full_vol_g_param,empty_indices);
	std::cout<<"Vol Param after full reset : "<<std::endl<<*pHGVolatilityParam<<std::endl;
	pHGVolatilityParam->print("test_HGVolatility_setter_by_column_indices_VolInit.csv");

	std::vector<Shifted_HGVolatilityParam::VectorOfMatrixIndices> indices_matrix = createIndexMatrix(horizon,2);
	std::vector<QuantLib::Array > values_matrix = createSparseMatrix(indices_matrix);

	std::ostringstream file_vol; 
	file_vol<<"test_HGVolatility_setter_by_column_indices";
	for(size_t i=0;i<indices_matrix.size();++i)
	{
		size_t iYR = i+1;
		std::ostringstream file_vol; 
		file_vol<<"test_HGVolatility_setter_by_column_indices_VolResetted_"<<iYR<<"YR.csv";
		pHGVolatilityParam->reset_G_FromArray(values_matrix[i],indices_matrix[i],true);
		pHGVolatilityParam->print(file_vol.str() );
	}
}

void test_HGVolatility_getter_by_indices()
{
	std::cout<<"test_HGVolatility_getter_by_indices() Called"<<std::endl;

	Tenor tenorType = Tenor::_6M;
	size_t nbYear = 2;
	LMMTenorStructure_PTR pLMMTenorStructure(new LMMTenorStructure(tenorType, nbYear));
	size_t nbLIBOR = pLMMTenorStructure->get_nbLIBOR();

	//T   0  0.5   1   1.5  2.0
	//h0  0   0    0    0   0 
	//h1  0   1
	//h2  0 1.5    1
	//h3  0   2  1.5    1 
	//h4  0 2.5    2  1.5   1
	//! volatility function
	double a = 1.0;
	double b = 1.0;
	double c = 0.0;
	double d = 0.0;

	double g_constParam = 1.0;
	double shift_constParam = -0.01;
	ConstShifted_HGVolatilityParam_PTR hgParam_ptr(new TestHGVolatilityParam(pLMMTenorStructure, a,b,c,d, g_constParam, shift_constParam) );

	//! Correlation 1
	size_t nbFactor       = 3; // need to test nbFactor  = 3, and nbFactor = 
	size_t correlFullRank = pLMMTenorStructure->get_horizon()+1;
	size_t correlReducedRank = nbFactor;
	CorrelationReductionType::CorrelationReductionType correlReductionType = CorrelationReductionType::PCA;
	double correlAlpha = 0.0;
	double correlBeta  = 0.1;
	Correlation_PTR correlation(new XY_beta_Correlation(correlFullRank,correlReducedRank, correlReductionType,correlAlpha,correlBeta));
	correlation->calculate(); // for print.
	correlation->print("test_McTerminalLmm_Correlation.csv");

	//McLmm
	//double shiftValue = -0.01;
	//std::vector<double> shifts(pLMMTenorStructure->get_horizon()+1, shiftValue);


	ConstShifted_HGVolatilityFunction_PTR pHGVolatilityFunction (new ConstShifted_HGVolatilityFunction( pLMMTenorStructure, correlation, hgParam_ptr)); 

	std::cout<<"Vol_1 at t=0.  is "<< pHGVolatilityFunction->get_varSigma_timeContinuousVersion( 1,0. ) <<std::endl;
	std::cout<<"Vol_4 at t=0.  is "<< pHGVolatilityFunction->get_varSigma_timeContinuousVersion( 4,0. ) <<std::endl;
	std::cout<<"Vol_4 at t=2.  is "<< pHGVolatilityFunction->get_varSigma_timeContinuousVersion( 4,2. ) <<std::endl;
		
	std::cout<<"Vol_4 at t=1.4999999999999 is "<< pHGVolatilityFunction->get_varSigma_timeContinuousVersion( 4, 1.4999999999999 ) <<std::endl;
	std::cout<<"Vol_4 at t=1.5  is "<< pHGVolatilityFunction->get_varSigma_timeContinuousVersion( 4, 1.5)<<std::endl;
	std::cout<<"Vol_4 at t=1.5000000000001 is "<< pHGVolatilityFunction->get_varSigma_timeContinuousVersion( 4, 1.5000000000001 ) <<std::endl;

	//! test reset from array for TestHGVolatilityParam
	TestHGVolatilityParam testHGVolParam( pLMMTenorStructure , 0 , 0 , 0 , 1 , g_constParam, shift_constParam);
	size_t nbVol = nbLIBOR*(nbLIBOR-1)/2;
	QuantLib::Array plat_vol_array(nbVol);
	TestHGVolatilityParam::VectorOfMatrixIndices indices_vector;
	size_t array_index = 0;
	for(size_t liborIndex=1;liborIndex<nbLIBOR;++liborIndex)
	{
		for(size_t timeIndex=1;timeIndex<=liborIndex;++timeIndex)
		{
			plat_vol_array[array_index]=array_index+1;
			indices_vector.push_back( std::pair<size_t,size_t>(liborIndex,timeIndex) );
			++array_index;
		}	
	}

	std::cout<<"HGVol before reset  "<<std::endl<< testHGVolParam <<std::endl;
	testHGVolParam.reset_H_FromArray(plat_vol_array,indices_vector );
	std::cout<<"HGVol afer H reset  "<<std::endl<< testHGVolParam <<std::endl;
	testHGVolParam.reset_G_FromArray(plat_vol_array,indices_vector );
	std::cout<<"HGVol afer G reset  "<<std::endl<< testHGVolParam <<std::endl;

	std::vector< std::pair<size_t,size_t> > indice_vector;

	indice_vector.push_back(std::pair<size_t,size_t>(1,1) );
	indice_vector.push_back(std::pair<size_t,size_t>(4,1) );
	indice_vector.push_back(std::pair<size_t,size_t>(4,4) );

	QuantLib::Array sparse_g_vol = testHGVolParam.get_ArrayFrom_G(indice_vector);

	std::cout<<sparse_g_vol<<std::endl;
}





//////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<Shifted_HGVolatilityParam::VectorOfMatrixIndices> createIndexMatrix(size_t horizon, size_t sparse_step)
{
	std::vector<Shifted_HGVolatilityParam::VectorOfMatrixIndices> indice_matrix;

	for(size_t j_sparse_col = sparse_step; j_sparse_col<horizon ; j_sparse_col+=sparse_step )
	{
		Shifted_HGVolatilityParam::VectorOfMatrixIndices column_indices;
		for(size_t i_sparse_row = j_sparse_col; i_sparse_row<horizon ; i_sparse_row+=sparse_step )
		{
			column_indices.push_back(std::pair<size_t,size_t>(i_sparse_row,j_sparse_col) );
		}
		indice_matrix.push_back(column_indices)	;
	}


	return indice_matrix;
}
std::vector<QuantLib::Array > createSparseMatrix(const std::vector<Shifted_HGVolatilityParam::VectorOfMatrixIndices>& indices_matrix)
{
	std::vector<QuantLib::Array > sparse_vol_matrix;
	double sparse_step = 10.;
	double sparse_value_counter = sparse_step;

	for(size_t i=0;i<indices_matrix.size();++i)
	{
		size_t row_size = indices_matrix[i].size();
		
		QuantLib::Array row_values(row_size,-1e10);
		for(size_t j=0;j<row_size;++j)
		{
			row_values[j]=sparse_value_counter;
			sparse_value_counter+=sparse_step;		
		}	

		sparse_vol_matrix.push_back(row_values);
	}

	return sparse_vol_matrix;
}