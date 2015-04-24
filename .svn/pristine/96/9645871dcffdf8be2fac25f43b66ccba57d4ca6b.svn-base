//#pragma once
//#include <ql/types.hpp>
//#include <boost/numeric/ublas/vector.hpp>
//#include <boost/numeric/ublas/matrix.hpp>
//
//using namespace boost;
////using namespace QuantLib;
//
//namespace Instruments {
//
//	class InstrumentIndex // YY Why call it index ? 
//	{
//	};
//
//
//	class Fixed: public InstrumentIndex
//	{
//
//	private:
//		double K_; 
//
//	public:
//		Fixed(double K);
//	};
//
//	class Libor: public InstrumentIndex
//	{
//	private:
//
//		double tenor_;
//		double T1_;
//		double T2_;
//
//		// constructor: need to check tenor = T2-T1;  // YY shit, what's this ? 
//	public:
//
//		Libor();
//		Libor(double T1, double T2, double tenor);
//	};
//
//	class DiscountFactor: public InstrumentIndex
//	{
//
//	};
//
//	class Caplet: public InstrumentIndex 
//	{
//	private:
//		double T_expiry_;
//		double tenor_;
//		double strike_;
//		Libor liborIndex_; 
//
//	public:
//		Caplet(double T_expiry, double tenor, double strike);
//		double payoff();
//	};
//
//	class Swap: public InstrumentIndex
//	{
//
//	private:
//
//		double startDate_; // T_0
//		double endDate_;   // T_N
//		InstrumentIndex *Leg1_index_; // receiver  // YY why pointer dangerous !! 
//		InstrumentIndex *Leg2_index_; // payer
//		double Leg1_payementFrequency_;
//		double Leg2_payementFrequency_;
//		//schedulefixing;
//		//schedulefloting;
//
//	public:
//
//		Swap(double startDate, 
//			double endDate, 
//			const InstrumentIndex & leg1, 
//			const InstrumentIndex & leg2, 
//			double leg1_payment_freq, 
//			double leg2_payment_freq);
//
//		Swap(const Swap &swap);
//		~Swap();
//
//		double pvFloating(double T);
//		double pvFixing(double T);
//		double pvSwap(double T);
//	}; 
//
//
//	class StandardSwap : public Swap
//	{
//
//	};
//
//	class CMS: public Swap
//	{
//
//	};
//
//	class Swaption: public Swap
//	{
//	private:
//		Swap underlyingSwap_;
//		double maturity; // normally = underlyingSwap_.startDate_; // pas sure ... 
//		double strike_; 
//	};
//}
