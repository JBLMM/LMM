#pragma once

#include <JBLMM/Element/CappedFlooredCoupon.h>

#include <boost/shared_ptr.hpp>



class RatchetCoupon : public CappedFlooredCoupon
{
	double leverage_;
	std::string dependentCoupon_;  // lastcoupon 

public:

	// getter 
	double getLeverage()const{return leverage_;}
	std::string getDependentCoupon()const{return dependentCoupon_;}
	// Constructor,destructor
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
	virtual ~RatchetCoupon(){}
};
typedef boost::shared_ptr<RatchetCoupon> RatchetCoupon_PTR;
typedef boost::shared_ptr<const RatchetCoupon> RatchetCoupon_CONSTPTR;


