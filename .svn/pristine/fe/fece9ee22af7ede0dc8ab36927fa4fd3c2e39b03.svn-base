#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/numeric/ublas/triangular.hpp>  // Triangular matrix

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/LMMTenorStructure.h>

// specialise for printing upper triangular matrix
typedef boost::numeric::ublas::matrix<double> UpperTriangularDoubleMatrix; // UTDM
typedef boost::numeric::ublas::triangular_matrix<double, boost::numeric::ublas::lower> LowerTriangularDoubleMatrix; // LTDM


/*! LmmGnuplotPrinterMatrix print for gnuplot so the first row and column will not be printed
 *
 */
class LmmGnuplotPrinterMatrix
{
public :
	LmmGnuplotPrinterMatrix(LMMTenorStructure_PTR pLMMTenorStructure) 
	: pLMMTenorStructure_(pLMMTenorStructure)
	, graphic_title_("Vol Matrix"){}


	void printVolMatrix(const LowerTriangularDoubleMatrix& lower_matrix, const std::string& base_filename) const
	{
		printVolMatrixDataFile(lower_matrix,base_filename);
		printVolMatrixGnuplotFile(lower_matrix,base_filename);
	}

	void printFullMatrix(const boost::numeric::ublas::matrix<double>& matrix
						, const std::vector<double>& xticks
						, const std::vector<double>& yticks
		                , const std::string& base_filename) const;

private:
	LMMTenorStructure_PTR pLMMTenorStructure_;

	std::string graphic_title_;

	void printVolMatrixDataFile(const LowerTriangularDoubleMatrix& lower_matrix, const std::string& base_filename) const;
	void printVolMatrixGnuplotFile(const LowerTriangularDoubleMatrix& lower_matrix, const std::string& base_filename) const;

	std::string get_dataFileName(const std::string& base_filename) const {std::string data_file_name = base_filename + ".dat"; return data_file_name;}
	std::string get_imageFileName(const std::string& base_filename) const {std::string data_file_name = base_filename + ".eps"; return data_file_name;}
	std::string get_gnuplotFileName(const std::string& base_filename) const {std::string gp_file_name = base_filename + ".gp"; return gp_file_name;	}
};
