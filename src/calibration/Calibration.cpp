#include <LMM/calibration/Calibration.h>

#include <iostream>
#include <fstream>
#include <cstring>

#include <ql/math/optimization/levenbergmarquardt.hpp>
#include <ql/math/optimization/conjugategradient.hpp>
#include <ql/math/optimization/bfgs.hpp> 


//using namespace std;

Calibration::Calibration()
{}

Array Calibration::Calibrate(ofstream& outputFile, CalibrationVolCostFunction& func, const Array& startValues)
{

	std::cout << "Start calibration............" << std::endl << std::endl;

	//-- Set end criteria 
	Size minStatIterations = 100   ;
	Size maxIterations     = 10000 ;

	Real rootEpsilon         = 1e-12 ;//=1e-8;
	Real functionEpsilon     = 1e-12 ;//=1e-9;
	Real gradientNormEpsilon = 1e-12 ;//=1e-5;

	EndCriteria myEndCrit (maxIterations, minStatIterations,	rootEpsilon,functionEpsilon,gradientNormEpsilon);

	NoConstraint constraint;
	
	Problem myProb(func,constraint,startValues);

	std::cout << "Begin solving............ " << std::endl << std::endl;

	//-- Minimization 
	//ConjugateGradient solver;
	//LevenbergMarquardt solver;
	BFGS solver;
	EndCriteria::Type solvedCrit = solver.minimize(myProb,myEndCrit);

	std::cout << "End of solving" << std::endl << std::endl;
	
	Array calibrationResuls = myProb.currentValue();

	saveCalibrationResults(outputFile,calibrationResuls);

	std::cout << "End of calibration!" << std::endl << std::endl;

	return calibrationResuls;
}

//-- save calibration results in an Excel file
//-- Assume file is already opened 
void Calibration::saveCalibrationResults(ofstream& outputFile, const Array& calibrationResults)
{
	outputFile << "Volatility matrix after calibration" << std::endl << std::endl;
	outputFile << "\t" << "\t";

	size_t cpt = 0;
	for (size_t i = 0; i < calibrationResults.size(); ++i)
	{
		if (cpt == calibrationResults.size())
			break;

		for (size_t j = 0; j <= i; ++j)
		{
			outputFile << calibrationResults[cpt] << "\t";
			cpt++;
		}
		outputFile<< "\n" << "\t" << "\t";
	}
	
	outputFile << std::endl;
}

/*
void Calibration::test_Minimizer(TestCostFunction& func)
{
	//-- Set end criteria 
	Size maxIterations =1000;
	Size minStatIterations =500;
	Real rootEpsilon =1e-8;
	Real functionEpsilon =1e-9;
	Real gradientNormEpsilon =1e-5;

	EndCriteria myEndCrit (maxIterations,minStatIterations,
		rootEpsilon,functionEpsilon,gradientNormEpsilon);

	Array startValues(1);
	startValues[0] = 10.;

	NoConstraint constraint;
	Problem myProb(func,constraint,startValues);

	std::cout << "Begin solving............ " << std::endl << std::endl;

	//-- Minimization 
	ConjugateGradient solver;
	EndCriteria::Type solvedCrit = solver.minimize(myProb,myEndCrit);

	std::cout << "End of solving" << std::endl << std::endl;

	std::cout << "Minimization result: " << myProb.currentValue() << std::endl;

	double b = func.get_b();
	double c = func.get_c();
	double expected_min = -b/(2*c);
	std::cout << "Expected result: " << expected_min << std::endl << std::endl;

}*/