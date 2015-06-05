#pragma once
#include <boost/shared_ptr.hpp>

#include <Rate.h>
#include <LMM/helper/Name.h>

class Coupon
{
protected:

	LMM::Index paymentIndex_;  //T_i


public:

	//getter
	LMM::Index getPaymentIndex()const{return paymentIndex_;}


	//constructor
	Coupon(const LMM::Index paymentIndex);

	// destructor
	virtual ~Coupon(){} 

	//clone
	virtual boost::shared_ptr<Coupon> clone()const;	 
};
typedef boost::shared_ptr<Coupon> Coupon_PTR;
typedef boost::shared_ptr<const Coupon> Coupon_CONSTPTR;

