#include <LMM/Test/Tests.h>
#include <LMM/helper/Noise.h>
#include <LMM/helper/LmmGnuplotPrinterMatrix.h>

#include <iostream>
#include <fstream>
#include <LMM/helper/GenericPath.h>

#include <climits>//test

void test_PrintDataPenaltyAnalysis();

void test_LmmGnuplotPrinterMatrix()
{

	//Print matrix for penalty analisys
	test_PrintDataPenaltyAnalysis();

}


void test_PrintDataPenaltyAnalysis()
{
	//empty
	size_t nbYear = 5;
	Tenor tenorfixedleg = Tenor::_1YR;
	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorfixedleg, nbYear) );


	LmmGnuplotPrinterMatrix gnuplot_printer(pLmmTenorStructure);

	std::vector<double> xtick;
	xtick.push_back(0);xtick.push_back(0.001);xtick.push_back(0.01);xtick.push_back(0.1);xtick.push_back(1);//penalty Time Homogeneity
	std::vector<double> ytick;
	ytick.push_back(0);ytick.push_back(1e-6);ytick.push_back(1e-4);ytick.push_back(0.01);ytick.push_back(1);//penalty Libor

	boost::numeric::ublas::matrix<double> matrix_error_gVirtual(5,5);

	matrix_error_gVirtual(0,0) = 1.00E-12	;
	matrix_error_gVirtual(0,1) = 3.0675	;
	matrix_error_gVirtual(0,2) = 27.7316	;
	matrix_error_gVirtual(0,3) = 75.3	;
	matrix_error_gVirtual(0,4) = 154.035;
	matrix_error_gVirtual(1,0) = 2.31E-05	;
	matrix_error_gVirtual(1,1) = 3.06569	;
	matrix_error_gVirtual(1,2) = 26.7316	;
	matrix_error_gVirtual(1,3) = 75.3004	;
	matrix_error_gVirtual(1,4) = 154.035;
	matrix_error_gVirtual(2,0) = 2.26E-01;	
	matrix_error_gVirtual(2,1) = 2.96419	;
	matrix_error_gVirtual(2,2) = 26.5651	;
	matrix_error_gVirtual(2,3) = 75.1531	;
	matrix_error_gVirtual(2,4) = 154.005;
	matrix_error_gVirtual(3,0) = 2.39E+01;	
	matrix_error_gVirtual(3,1) = 23.6866	;
	matrix_error_gVirtual(3,2) = 18.074	;
	matrix_error_gVirtual(3,3) = 25.2515	;
	matrix_error_gVirtual(3,4) = 37.6008;
	matrix_error_gVirtual(4,0) = 5.90E+01;	
	matrix_error_gVirtual(4,1) = 58.4421	;
	matrix_error_gVirtual(4,2) = 38.3979	;
	matrix_error_gVirtual(4,3) = 30.1227	;
	matrix_error_gVirtual(4,4) = 22.549  ;


	boost::numeric::ublas::matrix<double> matrix_error_Quote(5,5);

	matrix_error_Quote(0,0)= 4.00E-14;
	matrix_error_Quote(0,1)= 0.0189747;	
	matrix_error_Quote(0,2)= 0.552726	;
	matrix_error_Quote(0,3)= 2.21675	;
	matrix_error_Quote(0,4)= 4.11175;
	matrix_error_Quote(1,0)= 1.64E-07;	
	matrix_error_Quote(1,1)= 0.0189747;	
	matrix_error_Quote(1,2)= 0.552726	;
	matrix_error_Quote(1,3)= 2.21675	;
	matrix_error_Quote(1,4)= 4.1175;
	matrix_error_Quote(2,0)= 1.61E-03;	
	matrix_error_Quote(2,1)= 0.019043;	
	matrix_error_Quote(2,2)= 0.552948;	
	matrix_error_Quote(2,3)= 2.21705	;
	matrix_error_Quote(2,4)= 4.1123;
	matrix_error_Quote(3,0)= 8.22E-01	;
	matrix_error_Quote(3,1)= 0.823031	;
	matrix_error_Quote(3,2)= 0.925839	;
	matrix_error_Quote(3,3)= 2.271128	;
	matrix_error_Quote(3,4)= 5.73203;
	matrix_error_Quote(4,0)= 4.14E+00;	
	matrix_error_Quote(4,1)= 4.1434	;
	matrix_error_Quote(4,2)= 4.15509	;
	matrix_error_Quote(4,3)= 4.32499	;
	matrix_error_Quote(4,4)= 6.0921;


	gnuplot_printer.printFullMatrix(matrix_error_gVirtual,xtick,ytick,"penalty_analysis_gVirtual_error");
	gnuplot_printer.printFullMatrix(matrix_error_Quote,xtick,ytick,"penalty_analysis_quoteVirtual_error");
}