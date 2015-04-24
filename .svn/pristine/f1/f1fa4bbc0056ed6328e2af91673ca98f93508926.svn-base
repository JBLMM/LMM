#include <vector>

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>
#include <LMM/helper/LmmGnuplotPrinterMatrix.h>

#include <LMM/LmmModel/GMatrixMapping.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>

//#include <LMM/calibration/TestHGVolatilityParam.h>


void Initiate_UpperTriangularDoubleMatrixConstantDiagonal(UpperTriangularDoubleMatrix& upper_matrix, double increment);
void Initiate_UpperTriangularDoubleMatrix(UpperTriangularDoubleMatrix& matrix, double increment);
std::vector<size_t> get_NonTruncatedIndices(const std::vector<size_t>& truncated_indices, size_t nbYear);
UpperTriangleVanillaSwaptionQuotes_ConstPTR create_UpperTriangleVanillaSwaptionQuotes(LMMTenorStructure_PTR, const Tenor& fixedtenor, const Tenor& floattenor);

void check_gMatrix(const LowerTriangularDoubleMatrix& gMatrix, const double increment);

void test_GlobalResetGMatrixMapping();
void test_LocalResetGMatrixMapping();

void test_CascadeResetGMatrixMapping();

void test_TruncatedGlobalResetGMatrixMapping();
void test_TruncatedLocalResetGMatrixMapping();

void test_TruncatedCascadeResetGMatrixMapping();

void test_GMatrixMapping()
{
	test_GlobalResetGMatrixMapping();
	//test_LocalResetGMatrixMapping();

	test_TruncatedGlobalResetGMatrixMapping();

	//test_TruncatedLocalResetGMatrixMapping();
	
	//test_CascadeResetGMatrixMapping();
	//test_TruncatedCascadeResetGMatrixMapping();
}

void test_CascadeResetGMatrixMapping()
{
	{
		size_t nbYear = 5;
		Tenor tenorfixedleg = Tenor::_1YR;
		Tenor tenorfloatleg = Tenor::_6M;
		const double increment_matrix = 10;

		LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorfloatleg, nbYear) );

		size_t fixedfloatRatio = tenorfixedleg.ratioTo(tenorfloatleg);
		size_t horizon = pLmmTenorStructure->get_horizon() ;

		size_t        g_matrix_size = GMatrixMapping::get_gSizeFromNbYear(nbYear,fixedfloatRatio );
		size_t delegate_matrix_size = GMatrixMapping::get_gDelegateSizeFromHorizon(horizon,fixedfloatRatio );

		UpperTriangularDoubleMatrix empty_delegate_matrix(delegate_matrix_size,delegate_matrix_size);

		UpperTriangleVanillaSwaptionQuotes_ConstPTR atm_swaption_implied_vol_ptr = create_UpperTriangleVanillaSwaptionQuotes(pLmmTenorStructure, tenorfixedleg, tenorfloatleg);

		GMatrixMapping gMatrixMapping(
			g_matrix_size
			, empty_delegate_matrix
			, atm_swaption_implied_vol_ptr->get_UpperTriangularIndexPairMatrix() );

		gMatrixMapping.print("test_CascadeResetGMatrixMapping_init.csv");

		Initiate_UpperTriangularDoubleMatrix(empty_delegate_matrix, increment_matrix);

		// loop reset delegate rows
		size_t max_DelegateRowIndex =	gMatrixMapping.get_MaxDelegateRowIndex();

		for(size_t iRowDelegate=1;iRowDelegate<=max_DelegateRowIndex;++iRowDelegate)
		{
			size_t max_DelegateColIndex = delegate_matrix_size - (iRowDelegate+1);

			for(size_t jColDelegate=1;jColDelegate<=max_DelegateColIndex;++jColDelegate)
			{
				std::pair<size_t,size_t> gDelegate_cell(iRowDelegate,jColDelegate);
				gMatrixMapping.reset_gDelegate(empty_delegate_matrix(iRowDelegate,jColDelegate) ,gDelegate_cell);

				std::ostringstream local_gDelegate_file; 
				local_gDelegate_file<< "test_CascadeResetGMatrixMapping_reset_"<<iRowDelegate<<"row_"<< jColDelegate<<"col"  <<".csv";
				gMatrixMapping.print( local_gDelegate_file.str() );	
			}	
		}
	}
}


void test_LocalResetGMatrixMapping()
{
	std::cout<<"hello test_LocalResetGMatrixMapping() "<<std::endl;

	size_t nbYear = 5;
	Tenor tenorfixedleg = Tenor::_1YR;
	Tenor tenorfloatleg = Tenor::_6M;
	const double increment_matrix = 10;

	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorfloatleg, nbYear) );

	size_t fixedfloatRatio = tenorfixedleg.ratioTo(tenorfloatleg);
	size_t horizon = pLmmTenorStructure->get_horizon() ;

	size_t        g_matrix_size = GMatrixMapping::get_gSizeFromNbYear(nbYear,fixedfloatRatio );
	size_t delegate_matrix_size = GMatrixMapping::get_gDelegateSizeFromHorizon(horizon,fixedfloatRatio );

	UpperTriangularDoubleMatrix empty_delegate_matrix(delegate_matrix_size,delegate_matrix_size);

	UpperTriangleVanillaSwaptionQuotes_ConstPTR atm_swaption_implied_vol_ptr = create_UpperTriangleVanillaSwaptionQuotes(pLmmTenorStructure, tenorfixedleg, tenorfloatleg);

	GMatrixMapping gMatrixMapping(
		g_matrix_size
		, empty_delegate_matrix
		, atm_swaption_implied_vol_ptr->get_UpperTriangularIndexPairMatrix() );

	gMatrixMapping.print("test_LocalResetGMatrixMapping_init.csv");

	Initiate_UpperTriangularDoubleMatrix(empty_delegate_matrix, increment_matrix);

	// loop reset delegate rows
	size_t max_DelegateRowIndex =	gMatrixMapping.get_MaxDelegateRowIndex();

	for(size_t iRowDelegate=1;iRowDelegate<=max_DelegateRowIndex;++iRowDelegate)
	{
		QuantLib::Array row_values = gMatrixMapping.get_DelegateArray(iRowDelegate);
		for(size_t jarray=0;jarray<row_values.size();++jarray)
		{
			size_t jColDelegate = jarray+1;
			row_values[jarray] = empty_delegate_matrix(iRowDelegate,jColDelegate);
			gMatrixMapping.reset_gDelegate(row_values,iRowDelegate);

			std::ostringstream local_gDelegate_file; 
			local_gDelegate_file<< "test_LocalResetGMatrixMapping_reset_"<<iRowDelegate<<"th-Col.csv";
			gMatrixMapping.print( local_gDelegate_file.str() );
		}	
	}

	check_gMatrix(gMatrixMapping.get_g_Ref(), increment_matrix);
}

void test_GlobalResetGMatrixMapping()
{
	std::cout<<"hello test_GlobalResetGMatrixMapping() "<<std::endl;

	size_t nbYear = 5;
	Tenor tenorfixedleg = Tenor::_1YR;
	Tenor tenorfloatleg = Tenor::_6M;
	const double increment_matrix = 10;

	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorfloatleg, nbYear) );

	size_t fixedfloatRatio = tenorfixedleg.ratioTo(tenorfloatleg);
	size_t horizon = pLmmTenorStructure->get_horizon() ;

	size_t        g_matrix_size = GMatrixMapping::get_gSizeFromNbYear(nbYear,fixedfloatRatio );
	size_t delegate_matrix_size = GMatrixMapping::get_gDelegateSizeFromHorizon(horizon,fixedfloatRatio );

	UpperTriangularDoubleMatrix empty_delegate_matrix(delegate_matrix_size,delegate_matrix_size);

	Initiate_UpperTriangularDoubleMatrix(empty_delegate_matrix, increment_matrix);

	UpperTriangleVanillaSwaptionQuotes_ConstPTR atm_swaption_implied_vol_ptr = create_UpperTriangleVanillaSwaptionQuotes(pLmmTenorStructure, tenorfixedleg, tenorfloatleg);

	GMatrixMapping gMatrixMapping(
		g_matrix_size
		, empty_delegate_matrix
		, atm_swaption_implied_vol_ptr->get_UpperTriangularIndexPairMatrix() );

	gMatrixMapping.print("test_GlobalResetGMatrixMapping.csv");

	//check_gMatrix(gMatrixMapping.get_g_Ref(), increment_matrix);

	QuantLib::Array x = gMatrixMapping.get_DelegateArray();

	std::cout<<std::endl<<std::endl<<x<<std::endl;

	LmmGnuplotPrinterMatrix gnuplot_printer(pLmmTenorStructure);
	const LowerTriangularDoubleMatrix& lower_matrix = gMatrixMapping.get_g_Ref();

	gnuplot_printer.printVolMatrix(lower_matrix,"test_GlobalResetGMatrixMapping");
}

void test_TruncatedGlobalResetGMatrixMapping()
{
	{
		size_t nbYear = 6;
		Tenor tenorfixedleg = Tenor::_1YR;
		Tenor tenorfloatleg = Tenor::_6M;
		const double increment_matrix = 10;

		LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorfloatleg, nbYear) );

		size_t fixedfloatRatio = tenorfixedleg.ratioTo(tenorfloatleg);
		size_t horizon = pLmmTenorStructure->get_horizon() ;

		size_t        g_matrix_size = GMatrixMapping::get_gSizeFromNbYear(nbYear,fixedfloatRatio );
		size_t delegate_matrix_size = GMatrixMapping::get_gDelegateSizeFromHorizon(horizon,fixedfloatRatio );

		UpperTriangularDoubleMatrix empty_delegate_matrix(delegate_matrix_size,delegate_matrix_size);

		Initiate_UpperTriangularDoubleMatrix(empty_delegate_matrix, increment_matrix);

		UpperTriangleVanillaSwaptionQuotes_ConstPTR atm_swaption_implied_vol_ptr = create_UpperTriangleVanillaSwaptionQuotes(pLmmTenorStructure, tenorfixedleg, tenorfloatleg);

		GMatrixMapping gMatrixMapping(
			g_matrix_size
			, empty_delegate_matrix
			, atm_swaption_implied_vol_ptr->get_UpperTriangularIndexPairMatrix() );

		std::vector<std::pair<size_t,size_t> > truncated_cells;
		truncated_cells.push_back( std::pair<size_t,size_t>(1,1) );
		truncated_cells.push_back( std::pair<size_t,size_t>(1,5) );
		truncated_cells.push_back( std::pair<size_t,size_t>(5,1) );
				
		for(size_t iCell=0;iCell<truncated_cells.size();++iCell)
		{
			gMatrixMapping.add_Truncated_gDelegate_Cells( truncated_cells[iCell] );
		}

		QuantLib::Array x = gMatrixMapping.get_DelegateArray();

		for(size_t i=0;i<x.size();++i){ x[i]=(1+i)*0.1;}
			
		gMatrixMapping.reset_gDelegate(x);

		gMatrixMapping.print("test_TruncatedGlobalResetGMatrixMapping_Extrapolation.csv");
	}

		{
		size_t nbYear = 6;
		Tenor tenorfixedleg = Tenor::_1YR;
		Tenor tenorfloatleg = Tenor::_6M;
		const double increment_matrix = 10;

		LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorfloatleg, nbYear) );

		size_t fixedfloatRatio = tenorfixedleg.ratioTo(tenorfloatleg);
		size_t horizon = pLmmTenorStructure->get_horizon() ;

		size_t        g_matrix_size = GMatrixMapping::get_gSizeFromNbYear(nbYear,fixedfloatRatio );
		size_t delegate_matrix_size = GMatrixMapping::get_gDelegateSizeFromHorizon(horizon,fixedfloatRatio );

		UpperTriangularDoubleMatrix empty_delegate_matrix(delegate_matrix_size,delegate_matrix_size);

		Initiate_UpperTriangularDoubleMatrix(empty_delegate_matrix, increment_matrix);

		UpperTriangleVanillaSwaptionQuotes_ConstPTR atm_swaption_implied_vol_ptr = create_UpperTriangleVanillaSwaptionQuotes(pLmmTenorStructure, tenorfixedleg, tenorfloatleg);

		GMatrixMapping gMatrixMapping(
			g_matrix_size
			, empty_delegate_matrix
			, atm_swaption_implied_vol_ptr->get_UpperTriangularIndexPairMatrix() );

		std::vector<std::pair<size_t,size_t> > truncated_cells;
		truncated_cells.push_back( std::pair<size_t,size_t>(2,2) );
		truncated_cells.push_back( std::pair<size_t,size_t>(1,3) );
		truncated_cells.push_back( std::pair<size_t,size_t>(3,1) );
		truncated_cells.push_back( std::pair<size_t,size_t>(3,3) );

		for(size_t iCell=0;iCell<truncated_cells.size();++iCell)
		{
			gMatrixMapping.add_Truncated_gDelegate_Cells( truncated_cells[iCell] );
		}

		QuantLib::Array x = gMatrixMapping.get_DelegateArray();

		for(size_t i=0;i<x.size();++i){ x[i]=(1+i)*0.1;}
			
		gMatrixMapping.reset_gDelegate(x);

		gMatrixMapping.print("test_TruncatedGlobalResetGMatrixMapping_Interpolation.csv");
	}


	//{
	//	size_t nbYear = 6;
	//	Tenor tenorfixedleg = Tenor::_1YR;
	//	Tenor tenorfloatleg = Tenor::_6M;
	//	const double increment_matrix = 10;

	//	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorfloatleg, nbYear) );

	//	size_t fixedfloatRatio = tenorfixedleg.ratioTo(tenorfloatleg);
	//	size_t horizon = pLmmTenorStructure->get_horizon() ;

	//	size_t        g_matrix_size = GMatrixMapping::get_gSizeFromNbYear(nbYear,fixedfloatRatio );
	//	size_t delegate_matrix_size = GMatrixMapping::get_gDelegateSizeFromHorizon(horizon,fixedfloatRatio );

	//	UpperTriangularDoubleMatrix empty_delegate_matrix(delegate_matrix_size,delegate_matrix_size);

	//	Initiate_UpperTriangularDoubleMatrix(empty_delegate_matrix, increment_matrix);

	//	UpperTriangleVanillaSwaptionQuotes_ConstPTR atm_swaption_implied_vol_ptr = create_UpperTriangleVanillaSwaptionQuotes(pLmmTenorStructure, tenorfixedleg, tenorfloatleg);

	//	GMatrixMapping gMatrixMapping(
	//		g_matrix_size
	//		, empty_delegate_matrix
	//		, atm_swaption_implied_vol_ptr->get_UpperTriangularIndexPairMatrix() );

	//	std::vector<size_t> truncated_rows;
	//	truncated_rows.push_back(1);
	//	truncated_rows.push_back(3);
	//	truncated_rows.push_back(5);

	//	gMatrixMapping.reset_Truncated_gDelegate_Rows(truncated_rows);

	//	QuantLib::Array x = gMatrixMapping.get_DelegateArray();
	//	for(size_t i=0;i<x.size();++i){ x[i]=(1+i)*0.1;}
	//	gMatrixMapping.reset_gDelegate(x);

	//	gMatrixMapping.print("test_TruncatedGlobalResetGMatrixMapping_Row.csv");
	//}

	//{
	//	size_t nbYear = 6;
	//	Tenor tenorfixedleg = Tenor::_1YR;
	//	Tenor tenorfloatleg = Tenor::_6M;
	//	const double increment_matrix = 10;

	//	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorfloatleg, nbYear) );

	//	size_t fixedfloatRatio = tenorfixedleg.ratioTo(tenorfloatleg);
	//	size_t horizon = pLmmTenorStructure->get_horizon() ;

	//	size_t        g_matrix_size = GMatrixMapping::get_gSizeFromNbYear(nbYear,fixedfloatRatio );
	//	size_t delegate_matrix_size = GMatrixMapping::get_gDelegateSizeFromHorizon(horizon,fixedfloatRatio );

	//	UpperTriangularDoubleMatrix empty_delegate_matrix(delegate_matrix_size,delegate_matrix_size);

	//	Initiate_UpperTriangularDoubleMatrix(empty_delegate_matrix, increment_matrix);

	//	UpperTriangleVanillaSwaptionQuotes_ConstPTR atm_swaption_implied_vol_ptr = create_UpperTriangleVanillaSwaptionQuotes(pLmmTenorStructure, tenorfixedleg, tenorfloatleg);

	//	GMatrixMapping gMatrixMapping(
	//		g_matrix_size
	//		, empty_delegate_matrix
	//		, atm_swaption_implied_vol_ptr->get_UpperTriangularIndexPairMatrix() );

	//	std::vector<size_t> truncated_cols;
	//	truncated_cols.push_back(2);
	//	truncated_cols.push_back(3);

	//	std::vector<size_t> truncated_rows;
	//	truncated_rows.push_back(2);
	//	truncated_rows.push_back(3);

	//	gMatrixMapping.reset_Truncated_gDelegate_Rows(truncated_rows);
	//	gMatrixMapping.reset_Truncated_gDelegate_Cols(truncated_cols);

	//	QuantLib::Array x = gMatrixMapping.get_DelegateArray();
	//	for(size_t i=0;i<x.size();++i){ x[i]=(1+i)*0.1;}
	//	gMatrixMapping.reset_gDelegate(x);

	//	gMatrixMapping.print("test_TruncatedGlobalResetGMatrixMapping_ColRow.csv");
	//}

}

void test_TruncatedLocalResetGMatrixMapping()
{
	{
		size_t nbYear = 6;
		Tenor tenorfixedleg = Tenor::_1YR;
		Tenor tenorfloatleg = Tenor::_6M;
		const double increment_matrix = 10;

		LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorfloatleg, nbYear) );

		size_t fixedfloatRatio = tenorfixedleg.ratioTo(tenorfloatleg);
		size_t horizon = pLmmTenorStructure->get_horizon() ;

		size_t        g_matrix_size = GMatrixMapping::get_gSizeFromNbYear(nbYear,fixedfloatRatio );
		size_t delegate_matrix_size = GMatrixMapping::get_gDelegateSizeFromHorizon(horizon,fixedfloatRatio );

		UpperTriangularDoubleMatrix empty_delegate_matrix(delegate_matrix_size,delegate_matrix_size);

		Initiate_UpperTriangularDoubleMatrix(empty_delegate_matrix, increment_matrix);

		UpperTriangleVanillaSwaptionQuotes_ConstPTR atm_swaption_implied_vol_ptr = create_UpperTriangleVanillaSwaptionQuotes(pLmmTenorStructure, tenorfixedleg, tenorfloatleg);

		GMatrixMapping gMatrixMapping(
			g_matrix_size
			, empty_delegate_matrix
			, atm_swaption_implied_vol_ptr->get_UpperTriangularIndexPairMatrix() );

		std::vector<size_t> truncated_cols;
		truncated_cols.push_back(2);
		truncated_cols.push_back(3);

		std::vector<size_t> truncated_rows;
		truncated_rows.push_back(2);
		truncated_rows.push_back(3);

		gMatrixMapping.reset_Truncated_gDelegate_Rows(truncated_rows);
		gMatrixMapping.reset_Truncated_gDelegate_Cols(truncated_cols);
		std::vector<size_t> non_truncated_row = get_NonTruncatedIndices(truncated_rows,nbYear);

		size_t value_counter=1;
		for(size_t iRow=0;iRow<non_truncated_row.size();++iRow)
		{
			const size_t iDelegate = non_truncated_row[iRow];

			QuantLib::Array x = gMatrixMapping.get_DelegateArray(iDelegate);
			for(size_t jX=0;jX<x.size();++jX)
			{
				x[jX]=value_counter*0.1;
				++value_counter;
			}
			gMatrixMapping.reset_gDelegate(x,iDelegate);
			std::cout<<" Delegate Row["<<iDelegate<<"]   "<<x<<std::endl;
			std::ostringstream local_gDelegate_file; 
			local_gDelegate_file<< "test_TruncatedLocalResetGMatrixMapping_"<<iDelegate<<"thRow.csv";

			gMatrixMapping.print(local_gDelegate_file.str() );
		}
	}

}

void test_TruncatedCascadeResetGMatrixMapping()
{
	// use of reset parallelograme truncated row
	{
		size_t nbYear = 5;
		Tenor tenorfixedleg = Tenor::_1YR;
		Tenor tenorfloatleg = Tenor::_6M;
		const double increment_matrix = 10;

		LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorfloatleg, nbYear) );

		size_t fixedfloatRatio = tenorfixedleg.ratioTo(tenorfloatleg);
		size_t horizon = pLmmTenorStructure->get_horizon() ;

		size_t        g_matrix_size = GMatrixMapping::get_gSizeFromNbYear(nbYear,fixedfloatRatio );
		size_t delegate_matrix_size = GMatrixMapping::get_gDelegateSizeFromHorizon(horizon,fixedfloatRatio );

		UpperTriangularDoubleMatrix empty_delegate_matrix(delegate_matrix_size,delegate_matrix_size);

		UpperTriangleVanillaSwaptionQuotes_ConstPTR atm_swaption_implied_vol_ptr = create_UpperTriangleVanillaSwaptionQuotes(pLmmTenorStructure, tenorfixedleg, tenorfloatleg);

		GMatrixMapping gMatrixMapping(
			g_matrix_size
			, empty_delegate_matrix
			, atm_swaption_implied_vol_ptr->get_UpperTriangularIndexPairMatrix() );

		gMatrixMapping.print("test_TruncatedCascadeResetGMatrixMapping_init.csv");

		//Initiate_UpperTriangularDoubleMatrixConstantDiagonal(empty_delegate_matrix, increment_matrix);
		Initiate_UpperTriangularDoubleMatrix(empty_delegate_matrix, increment_matrix);

		std::vector<std::pair<size_t,size_t> > truncated_cells;

		truncated_cells.push_back(std::pair<size_t,size_t>(1,1) );
		//truncated_cells.push_back(std::pair<size_t,size_t>(1,4) );
		//truncated_cells.push_back(std::pair<size_t,size_t>(2,2) );
		//truncated_cells.push_back(std::pair<size_t,size_t>(4,1) );
				
		for(size_t i=0;i<truncated_cells.size();++i)
		{
			gMatrixMapping.add_Truncated_gDelegate_Cells(truncated_cells[i]);
		}
		
		// loop reset delegate rows
		size_t max_DelegateRowIndex =	gMatrixMapping.get_MaxDelegateRowIndex();

		for(size_t iRowDelegate=1;iRowDelegate<=max_DelegateRowIndex;++iRowDelegate)
		{

			size_t max_DelegateColIndex = delegate_matrix_size - (iRowDelegate+1);

			for(size_t jColDelegate=1;jColDelegate<=max_DelegateColIndex;++jColDelegate)
			{
				std::pair<size_t,size_t> gDelegate_cell(iRowDelegate,jColDelegate);
				if( std::find(truncated_cells.begin(),truncated_cells.end(),gDelegate_cell) == truncated_cells.end() )
				{
						std::pair<size_t,size_t> gDelegate_cell(iRowDelegate,jColDelegate);
						gMatrixMapping.reset_gDelegate(empty_delegate_matrix(iRowDelegate,jColDelegate) , gDelegate_cell);

						std::ostringstream local_gDelegate_file; 
						local_gDelegate_file<< "test_TruncatedCascadeResetGMatrixMapping_reset_"<<iRowDelegate<<"row_"<< jColDelegate<<"col"  <<".csv";
						gMatrixMapping.print( local_gDelegate_file.str() );	
				}
			}	
		}
	}
}


void Initiate_UpperTriangularDoubleMatrix(UpperTriangularDoubleMatrix& matrix, double increment)
{
	size_t nbRow = matrix.size1();
	size_t nbCol = matrix.size2();
	assert(nbRow == nbCol);
	for(size_t k=0;k<nbRow;++k) // empty first row and first column
	{
		matrix(0,k) = -1000000000;
		matrix(k,0) = -1000000000;
	}

	double value_counter = increment;

	for(size_t i=1;i<nbRow;++i)
	{
		for(size_t j=1;j<nbRow-i;++j)
		{
			matrix(i,j)=value_counter;
			value_counter+=increment;
		}
	}
}

void Initiate_UpperTriangularDoubleMatrixConstantDiagonal(UpperTriangularDoubleMatrix& upper_matrix, double increment)
{

	const size_t matrix_size = upper_matrix.size1();
	assert( matrix_size == upper_matrix.size2());

	size_t nbSubDiagonal = matrix_size-2;
	size_t nbElement     = nbSubDiagonal*(nbSubDiagonal+1)/2;


	const double max_value = increment*matrix_size;
	double diag_value = max_value;
	size_t element_counter=0;

	for(size_t iDiag=0;iDiag<nbSubDiagonal;++iDiag)
	{
		size_t nbElementOnDiag = iDiag+1;

		for(size_t iElementOnDiag=0;iElementOnDiag<nbElementOnDiag;++iElementOnDiag)
		{
			size_t iRow = nbElementOnDiag-iElementOnDiag;
			size_t jCol = (nbElementOnDiag+1)-iRow;

			upper_matrix(iRow,jCol) = diag_value;

			++element_counter;
		}
		diag_value -= increment;
	}

	assert(element_counter == nbElement);
}



UpperTriangleVanillaSwaptionQuotes_ConstPTR create_UpperTriangleVanillaSwaptionQuotes(LMMTenorStructure_PTR pLMMTenorStructure, const Tenor& tenorfixedleg, const Tenor& tenorfloatleg)
{
	/// Creation of Rebonato Approx ==================================

	size_t nbFactor       = 3; // need to test nbFactor  = 3, and nbFactor = 
	size_t correlFullRank = pLMMTenorStructure->get_horizon()+1;
	size_t correlReducedRank = nbFactor;
	CorrelationReductionType::CorrelationReductionType correlReductionType = CorrelationReductionType::PCA;
	double correlAlpha = 0.0;
	double correlBeta  = 0.1;
	Correlation_PTR correlation(new XY_beta_Correlation(correlFullRank,correlReducedRank, correlReductionType,correlAlpha,correlBeta));
	correlation->calculate(); // for print.

	double a=0.22,b=0.4,c=0.6,d=0.15;
	double g_constParam = 1.;
	double shift_constParam = 0.0;

	Shifted_HGVolatilityParam::ABCDParameter abcdParam(a,b,c,d);
	ConstShifted_HGVolatilityParam_PTR pConstShifted_HGVolatilityParam( new ConstShifted_HGVolatilityParam(pLMMTenorStructure, abcdParam, g_constParam, shift_constParam));


	Shifted_HGVolatilityFunction_PTR pVolatilityFunction (new ConstShifted_HGVolatilityFunction(pLMMTenorStructure, correlation, pConstShifted_HGVolatilityParam)); 

	//! Dispersion
	Dispersion dispersion(pVolatilityFunction);

	Lmm_PTR lmm_ptr(new Lmm(dispersion) );

	LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmApproxVanillaSwaptionPricer(new LmmVanillaSwaptionApproxPricer_Rebonato(lmm_ptr));	
	/// End of creation of Rebonato Approx ==================================


	LiborQuotes_ConstPTR libor_quote_ptr = LiborQuotes::create_LiborInit(pLMMTenorStructure, 0.02);

	UpperTriangleVanillaSwaptionQuotes_ConstPTR atm_swaption_implied_vol_ptr = UpperTriangleVanillaSwaptionQuotes::create_ATMSwaptionImpliedVol(
		libor_quote_ptr,
		tenorfixedleg,
		tenorfloatleg,
		pLmmApproxVanillaSwaptionPricer
		);

	//assert( atm_swaption_implied_vol_ptr->check_swaprate_consistency(libor_quote_ptr) );

	return atm_swaption_implied_vol_ptr;
}

void check_gMatrix(const LowerTriangularDoubleMatrix& gMatrix, const double increment)
{
	// check values on nodes
	double node_value_counter=increment;
	for(size_t jNode=1;jNode<5;++jNode)
	{
		for(size_t iNode=jNode;iNode<5;++iNode)
		{
			size_t iRow_g = iNode*2+1;
			size_t jCol_g = jNode*2;

			if(gMatrix(iRow_g,jCol_g) != node_value_counter)
			{
				std::cout<<LMM::ERROR_MSG<< "check_gMatrix gMatrix("<<iRow_g<<","<<jCol_g<<") != "<<node_value_counter<<std::endl;
				assert(gMatrix(iRow_g,jCol_g) == node_value_counter);
			}
			else
			{
				std::cout<<LMM::NOTIF_MSG<< "check_gMatrix gMatrix("<<iRow_g<<","<<jCol_g<<") == "<<node_value_counter<<" OK"<<std::endl;
			}
			node_value_counter+=increment;
		}
	}

	// check extrapolated values
	for(size_t iRow_g=1;iRow_g<9;++iRow_g)	assert(gMatrix(iRow_g,1) == gMatrix(iRow_g+1,2) ); // extrapolation first g's column
	assert(gMatrix(9,1) == gMatrix(9,2) );
	for(size_t jCol_g=1;jCol_g<10;++jCol_g)assert(gMatrix(10,jCol_g) == gMatrix(9,jCol_g) );   // extrapolation last g's row
	assert(gMatrix(10,10) == gMatrix(9,9) );
	for(size_t iDiag_g=1;iDiag_g<9;++iDiag_g) assert(gMatrix(iDiag_g,iDiag_g) == gMatrix(iDiag_g+1,iDiag_g) ); // extrapolation g's diag
	assert(gMatrix(9,9) == gMatrix(9,8) );

	// check interpolated values
	assert( gMatrix(4,2) == (gMatrix(3,2) + gMatrix(5,2))/2 ) ;
	assert( gMatrix(6,2) == (gMatrix(5,2) + gMatrix(7,2))/2 ) ;
	assert( gMatrix(8,2) == (gMatrix(7,2) + gMatrix(9,2))/2 ) ;
	assert( gMatrix(6,4) == (gMatrix(5,4) + gMatrix(7,4))/2 ) ;
	assert( gMatrix(8,4) == (gMatrix(7,4) + gMatrix(9,4))/2 ) ;
	assert( gMatrix(8,6) == (gMatrix(7,6) + gMatrix(9,6))/2 ) ;

	assert( gMatrix(5,3) == (gMatrix(5,2) + gMatrix(5,4))/2 ) ;
	assert( gMatrix(7,3) == (gMatrix(7,2) + gMatrix(7,4))/2 ) ;
	assert( gMatrix(7,5) == (gMatrix(7,4) + gMatrix(7,6))/2 ) ;
	//assert( gMatrix(9,3) == (gMatrix(9,2) + gMatrix(9,4))/2 ) ;
	//assert( gMatrix(9,5) == (gMatrix(9,4) + gMatrix(9,6))/2 ) ;
	//assert( gMatrix(9,7) == (gMatrix(9,6) + gMatrix(9,8))/2 ) ;

	assert( gMatrix(4,3) == (gMatrix(3,2) + gMatrix(5,4))/2 ) ;
	assert( gMatrix(6,5) == (gMatrix(5,4) + gMatrix(7,6))/2 ) ;
	assert( gMatrix(8,7) == (gMatrix(7,6) + gMatrix(9,8))/2 ) ;
	assert( gMatrix(6,3) == (gMatrix(5,2) + gMatrix(7,4))/2 ) ;
	assert( gMatrix(8,5) == (gMatrix(7,4) + gMatrix(9,6))/2 ) ;
	assert( gMatrix(8,3) == (gMatrix(7,2) + gMatrix(9,4))/2 ) ;
}


std::vector<size_t> get_NonTruncatedIndices(const std::vector<size_t>& truncated_indices, size_t nbYear)
{
	assert(truncated_indices.back() < nbYear);
	std::vector<size_t> non_truncated;

	for(size_t iDelegate=1;iDelegate<nbYear;++iDelegate)
	{
		if(std::find(truncated_indices.begin(),truncated_indices.end(),iDelegate) == truncated_indices.end() )
		{
			non_truncated.push_back(iDelegate);
		}
	}
	return non_truncated;
}