#pragma once

#include <cassert>

#include <boost/numeric/ublas/triangular.hpp>  // Triangular matrix
#include <boost/numeric/ublas/io.hpp>
#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>

//typedef boost::numeric::ublas::triangular_matrix<std::pair<LMM::Index,LMM::Index>, boost::numeric::ublas::upper> UpperTriangularIndexPairMatrix; // UTIPM
//typedef boost::numeric::ublas::triangular_matrix<double, boost::numeric::ublas::upper> UpperTriangularDoubleMatrix; // UTDM
//typedef boost::numeric::ublas::triangular_matrix<std::pair<VanillaSwaption, double>, boost::numeric::ublas::upper > UpperTriangularVanillaSwaptionQuotes;

typedef std::pair<VanillaSwaption, double> SwaptionQuote;
typedef boost::numeric::ublas::matrix<SwaptionQuote > UpperTriangularVanillaSwaptionQuotes;
typedef boost::numeric::ublas::matrix<double>         UpperTriangularDoubleMatrix; // UTDM
typedef boost::numeric::ublas::matrix<std::pair<LMM::Index,LMM::Index> >   UpperTriangularIndexPairMatrix; // UTIPM
	
// ====   LIBOR QUOTE   ====  LIBOR QUOTE   ====  LIBOR QUOTE   ====  LIBOR QUOTE   ====  //
/*! ~ swapRate ~ df
 *	TODO  Test: check the difference between df implied swapRate with mktSwapRate 
 */ 
class LiborQuotes  // ! Non teste
{
	//! YY TODO:
	// 1. input: vector<matuirty, df>
	// 2. implement interpolator: to interpolate df curve
	// 3. initlibor = df/df -1... 
public:

	//! constructor using constant rate r to generate all rate, libor
	LiborQuotes(LMMTenorStructure_PTR lmmTenorStructure_ptr, const double constant_rate);
	LiborQuotes(LMMTenorStructure_PTR lmmTenorStructure_ptr, const std::vector<double>& discountfactor);
	static boost::shared_ptr<const LiborQuotes> create_LiborInit(LMMTenorStructure_PTR lmmTenorStructure, const double constant_rate);

	LMMTenorStructure_PTR get_LMMTenorStructure_PTR() const {return lmmTenorStructure_ ; }

	void reset_Libor(const std::vector<double>& libor_quotes);

	//! getters
	size_t get_nbLIBOR() const { return liborInit_.size(); }
	size_t get_nbDiscountFactor() const { return discountFactor_.size(); }
		
	const std::vector<double>& get_InitLibor     () const { return liborInit_ ; }
	const std::vector<double>& get_DiscountFactor() const { return discountFactor_ ;}

	double get_InitLibor(size_t liborIndex) const 
	{
		assert(liborIndex<lmmTenorStructure_->get_nbLIBOR() ) ; 
		return liborInit_[liborIndex] ; 
	}
	
	double get_DiscountFactor(size_t dateIndex) const 
	{
		assert(dateIndex<(lmmTenorStructure_->get_nbLIBOR()+1) ) ;  
		return discountFactor_[dateIndex] ; 
	}

	void print(const std::string& filename, const bool erase_file=true) const;

	bool check_data_consistency() const;

private :

	LMMTenorStructure_PTR lmmTenorStructure_;
	std::vector<double> liborInit_     ;
	std::vector<double> discountFactor_;

	void generate_DiscountFactor(const double constant_rate);
	void initiate_Libors_from_DiscountFactor();
	void initiate_DiscountFactor_From_Libors();
};
typedef boost::shared_ptr<const LiborQuotes> LiborQuotes_ConstPTR;
typedef boost::shared_ptr<LiborQuotes> LiborQuotes_PTR;

// ====    END LIBOR QUOTE   ====    END LIBOR QUOTE   ====    END LIBOR QUOTE   ====    END LIBOR QUOTE   ====  //
 
class UpperTriangleVanillaSwaptionQuotes
{
	LMMTenorStructure_PTR lmmTenorStructure_;
	size_t lastYear_;         // upperTriangleSwaption(i year, j year), i,j \in [1, lastYear_]
	Tenor  fixedTenor_;
	Tenor  floatingTenor_; 
	size_t indexRatio_;       // fixedTenor/floatingTenor
	UpperTriangularVanillaSwaptionQuotes upperTriangleVanillaSwaptionQuotes_; // 1st row and column not used! size = nbYear + 1
	
	//! index mapping from gDelegate to gTransformed size = nbYear + 1
	UpperTriangularIndexPairMatrix indexMapping_gDelegate_gTransformed_;

	std::string data_file_name_;

public:	

	static boost::shared_ptr<const UpperTriangleVanillaSwaptionQuotes> create_ATMSwaptionImpliedVol
		(
			LiborQuotes_ConstPTR libor_quotes_ptr,
			const Tenor&  fixedTenor,
			const Tenor&  floatingTenor,
			LmmVanillaSwaptionApproxPricer_Rebonato_PTR black_vol_approx_ptr,
			const double& strike_bump = 0
		);

	UpperTriangleVanillaSwaptionQuotes(
		LMMTenorStructure_PTR lmmTenorStructure,
		int   lastYear, 	
		const Tenor& fixedTenor,
		const Tenor& floatingTenor,
		const UpperTriangularDoubleMatrix& strikes, // 1st row and column not used like g!
		const UpperTriangularDoubleMatrix& quotes);  // 1st row and column not used like g!

	//! getters
	size_t get_nbSwaption() const { return (size1()-2)*(size1()-1)/2; }
	size_t size1() const{ return upperTriangleVanillaSwaptionQuotes_.size1(); }
	size_t size2() const{ return upperTriangleVanillaSwaptionQuotes_.size2(); }
	LMMTenorStructure_PTR get_LMMTenorStructure_PTR() const {return lmmTenorStructure_;}
	size_t get_lastYear() const {return lastYear_;}
	Tenor  get_fixedTenor() const{return fixedTenor_;}
	Tenor  get_floatingTenor() const{ return floatingTenor_;}
	size_t get_indexRatio() const {return indexRatio_;}
	const UpperTriangularVanillaSwaptionQuotes & get_UpperTriangularVanillaSwaptionQuotes() const {return upperTriangleVanillaSwaptionQuotes_;}
	const UpperTriangularIndexPairMatrix & get_UpperTriangularIndexPairMatrix() const {return indexMapping_gDelegate_gTransformed_;}
	
	double get_MinQuote() const ;
	double get_MaxQuote() const ;

	//! these methods create copies, do not use in a loop
	UpperTriangularDoubleMatrix get_UpperTriangularQuoteValues() const ;
	UpperTriangularDoubleMatrix get_UpperTriangularStrike() const;
	UpperTriangularIndexPairMatrix get_UpperTriangularSwaptionIndexMatrix() const;
	const std::string get_Data_FileName()const { return data_file_name_;}
	void set_Data_FileName(const std::string& mkt_data_filename) { data_file_name_=mkt_data_filename;}
	void print(const std::string& filename,  const bool erase_file=true) const;

private:
	// 1st row and column not used! as g!  // UTM: upperTriangleMatrix
	void initialize_gDelegate_IndexIngTransformed() ;
};

typedef boost::shared_ptr<const UpperTriangleVanillaSwaptionQuotes> UpperTriangleVanillaSwaptionQuotes_ConstPTR;
typedef boost::shared_ptr<UpperTriangleVanillaSwaptionQuotes> UpperTriangleVanillaSwaptionQuotes_PTR;
