#

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/numeric/ublas/triangular.hpp>  // Triangular matrix

#include <LMM/helper/LmmGnuplotPrinterMatrix.h>



void LmmGnuplotPrinterMatrix::printVolMatrixDataFile(const LowerTriangularDoubleMatrix& lower_matrix, const std::string& base_filename) const
{
	std::string data_file_name = get_dataFileName( base_filename);

	std::string path_OutPut = LMMPATH::get_output_path() + data_file_name;


	std::ofstream matrix_data_file ;
	matrix_data_file.open(path_OutPut.c_str() );
	matrix_data_file<<"#Lmm Matrix output for gnuplot printing surface"<<std::endl;

	size_t nbLibor = lower_matrix.size1();
	assert(nbLibor == pLMMTenorStructure_->get_nbLIBOR() );

	for(size_t iLibor=1;iLibor<nbLibor;++iLibor)
	{
		const double x_value = pLMMTenorStructure_->get_tenorDate(iLibor);

		for(size_t jTime=1;jTime<nbLibor;++jTime)
		{			
			const double y_value = pLMMTenorStructure_->get_tenorDate(jTime);
			const double z_value = lower_matrix(iLibor,jTime);
			matrix_data_file<<"	"<<x_value<<"	"<<y_value<<"	"<<z_value<<std::endl;
		}	
		matrix_data_file<<std::endl;
	}

	matrix_data_file.close();
}

void LmmGnuplotPrinterMatrix::printVolMatrixGnuplotFile(const LowerTriangularDoubleMatrix& lower_matrix, const std::string& base_filename) const
{
	std::string gp_file_name = get_gnuplotFileName(base_filename);

	std::string path_OutPut = LMMPATH::get_output_path() + gp_file_name;

	std::ofstream gnuplot_file ;
	gnuplot_file.open(path_OutPut.c_str() );

	gnuplot_file<<"#gnuplot file"<<std::endl;

	gnuplot_file<<"set xlabel \"Libor\""<<std::endl;
	gnuplot_file<<"set ylabel \"Time\""<<std::endl;
	gnuplot_file<<"set title \""<< graphic_title_ <<"\"" <<std::endl;
	gnuplot_file<<"set style data lines"<<std::endl;
	gnuplot_file<<"set hidden3d"<<std::endl;
	gnuplot_file<<"set pm3d ; set palette"<<std::endl;
	gnuplot_file<<"set cntrparam levels 10"<<std::endl;
	gnuplot_file<<"set contour base"<<std::endl;
	
	std::string data_file_name = get_dataFileName(base_filename);

	gnuplot_file<<"splot \""<< data_file_name <<"\"" <<std::endl;

	gnuplot_file<<"pause -1 \" Hit return to save image\" "<<std::endl;

	gnuplot_file<<"set terminal postscript eps"<<std::endl;

	std::string image_file_name = get_imageFileName(base_filename);

	gnuplot_file<<"set output \""<<image_file_name <<"\""<<std::endl;

	gnuplot_file<<"replot"<<std::endl;


	gnuplot_file.close();
}

void LmmGnuplotPrinterMatrix::printFullMatrix(const boost::numeric::ublas::matrix<double>& matrix
											  , const std::vector<double>& xticks
											  , const std::vector<double>& yticks
											  ,const std::string& base_filename) const
{
	assert(xticks.size() == matrix.size2() );
	assert(yticks.size() == matrix.size1() );


	{//print matrix data files
		std::string data_file_name = get_dataFileName( base_filename);

		std::string path_OutPut = LMMPATH::get_output_path() + data_file_name;

		std::ofstream matrix_data_file ;
		matrix_data_file.open(path_OutPut.c_str() );
		matrix_data_file<<"### Lmm Matrix output for gnuplot printing surface"<<std::endl;
		matrix_data_file<<"### grid xtics ";for(size_t i=0;i<xticks.size();++i){ matrix_data_file<<", "<<xticks[i]; }
		matrix_data_file<<std::endl;
		matrix_data_file<<"### grid ytics ";for(size_t i=0;i<yticks.size();++i){ matrix_data_file<<", "<<yticks[i]; }

		for(size_t i=0;i<matrix.size1();++i)
		{
			const double x_value = xticks[i] ;
			for(size_t j=0;j<matrix.size2();++j)
			{							
				const double y_value = yticks[j];
				const double z_value = matrix(i,j);
				matrix_data_file<<"	"<<x_value<<"	"<<y_value<<"	"<<z_value<<std::endl;
			}	
			matrix_data_file<<std::endl;
		}

		matrix_data_file.close();
	}

	{// print gnuplot file
		std::string gp_file_name = get_gnuplotFileName(base_filename);

		std::string path_OutPut = LMMPATH::get_output_path() + gp_file_name;

		std::ofstream gnuplot_file ;
		gnuplot_file.open(path_OutPut.c_str() );

		gnuplot_file<<"### gnuplot file"<<std::endl;
		gnuplot_file<<"### grid xtics ";for(size_t i=0;i<xticks.size();++i){ gnuplot_file<<", "<<xticks[i]; }
		gnuplot_file<<std::endl;
		gnuplot_file<<"### grid ytics ";for(size_t i=0;i<yticks.size();++i){ gnuplot_file<<", "<<yticks[i]; }


		gnuplot_file<<"set xlabel \"xLabel\""<<std::endl;
		gnuplot_file<<"set ylabel \"yLabel\""<<std::endl;
		gnuplot_file<<"set title \"Graph title\" "<<std::endl;
		gnuplot_file<<"set style data lines"<<std::endl;
		gnuplot_file<<"set hidden3d"<<std::endl;
		gnuplot_file<<"set pm3d ; set palette"<<std::endl;
		gnuplot_file<<"set cntrparam levels 10"<<std::endl;
		gnuplot_file<<"set contour base"<<std::endl;
		
		std::string data_file_name = get_dataFileName(base_filename);

		gnuplot_file<<"splot \""<< data_file_name <<"\"" <<std::endl;

		gnuplot_file<<"pause -1 \" Hit return to save image\" "<<std::endl;

		gnuplot_file<<"set terminal postscript eps"<<std::endl;

		std::string image_file_name = get_imageFileName(base_filename);

		gnuplot_file<<"set output \""<<image_file_name <<"\""<<std::endl;

		gnuplot_file<<"replot"<<std::endl;

		gnuplot_file.close();
	}
}