#pragma once
#include <boost/shared_ptr.hpp>

class Coupon
{
	double paymentDate_;  //T_i
public:
	virtual ~Coupon(){} // destructor
	virtual boost::shared_ptr<Coupon> clone()const{return boost::shared_ptr<Coupon>(new Coupon(*this));}
	double getPaymentDate()const{return paymentDate_;}		
};
typedef boost::shared_ptr<Coupon> Coupon_PTR;
typedef boost::shared_ptr<const Coupon> Coupon_CONSTPTR;

