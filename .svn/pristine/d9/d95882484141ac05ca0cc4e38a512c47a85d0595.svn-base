//#include <iostream>
//#include "DateCreator.h"
//
//
//
////! YY shit code, need to delete all ! 
//std::vector<double> DateCreator::createLmmTenorDates(const std::vector<double>& lmmDeltaT)
//{
//	size_t size = lmmDeltaT.size()+1;
//	std::vector<double> lmmTenorDates(size);
//	lmmTenorDates[0] = 0.;
//	for (size_t i = 1; i < size; ++i)
//		lmmTenorDates[i] = lmmTenorDates[i-1] + lmmDeltaT[i-1];
//	
//	return lmmTenorDates;
//}
//
////-- Size = horizon + 1

//Matrix_PairOfYears DateCreator::createSwaptionMatrixDates_Years(size_t size, double tenor)
//{
//	//size_t size = (size_t)(lastYear/tenor)+1;
//	Matrix_PairOfYears res(size,std::vector<std::pair<double,double>>(size));
//
//	for(size_t j = 0; j < size; ++j)
//	{
//		size_t row_cpt = 0;
//		for (size_t i = 0; i < size-j; ++i)
//		{
//			std::pair<double,double>p(row_cpt*tenor,j*tenor);
//			res[i][j] = p;
//			row_cpt++;
//		}
//	}
//
//	return res;
//}
//
//matrix_pairOfMonths DateCreator::convertSwaptionDates_YearsToMonths(const Matrix_PairOfYears& matrixYears)
//{
//	size_t size = matrixYears.size();
//	matrix_pairOfMonths res(size,std::vector<std::pair<size_t,size_t>>(size));
//
//	for (size_t i = 0; i < size; ++i)
//	{
//		for (size_t j = 0; j < size; ++j)
//		{
//			std::pair<size_t,size_t> p_month;
//			p_month.first = (size_t)(matrixYears[i][j].first*12);
//			p_month.second = (size_t)(matrixYears[i][j].second*12);
//			res[i][j] = p_month;
//		}
//	}
//
//	return res;
//}
//
////-- TODO: Not generic enough!!! Need to handle other cases e.g. trimestrial tenors 
//matrix_pairOfIndices DateCreator::convertSwaptionDates_MonthsToIndices(double tenor, const matrix_pairOfMonths& matrixMonths)
//{
//	size_t size = matrixMonths.size();
//	matrix_pairOfIndices res(size,std::vector<std::pair<size_t,size_t>>(size));
//	size_t tenorInMonths = (size_t)(12*tenor);
//
//	for (size_t i = 0; i < size; ++i)
//	{
//		for (size_t j = 0; j < size; ++j)
//		{
//			std::pair<size_t,size_t> p_indices;
//			p_indices.first = matrixMonths[i][j].first/tenorInMonths;
//			p_indices.second = (matrixMonths[i][j].first + matrixMonths[i][j].second)/tenorInMonths;
//			res[i][j] = p_indices;
//		}
//	}
//
//	return res;
//}
//
//
//void DateCreator::printMatrixOfYears(std::ofstream& outputFile, const Matrix_PairOfYears& mat) 
//{
//	for (size_t i = 0; i < mat.size(); ++i)
//	{
//		for (size_t j = 0; j < mat[i].size(); ++j)
//			outputFile << mat[i][j].first << "Y--" << mat[i][j].second << "Y"<< "\t";
//
//		outputFile << std::endl;
//	}
//	outputFile << std::endl << std::endl;
//}
//
//
//void DateCreator::printMatrixOfMonths(std::ofstream& outputFile, const matrix_pairOfMonths& mat) 
//{
//	for (size_t i = 0; i < mat.size(); ++i)
//	{
//		for (size_t j = 0; j < mat[i].size(); ++j)
//			outputFile << mat[i][j].first << "M--" << mat[i][j].second << "M" << "\t";
//
//		outputFile << std::endl;
//	}
//	outputFile << std::endl << std::endl;
//}
//
//
//void DateCreator::printMatrixOfIndices(std::ofstream& outputFile, const matrix_pairOfIndices& mat) 
//{
//	for (size_t i = 0; i < mat.size(); ++i)
//	{
//		for (size_t j = 0; j < mat[i].size(); ++j)
//			outputFile << mat[i][j].first << "--" << mat[i][j].second << "\t";
//
//		outputFile << std::endl;
//	}
//	outputFile << std::endl << std::endl;
//}
//
////--------------------------------------------------------------------------//
////                                 TESTS                                    //
////--------------------------------------------------------------------------//
//
//void DateCreator::testMatrixDatesCreator()
//{
//	double lastYear = 6.5;
//	double tenor = 0.5;
//	Matrix_PairOfYears   dateMatrix   = createSwaptionMatrixDates_Years(lastYear,tenor);  // YY shit: bug 
//	matrix_pairOfMonths  dateMatrix_M = convertSwaptionDates_YearsToMonths(dateMatrix);
//	matrix_pairOfIndices dateMatrix_I = convertSwaptionDates_MonthsToIndices(tenor,dateMatrix_M);
//
//	std::ofstream outputFile;
//	outputFile.open("E:\\swaptionDates.xls");
//
//	printMatrixOfYears(outputFile,dateMatrix);
//	printMatrixOfMonths(outputFile,dateMatrix_M);
//	printMatrixOfIndices(outputFile,dateMatrix_I);
//
//	outputFile.close();
//}