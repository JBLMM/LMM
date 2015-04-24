#pragma once

#include <boost/function.hpp>
#include <boost/shared_array.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>

//! only operator available: "+", "-", "*", "/"
class combineoperator
{
	std::string operatorname_;
public:
	combineoperator(const std::string& operatorname):operatorname_(operatorname){}
	std::string get_operatorname() const {return operatorname_;}
};


//! yy todo: need to implement copy constructor, assignment operator ... 
class combinedderivatiblefunction //: public derivatiblefunction
{
	boost::shared_ptr<combinedderivatiblefunction> derivatiblefunction1_;
	boost::shared_ptr<combinedderivatiblefunction> derivatiblefunction2_;
	combineoperator combineoperator_;
public:
	virtual double func(double x) const
	{
		if(combineoperator_.get_operatorname() == "+")
			return derivatiblefunction1_->func(x) + derivatiblefunction2_->func(x);
		if(combineoperator_.get_operatorname() == "-")
			return derivatiblefunction1_->func(x) - derivatiblefunction2_->func(x);
		if(combineoperator_.get_operatorname() == "*")
			return derivatiblefunction1_->func(x) * derivatiblefunction2_->func(x);
		if(combineoperator_.get_operatorname() == "/")
			return derivatiblefunction1_->func(x) / derivatiblefunction2_->func(x);
		throw("error: operator not defined!");
	}

	virtual double firstderivative(double x) const
	{
		if(combineoperator_.get_operatorname() == "+")
		{
			return derivatiblefunction1_->firstderivative(x) + derivatiblefunction2_->firstderivative(x);
		}
		if(combineoperator_.get_operatorname() == "-")
		{
			return derivatiblefunction1_->firstderivative(x) - derivatiblefunction2_->firstderivative(x);
		}
		if(combineoperator_.get_operatorname() == "*")
		{
			return derivatiblefunction1_->firstderivative(x) * derivatiblefunction2_->func(x) 
					+  derivatiblefunction1_->func(x) * derivatiblefunction2_->firstderivative(x);
		}
		if(combineoperator_.get_operatorname() == "/")
		{
			double numerator = derivatiblefunction1_->firstderivative(x) * derivatiblefunction2_->func(x) 
				-  derivatiblefunction1_->func(x) * derivatiblefunction2_->firstderivative(x);
			double denominator = derivatiblefunction2_->func(x)*derivatiblefunction2_->func(x);  
			return numerator/denominator;
		}
		throw("error: operator not defined!");
	}

	virtual double secondderivative(double x) const
	{
		if(combineoperator_.get_operatorname() == "+")
		{
			return derivatiblefunction1_->secondderivative(x) + derivatiblefunction2_->secondderivative(x);
		}
		if(combineoperator_.get_operatorname() == "-")
		{
			return derivatiblefunction1_->secondderivative(x) - derivatiblefunction2_->secondderivative(x);
		}
		if(combineoperator_.get_operatorname() == "*")
		{
			return derivatiblefunction1_->secondderivative(x) * derivatiblefunction2_->func(x)
			    + 2 * derivatiblefunction1_->firstderivative(x) * derivatiblefunction2_->firstderivative(x)
				+ derivatiblefunction1_->func(x) * derivatiblefunction2_->secondderivative(x);
		}
		if(combineoperator_.get_operatorname() == "/")
		{
			double f       =   derivatiblefunction1_->firstderivative(x) * derivatiblefunction2_->func(x) 
							 - derivatiblefunction1_->func(x) * derivatiblefunction2_->firstderivative(x);

			double f_prime =   derivatiblefunction1_->secondderivative(x)*derivatiblefunction2_->func(x) 
				             - derivatiblefunction1_->func(x)*derivatiblefunction2_->secondderivative(x); 

			double g       =  derivatiblefunction2_->func(x) * derivatiblefunction2_->func(x);
			double g_prime =  2 * derivatiblefunction2_->func(x) * derivatiblefunction2_->firstderivative(x);

			double numerator   = f_prime*g - f*g_prime;
			double denominator = g*g;
			return numerator/denominator;
		}
		throw("error: operator not defined!");
	}

	combinedderivatiblefunction(const boost::shared_ptr<combinedderivatiblefunction>& deriablefunc1, 
								const boost::shared_ptr<combinedderivatiblefunction>& deriablefunc2,
								const combineoperator& combineoperator)
								:derivatiblefunction1_(deriablefunc1),derivatiblefunction2_(deriablefunc2), combineoperator_(combineoperator)
	{}


static const boost::shared_ptr<combinedderivatiblefunction> add ( const boost::shared_ptr<combinedderivatiblefunction>& combinedderivablefunc1,
																  const boost::shared_ptr<combinedderivatiblefunction>& combinedderivablefunc2) 
{
	const boost::shared_ptr<combinedderivatiblefunction> ptr (new combinedderivatiblefunction(combinedderivablefunc1, 
		combinedderivablefunc2,
		combineoperator("+")));
	return ptr;
}

static const boost::shared_ptr<combinedderivatiblefunction> sub ( const boost::shared_ptr<combinedderivatiblefunction>& combinedderivablefunc1,
																  const boost::shared_ptr<combinedderivatiblefunction>& combinedderivablefunc2) 
{
	const boost::shared_ptr<combinedderivatiblefunction> ptr (new combinedderivatiblefunction(combinedderivablefunc1, 
		combinedderivablefunc2,
		combineoperator("-")));
	return ptr;
}


static const boost::shared_ptr<combinedderivatiblefunction> mul ( const boost::shared_ptr<combinedderivatiblefunction>& combinedderivablefunc1,
																  const boost::shared_ptr<combinedderivatiblefunction>& combinedderivablefunc2) 
{
	const boost::shared_ptr<combinedderivatiblefunction> ptr (new combinedderivatiblefunction(combinedderivablefunc1, 
		combinedderivablefunc2,
		combineoperator("*")));
	return ptr;
}


static const boost::shared_ptr<combinedderivatiblefunction> div ( const boost::shared_ptr<combinedderivatiblefunction>& combinedderivablefunc1,
																 const boost::shared_ptr<combinedderivatiblefunction>& combinedderivablefunc2) 
{
	const boost::shared_ptr<combinedderivatiblefunction> ptr (new combinedderivatiblefunction(combinedderivablefunc1, 
		combinedderivablefunc2,
		combineoperator("/")));
	return ptr;
}

//combinedderivatiblefunction operator+ (const combinedderivatiblefunction& combinedderivablefunc) 
//{
//	return combinedderivatiblefunction(this, 
//		                               &combinedderivablefunc,
//									   combineoperator("+"));
//}
};
typedef boost::shared_ptr<combinedderivatiblefunction> combinedderivatiblefunction_ptr;

class derivatiblefunction : public combinedderivatiblefunction
{
public:

	derivatiblefunction():combinedderivatiblefunction(boost::shared_ptr<combinedderivatiblefunction>(), 
													  boost::shared_ptr<combinedderivatiblefunction>(),
													  combineoperator("non")){} // so you have to implement the func, 1stdev, 2nddev
};
typedef boost::shared_ptr<combinedderivatiblefunction> derivatiblefunction_ptr;

