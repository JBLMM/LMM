#pragma once
#include <boost/shared_ptr.hpp>

#include <Rate.h>

class Coupon
{
	double paymentDate_;  //T_i
	double nominal_;
	double period_; // change the name

	//bool   ifFloored_;
	double floor_;

	//bool   ifCapped_;
	double cap_; 

	Rate_PTR rate_;
	double multiFactor_;
	double addFactor_; 
	double valuationDate_; // t_i
public:
	//getter
	virtual double getPaymentDate()const {return paymentDate_;}
	virtual double getNominal()const{return nominal_;}
	virtual double getPeriod()const{return period_;}
	//virtual bool getIfFloored()const{return ifFloored_;}
	virtual double getFloor()const{return floor_;}
	//virtual bool getIfCapped()const{return ifCapped_;}
	virtual double getCap()const{return cap_;}
	virtual Rate_PTR getRate()const{return rate_;}
	virtual double getMultiFactor()const{return multiFactor_;}
	virtual double getAddFactor()const{return addFactor_;}
	virtual double getValuationDate()const{return valuationDate_;}

	//destructor
	virtual ~Coupon(){} // destructor
	virtual boost::shared_ptr<Coupon> clone()const;	
};
typedef boost::shared_ptr<Coupon> Coupon_PTR;
typedef boost::shared_ptr<const Coupon> Coupon_CONSTPTR;

