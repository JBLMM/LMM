//#pragma once 
//#include <ql/types.hpp>
//#include <boost/numeric/ublas/vector.hpp>
//#include <boost/numeric/ublas/matrix.hpp>
//
//
//using namespace boost;
//using namespace boost::numeric::ublas;
//using namespace QuantLib;
//
//
///// Class defining a swap tenor structure
//class SwapModelMc {
//
//private :
//
//	/// Tenor lenght
//	size_t tenor;
//
//	/// Swap's entering date
//	Time beginDate;
//
//	/// Swap's end date
//	Time endDate; 
//
//	/// Index of the swap's entering date
//	size_t beginDateIndex;
//
//	/// Index of the last swap tenor date
//	size_t endDateIndex;
//
//	/// Floating leg period (in months)
//	size_t floatPer; 
//
//	/// Fixing leg period (in months)
//	size_t fixPer;
//
//	/// Floatting leg's tenor dates
//	numeric::ublas::vector<Time> floatDates;
//
//	/// Fixing leg's tenor dates
//	numeric::ublas::vector<Time> fixDates;
//
//public:
//	
//	SwapModelMc();
//
//	// TODO : add time in place of indices and make operator = for testpricing class (swapmodel = swapmodelother)
//	SwapModelMc(size_t tenor, size_t beginDateIndex, size_t endDateIndex, size_t floatPer, size_t fixPer);
//	SwapModelMc(const SwapModelMc &swapStruct);
//
//	// getters //
//	size_t getTenor();
//	size_t getFloatPer();
//	size_t getFixPer();
//	const numeric::ublas::vector<Time> &getFloatDates();
//	const numeric::ublas::vector<Time> &getFixDates();
//
//};
//
