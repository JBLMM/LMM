#pragma once

#include <boost/shared_ptr.hpp>

#include<Coupon.h>
#include<Rate.h>

class CappedFlooredCoupon : public Coupon
{
	double paymentDate_;  //T_i
	double nominal_;
	double period_; // change the name

	bool   ifFloored_;
	double floor_;

	bool   ifCapped_;
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
	bool getIfFloored()const{return ifFloored_;}
	double getFloor()const{return floor_;}
	bool getIfCapped()const{return ifCapped_;}
	double getCap()const{return cap_;}
	Rate_PTR getRate()const{return rate_;}
	double getMultiFactor()const{return multiFactor_;}
	double getAddFactor()const{return addFactor_;}
	double getValuationDate()const{return valuationDate_;}

	// constructor destructor
	CappedFlooredCoupon(){};
	CappedFlooredCoupon(double paymentDate, double nominal, double period, bool ifFloored, bool floor, bool ifCapped, double cap, Rate_PTR rate, double multiFactor,double addFactor, double valuationDate)
	{
		paymentDate_=paymentDate;
		nominal_=nominal;
		period_=period;
		ifFloored_=ifFloored;
		floor_=floor;
		ifCapped_=ifCapped;
		cap_=cap;
		rate_=rate;
		multiFactor_=multiFactor;
		addFactor_=addFactor;
		valuationDate_=valuationDate;
	}
	CappedFlooredCoupon(const CappedFlooredCoupon& c)
	{		
		paymentDate_=c.getPaymentDate();
		nominal_=c.getNominal();
		period_=c.getPeriod();
		ifFloored_=c.getIfFloored();
		floor_=c.getFloor();
		ifCapped_=c.getIfCapped();
		cap_=c.getCap();
		rate_=(c.getRate()->clone());
		multiFactor_=c.getMultiFactor();
		addFactor_=c.getAddFactor();
		valuationDate_=c.getValuationDate();
	}
	virtual Coupon_PTR clone()const{return Coupon_PTR(new CappedFlooredCoupon(*this));}
	virtual ~CappedFlooredCoupon(){}
};
typedef boost::shared_ptr<CappedFlooredCoupon> CappedFlooredCoupon_PTR;
typedef boost::shared_ptr<const CappedFlooredCoupon> CappedFlooredCoupon_CONSTPTR;


