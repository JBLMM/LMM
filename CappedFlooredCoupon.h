#pragma once

#include <boost/shared_ptr.hpp>

#include<Coupon.h>
#include<Rate.h>

class CappedFlooredCoupon : public Coupon
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
	// getter ... 
	double getPaymentDate()const {return paymentDate_;}
	double getNominal()const{return nominal_;}
	double getPeriod()const{return period_;}
	//bool getIfFloored()const{return ifFloored_;}
	double getFloor()const{return floor_;}
	//bool getIfCapped()const{return ifCapped_;}
	double getCap()const{return cap_;}
	Rate_PTR getRate()const{return rate_;}
	double getMultiFactor()const{return multiFactor_;}
	double getAddFactor()const{return addFactor_;}
	double getValuationDate()const{return valuationDate_;}

	// constructor destructor
	CappedFlooredCoupon(){};
	CappedFlooredCoupon(double paymentDate, double nominal, double period, double floor, double cap, Rate_PTR rate, double multiFactor,double addFactor, double valuationDate);
	CappedFlooredCoupon(const CappedFlooredCoupon& c);
	virtual Coupon_PTR clone()const;
	virtual ~CappedFlooredCoupon(){}
};	
typedef boost::shared_ptr<CappedFlooredCoupon> CappedFlooredCoupon_PTR;
typedef boost::shared_ptr<const CappedFlooredCoupon> CappedFlooredCoupon_CONSTPTR;


