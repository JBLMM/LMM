#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/shared_ptr.hpp>

#include <JBLMM/Element/Coupon.h>



class CouponLeg
{
	std::vector<Coupon_CONSTPTR>  leg_;	// couponPaymentDate in [indexStart, indexEnd[
public:
	//getter and setter		
	const std::vector<Coupon_CONSTPTR>& getLeg()const{return leg_;}
	void addToLeg(Coupon_CONSTPTR coupon);
	//constructor, destructor ... 
	CouponLeg(const std::vector<Coupon_CONSTPTR>&  leg); 
	virtual ~CouponLeg(){}
	//extrait a subCouponLeg
	boost::shared_ptr<const CouponLeg> getSubCouponLeg(size_t indexStart, size_t indexEnd) const;  
};
typedef boost::shared_ptr<CouponLeg> CouponLeg_PTR;
typedef boost::shared_ptr<const CouponLeg> CouponLeg_CONSTPTR;