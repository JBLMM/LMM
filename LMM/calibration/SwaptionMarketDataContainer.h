#ifndef LMM_CALIBRATION_SWAPTION_MARKET_DATA_CONTAINER_H
#define LMM_CALIBRATION_SWAPTION_MARKET_DATA_CONTAINER_H
#pragma once

#include <stdlib.h>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

#include <LMM/helper/Name.h>
#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/calibration/TestHGVolatilityParam.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>
#include <LMM/calibration/ATMSwaptionMarketData.h>

class SwaptionMarketDataManager;

/*! \class SwaptionMarketDataContainer
*
*  A simple class that hold all data for ATMSwaption at the dates defined in the LMMTenorStructure
*  The big different of this data class from ATMSwaptionMarketData is the latter is read-only, the former is 
*  a read-write data, that have several additional then the data loading from a csv file.
*
*  These data are similarly as ATMSwaptionMarketData, but only at the date choosen, setted in LMMTenorStructure
*  If a date node at LMMTenorStructure that does not exist in market data, an interpolation will perform
*
*  - start dates of LIBORS          Tk           k=0,1... 
*  - Libors                         L(0,Ti,Tj)
*  - Libors shift                   
*  - Maturities of ZC               Tk           k=0,1...
*  - ZeroCoupons                    P(0,Tk)
*  - Numeraires                     Dk           k=0,1...    // computed from Zero Coupon
*
* This data structure can not hold the same loaded data structure as a matrix because for a fixed horizon
* in number of years, not all Swaptions can be traited. So here put all plat data in vector structure.
* The most important is these data are coherent in their indices.
*
*  - Vector of Swap Rate (Strike)
*  - Vector of Volatilities
*  - Vector of Swaption
*  
* We use also methods in VanilaSwaptionPricer to precompute the
*  - Vector of Annuities ( each annuities correspond to a swaption)
*  - Matrix of Omega0    ( each row corerspond to a Vector of Omega0 of a swaption)
*
* Note that a Omega0 vector correspond to a swaption need only be computed from startSwaptionIndex to endSwaptionIndex
* But we compute the Omega0 vector on the whole timeline of Libor, i.e from 0 to N. This is a precompute so that do not 
* take to much time, and that help to have the coherent of the mathematical formula
*
*    S_{\alpha,\beta} = \sum_{\alpha}^{\beta-1} \omega_i L_i
*/


class SwaptionMarketDataContainer
{
public:
	friend class SwaptionMarketDataManager;

	typedef std::vector<double>          RealVector     ;
	typedef std::vector<RealVector>      RealMatrix     ;
	typedef std::vector<VanillaSwaption> SwaptionVector ;
	typedef std::vector<SwaptionVector>  SwaptionMatrix ;

	typedef TestHGVolatilityParam::VectorOfMatrixIndices VectorOfMatrixIndices;
	typedef std::vector<VectorOfMatrixIndices>           MatrixOfMatrixIndices;

	SwaptionMarketDataContainer();

	//! get the number of Libors init, which is the same number of ZC, libor shift, numeraire, 
	size_t get_nbLIBOR()    const ;

	//! get the number of Swaptions, which is the same number of Vols, Strikes retreived from ATM Swaption data
	size_t get_nbSWAPTION() const ;

	//! get the swaption sparse step, in others words the ratio of fixed tenor / float tenor
	size_t get_SwaptionSparseStep() const ;

	double get_StrikeBump() const;

	//! getters
	const std::vector<double> & get_LIBOR_INIT()         const ;
	//const std::vector<double> & get_LIBOR_SHIFT()        const ;
	const std::vector<double> & get_ZC_BOND()            const ;
	const std::vector<double> & get_ZC_MATURITIES()      const ;
	const std::vector<double> & get_LIBOR_STARTDATES()   const ;
	const std::vector<double> & get_NUMERAIRE()          const ;

	const RealVector &           get_STRIKE_VECTOR()         const ;
	const RealVector &           get_VOLATILITY_VECTOR()     const ;
	const SwaptionVector &       get_SWAPTION_VECTOR()       const ;
	const std::vector<size_t>&   get_SWPN_MATURITY_INDICES() const ;
	const VectorOfMatrixIndices& get_HGVOL_VECTOR_INDICES()  const ;

	const RealMatrix    &        get_STRIKE_MATRIX()         const ;
	const RealMatrix    &        get_MKT_VOL_MATRIX()        const ;
	const SwaptionMatrix&        get_SWPN_MATRIX()           const ;
	const MatrixOfMatrixIndices& get_HGVOL_NODE_MAPPING()    const ;

	//! check if the swaption data container is ATM Swaptions using a VanillaSwapPricer
	bool is_ATMSwaptionData(const LmmVanillaSwapPricer_PTR swap_pricer_ptr) const ; 

	//! use for test and debug, check if the parsed data are coherent 
	//! Note that this check only the data format, and the coherent of strke stored in strike vector with strike in swaptions vector
	bool check_data_consistance()          const ; 
	// print information into a file
	void print(const std::string& filename) const;	

private:

	size_t swaption_sparse_step_ ;
	double strike_bump_ ; // If this container contain skew data, the strike_bump is store here, if not, strike_bump is set to absurd  value -1e9

	//! not that these data of libor, ZC and numeraire does not exist for date 0 (which is theoretical date)
	//  We add this date in our structure in order to have the conveniant theoretical.
	std::vector<double> LIBOR_STARTDATES_ ;
	std::vector<double> LIBOR_INIT_       ;
	std::vector<double> ZC_MATURITIES_    ;
	std::vector<double> ZC_BOND_          ;	
	std::vector<double> NUMERAIRE_        ;

	RealVector     STRIKE_VECTOR_                ;
	RealVector     VOLATILITY_VECTOR_            ;		
	SwaptionVector SWAPTION_VECTOR_              ;
	std::vector<size_t>   SWPN_MATURITY_INDICES_ ;
	VectorOfMatrixIndices HGVOL_VECTOR_INDICES_  ; //! all indices in HG vol match will all swaption in SWAPTION_VECTOR_

	/*! for each maturity, there are a set of swaption that has EXACTLY this maturity
	*  this set of swaption depend into a set of vols coeff in the hg matrix,
	*  this matrix store every dependent pair indices of vol corresponding to this specific maturity
	*  NOTE : this matrix does not store full dependent indices, but sparsely
	*         In the way that the number of stored vol coeff equal to the number of swaption
	*/ 	
	RealMatrix            STRIKE_MATRIX_       ;
	RealMatrix            MKT_VOL_MATRIX_      ;
	SwaptionMatrix        SWPN_MATRIX_         ;
	MatrixOfMatrixIndices HGVOL_NODE_MAPPING_  ;	


	//clear all data, only ATMSwaptionMarketData can call this functions
	void clear_all_data()          ; // clear every data
	void clear_all_LIBOR_ZC()      ; // clear all LIBOR, ZC their maturities, and NUMERAIRE 
	void clear_all_SWAPTION_DATA() ; // clear all SWAPRATE, VOLATILITY, and SWAPTION vectors
	void clear_all_Matrix_data()   ; // Swaption data contain vector format and matrix format, this method clear the matrix part

	void compute_numeraire();
	void build_MatrixDataFromVectorData();
	void refresh_AllSwaptionStrike();

	void printToStreamLiborDate(std::stringstream & stream) const;
	void printToStreamLiborValue(std::stringstream & stream) const;	
	void printToStreamZCDate(std::stringstream & stream) const;
	void printToStreamZCValue(std::stringstream & stream) const;

	template<typename ELEMENT>
	void printToStreamMatrixData(std::stringstream & stream,  const std::vector<std::vector<ELEMENT> > & matrix) const;

	template<typename ELEMENT>
	void printToStreamMatrixDataLine(std::stringstream & stream, const size_t iRowSwaption, const std::vector<ELEMENT>& line) const ;

};

typedef boost::shared_ptr<SwaptionMarketDataContainer> SwaptionMarketDataContainer_PTR;

template<typename ELEMENT>
void SwaptionMarketDataContainer::printToStreamMatrixDataLine(std::stringstream & stream, const size_t iRowSwaption, const std::vector<ELEMENT>& dataline) const
{
	size_t iLibor = (iRowSwaption+1) * get_SwaptionSparseStep();
	stream<<LIBOR_STARTDATES_[iLibor]<<", ";

	for(size_t iSwaption=0;iSwaption<dataline.size();++iSwaption)	
	{
		for(size_t i_unavalable=1;i_unavalable<get_SwaptionSparseStep();++i_unavalable){ stream<<", "; }
		stream<<dataline[iSwaption]<<", ";
	}
}

template<typename ELEMENT>
void SwaptionMarketDataContainer::printToStreamMatrixData(std::stringstream & stream,  const std::vector<std::vector<ELEMENT> > & matrix_data) const
{
	printToStreamLiborDate(stream); stream<<std::endl;
	
	size_t nbSwaptionRow = matrix_data.size();

	for(size_t iSwaptionRow=0;iSwaptionRow<nbSwaptionRow;++iSwaptionRow)
	{
		for(size_t i_unavalable=1;i_unavalable<get_SwaptionSparseStep();++i_unavalable){ stream<<std::endl; }
		const std::vector<ELEMENT> & row_data = matrix_data[iSwaptionRow];
		printToStreamMatrixDataLine(stream,iSwaptionRow,row_data) ;stream<<std::endl;		
	}
}



#endif /* LMM_CALIBRATION_SWAPTION_MARKET_DATA_CONTAINER_H */