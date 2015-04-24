#include <LMM/LmmModel/LmmPenalty.h>

size_t LmmPenalty::get_NbPenaltyOnDiagonal(const GMatrixMapping_PTR gMatrixMapping_ptr) const 
{
	if(coeff_time_homogene_<=0.) return 0;

	const UpperTriangularDoubleMatrix& gDelegate = gMatrixMapping_ptr->get_gDelegate_Ref();
	size_t N = gDelegate.size2() - 3; // nb of Columns in gDelegate that has penalties
	assert(N == gMatrixMapping_ptr->get_MaxDelegateRowIndex() - 1);
	size_t nbPenalties = N*(N+1)/2;
	return nbPenalties;
}

size_t LmmPenalty::get_NbPenaltyOnMaturity(const GMatrixMapping_PTR gMatrixMapping_ptr) const 
{
	if(coeff_smooth_matutiry_<=0.) return 0;

	const UpperTriangularDoubleMatrix& gDelegate = gMatrixMapping_ptr->get_gDelegate_Ref();
	size_t N = gDelegate.size2() - 4; // nb of Row in gDelegate that has second derivative penalties
	assert(N == gMatrixMapping_ptr->get_MaxDelegateRowIndex() - 2);
	size_t nbPenalties = N*(N+1)/2;
	return nbPenalties;
}

std::vector<double> LmmPenalty::get_PenaltyTimeHomogeneity(const GMatrixMapping_PTR gMatrixMapping_ptr) const
{
	if( coeff_time_homogene_ <=0. ) return std::vector<double>(); // return an empty vector

	const UpperTriangularDoubleMatrix& gDelegate = gMatrixMapping_ptr->get_gDelegate_Ref();
	size_t N = gDelegate.size2() - 3; // nb of Columns in gDelegate that has penalties
	assert(N == gMatrixMapping_ptr->get_MaxDelegateRowIndex() - 1);
	size_t nbPenalties = get_NbPenaltyOnDiagonal(gMatrixMapping_ptr) ;	
	std::vector<double> buffer_penaltyValues(nbPenalties);
	
	size_t penalty_conter = 0;
	//! test indices checked : For a 5YR horizon test, gDelegate is a upper 5x5 matrix
	// jCol = 1, iRow = 1,2,3  iiRow = 2,3,4
	// jCol = 2, iRow = 1,2    iiRow = 2,3
	// jCol = 3, iRow = 1      iiRow = 2
	// nbPenalties = 6
	for(size_t jCol=1;jCol<=N;++jCol)
	{
		size_t nbElementInCol   = gDelegate.size1() - (jCol+1);
		size_t nbPenaltiesInCol = nbElementInCol - 1;
		for(size_t iRow=1;iRow<nbElementInCol;++iRow)
		{
			size_t iiRow = iRow+1;

			// do not use absolute value here, see LmmCostFunction::values
			buffer_penaltyValues[penalty_conter] = coeff_time_homogene_ * ( gDelegate(iiRow,jCol) - gDelegate(iRow,jCol) );

			//std::cout<<"			PenaltyTimeHomogeneity["<<penalty_conter<<"]	jCol["<<jCol<<"]	iRow["<<iRow<<"]	iiRow["<<iiRow<<"]"<<std::endl;
			++penalty_conter;
		}	
	}
	assert(penalty_conter == nbPenalties);//check if penalties are fully computed

	return buffer_penaltyValues;
}

std::vector<double> LmmPenalty::get_PenaltySmoothMaturity(const GMatrixMapping_PTR gMatrixMapping_ptr) const
{
	if( coeff_smooth_matutiry_ <=0. ) return std::vector<double>(); // return an empty vector

	const UpperTriangularDoubleMatrix& gDelegate = gMatrixMapping_ptr->get_gDelegate_Ref();
	size_t N = gDelegate.size2() - 4; // nb of Columns in gDelegate that has penalties
	assert(N == gMatrixMapping_ptr->get_MaxDelegateRowIndex() - 2);
	size_t nbPenalties = get_NbPenaltyOnMaturity(gMatrixMapping_ptr) ;	
	std::vector<double> buffer_penaltyValues(nbPenalties);
	
	size_t penalty_conter = 0;
	//! test indices checked : For a 5YR horizon test, gDelegate is a upper 5x5 matrix
	// iRow = 1, jColmm = 1,2  jCol = 2,3   jColpp = 3,4
	// iRow = 2, jColmm = 1    jCol = 2     jColpp = 3
	// nbPenalties = 3
	for(size_t iRow=1;iRow<=N;++iRow)
	{
		size_t nbElementInRow   = gDelegate.size2() - (iRow+1);
		size_t nbPenaltiesInRow = nbElementInRow - 2;
		for(size_t jColmm=1;jColmm<=nbPenaltiesInRow;++jColmm)
		{
			size_t jCol   = jColmm+1;
			size_t jColpp = jColmm+2;

			// do not use absolute value here, see LmmCostFunction::values
			// for instance, penalty does not know the time step discretization, so penalty class can only compute finite difference with time step == 1
			// After all, user can always set the timestep by giving the penalty coeff
			buffer_penaltyValues[penalty_conter] =  coeff_smooth_matutiry_ * (gDelegate(iRow,jColpp) + gDelegate(iRow,jColmm) - 2*gDelegate(iRow,jCol) );

			//std::cout<<"		PenaltySmoothMaturity["<<penalty_conter<<"]	iRow["<<iRow<<"]	--jCol["<<jColmm<<"]	jCol["<<jCol<<"]	++jCol["<<jColpp<<"]"<<std::endl;
			++penalty_conter;
		}	
	}
	assert(penalty_conter == nbPenalties);//check if penalties are fully computed

	return buffer_penaltyValues;
}

double LmmPenalty::calc_PenaltyTimeHomogeneity_L2(const GMatrixMapping_PTR gMatrixMapping_ptr) const 
{
	assert( !this->isEmpty() );
	if(coeff_time_homogene_>0.)
		return calc_L2(get_PenaltyTimeHomogeneity(gMatrixMapping_ptr))/coeff_time_homogene_;
	else
		return 0.;
}

double LmmPenalty::calc_PenaltyTimeHomogeneity_L1(const GMatrixMapping_PTR gMatrixMapping_ptr) const 
{
	assert( !this->isEmpty() );
	if(coeff_time_homogene_>0.)
		return calc_L1(get_PenaltyTimeHomogeneity(gMatrixMapping_ptr))/coeff_time_homogene_;
	else
		return 0.;
}

double LmmPenalty::calc_PenaltyTimeHomogeneity_L_INF(const GMatrixMapping_PTR gMatrixMapping_ptr) const 
{
	assert( !this->isEmpty() );
	if(coeff_time_homogene_>0.)
		return calc_L_INF(get_PenaltyTimeHomogeneity(gMatrixMapping_ptr))/coeff_time_homogene_;
	else
		return 0.;
}
	
double LmmPenalty::calc_PenaltySmoothMaturity_L2(const GMatrixMapping_PTR gMatrixMapping_ptr) const 
{
	assert( !this->isEmpty() );
	if(coeff_smooth_matutiry_>0.)
		return calc_L2(get_PenaltySmoothMaturity(gMatrixMapping_ptr))/coeff_smooth_matutiry_;
	else
		return 0.;
}

double LmmPenalty::calc_PenaltySmoothMaturity_L1(const GMatrixMapping_PTR gMatrixMapping_ptr) const 
{
	assert( !this->isEmpty() );
	if(coeff_smooth_matutiry_>0.)
		return calc_L1(get_PenaltySmoothMaturity(gMatrixMapping_ptr))/coeff_smooth_matutiry_;
	else
		return 0.;
}

double LmmPenalty::calc_PenaltySmoothMaturity_L_INF(const GMatrixMapping_PTR gMatrixMapping_ptr) const 
{
	assert( !this->isEmpty() );
	if(coeff_smooth_matutiry_>0.)
		return calc_L_INF(get_PenaltySmoothMaturity(gMatrixMapping_ptr))/coeff_smooth_matutiry_;
	else
		return 0.;
}

double LmmPenalty::calc_L2(std::vector<double>& vector) const 
{
	assert( !vector.empty() );
	double penalty_l2(0);
	
	const size_t N = vector.size();
	for(size_t i=0;i<N;++i)
	{
		penalty_l2+=vector[i]*vector[i];
	}
	
	return std::sqrt(penalty_l2/N);
}

double LmmPenalty::calc_L1(std::vector<double>& vector) const 
{
	assert( !vector.empty() );
	double penalty_l1(0);
	
	const size_t N = vector.size();
	for(size_t i=0;i<N;++i)
	{
		penalty_l1+= std::abs(vector[i]);
	}

	return penalty_l1/N;
}

double LmmPenalty::calc_L_INF(std::vector<double>& vector) const 
{
	assert( !vector.empty() );
	double penalty_linf(0.);

	const size_t N = vector.size();
	for(size_t i=0;i<N;++i)
	{
		if( penalty_linf < std::abs(vector[i]) )
				penalty_linf = std::abs(vector[i]);
	}

	return penalty_linf;
}
