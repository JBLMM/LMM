#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/shared_ptr.hpp>	

#include<LMM/helper/TenorType.h>
#include<LMM/helper/LMMTenorStructure.h>

#include <JBLMM/Element/CouponLeg.h>


//#include <LMM/helper/Name.h>
//#include <LMM/helper/TenorType.h>
//#include <LMM/helper/LMMTenorStructure.h>


/*! \class VanillaSwap manage leg's INDICES in the simulation structures see <LMM\lmmModel\LMMTenorStructure.h>
 * description 
*/

class GeneticSwap
{
	CouponLeg_CONSTPTR leg1_;
	CouponLeg_CONSTPTR leg2_;
public:
	//gettor
	CouponLeg_CONSTPTR getLeg1()const{return leg1_;}
	CouponLeg_CONSTPTR getLeg2()const{return leg2_;}
	// constructor, destructor
	GeneticSwap(CouponLeg_CONSTPTR leg1, CouponLeg_CONSTPTR leg2);
	virtual ~GeneticSwap(){}
	//get subGeneticSwap
	boost::shared_ptr<GeneticSwap> getSubGeneticSwap(const size_t indexStart, const size_t indexEnd) const;
};
typedef boost::shared_ptr<GeneticSwap> GeneticSwap_PTR;
typedef boost::shared_ptr<const GeneticSwap> GeneticSwap_CONSTPTR;





