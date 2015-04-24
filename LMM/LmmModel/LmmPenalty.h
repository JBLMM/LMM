#pragma once

#include <vector>

#include <LMM/LmmModel/GMatrixMapping.h>

/*! \class LmmPenalty
*
* Routine
*		- update_penalty
*		- get_penalty
*/
class LmmPenalty
{
public:
	
	LmmPenalty(const double & coeff_time_homogene, const double & coeff_smooth_matutiry) 
	: coeff_time_homogene_(coeff_time_homogene)
	, coeff_smooth_matutiry_(coeff_smooth_matutiry)
	{}

	//! penalty is empty and not be use if the coeffs was set to non positive number
	bool isEmpty() const { return (coeff_time_homogene_<=0. && coeff_smooth_matutiry_<=0.); }

	size_t get_TotalNbPenalty(const GMatrixMapping_PTR gMatrixMapping_ptr) const 
	{
		return get_NbPenaltyOnDiagonal(gMatrixMapping_ptr) + get_NbPenaltyOnMaturity(gMatrixMapping_ptr);
	}

	//! compute the penalty buffer and return its values
	std::vector<double> get_penalty(const GMatrixMapping_PTR gMatrixMapping_ptr) const
	{
		assert( !this->isEmpty() );

		std::vector<double> penalty = get_PenaltyTimeHomogeneity(gMatrixMapping_ptr);
		std::vector<double> penalty_on_maturity = get_PenaltySmoothMaturity(gMatrixMapping_ptr);

		penalty.insert( penalty.end(), penalty_on_maturity.begin(), penalty_on_maturity.end() );

		return penalty;
	}
	
	const double & get_coeff_time_homogene() const { return coeff_time_homogene_ ;}
	const double & get_coeff_smooth_matutiry() const { return coeff_smooth_matutiry_ ;}

	double calc_PenaltyTimeHomogeneity_L2(const GMatrixMapping_PTR gMatrixMapping_ptr) const ;
	double calc_PenaltyTimeHomogeneity_L1(const GMatrixMapping_PTR gMatrixMapping_ptr) const ;
	double calc_PenaltyTimeHomogeneity_L_INF(const GMatrixMapping_PTR gMatrixMapping_ptr) const ;
	
	double calc_PenaltySmoothMaturity_L2(const GMatrixMapping_PTR gMatrixMapping_ptr) const ;
	double calc_PenaltySmoothMaturity_L1(const GMatrixMapping_PTR gMatrixMapping_ptr) const ;
	double calc_PenaltySmoothMaturity_L_INF(const GMatrixMapping_PTR gMatrixMapping_ptr) const ;

private : 	
	
	const double coeff_time_homogene_;
	const double coeff_smooth_matutiry_;
	
	size_t get_NbPenaltyOnDiagonal(const GMatrixMapping_PTR gMatrixMapping_ptr) const ;
	size_t get_NbPenaltyOnMaturity(const GMatrixMapping_PTR gMatrixMapping_ptr) const ;

	std::vector<double> get_PenaltyTimeHomogeneity(const GMatrixMapping_PTR gMatrixMapping_ptr) const;
	std::vector<double> get_PenaltySmoothMaturity(const GMatrixMapping_PTR gMatrixMapping_ptr) const;

	double calc_L2(std::vector<double>& vector) const ;
	double calc_L1(std::vector<double>& vector) const ;
	double calc_L_INF(std::vector<double>& vector) const ;
};
typedef boost::shared_ptr<LmmPenalty> LmmPenalty_PTR;
