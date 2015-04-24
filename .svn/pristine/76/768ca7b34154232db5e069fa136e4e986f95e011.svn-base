#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/shared_ptr.hpp>

#include <LMM/helper/Name.h>
#include <LMM/helper/TenorType.h>
#include <LMM/helper/LMMTenorStructure.h>


/*! \class VanillaSwap manage leg's INDICES in the simulation structures see <LMM\lmmModel\LMMTenorStructure.h>
 * Each simulation index correspond to a simulated Libor
 *
 * Float Leg's indices                 S      fixedTenor        |       fixedTenor    | 
 * Simulation indices    0-----T1-----T2-----T3------T4--------*-------*------*------*--------T_{N+1}     // Simulation TimeLine
 * Fixed Leg's indices                 S  floatTenor   |   floatTenor   | floatTenor  | 
 *
 * S and E are start Index and end Index in the simulation base
 * Simulation Tenor st =  T_{i+1} - T_i  for all i
 * (endIndex-startIndex)  has to be multiple of (fixedTenor%st) AND (floatTenor%st) 
 *
 * !!! For instance, floatTenor=simulationTenor = T_{i+1} - T_i for a swap
 */
class VanillaSwap
{
public:

	VanillaSwap(); // add default constructor used for building Upper Triangle Vanilla Swap Quotes UpperTriangularVanillaSwaptionQuotes

	VanillaSwap(const double& strike,
		LMM::Index  indexStart, 
		LMM::Index  indexEnd, 
		const Tenor& floatingLegTenorType,		
		const Tenor& fixedLegTenorType,
		LMMTenorStructure_PTR lmmTenorStructure); // to compare with the pricer's LMM's structure-type.
													       // YY: bad name, to change it latter

	void    set_strike(double strike) {strike_ = strike;}
	double  get_strike() const        {return strike_;} 
	double& get_strikeRef()           {return strike_;} 
	
	Tenor get_fixedLegTenorType()	 const { return fixedLegTenorType_     ; }
	Tenor get_floatingLegTenorType() const { return floatingLegTenorType_  ; }	
	Tenor get_lmmTenorStructureTenorType()  const { return lmmTenorStructure_->get_tenorType();} // to compare with the pricer's LMM's structure
	LMMTenorStructure_PTR get_LMMTenorStructure() const  {return lmmTenorStructure_; }

	//! get the euclidean quotien of legs's TenorType to simulation's TenorType
	size_t    get_fixedLegTenorLmmTenorRatio() const { return    fixedVsLiborTenorTypeRatio_; }
	size_t get_floatingLegTenorLmmTenorRatio() const { return floatingVsLiborTenorTypeRatio_; }		
	
	const std::vector<LMM::Index>& get_floatingLegPaymentIndexSchedule() const { return floatingLegPaymentIndexSchedule_; }
	const std::vector<LMM::Index>& get_fixedLegPaymentIndexSchedule()    const { return fixedLegPaymentIndexSchedule_   ; }

	const std::vector<double>& get_DeltaTFloatLeg()             const {return deltaTFloatingLeg_;}
	double                     get_DeltaTFloatLeg(size_t index) const {return deltaTFloatingLeg_[index];} 
	const std::vector<double>& get_DeltaTFixedLeg()				const {return deltaTFixedLeg_;}  
	double                     get_DeltaTFixedLeg(size_t index) const {return deltaTFixedLeg_[index];}

	LMM::Index get_indexStart() const {return indexStart_;}
	LMM::Index get_indexEnd()   const {return indexEnd_  ;}

	double get_StartDate() const { return lmmTenorStructure_->get_tenorDate()[get_indexStart()] ; }
	double get_EndDate()   const { return lmmTenorStructure_->get_tenorDate()[get_indexEnd()]   ; }

	// print swap informations to csv file
	void print(const std::string& filename) const ;

private:

	double strike_;

	LMM::Index indexStart_; // S      
	LMM::Index indexEnd_  ; // E       
		
	/*! The TenorType of Simulation (LMMTenorStructure) has to be a common divisor
	 *  of TenorType for floatingLeg and fixedLeg
	 */
	Tenor floatingLegTenorType_ ; // floatingLeg payment frequency = Libor's tenorType
	Tenor fixedLegTenorType_    ; // fixedLeg payment frequency: each "1M", "3M", "6M", "1Y"
	LMMTenorStructure_PTR lmmTenorStructure_; // simulation informations stored in LMMTenorStructure

	size_t floatingVsLiborTenorTypeRatio_; // = floatingLegTenorType_ % simulationTenorType_
	size_t fixedVsLiborTenorTypeRatio_;    // = fixedLegTenorType_    % simulationTenorType_

	std::vector<LMM::Index> floatingLegPaymentIndexSchedule_; // give index in LMMTenorStructure 
	std::vector<LMM::Index> fixedLegPaymentIndexSchedule_   ;    
	
	std::vector<double> deltaTFloatingLeg_;
	std::vector<double> deltaTFixedLeg_   ;	

public:
	void write_to_stream(std::ostream& outputstream)const ;
};
typedef boost::shared_ptr<VanillaSwap> VanillaSwap_PTR;



std::ostream& operator<<(std::ostream& os, const VanillaSwap& swap);

