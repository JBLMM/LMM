//#pragma once
//
//#include <fstream>
//#include "InstantaneousVolatility.h"
//#include "Correlation.h"
//
//typedef std::vector<std::vector<double>> matrix_;
//
////-- TODO : Make it inherit from a generic vol class
//class PiecewiseConstantVolatility: public InstantaneousVolatility
//{
//private:
//	matrix_ h_; //-- Contains all values taken by volatilities, depending on the time period 
//	std::vector<double> tenorDates_; //-- Date grid 
//	
//public:
//
//	PiecewiseConstantVolatility();
//
//	PiecewiseConstantVolatility(const matrix_& h,
//								const std::vector<double>& tenorDates,
//		                        size_t reducedRank, 
//					            size_t fullRank);
//
//	PiecewiseConstantVolatility(const PiecewiseConstantVolatility&);
//
//	
//	double covIntegral(size_t i, size_t j, Time Ti, Time Tj, Time S, Time T);
//	double varIntegral(size_t i, Time Ti, Time S, Time T);
//
//	//-- For calibration
//	double covIntegral(const Array& x, size_t i, size_t j, Time Ti, Time Tj, Time S, Time T);
//	double varIntegral(const Array& x, size_t i, Time Ti, Time S, Time T);
//
//	//-- Helpers
//	size_t findIndexAfterGivenDate(double date); // For a given date, find the tenor date right after it
//	size_t findIndexBeforeGivenDate(double date); 
//	void printVolMatrix();
//	void printTenorDates();
//	Array map_MatrixToArray(const matrix_& h);
//	matrix_ map_ArrayToMatrix(const Array& x);
//	void saveMatrix(ofstream& outputFile); //-- Save matrix h_ in a .xls file
//
//	//-- Getters/Setters
//	const std::vector<double>& get_tenorDates();
//	void set_tenorDates(const std::vector<double>& tenorDates);
//
//	//-- Tests
//	void test_findDate(); 
//	void test_computeCovariance();
//	void test_computeVariance();
//	void test_computeAllCovariances(size_t N_libor);
//};
