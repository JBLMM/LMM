//#pragma once
//
//#include <vector>
//
//class Tarn
//{
//private:
//	double strike_;
//	double target_; // Pre-agreed upon target, which determines the note's termination
//
//	size_t    indexStart_;
//	size_t    indexEnd_;
//
//	size_t    exoticLegTenor_;    // in Month 
//	size_t    liborLegTenor_;      // in Month 
//	size_t    exoLiborTenorRatio_; 
//
//	std::vector<size_t> exoticPaymentIndexSchedule;
//	std::vector<size_t> liborPaymentIndexSchedule;    
//
//
//public:
//	Tarn();
//
//	Tarn(double strike, 
//		 double target, 
//		 size_t indexStart, 
//		 size_t indexEnd, 
//		 size_t liborLegTenor, 
//		 size_t exoticLegTenor);
//
//	void print_details();
//
//	//-- Getters/Setters
//	double get_strike();
//	double& strike();
//	double get_target();
//	void   set_target(double target);
//	double get_indexStart();
//	double get_indexEnd();
//	double get_liborLegTenor();
//	double get_exoticLegTenor();
//	const std::vector<size_t>& getExoticPaymentIndices();
//	const std::vector<size_t>& getLiborPaymentIndices();
//
//	//-- Tests
//	void showTarnStructure();
//};
