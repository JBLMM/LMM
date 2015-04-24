#pragma once



#include <vector>
#include <string>
#include <cassert>

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

#include <LMM/helper/Name.h>
#include <LMM/helper/TenorType.h>


/*! \class LMMTenorStructure used as a basis topological, timeline, timestep for simulation
 *
 *  Implement the structure of LMM model
 *   - number of setlement dates ,T_k, k from 0 to N+1 (horizon)   
 *   - the setlement date set (tenorDates)    (converted to year unit)
 *   - the tenor set (\tau_k)                 (converted to year unit)
 *
 *     T[i]    0   1    2    3    ...                N   N+1   \\ nbDates  = N+2
 * timeline    *---*----*----*---*----*----*---*----*----*                         \\ horizon_ = N
 *  \tau[i]    0   1    2                            N         \\ nbDeltaT = N+1
 *   L_i(t)    0   1    2                            N         \\ nbLibor  = N+1
 *
 * T_{i+1}-T_i = \tau[i]   <-- use of -->   L_i(t) = L(t,T_i,T_{i+1})
 *
 * /!\ - LIBORs here are Forward Libors, which is different from Spot Libor taking from market
 *
 *              L(0,T_i,T_{i+1}) = Libor_Init[i]  #  L[i]  = L(t,T_i,T_{i+1})
 *
 *     - The reason there are N+2 date because in our code, N is a number of LIBORs
 *       Our convention  : L_i is the forward LIBOR L(t,T_i,T_{i+1}), i-LiborIndex run from 1 to N
 *       Hence for having N LIBORs, we create N+2 dates nodes
 */

class LMMTenorStructure //ctntodo this class has to be singleton
{
public:

	/*! constructor
	 * - compute from number of years into multiple of tenortyp --> horizon N=nbYear*12/TenorType.nbMonth 
	 * - compute and store T[i], \tau[i]
	 */
	LMMTenorStructure(const Tenor&  tenorType, int max_nbOfYear);
	
	//! approximatively compute and return the approximative TenorType giving a year fraction
	Tenor get_tenorType(const double& year_fraction) const ;
	
	//! approximatively compute the index of the nearest LMMTenorStructure'sdate giving an arbitrary date
	LMM::Index   get_Index(const double& date) const ;

	//! getter
	LMM::Index   get_horizon()   const ;  // N
	LMM::Index   get_nbLIBOR()   const ;  // N+1	

	//! get the TenorType of this LMMTenorStructure
	Tenor get_tenorType() const;

	double                     get_EndDate()               const ;
	double                     get_deltaT(size_t index)    const ;
	const std::vector<double>& get_deltaT(            )    const ;

	double					   get_tenorDate(size_t index) const ;
	const std::vector<double>& get_tenorDate(            ) const ;
	bool                       is_TenorDate(const double&) const ; //check if a date exist in the tenor date list

	//! equal operator: 
    bool operator == (const LMMTenorStructure& LMMTenorStructure) const;
	bool operator != (const LMMTenorStructure& LMMTenorStructure) const;


	void write_to_stream(std::ostream& outputstream) const ;
	//! print in Excel
	void print(const std::string& filename) const;
private:

	const Tenor          tenorType_   ;    // eg: "3M", "6M", "9M" smallest timestep for simulation 
	LMM::Index           horizon_     ;    // N 

	std::vector<double>  tenorDates_  ;    // size = N+2, T_0, T_1,        ..........     T_{N+1}
	std::vector<double>  tenorDeltaT_ ;    // size = N+1, \tau_0, \tau_1,  ..... \tau_{N}
};

typedef boost::shared_ptr<LMMTenorStructure> LMMTenorStructure_PTR;


