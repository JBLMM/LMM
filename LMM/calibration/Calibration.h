#pragma once

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <ql/math/optimization/endcriteria.hpp>
#include <ql/math/optimization/constraint.hpp>
#include <ql/math/optimization/problem.hpp>

//#include "TestCostFunction.h"
#include <LMM/calibration/CalibrationVolCostFunction.h>


class Calibration 
{	
public:
	
	Calibration();
	
	Array Calibrate(ofstream& outputFile, CalibrationVolCostFunction& func, const Array& startValues);

	void saveCalibrationResults(ofstream& outputFile, const Array& calibrationResults);


	//-- Tests --//
	//void test_Minimizer(TestCostFunction&);
};

