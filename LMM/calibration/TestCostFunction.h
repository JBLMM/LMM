#ifndef LMM_CALIBRATION_CALIBRATION_H
#define LMM_CALIBRATION_CALIBRATION_H
#pragma once

#include <ql/types.hpp>
#include <ql/math/array.hpp>
#include <ql/math/optimization/costfunction.hpp>

typedef std::vector<std::vector<double> > matrix_;

class TestCostFunction : public QuantLib::CostFunction
{
public:

	TestCostFunction(const double& a, const double& b, const double& c);
	
	~TestCostFunction();
	
	QuantLib::Real value(const QuantLib::Array & x) const; 
	
	QuantLib::Disposable<QuantLib::Array> values(const QuantLib::Array& x) const; 

	const double& get_a() const;
	const double& get_b() const;
	const double& get_c() const; 

private:
	const double a_;
	const double b_;
	const double c_;
};

#endif /* LMM_CALIBRATION_CALIBRATION_H */