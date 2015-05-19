#pragma once

#include<CappedFlooredCoupon.h>

#include <boost/shared_ptr.hpp>



class RatchetCoupon : public CappedFlooredCoupon
{
	double leverage_;
	std::string dependentCoupon_;  // lastcoupon 

public:

	// getter 
	double getLeverage()const{return leverage_;}
	std::string getDependentCoupon()const{return dependentCoupon_;}

	// Constructor 
	RatchetCoupon(	LMM::Index paymentDate,
		                double nominal,
						double period, 
						bool   ifFloored,
						double floorStrike,
						bool   ifCapped, 
						double capStrike, 
						Rate_CONSTPTR rate,
						double multiFactor,
						double addFactor, 
						LMM::Index valuationDateIndex,
						double leverage,
						std::string dependentCoupon);  
	// destructor 
	virtual ~RatchetCoupon(){}
};
typedef boost::shared_ptr<RatchetCoupon> RatchetCoupon_PTR;
typedef boost::shared_ptr<const RatchetCoupon> RatchetCoupon_CONSTPTR;


