#pragma once

#include <boost/shared_ptr.hpp>

#include<JBLMM/Element/Coupon.h>
#include<JBLMM/Element/Rate.h>

class CappedFlooredCoupon : public Coupon
{
	double period_; // change the name
	double nominal_;

	bool   ifFloored_;
	double floorStrike_;

	bool   ifCapped_;
	double capStrike_; 

	Rate_CONSTPTR rate_;
	double multiFactor_;
	double addFactor_; 
	double valuationDate_; // t_i

public:
	// getter ... 
	double			getPeriod()			const{return period_;}
	double			getNominal()		const{return nominal_;}
	bool			getIfFloored()		const{return ifFloored_;}
	double			getFloorStrike()	const{return floorStrike_;}
	bool			getIfCapped()		const{return ifCapped_;}
	double			getCapStrike()		const{return capStrike_;}
	Rate_CONSTPTR	getRate()			const{return rate_;}
	double			getMultiFactor()	const{return multiFactor_;}
	double			getAddFactor()		const{return addFactor_;}
	double			getValuationDate()	const{return valuationDate_;}

	// constructor, constructor copie, destructor
	CappedFlooredCoupon(LMM::Index		paymentDate,
		                double			nominal,
						double			period, 
						bool			ifFloored,
						double			floorStrike,
						bool			ifCapped, 
						double			capStrike, 
						Rate_CONSTPTR	rate,
						double			multiFactor,
						double			addFactor, 
						LMM::Index		valuationDateIndex);  

	CappedFlooredCoupon(const CappedFlooredCoupon& c);
	virtual ~CappedFlooredCoupon(){}
	//clone
	virtual Coupon_PTR clone()const;
};	
typedef boost::shared_ptr<CappedFlooredCoupon> CappedFlooredCoupon_PTR;
typedef boost::shared_ptr<const CappedFlooredCoupon> CappedFlooredCoupon_CONSTPTR;


