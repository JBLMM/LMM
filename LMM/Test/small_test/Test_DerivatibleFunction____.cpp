#define BOOST_TEST_MODULE LMM_unit_test_helper
#include <boost/test/included/unit_test.hpp>

#include <boost/bind.hpp>
#include <vector>

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>
#include <LMM/helper/DerivatibleFunction.h>


BOOST_AUTO_TEST_SUITE(lmm_test_derivative)


class Helper_CubicFunction : public DerivatibleFunction // a*x^3 + b*x^2 + c*x + d
{
	double a_;
	double b_;
	double c_;
	double d_;

public:
	Helper_CubicFunction (double a, double b, double c, double d)
		:a_(a), b_(b), c_(c), d_(d){}

	double func(double x) const
	{
		return a_*x*x*x + b_*x*x + c_*x + d_;
	}

	double firstDerivative(double x) const
	{
		return 3*a_*x*x + 2*b_*x + c_;
	}

	double secondDerivative(double x) const 
	{
		return 6*a_*x + 2*b_;
	}
};

class Helper_ExpFunction : public DerivatibleFunction // exp(a*x*x + b*x + c)
{
	double a_;
	double b_;
	double c_;

public:
	Helper_ExpFunction(double a, double b, double c): a_(a), b_(b), c_(c){}

	double func(double x) const 
	{
		return std::exp(a_*x*x + b_*x + c_);
	}

	double firstDerivative(double x) const
	{
		return (2*a_*x + b_)*func(x);
	}

	double secondDerivative(double x) const
	{
		return (2*a_*x + b_)*firstDerivative(x) + (2*a_)*func(x);
	}
};

BOOST_AUTO_TEST_CASE(test_DerivatibleFunction) // OK Test passed.
{
	CombinedDerivatibleFunction_PTR cubicFuncHelper (new Helper_CubicFunction(0.5, 0.85, 3.14, 0.17));
	CombinedDerivatibleFunction_PTR expFuncHelper   (new Helper_ExpFunction(1.1, 1.3, 0.75));

	CombinedDerivatibleFunction_PTR addFunc = CombinedDerivatibleFunction::add(cubicFuncHelper,expFuncHelper); 
	CombinedDerivatibleFunction_PTR subFunc = CombinedDerivatibleFunction::sub(cubicFuncHelper,expFuncHelper); 
	CombinedDerivatibleFunction_PTR mulFunc = CombinedDerivatibleFunction::mul(cubicFuncHelper,expFuncHelper); 
	CombinedDerivatibleFunction_PTR divFunc = CombinedDerivatibleFunction::div(cubicFuncHelper,expFuncHelper); 

	double xBegin = 0.0;
	double xEnd   = 1.0;
	size_t xSize  = 10;
	double xStep  = (xEnd-xBegin)/(xSize-1);
	std::vector<double> xGrid(xSize);
	for(size_t i=0; i<xGrid.size(); ++i)
	{
		xGrid[i] = xBegin + i*xStep;
	}

	std::vector<double> yAdd(xSize);
	std::vector<double> yAdd1st(xSize);
	std::vector<double> yAdd2nd(xSize);
	std::vector<double> yAddCompare(xSize);
	std::vector<double> yAdd1stCompare(xSize);
	std::vector<double> yAdd2ndCompare(xSize);

	std::vector<double> ySub(xSize);
	std::vector<double> ySub1st(xSize);
	std::vector<double> ySub2nd(xSize);
	std::vector<double> ySubCompare(xSize);
	std::vector<double> ySub1stCompare(xSize);
	std::vector<double> ySub2ndCompare(xSize);

	std::vector<double> yMul(xSize);
	std::vector<double> yMul1st(xSize);
	std::vector<double> yMul2nd(xSize);
	std::vector<double> yMulCompare(xSize);
	std::vector<double> yMul1stCompare(xSize);
	std::vector<double> yMul2ndCompare(xSize);

	std::vector<double> yDiv(xSize);
	std::vector<double> yDiv1st(xSize);
	std::vector<double> yDiv2nd(xSize);
	std::vector<double> yDivCompare(xSize);
	std::vector<double> yDiv1stCompare(xSize);
	std::vector<double> yDiv2ndCompare(xSize);

	double bump   = 1.0e-4;
	for(size_t i=0; i<xGrid.size(); ++i)
	{
		double x     = xGrid[i];
		double xUp   = x+bump;
		double xDown = x-bump;

		//double li = cubicFuncHelperfunc(x);
		//double li2 = derivatibleCubic.func_(x);

		//! add
		yAdd[i]        = addFunc->func(x);
		yAddCompare[i] = cubicFuncHelper->func(x) + expFuncHelper->func(x);

		yAdd1st[i] = addFunc->firstDerivative(x);
		yAdd1stCompare[i] = ((cubicFuncHelper->func(xUp)+expFuncHelper->func(xUp)) - (cubicFuncHelper->func(xDown)+expFuncHelper->func(xDown))) / (2* bump);

		yAdd2nd[i] = addFunc->secondDerivative(x);
		yAdd2ndCompare[i] = (   (cubicFuncHelper->func(xUp)+expFuncHelper->func(xUp)) 
			                    -2*(cubicFuncHelper->func(x)+expFuncHelper->func(x))
							    +(cubicFuncHelper->func(xDown)+expFuncHelper->func(xDown)) 
							) / (bump * bump);

		//! sub
		ySub[i]    = subFunc->func(x);
		ySubCompare[i] = cubicFuncHelper->func(x) - expFuncHelper->func(x);
		ySub1st[i] = subFunc->firstDerivative(x);
		ySub1stCompare[i] = ((cubicFuncHelper->func(xUp)-expFuncHelper->func(xUp)) - (cubicFuncHelper->func(xDown)-expFuncHelper->func(xDown))) / (2*bump);
		ySub2nd[i] = subFunc->secondDerivative(x);
		ySub2ndCompare[i] = (   (cubicFuncHelper->func(xUp)-expFuncHelper->func(xUp)) 
			-2*(cubicFuncHelper->func(x)-expFuncHelper->func(x))
			+(cubicFuncHelper->func(xDown)-expFuncHelper->func(xDown)) 
			) / (bump * bump);

		//! mul
		yMul[i]    = mulFunc->func(x);
		yMulCompare[i] = cubicFuncHelper->func(x) * expFuncHelper->func(x);
		yMul1st[i] = mulFunc->firstDerivative(x);
		yMul1stCompare[i] = ((cubicFuncHelper->func(xUp)*expFuncHelper->func(xUp)) - (cubicFuncHelper->func(xDown)*expFuncHelper->func(xDown))) / (2*bump);
		yMul2nd[i] = mulFunc->secondDerivative(x);
		yMul2ndCompare[i] = (   (cubicFuncHelper->func(xUp)*expFuncHelper->func(xUp)) 
			-2*(cubicFuncHelper->func(x)*expFuncHelper->func(x))
			+(cubicFuncHelper->func(xDown)*expFuncHelper->func(xDown)) 
			) / (bump * bump);

		//!div
		yDiv[i]    = divFunc->func(x);
		yDivCompare[i] = cubicFuncHelper->func(x) / expFuncHelper->func(x);
		yDiv1st[i] = divFunc->firstDerivative(x);
		yDiv1stCompare[i] = ((cubicFuncHelper->func(xUp)/expFuncHelper->func(xUp)) - (cubicFuncHelper->func(xDown)/expFuncHelper->func(xDown))) / (2*bump);
		yDiv2nd[i] = divFunc->secondDerivative(x);
		yDiv2ndCompare[i] = (   (cubicFuncHelper->func(xUp)/expFuncHelper->func(xUp)) 
			-2*(cubicFuncHelper->func(x)/expFuncHelper->func(x))
			+(cubicFuncHelper->func(xDown)/expFuncHelper->func(xDown)) 
			) / (bump * bump);
	}

	std::string fileName = "test_DerivatibleFunction.csv";
	std::string path = LMM::get_output_path() + fileName;
	std::vector<PrintElement_PTR> elements_print;



	//seems a problem of shared ptr polymorphisms ... 
	PrintElement_PTR x_print    = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("xGrid", xGrid));

	PrintElement_PTR yAdd_print    = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yAdd", yAdd));
	PrintElement_PTR yAddCompare_print    = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yAddCompare", yAddCompare));
	PrintElement_PTR yAdd1st_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yAdd1st", yAdd1st));
	PrintElement_PTR yAdd1stCompare_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yAdd1stCompare", yAdd1stCompare));
	PrintElement_PTR yAdd2nd_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yAdd2nd", yAdd2nd));
	PrintElement_PTR yAdd2ndCompare_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yAdd2ndCompare", yAdd2ndCompare));

	PrintElement_PTR ySub_print    = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("ySub", ySub));
	PrintElement_PTR ySubCompare_print    = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("ySubCompare", ySubCompare));
	PrintElement_PTR ySub1st_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("ySub1st", ySub1st));
	PrintElement_PTR ySub1stCompare_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("ySub1stCompare", ySub1stCompare));
	PrintElement_PTR ySub2nd_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("ySub2nd", ySub2nd));
	PrintElement_PTR ySub2ndCompare_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("ySub2ndCompare", ySub2ndCompare));

	PrintElement_PTR yMul_print    = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yMul", yMul));
	PrintElement_PTR yMulCompare_print    = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yMulCompare", yMulCompare));
	PrintElement_PTR yMul1st_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yMul1st", yMul1st));
	PrintElement_PTR yMul1stCompare_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yMul1stCompare", yMul1stCompare));
	PrintElement_PTR yMul2nd_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yMul2nd", yMul2nd));
	PrintElement_PTR yMul2ndCompare_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yMul2ndCompare", yMul2ndCompare));

	PrintElement_PTR yDiv_print    = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yDiv", yDiv));
	PrintElement_PTR yDivCompare_print    = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yDivCompare", yDivCompare));
	PrintElement_PTR yDiv1st_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yDiv1st", yDiv1st));
	PrintElement_PTR yDiv1stCompare_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yDiv1stCompare", yDiv1stCompare));
	PrintElement_PTR yDiv2nd_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yDiv2nd", yDiv2nd));
	PrintElement_PTR yDiv2ndCompare_print = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("yDiv2ndCompare", yDiv2ndCompare));


	elements_print.push_back(yAdd_print);
	elements_print.push_back(yAddCompare_print);
	elements_print.push_back(yAdd1st_print);
	elements_print.push_back(yAdd1stCompare_print);
	elements_print.push_back(yAdd2nd_print);
	elements_print.push_back(yAdd2ndCompare_print);

	elements_print.push_back(ySub_print);
	elements_print.push_back(ySubCompare_print);
	elements_print.push_back(ySub1st_print);
	elements_print.push_back(ySub1stCompare_print);
	elements_print.push_back(ySub2nd_print);
	elements_print.push_back(ySub2ndCompare_print);

	elements_print.push_back(yMul_print);
	elements_print.push_back(yMulCompare_print);
	elements_print.push_back(yMul1st_print);
	elements_print.push_back(yMul1stCompare_print);
	elements_print.push_back(yMul2nd_print);
	elements_print.push_back(yMul2ndCompare_print);

	elements_print.push_back(yDiv_print);
	elements_print.push_back(yDivCompare_print);
	elements_print.push_back(yDiv1st_print);
	elements_print.push_back(yDiv1stCompare_print);
	elements_print.push_back(yDiv2nd_print);
	elements_print.push_back(yDiv2ndCompare_print);

	Printer printer(path, elements_print);
	printer.print();

	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
