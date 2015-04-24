//#ifndef LMM_PRICER_STATICREPLICATIONCMS_H
//#define LMM_PRICER_STATICREPLICATIONCMS_H
//#pragma once
//
//#include <cmath>
//#include <cassert>
//
//#include <LMM/helper/DerivatibleFunction.h>
//#include <LMM/helper/StaticReplicationFormula.h>
//#include <LMM/instrument/VanillaSwap.h>
//#include <LMM/helper/LMMTenorStructure.h>
//#include <LMM/pricer/LmmVanillaSwapPricer.h>
//
//
//class StaticReplicationCMSPricer
//{
//
//};
//
////! only one floating cashFlow: at T_p \in [T_0,T_1], pays swap Rate S_{T_0,T_N}(T_0)
////  we evaluate its value at time 0 by static replication:
//
//class StaticReplicationCMSwapletPricer : public LmmVanillaSwapPricer
//{
//	double maturityTime_; // payment Date in year
//	LMMTenorStructure pLMMTenorStructure_;
//	boost::function<double (double)> initZCCurve_;
//	double volHoLee_;
//
//	StaticReplicationCMSwapletPricer( double maturityTime, // payment Date in year
//		                              const  LMMTenorStructure& lmmTenorStructure,
//									  const  boost::function<double (double)>& initZCCurve,
//									  double volHoLee)
//		:maturityTime_(maturityTime),
//		 pLMMTenorStructure_(lmmTenorStructure),
//		 initZCCurve_(initZCCurve),
//		 volHoLee_(volHoLee)
//	{}
//
//	class ZCProjectionOnG : public DerivatibleFunction // ZC(t1,t2) = function(G)
//	{
//		double t1_;
//		double t2_;
//		const StaticReplicationCMSwapletPricer& outer_;
//
//		ZCProjectionOnG(const StaticReplicationCMSwapletPricer& outer, double t1, double t2)
//			: t1_(t1), t2_(t2), outer_(outer) {}
//
//		double a_ZCProjectionOnG(double G)
//		{
//			return outer_.volHoLee_*(t2_-t1_)*std::sqrt(t1_);
//		}
//	public:
//		double func(double x) const // ZC projection on G
//		{
//			double fwdZC = outer_.initZCCurve_(t2_)/outer_.initZCCurve_(t1_);
//			double a  = a_ZCProjectionOnG(G);
//			return fwdZC*std::exp(a*g - 0.5*a*a);
//		}
//		double firstDerivative(double x) const
//		{
//			double a = a_ZCProjectionOnG( G);
//			return a*ZCProjectionOnG(G);
//		}
//		double secondDerivative(double x) const
//		{
//			double a = a_ZCProjectionOnG(G);
//			return a*a*ZCProjectionOnG(G);
//		}
//	};
//
//	////! P(t1,t2) projection onto G in HoLee model:
//	////! t1 = T0, Swap's start date
//	////! t2 may variate depending on the ZC
//	//// Z = function(G)
//	//double a_ZCProjectionOnG(double t1, double t2, double G)
//	//{
//	//	return volHoLee_*(t2-t1)*std::sqrt(t1);
//	//}
//
//	//double ZCProjectionOnG(double t1, double t2, double G)
//	//{
//	//	double fwdZC = initZCCurve_(t2)/initZCCurve_(t1);
//	//	double a  = a_ZCProjectionOnG(t1, t2, G);
//	//	return fwdZC*std::exp(a*g - 0.5*a*a);
//	//}
//
//	//double ZCProjectionOnG_firstDerivative(double t1, double t2, double G)
//	//{
//	//	double a = a_ZCProjectionOnG(t1, t2, G);
//	//	return a*ZCProjectionOnG(t1,t2,G);
//	//}
//
//	//double ZCProjectionOnG_secondDerivative(double t1, double t2, double G)
//	//{
//	//	double a = a_ZCProjectionOnG(t1, t2, G);
//	//	return a*a*ZCProjectionOnG(t1,t2,G);
//	//}
//
//	//! A = function(G)
//	double SwapAnnuityProjectionOnG(const VanillaSwap& vanillaSwap, double G)
//	{
//		//YY TODO: implement == operator for enum and then active this test!
//		//assert(vanillaSwap.get_simulationTenorType() =  pLMMTenorStructure_.get_tenorType());
//
//		size_t indexStartSwap = vanillaSwap.get_indexStart();
//		double t1 = pLMMTenorStructure_.get_tenorDate(indexStartSwap);
//
//		double annuity = 0.0;
//		const std::vector<LMM::Index> fixedLegPaymentIndexSchedule = vanillaSwap.get_fixedLegPaymentIndexSchedule();
//		for(size_t indexItr = 0; indexItr < fixedLegPaymentIndexSchedule.size(); ++indexItr)
//		{
//			double t2 = fixedLegPaymentIndexSchedule[indexItr];
//			double deltaT = deltaTFloatingLeg_[indexItr];
//			annuity  += ZCProjectionOnG(t1,t2,G);
//		}
//		return annuity;
//	}
//
//	//! S=f(G)
//	double SwapRateProjectionOnG(const VanillaSwap& vanillaSwap, double G)
//	{
//		//YY TODO: implement == operator for enum and then active this test!
//		//assert(vanillaSwap.get_simulationTenorType() =  pLMMTenorStructure_.get_tenorType());
//
//		size_t indexStartSwap = vanillaSwap.get_indexStart();
//		double t1 = pLMMTenorStructure_.get_tenorDate(indexStartSwap);
//
//		size_t indexEndSwap = vanillaSwap.get_indexEnd();
//		double t2 = pLMMTenorStructure_.get_tenorDate(indexEndSwap);
//
//		double pvFloatingLeg = 1 - ZCProjectionOnG(t1,t2,G);
//
//		double pvFixedLeg = 0.0;
//		const std::vector<LMM::Index> fixedLegPaymentIndexSchedule = vanillaSwap.get_fixedLegPaymentIndexSchedule();
//		for(size_t indexItr = 0; indexItr < fixedLegPaymentIndexSchedule.size(); ++indexItr)
//		{
//			t2 = fixedLegPaymentIndexSchedule[indexItr];
//			double deltaT = deltaTFloatingLeg_[indexItr];
//			pvFixedLeg  += ZCProjectionOnG(t1,t2,G);
//		}
//		return pvFixedLeg/pvFloatingLeg;
//	}
//
//	//! dS/dG = f'(G)
//	double SwapRateProjectionOnG_firstDerivative(const VanillaSwap& vanillaSwap, double G)
//	{
//		//YY TODO: implement == operator for enum and then active this test!
//		//assert(vanillaSwap.get_simulationTenorType() =  pLMMTenorStructure_.get_tenorType());
//
//		size_t indexStartSwap = vanillaSwap.get_indexStart();
//		double t1 = pLMMTenorStructure_.get_tenorDate(indexStartSwap);
//
//		size_t indexEndSwap = vanillaSwap.get_indexEnd();
//		double t2 = pLMMTenorStructure_.get_tenorDate(indexEndSwap);
//
//		double pvFloatingLeg = 1 - ZCProjectionOnG(t1,t2,G);
//
//		double pvFixedLeg = 0.0;
//		const std::vector<LMM::Index> fixedLegPaymentIndexSchedule = vanillaSwap.get_fixedLegPaymentIndexSchedule();
//		for(size_t indexItr = 0; indexItr < fixedLegPaymentIndexSchedule.size(); ++indexItr)
//		{
//			t2 = fixedLegPaymentIndexSchedule[indexItr];
//			double deltaT = deltaTFloatingLeg_[indexItr];
//			pvFixedLeg  += ZCProjectionOnG(t1,t2,G);
//		}
//		return pvFixedLeg/pvFloatingLeg;
//	}
//
//
//	double SwapRateProjectionOnG_firstDerivative(const VanillaSwap& vanillaSwap, double G)
//	{
//		//YY TODO: implement == operator for enum and then active this test!
//		//assert(vanillaSwap.get_simulationTenorType() =  pLMMTenorStructure_.get_tenorType());
//
//		size_t indexStartSwap = vanillaSwap.get_indexStart();
//		double t1 = pLMMTenorStructure_.get_tenorDate(indexStartSwap);
//
//		size_t indexEndSwap = vanillaSwap.get_indexEnd();
//		double t2 = pLMMTenorStructure_.get_tenorDate(indexEndSwap);
//
//		double pvFloatingLeg = 1 - ZCProjectionOnG(t1,t2,G);
//
//		double pvFixedLeg = 0.0;
//		const std::vector<LMM::Index> fixedLegPaymentIndexSchedule = vanillaSwap.get_fixedLegPaymentIndexSchedule();
//		for(size_t indexItr = 0; indexItr < fixedLegPaymentIndexSchedule.size(); ++indexItr)
//		{
//			t2 = fixedLegPaymentIndexSchedule[indexItr];
//			double deltaT = deltaTFloatingLeg_[indexItr];
//			pvFixedLeg  += ZCProjectionOnG(t1,t2,G);
//		}
//		return pvFixedLeg/pvFloatingLeg;
//	}
//};
//
//#endif /* LMM_PRICER_STATICREPLICATIONCMS_H */