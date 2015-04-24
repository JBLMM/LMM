//#include <iostream>
//#include <fstream>
//#include <LMM/ModelMCLMM/VolatilityGridBuilder.h>
//#include "BilinearInterpolation.h"
//
////-- Constructor
////! Vector of pillar values must be properly ordered 
//VolatilityGridBuilder::VolatilityGridBuilder(size_t dim, size_t jumpsize, const std::vector<double>& pillarValues)
//	: dim_(dim), grid_(dim,std::vector<double>(dim)), jumpsize_(jumpsize)
//{
//	//-- Assign values to pillars
//	createPillars(pillarValues);
//
//	//-- Add pillars to the grid
//	for each (Pillar p in pillars_)
//	{
//		size_t row = p.getX();
//		size_t col = p.getY();
//		grid_[row][col] = p.getValue();
//	}
//
//	//-- Build the rest of the grid by interpolation/extrapolation
//	buildGrid(); 
//}
//
//void VolatilityGridBuilder::createPillars(const std::vector<double>& pillarValues)
//{
//	//-- Define pillar coordinates using jumpsize
//	size_t i = jumpsize_;
//	size_t j = jumpsize_;
//	size_t dim_tmp = dim_;
//	size_t p_index = 0;
//
//	while (i < dim_)
//	{
//		while (j < dim_tmp)
//		{
//			Pillar p (i,j,pillarValues[p_index]); 
//			pillars_.push_back(p);
//
//			j += jumpsize_;
//			p_index++;
//		}
//		i += jumpsize_;
//		j = jumpsize_;
//		dim_tmp -= jumpsize_;
//	}
//}
//
//void VolatilityGridBuilder::buildGrid()
//{
//	//-- Linear interpolations on rows and columns of pillars
//	interpolateAllRows();
//
//	interpolateAllColumns();
//
//	bilinearInterpolations();
//
//	borderExtrapolations();
//
//	//-- Anti-diagonal
//	fillAntidiagonal();
//}
//
//void VolatilityGridBuilder::interpolateAllRows()
//{
//	NumericalTools::Interpolation interpolator;
//
//	size_t row_index = jumpsize_;
//	while(row_index < dim_) // Loop on rows 
//	{
//		//-- take all pillars from current row
//		std::vector<Pillar> pillarsByRow = Pillar::findPillarsByRow(row_index,pillars_);
//
//		for (size_t i = 0; i < pillarsByRow.size()-1; ++i)
//		{
//			double x1 = pillarsByRow[i].getY();
//			double y1 = pillarsByRow[i].getValue();
//
//			double x2 = pillarsByRow[i+1].getY();
//			double y2 = pillarsByRow[i+1].getValue();
//
//			double x_interp = (x1+x2)/2.; //-- TODO: Not generic enough!! 
//			//-- Must also handle the case jumpsize > 2
//			
//			double y_interp = interpolator.linearInterpolation(x1,x2,y1,y2,x_interp);
//
//			size_t p_col = (size_t)x_interp;
//			grid_[row_index][p_col] = y_interp;
//
//		}
//		row_index += jumpsize_;
//	}
//}
//
//void VolatilityGridBuilder::interpolateAllColumns()
//{
//	NumericalTools::Interpolation interpolator;
//
//	size_t col_index = jumpsize_;
//	while(col_index < dim_) // Loop on columns 
//	{
//		//-- take all pillars from current columns
//		std::vector<Pillar> pillarsByCol = Pillar::findPillarsByColumn(col_index,pillars_);
//
//		for (size_t i = 0; i < pillarsByCol.size()-1; ++i)
//		{
//			double x1 = pillarsByCol[i].getX();
//			double y1 = pillarsByCol[i].getValue();
//
//			double x2 = pillarsByCol[i+1].getX();
//			double y2 = pillarsByCol[i+1].getValue();
//
//			double x_interp = (x1+x2)/2.; //-- TODO: Not generic enough!! 
//			//-- Must also handle the case jumpsize > 2
//			
//			double y_interp = interpolator.linearInterpolation(x1,x2,y1,y2,x_interp);
//
//			size_t p_row = (size_t)x_interp;
//			grid_[p_row][col_index] = y_interp;
//
//		}
//		col_index += jumpsize_;
//	}
//}
//
//void VolatilityGridBuilder::bilinearInterpolations()
//{
//	BilinearInterpolation bilin_interp;
//
//	//-- Bilinear interpolations between four pillars
//	//-- Select pillars row after row
//	for (size_t i = jumpsize_; i < dim_-1; i=i+jumpsize_)
//	{
//		std::vector<Pillar> currentPillar1 = Pillar::findPillarsByRow(i,pillars_);
//		if (currentPillar1.size() <= 2) 
//			break;
//
//		std::vector<Pillar> currentPillar2 = Pillar::findPillarsByRow(i+jumpsize_,pillars_);
//
//		//-- Select four pillars
//		for (size_t j = 0; j < currentPillar1.size()-1; ++j)
//		{
//			if (j+1 == currentPillar2.size()) // triangle
//				break;
//
//			double d = currentPillar1[j].getValue();
//			double c = currentPillar1[j+1].getValue();
//			double b = currentPillar2[j+1].getValue();
//			double a = currentPillar2[j].getValue();
//
//			double x1 = currentPillar2[j].getY();
//			double y1 = currentPillar2[j].getX();
//			double x2 = currentPillar2[j+1].getY();
//			double y2 = currentPillar1[j].getX();
//
//			double xp = (x1+x2)/2;
//			double yp = (y1+y2)/2;
//			
//			double res_interp = bilin_interp.interpolate(xp,yp,x1,x2,y1,y2,a,b,c,d);
//			size_t p_row = (size_t)xp;
//			size_t p_col = (size_t)yp;
//			grid_[p_row][p_col] = res_interp;
//		}
//	}
//}
//
//void VolatilityGridBuilder::borderExtrapolations()
//{
//	//-- Constant extrapolation on the borders
//	for (size_t j = jumpsize_; j < dim_-1; ++j)
//		grid_[jumpsize_-1][j] = grid_[jumpsize_][j]; // Not generic enough
//
//	grid_[jumpsize_-1][jumpsize_-1] = grid_[jumpsize_-1][jumpsize_];
//
//	for (size_t i = jumpsize_; i < dim_-1; ++i)
//		grid_[i][jumpsize_-1] = grid_[i][jumpsize_]; // Not generic enough
//}
//
//void VolatilityGridBuilder::fillAntidiagonal()
//{
//	grid_[jumpsize_-1][dim_+1-jumpsize_] = grid_[jumpsize_][dim_-jumpsize_];
//
//	for (size_t i = 2*jumpsize_; i <= dim_; i=i+jumpsize_)
//		grid_[i-1][dim_+1-i] = grid_[i-2][dim_-i+2];
//	
//}
//
//void VolatilityGridBuilder::printGrid(std::string fileName)
//{
//	std::ofstream fout;
//	fout.open(fileName);
//	for each (std::vector<double> gridrow in grid_)
//	{
//		for each (double gridval in gridrow)
//			fout << gridval << ";";
//
//		fout << std::endl;
//	}
//	fout << std::endl;
//	fout.close();
//}
//
//
//matrix_& VolatilityGridBuilder::get_grid() {return grid_;}
//
//std::vector<Pillar>& VolatilityGridBuilder::get_pillars() {return pillars_;}
//
//
////-- TEST
//void VolatilityGridBuilder::test_insertPillarsInGrid()
//{
//	size_t dim = 8;
//	size_t jumpsize = 2;
//	std::vector<double> pillarValues(6);
//	for (size_t i = 0; i < pillarValues.size(); ++i)
//		pillarValues[i] = i+1;
//
//	VolatilityGridBuilder volGrid(dim,jumpsize,pillarValues);
//	volGrid.interpolateAllRows();
//	volGrid.interpolateAllColumns();
//	volGrid.bilinearInterpolations();
//	volGrid.borderExtrapolations();
//	volGrid.fillAntidiagonal();
//
//	volGrid.printGrid("E:\\test_grid_calib.csv");
//}
//
//
//
////--------------------------------------------//
////                 PILLAR CLASS               //
////--------------------------------------------//
//
//Pillar::Pillar() : coordinates_(0,0),value_(0.)
//{}
//
//Pillar::Pillar(size_t i, size_t j, double value)
//	: coordinates_(i,j),value_(value)
//{}
//
//std::vector<Pillar> Pillar::findPillarsByRow(size_t row, const std::vector<Pillar>& pillars)
//{
//	std::vector<Pillar> res;
//	for each (Pillar p in pillars)
//	{
//		if (p.coordinates_.first == row)
//			res.push_back(p);
//	}
//	return res;
//}
//
//std::vector<Pillar> Pillar::findPillarsByColumn(size_t col, const std::vector<Pillar>& pillars)
//{
//	std::vector<Pillar> res;
//	for each (Pillar p in pillars)
//	{
//		if (p.coordinates_.second == col)
//			res.push_back(p);
//	}
//	return res;
//}
//
//
//void Pillar::print()
//{
//	std::cout << "Coordinates: (" << coordinates_.first << ";" << coordinates_.second << ")" << std::endl;
//	std::cout << "Value: " << value_ << std::endl;
//}
//
//size_t Pillar::getX() {return coordinates_.first;}
//size_t Pillar::getY() {return coordinates_.second;}
//double Pillar::getValue() {return value_;}
//
////-- TEST
//void Pillar::test_pillar()
//{
//	size_t i = 2;
//	size_t j = 3;
//	double value = 5.2;
//	Pillar pil(i,j,value);
//	pil.print();
//}
//
//void Pillar::test_findPillarsWithSameRow()
//{
//	size_t row = 2;
//	Pillar p1(2,3,1.1);
//	Pillar p2(2,4,2.2);
//	Pillar p3(2,5,3.3);
//
//	Pillar p4(3,8,4.4);
//	Pillar p5(4,7,5.5);
//	Pillar p6(5,8,6.6);
//
//	std::vector<Pillar> pillar_vect;
//	pillar_vect.push_back(p1);
//	pillar_vect.push_back(p2);
//	pillar_vect.push_back(p3);
//	pillar_vect.push_back(p4);
//	pillar_vect.push_back(p5);
//	pillar_vect.push_back(p6);
//
//	std::vector<Pillar> pillarsByRow = Pillar::findPillarsByRow(row,pillar_vect);
//	std::cout << "//---- Pillars with row = " << row << std::endl;
//	for each (Pillar pil in pillarsByRow)
//		std::cout << pil.value_ << " ";
//
//	std::cout << std::endl;
//}
//
//void Pillar::test_findPillarsWithSameColumn()
//{
//	size_t col = 2;
//	Pillar p1(1,8,1.1);
//	Pillar p2(2,2,2.2);
//	Pillar p3(3,3,3.3);
//
//	Pillar p4(4,2,4.4);
//	Pillar p5(5,7,5.5);
//	Pillar p6(6,2,6.6);
//
//	std::vector<Pillar> pillar_vect;
//	pillar_vect.push_back(p1);
//	pillar_vect.push_back(p2);
//	pillar_vect.push_back(p3);
//	pillar_vect.push_back(p4);
//	pillar_vect.push_back(p5);
//	pillar_vect.push_back(p6);
//
//	std::vector<Pillar> pillarsByCol = Pillar::findPillarsByColumn(col,pillar_vect);
//	std::cout << "//---- Pillars with col = " << col << std::endl;
//	for each (Pillar pil in pillarsByCol)
//		std::cout << pil.value_ << " ";
//
//	std::cout << std::endl;
//}