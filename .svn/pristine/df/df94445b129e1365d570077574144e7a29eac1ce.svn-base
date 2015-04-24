#pragma once

#include <ql/math/array.hpp>
#include <string>

#include <LMM/LmmModel/LmmSwaptionMarketData.h>
#include <LMM/calibration/LmmCalibrationConfig.h>
#include <LMM/LmmModel/GMatrixMapping.h>

void test_BlackGreek();
void test_VanillaSwapCreation();
void test_McTerminalLmm();

//! for validating MC simulation. 
void test_VanillaFRAPricer();
void test_VanillaCapletPricer();
void test_VanillaSwapPricer();

//! for validating Roboato formula
void test_VanillaSwaptionPricer();
void test_VanialSwaptionPricer_MCvsApprox(); // what's the difference between this and the last one ???

void test_Functional();

void test_HGVolatility();

void test_LmmCalibrationConfig();

void test_LmmGnuplotPrinterMatrix();
void test_Interpolation();
void test_Noise();
void test_SwaptionMarketDataContainer();
void test_CalibrationWithATMSwaptionVol();
void test_CalibrationShiftWithSwaptionVolSkew();

void test_LmmVirtualCorrelationCalibration();
void test_LmmABCDFinder();
void test_GMatrixMapping();
void test_UpperTriangleVanillaSwaptionQuotes();
void test_LmmSwaptionMarketData();
void test_LmmVirtualCalibration();
void test_LmmCalibrationMarketData();	
void test_LmmCalibrationSensitivity();	
void test_LmmRegularizedCalibrationMarketData();

LmmSwaptionMarketData_PTR get_LmmSwaptionMarketData(const LmmCalibrationConfig& config, const std::string& input_file);


LMMTenorStructure_PTR create___LMMTenorStructure_PTR(const size_t nbyear);

Correlation_PTR create_InitCorrelation(const LmmCalibrationConfig& config);

QuantLib::Array marketData_LMM_ABCD_calibration( const LmmCalibrationConfig& config
											   , LmmSwaptionMarketData_PTR pLmmSwaptionMarketData
											   );

Correlation_PTR marketData_LMM_Correlation_calibration( const LmmCalibrationConfig& config
													  , LmmSwaptionMarketData_PTR pLmmSwaptionMarketData
													  , const QuantLib::Array& found_abcd
													  );

GMatrixMapping_PTR marketData_LMM_CascadeExact_calibration( const LmmCalibrationConfig& config
														  , LmmSwaptionMarketData_PTR pLmmSwaptionMarketData 
														  , const QuantLib::Array& abcd_param 
														  , Correlation_PTR found_correlation_ptr 
														  );

void marketData_LMM_Global_gCalibration( const LmmCalibrationConfig& config
										   , LmmSwaptionMarketData_PTR pLmmSwaptionMarketData 
										   , const QuantLib::Array& abcd_param 
										   , Correlation_PTR found_correlation_ptr 
										   , GMatrixMapping_PTR init_gMapping /*should be NULL*/
										   );

void marketData_LMM_Local_gCalibration( const LmmCalibrationConfig& config
										   , LmmSwaptionMarketData_PTR pLmmSwaptionMarketData 
										   , const QuantLib::Array& abcd_param 
										   , Correlation_PTR found_correlation_ptr 
										   , GMatrixMapping_PTR init_gMapping /*should be NULL*/
										   );