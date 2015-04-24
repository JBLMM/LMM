#include <fstream>
#include <iostream>
#include <cassert>

#include <ql/math/matrix.hpp>

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>
#include <LMM/Test/Tests.h>
#include <LMM/numeric/Interpolation.h>
#include <LMM/numeric/UpperTriangularMatrixInterpolator.h>


using namespace numeric;

numeric::UpperTriangularDoubleMatrix createUpperTriangularDoubleMatrix(const size_t matrix_size, const double& increment, const double& max_value=100);
numeric::FullMatrix createFullDoubleMatrix(const size_t matrix_size, const double& increment);
void pop_RowColUpper(numeric::UpperTriangularDoubleMatrix & upper_matrix, const std::vector<size_t>& row_indices, const std::vector<size_t>& col_indices);
void pop_RowColFull(numeric::FullMatrix & matrix, const std::vector<size_t>& row_indices, const std::vector<size_t>& col_indices);
void print_UpperTriangularDoubleMatrix(const numeric::UpperTriangularDoubleMatrix& matrix, const std::string& matrix_name , const std::string& path_OutPut, bool ereasefile=true);
void print_FullMatrix(const numeric::FullMatrix& matrix, const std::string& filename);

void test_LinearInterpolation();
void test_LinearInterpolation_approx();
void test_VectorInterpolation();

void test_UpperMatrixColRowInterpolation();
void test_FullMatrixInterpolation();
void test_LowerMatrixInterpolation();

void test_UpperMatrixBlockInterpolation();

void test_Interpolation()
{

	//test_LinearInterpolation();
	//test_LinearInterpolation_approx();
	//test_VectorInterpolation();

	//test_FullMatrixInterpolation();
	test_UpperMatrixColRowInterpolation();

	//test_UpperMatrixBlockInterpolation();
}

void test_VectorInterpolation()
{
	{
		//     0    ||  1  [2]  3  [4]  5  [6]  7   8         // indices
		//   10e100 || 0.2|0.2|0.3|0.4|0.5|0.6|0.6|0.6        // interpolated

		numeric::Interpolation interpolator;

		QuantLib::Array node_values(3);
		node_values[0] = 0.2;
		node_values[1] = 0.4;
		node_values[2] = 0.6;

		std::vector<size_t> node_indices;
		node_indices.push_back(2);
		node_indices.push_back(4);
		node_indices.push_back(6);

		QuantLib::Array interpolated_values = interpolator.interpolateVector(node_values,node_indices,9);

		assert(interpolated_values.size() == 9);
		assert(std::abs( interpolated_values[1] - 0.2) < 1e-15 );
		assert(std::abs( interpolated_values[2] - 0.2) < 1e-15 );
		assert(std::abs( interpolated_values[3] - 0.3) < 1e-15 );
		assert(std::abs( interpolated_values[4] - 0.4) < 1e-15 );
		assert(std::abs( interpolated_values[5] - 0.5) < 1e-15 );
		assert(std::abs( interpolated_values[6] - 0.6) < 1e-15 );
		assert(std::abs( interpolated_values[7] - 0.6) < 1e-15 );
		assert(std::abs( interpolated_values[8] - 0.6) < 1e-15 );	
	}

	{
		//     0    ||  1  [2]  3   4   5  [6]  7   8         // indices
		//   10e100 ||  2 | 2 | 3 | 4 | 5 | 6 | 6 | 6 |       // interpolated

		numeric::Interpolation interpolator;

		QuantLib::Array node_values(2);
		node_values[0] = 2;
		node_values[1] = 6;

		std::vector<size_t> node_indices;
		node_indices.push_back(2);
		node_indices.push_back(6);

		QuantLib::Array interpolated_values = interpolator.interpolateVector(node_values,node_indices,9);

		assert(interpolated_values.size() == 9);
		assert(std::abs( interpolated_values[1] - 2) < 1e-15 );
		assert(std::abs( interpolated_values[2] - 2) < 1e-15 );
		assert(std::abs( interpolated_values[3] - 3) < 1e-15 );
		assert(std::abs( interpolated_values[4] - 4) < 1e-15 );
		assert(std::abs( interpolated_values[5] - 5) < 1e-15 );
		assert(std::abs( interpolated_values[6] - 6) < 1e-15 );
		assert(std::abs( interpolated_values[7] - 6) < 1e-15 );
		assert(std::abs( interpolated_values[8] - 6) < 1e-15 );		
	}

	{
		//     0    ||  1  [2]  3   4   5  [6]  7   8         // indices
		//   10e100 ||  6 | 6 | 5 | 4 | 3 | 2 | 2 | 2 |       // interpolated

		numeric::Interpolation interpolator;

		QuantLib::Array node_values(2);
		node_values[0] = 6;
		node_values[1] = 2;

		std::vector<size_t> node_indices;
		node_indices.push_back(2);
		node_indices.push_back(6);

		QuantLib::Array interpolated_values = interpolator.interpolateVector(node_values,node_indices,9);

		assert(interpolated_values.size() == 9);
		assert(std::abs( interpolated_values[1] - 6) < 1e-15 );
		assert(std::abs( interpolated_values[2] - 6) < 1e-15 );
		assert(std::abs( interpolated_values[3] - 5) < 1e-15 );
		assert(std::abs( interpolated_values[4] - 4) < 1e-15 );
		assert(std::abs( interpolated_values[5] - 3) < 1e-15 );
		assert(std::abs( interpolated_values[6] - 2) < 1e-15 );
		assert(std::abs( interpolated_values[7] - 2) < 1e-15 );
		assert(std::abs( interpolated_values[8] - 2) < 1e-15 );		
	}
}

void test_LinearInterpolation()
{
	std::vector<double> someVector(3);
	someVector[0] = 0.01;  
	someVector[1] = 0.02; 
	someVector[2] = 0.03;

	std::vector<double> falseTenorDates(3);
	falseTenorDates[0] = 0.;
	falseTenorDates[1] = 0.5;
	falseTenorDates[2] = 1.;

	double maturity_date = 0.75; 

	double coeff_1 = (falseTenorDates[2]-maturity_date)/(falseTenorDates[2]-falseTenorDates[1]);
	double coeff_2 = (maturity_date-falseTenorDates[1])/(falseTenorDates[2]-falseTenorDates[1]);
	double expectedResult = coeff_1*someVector[1] + coeff_2*someVector[2];

	numeric::Interpolation interpolator;
	interpolator.linearInterpolation(someVector,falseTenorDates,maturity_date);

	assert(someVector.size()      == 4 );
	assert(falseTenorDates.size() == 4 );

	assert(falseTenorDates[2] = maturity_date  );//additioinal point
	assert(someVector[2]      = expectedResult );

	std::cout<<"someVector : ";
	for (auto inter_val : someVector) std::cout << inter_val << " "; std::cout<<std::endl;

	std::cout<<"falseTenorDates : ";
	for (auto inter_val : falseTenorDates) std::cout << inter_val << " ";  std::cout<<std::endl ;

	std::cout << std::endl;
	std::cout << "Expected result : " << expectedResult << std::endl;
}

void test_LinearInterpolation_approx()
{
	std::vector<double> someVector(3);
	someVector[0] = 0.01;  
	someVector[1] = 0.02; 
	someVector[2] = 0.03;

	std::vector<double> falseTenorDates(3);
	falseTenorDates[0] = 0.  ;
	falseTenorDates[1] = 0.5 ;
	falseTenorDates[2] = 1.  ;

	double maturity_date = 0.50000001; // very closed date to an existing date --> no need to interpolate

	numeric::Interpolation interpolator;
	interpolator.linearInterpolation(someVector,falseTenorDates,maturity_date,0.000001);

	assert(someVector.size()      == 3 );
	assert(falseTenorDates.size() == 3 );

	std::cout<<"someVector : ";
	for (auto inter_val : someVector) std::cout << inter_val << " "; std::cout<<std::endl;

	std::cout<<"falseTenorDates : ";
	for (auto inter_val : falseTenorDates) std::cout << inter_val << " ";  std::cout<<std::endl ;

}

void test_UpperMatrixColRowInterpolation()
{
	
	/////////test interpolation, extrapolation by row
	{			
		size_t matrix_size=11;
		const double increment=1.;
		const double max_value=50.;

		std::ostringstream base_file_name; base_file_name<<"test_UpperMatrixColRowInterpolation_"<< matrix_size <<"_RowInterpolation.csv";
		std::string path_OutPut = LMMPATH::get_output_path() + base_file_name.str();

		numeric::UpperTriangularDoubleMatrix init_upper_matrix = createUpperTriangularDoubleMatrix(matrix_size, increment,max_value);
		print_UpperTriangularDoubleMatrix(init_upper_matrix,"init Matrix",path_OutPut);

		std::vector<size_t> missed_rows;	
		missed_rows.push_back(1);
		missed_rows.push_back(5);missed_rows.push_back(6);
		missed_rows.push_back(9);

		std::vector<size_t> missed_cols;

		numeric::UpperTriangularDoubleMatrix truncated_upper_matrix = init_upper_matrix;// copy of init matrix 
		pop_RowColUpper(truncated_upper_matrix, missed_rows, missed_cols);

		{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
		print_UpperTriangularDoubleMatrix(truncated_upper_matrix,"truncated_upper_matrix",path_OutPut,false);

		UpperTriangularMatrixColRowInterpolator interpolator(truncated_upper_matrix);
		interpolator.interpolate(missed_rows, missed_cols);

		{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
		print_UpperTriangularDoubleMatrix(truncated_upper_matrix,"Interpolated Matrix",path_OutPut,false);

		numeric::UpperTriangularDoubleMatrix diff_matrix(matrix_size,matrix_size);
		diff_matrix = truncated_upper_matrix - init_upper_matrix;
		{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
		print_UpperTriangularDoubleMatrix(diff_matrix,"diff Interpolated-Init Matrix ",path_OutPut,false);		
	}

	/////// test interpolation, extrapolation by col
	{	
		size_t matrix_size=11;
		const double increment=1.;
		const double max_value=50.;

		std::ostringstream base_file_name; base_file_name<<"test_UpperMatrixColRowInterpolation_"<< matrix_size <<"_ColInterpolation.csv";
		std::string path_OutPut = LMMPATH::get_output_path() + base_file_name.str();

		numeric::UpperTriangularDoubleMatrix init_upper_matrix = createUpperTriangularDoubleMatrix(matrix_size, increment,max_value);
		print_UpperTriangularDoubleMatrix(init_upper_matrix,"init Matrix",path_OutPut);

		std::vector<size_t> missed_rows;	
		std::vector<size_t> missed_cols;
		missed_cols.push_back(1);
		missed_cols.push_back(5);missed_cols.push_back(6);
		missed_cols.push_back(9);

		numeric::UpperTriangularDoubleMatrix truncated_upper_matrix = init_upper_matrix;// copy of init matrix 
		pop_RowColUpper(truncated_upper_matrix, missed_rows, missed_cols);

		{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
		print_UpperTriangularDoubleMatrix(truncated_upper_matrix,"truncated_upper_matrix",path_OutPut,false);

		UpperTriangularMatrixColRowInterpolator interpolator(truncated_upper_matrix);
		interpolator.interpolate(missed_rows, missed_cols);
		
		{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
		print_UpperTriangularDoubleMatrix(truncated_upper_matrix,"Interpolated Matrix",path_OutPut,false);

		numeric::UpperTriangularDoubleMatrix diff_matrix(matrix_size,matrix_size);
		diff_matrix = truncated_upper_matrix - init_upper_matrix;
		{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
		print_UpperTriangularDoubleMatrix(diff_matrix,"diff Interpolated-Init Matrix ",path_OutPut,false);
	}

	{	
		size_t matrix_size=11;
		const double increment=1.;
		const double max_value=50.;

		std::ostringstream base_file_name; base_file_name<<"test_UpperMatrixColRowInterpolation_"<< matrix_size <<"_FullInterpolation.csv";
		std::string path_OutPut = LMMPATH::get_output_path() + base_file_name.str();

		numeric::UpperTriangularDoubleMatrix init_upper_matrix = createUpperTriangularDoubleMatrix(matrix_size, increment,max_value);
		print_UpperTriangularDoubleMatrix(init_upper_matrix,"init Matrix",path_OutPut);

		std::vector<size_t> missed_cols;
		missed_cols.push_back(1); 
		missed_cols.push_back(4);
		missed_cols.push_back(5);
		missed_cols.push_back(6);

		std::vector<size_t> missed_rows;	
		missed_rows.push_back(1);
		missed_rows.push_back(6);
		missed_rows.push_back(7);

		numeric::UpperTriangularDoubleMatrix truncated_upper_matrix = init_upper_matrix;// copy of init matrix 
		pop_RowColUpper(truncated_upper_matrix, missed_rows, missed_cols);

		{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
		print_UpperTriangularDoubleMatrix(truncated_upper_matrix,"truncated_upper_matrix",path_OutPut,false);

		UpperTriangularMatrixColRowInterpolator interpolator(truncated_upper_matrix);
		interpolator.interpolate(missed_rows, missed_cols);

		{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
		print_UpperTriangularDoubleMatrix(truncated_upper_matrix,"interpolated ",path_OutPut,false);

		numeric::UpperTriangularDoubleMatrix diff_matrix(matrix_size,matrix_size);
		diff_matrix = truncated_upper_matrix - init_upper_matrix;
		{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
		print_UpperTriangularDoubleMatrix(diff_matrix,"diff Interpolated-Init Matrix ",path_OutPut,false);

	}


	//// test interpolation, extrapolation by col and row
	{	
		size_t matrix_size=30;
		const double increment=1.;
		const double max_value=50.;

		std::ostringstream base_file_name; base_file_name<<"test_UpperMatrixColRowInterpolation_"<< matrix_size <<"_FullInterpolation.csv";
		std::string path_OutPut = LMMPATH::get_output_path() + base_file_name.str();

		numeric::UpperTriangularDoubleMatrix init_upper_matrix = createUpperTriangularDoubleMatrix(matrix_size, increment,max_value);
		print_UpperTriangularDoubleMatrix(init_upper_matrix,"init Matrix",path_OutPut);

		std::vector<size_t> missed_cols;
		missed_cols.push_back(1); 

		missed_cols.push_back(7);
		missed_cols.push_back(8);
		missed_cols.push_back(9);
		missed_cols.push_back(10);
		missed_cols.push_back(11);
		missed_cols.push_back(12);
		missed_cols.push_back(13);

		missed_cols.push_back(15);
		missed_cols.push_back(16);
		missed_cols.push_back(17);

		missed_cols.push_back(27);
		missed_cols.push_back(28);

		std::vector<size_t> missed_rows;	

		missed_rows.push_back(1);		

		missed_rows.push_back(13);
		missed_rows.push_back(14);
		missed_rows.push_back(15);
		missed_rows.push_back(16);
		missed_rows.push_back(17);		

		missed_rows.push_back(19);

		missed_rows.push_back(21);
		missed_rows.push_back(22);
		missed_rows.push_back(23);

		missed_rows.push_back(27);
		missed_rows.push_back(28);


		numeric::UpperTriangularDoubleMatrix truncated_upper_matrix = init_upper_matrix;// copy of init matrix 
		pop_RowColUpper(truncated_upper_matrix, missed_rows, missed_cols);

		{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
		print_UpperTriangularDoubleMatrix(truncated_upper_matrix,"truncated_upper_matrix",path_OutPut,false);

		UpperTriangularMatrixColRowInterpolator interpolator(truncated_upper_matrix);
		interpolator.interpolate(missed_rows, missed_cols);

		{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
		print_UpperTriangularDoubleMatrix(truncated_upper_matrix,"interpolated ",path_OutPut,false);

		numeric::UpperTriangularDoubleMatrix diff_matrix(matrix_size,matrix_size);
		diff_matrix = truncated_upper_matrix - init_upper_matrix;
		{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
		print_UpperTriangularDoubleMatrix(diff_matrix,"diff Interpolated-Init Matrix ",path_OutPut,false);

	}
}

void test_UpperMatrixBlockInterpolation()
{

	//{
	//	size_t matrix_size=11;
	//	const double increment=1.;
	//	const double max_value=50.;

	//	std::ostringstream base_file_name; base_file_name<<"newtest_UpperMatrixColRowInterpolation_"<< matrix_size <<"_Element.csv";
	//	std::string path_OutPut = LMMPATH::get_output_path() + base_file_name.str();

	//	numeric::UpperTriangularDoubleMatrix init_upper_matrix = createUpperTriangularDoubleMatrix(matrix_size, increment,max_value);
	//	print_UpperTriangularDoubleMatrix(init_upper_matrix,"init Matrix",path_OutPut);

	//	std::vector<size_t> missed_rows;	
	//	missed_rows.push_back(1);

	//	missed_rows.push_back(3);
	//	missed_rows.push_back(4);

	//	std::vector<size_t> missed_cols;
	//	missed_cols.push_back(1);

	//	missed_cols.push_back(3);
	//	missed_cols.push_back(4);

	//	numeric::UpperTriangularDoubleMatrix truncated_upper_matrix = init_upper_matrix;// copy of init matrix 
	//	pop_RowColUpper(truncated_upper_matrix, missed_rows, missed_cols);

	//	{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
	//	print_UpperTriangularDoubleMatrix(truncated_upper_matrix,"truncated_upper_matrix",path_OutPut,false);

	//	UpperTriangularMatrixBlockInterpolator interpolator(truncated_upper_matrix);

	//	interpolator.extrapolate_TopLeftCorner(2,2);

	//	//interpolator.interpolate_SubUpperTriangle(2,8, 2,8);
	//	//interpolator.interpolate_SubLowerTriangle(2,5, 2,5);

	//	interpolator.interpolate_ROW(2, 2,5);
	//	interpolator.interpolate_COL(2, 2,5);

	//	interpolator.interpolate_SubDiag(5,8, 2,5);
	//	interpolator.interpolate_SubDiag(2,5, 5,8);

	//	interpolator.interpolate_SubRectangle(2,5,2,5);

	//	interpolator.interpolate_SubUpperTriangle(5,8, 2,5);
	//	interpolator.interpolate_SubUpperTriangle(2,5, 5,8);

	//	interpolator.extrapolate_Top(2,2,8);
	//	interpolator.extrapolate_Left(2,2,8);

	//	{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
	//	print_UpperTriangularDoubleMatrix(truncated_upper_matrix,"Interpolated Matrix",path_OutPut,false);

	//	numeric::UpperTriangularDoubleMatrix diff_matrix(matrix_size,matrix_size);
	//	diff_matrix = truncated_upper_matrix - init_upper_matrix;
	//	{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
	//	print_UpperTriangularDoubleMatrix(diff_matrix,"diff Interpolated-Init Matrix ",path_OutPut,false);		
	//}


	//{
	//	size_t matrix_size=21;
	//	const double increment=1.;
	//	const double max_value=50.;

	//	std::ostringstream base_file_name; base_file_name<<"newtest_UpperMatrixColRowInterpolation_"<< matrix_size <<"_Full.csv";
	//	std::string path_OutPut = LMMPATH::get_output_path() + base_file_name.str();

	//	numeric::UpperTriangularDoubleMatrix init_upper_matrix = createUpperTriangularDoubleMatrix(matrix_size, increment,max_value);
	//	print_UpperTriangularDoubleMatrix(init_upper_matrix,"init Matrix",path_OutPut);

	//	std::vector<size_t> missed_rows;	
	//	missed_rows.push_back(1);
	//	missed_rows.push_back(2);

	//	missed_rows.push_back(7);
	//	missed_rows.push_back(8);
	//	missed_rows.push_back(9);
	//	
	//	std::vector<size_t> missed_cols;
	//	missed_cols.push_back(1);
	//	missed_cols.push_back(2);
	//	missed_cols.push_back(3);

	//	missed_cols.push_back(9);
	//	missed_cols.push_back(10);
	//	

	//	numeric::UpperTriangularDoubleMatrix truncated_upper_matrix = init_upper_matrix;// copy of init matrix 
	//	pop_RowColUpper(truncated_upper_matrix, missed_rows, missed_cols);

	//	{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
	//	print_UpperTriangularDoubleMatrix(truncated_upper_matrix,"truncated_upper_matrix",path_OutPut,false);

	//	UpperTriangularMatrixBlockInterpolator interpolator(truncated_upper_matrix);

	//	interpolator.extrapolate_TopLeftCorner(3,4);

	//	//interpolator.interpolate_ROW(3, 8,11);
	//	interpolator.interpolate_SubRectangle(3,6, 8 , 11);
	//	interpolator.extrapolate_Top(3,8,11);

	//	//interpolator.interpolate_COL(4,6,10);
	//	//interpolator.interpolate_SubRectangle(6,10, 4 , 8);
	//	interpolator.interpolate_SubUpperTriangle(6,10, 4 , 8);
	//	interpolator.interpolate_SubLowerTriangle(6,10, 4 , 8);
	//	interpolator.extrapolate_Left(4,6,10);

	//	interpolator.interpolate_SubUpperTriangle(6, 12, 8 , 14);
	//	

	//	{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
	//	print_UpperTriangularDoubleMatrix(truncated_upper_matrix,"Interpolated Matrix",path_OutPut,false);

	//	numeric::UpperTriangularDoubleMatrix diff_matrix(matrix_size,matrix_size);
	//	diff_matrix = truncated_upper_matrix - init_upper_matrix;
	//	{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
	//	print_UpperTriangularDoubleMatrix(diff_matrix,"diff Interpolated-Init Matrix ",path_OutPut,false);		
	//}


	//// test interpolate parrallelogram
	{
		size_t matrix_size=15;
		const double increment=1.;
		const double max_value=50.;

		std::ostringstream base_file_name; base_file_name<<"newtest_UpperMatrixParallelogramInterpolation_"<< matrix_size <<"_Element.csv";
		std::string path_OutPut = LMMPATH::get_output_path() + base_file_name.str();

		numeric::UpperTriangularDoubleMatrix init_upper_matrix = createUpperTriangularDoubleMatrix(matrix_size, increment,max_value);
		print_UpperTriangularDoubleMatrix(init_upper_matrix,"init Matrix",path_OutPut);

		std::vector<size_t> missed_rows;	
		
		missed_rows.push_back(2);
		missed_rows.push_back(3);
		missed_rows.push_back(4);
		missed_rows.push_back(5);

		std::vector<size_t> missed_cols;
		
		numeric::UpperTriangularDoubleMatrix truncated_upper_matrix = init_upper_matrix;// copy of init matrix 
		pop_RowColUpper(truncated_upper_matrix, missed_rows, missed_cols);

		{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
		print_UpperTriangularDoubleMatrix(truncated_upper_matrix,"truncated_upper_matrix",path_OutPut,false);

		UpperTriangularMatrixBlockInterpolator interpolator(truncated_upper_matrix);

		interpolator.interpolate_Parallelogram(1,6,6,8,11,13);

		interpolator.interpolate_Parallelogram(1,6,1,5,6,10);

		{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
		print_UpperTriangularDoubleMatrix(truncated_upper_matrix,"Interpolated Matrix",path_OutPut,false);

		numeric::UpperTriangularDoubleMatrix diff_matrix(matrix_size,matrix_size);
		diff_matrix = truncated_upper_matrix - init_upper_matrix;
		{std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app); gMatrixFile<<std::endl<<std::endl; gMatrixFile.close();}// print new lines
		print_UpperTriangularDoubleMatrix(diff_matrix,"diff Interpolated-Init Matrix ",path_OutPut,false);		
	}
}

void test_FullMatrixInterpolation()
{
	size_t matrix_size=11;
	const double increment=1.;
	numeric::UpperTriangularDoubleMatrix init_full_matrix = createFullDoubleMatrix(matrix_size, increment);

	print_FullMatrix(init_full_matrix,"test_FullMatrixInterpolation_init_matrix.csv");

	numeric::Interpolation interpolator;

	std::vector<size_t> missed_cols;
	missed_cols.push_back(3);missed_cols.push_back(4); missed_cols.push_back(9);
	std::vector<size_t> missed_rows;
	missed_rows.push_back(3);missed_rows.push_back(4);missed_rows.push_back(5); 
	missed_rows.push_back(9);

	numeric::FullMatrix truncated_full_matrix = init_full_matrix;// copy of init matrix 
	pop_RowColFull(truncated_full_matrix, missed_rows, missed_cols);

	print_FullMatrix(truncated_full_matrix,"test_FullMatrixInterpolation_truncated_matrix.csv");

	interpolator.fullMatrixInterpolate(truncated_full_matrix, missed_rows, missed_cols);

	print_FullMatrix(truncated_full_matrix,"test_FullMatrixInterpolation_interpolated_matrix.csv");
}

numeric::UpperTriangularDoubleMatrix createUpperTriangularDoubleMatrix(size_t matrix_size, const double& increment, const double& max_value)
{
	numeric::UpperTriangularDoubleMatrix upper_matrix(matrix_size, matrix_size);

	size_t nbSubDiagonal = matrix_size-2;
	size_t nbElement     = nbSubDiagonal*(nbSubDiagonal+1)/2;

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
	return upper_matrix;
}

void pop_RowColUpper(numeric::UpperTriangularDoubleMatrix & upper_matrix, const std::vector<size_t>& row_indices, const std::vector<size_t>& col_indices)
{
	size_t matrix_size = upper_matrix.size1();
	assert( matrix_size = upper_matrix.size2() ); 

	for(size_t iRow=1;iRow<matrix_size;++iRow)
	{
		for(size_t jCol=1;jCol<matrix_size-iRow;++jCol)
		{
			if(  std::find(row_indices.begin(), row_indices.end(), iRow)!=row_indices.end()
				|| std::find(col_indices.begin(), col_indices.end(), jCol)!=col_indices.end()
				)
			{
				upper_matrix(iRow,jCol) = -100;
			}
		}
	}
}

void print_UpperTriangularDoubleMatrix(const numeric::UpperTriangularDoubleMatrix& matrix, const std::string& matrix_name , const std::string& path_OutPut, bool ereasefile)
{
	std::vector<PrintElement_PTR> elements_print;

	PrintElement_PTR matrix_print = PrintElement_PTR(new MatrixPrintElement<numeric::UpperTriangularDoubleMatrix>(matrix_name, matrix));			

	elements_print.push_back(matrix_print);

	Printer printer(path_OutPut, elements_print);
	printer.print(ereasefile); 
}



numeric::FullMatrix createFullDoubleMatrix(const size_t matrix_size, const double& increment)
{
	numeric::FullMatrix matrix(matrix_size, matrix_size);

	double cell_counter = increment;
	for(size_t iRow=1;iRow<matrix_size;++iRow)
	{
		for(size_t jCol=1;jCol<matrix_size;++jCol)
		{
			matrix(iRow,jCol) = cell_counter;
			cell_counter += increment;
		}
	}
	return matrix;
}

void pop_RowColFull(numeric::FullMatrix & matrix, const std::vector<size_t>& row_indices, const std::vector<size_t>& col_indices)
{
	for(size_t iRow=1;iRow<matrix.size1();++iRow)
	{
		for(size_t jCol=1;jCol<matrix.size2();++jCol)
		{
			if(  std::find(row_indices.begin(), row_indices.end(), iRow)!=row_indices.end()
				|| std::find(col_indices.begin(), col_indices.end(), jCol)!=col_indices.end()
				)
			{
				matrix(iRow,jCol) = -10000;
			}
		}
	}
}

void print_FullMatrix(const numeric::FullMatrix& matrix, const std::string& filename)
{
	std::string path_FileOut = LMMPATH::get_output_path() + filename;
	std::ofstream outputFile  ;  
	outputFile.open(path_FileOut.c_str() );

	std::cout<<std::endl<<filename<<std::endl;

	for(size_t iRow=0;iRow<matrix.size1();++iRow)
	{
		for(size_t jCol=0;jCol<matrix.size2();++jCol)
		{			
			std::cout<<matrix(iRow,jCol) <<", ";
			outputFile<<matrix(iRow,jCol) <<", ";
		}
		std::cout<<std::endl;
		outputFile<<std::endl;
	}

	outputFile.close();
}