#include <boost/numeric/ublas/matrix.hpp>

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/BuildVariable.h>
#include <LMM/helper/Printer.h>
#include <LMM/helper/TypeConverter.h>
#include <LMM/LmmModel/Correlation.h>

// ------------------------------------------------------------------------------
//
//                          Correlation
//
// ------------------------------------------------------------------------------

Correlation::Correlation(size_t fullRank, size_t reducedRank, CorrelationReductionType::CorrelationReductionType& reductionType) // YY: TODO don't like default parameter: use boost::defaultable<size_t> for reduceRank latter.
	: fullRank_(fullRank),
	reducedRank_(reducedRank),
	originalCorrelMatrix_(fullRank,fullRank), 
	reducedCorrelMatrixB_(fullRank,reducedRank), 
	reducedCorrelMatrixApprox_(fullRank,fullRank), 
	reductionType_(reductionType),
	flagApproxConstruced_(false)
{
	assert(fullRank_>0 && reducedRank_>0 && reducedRank_<=fullRank_);
}


void Correlation::calculate()
{
	calculateCorrelOriginal();              // construct originalCorrelMatrix_
	calculateReducedCorrelB();	    // from originalCorrelMatrix_ --> reducedCorrelMatrixB_
	calculateReducedCorrelApprox(); // from reducedCorrelMatrixB_ --> reducedCorrelMatrixApprox_ = B*B^T
	flagApproxConstruced_ = true;
}

void Correlation::calculateCorrelOriginal()
{
	for(size_t i=0; i<fullRank_; ++i)
		for(size_t j=0; j<fullRank_; ++j)
			originalCorrelMatrix_[i][j] = correlOriginal(i,j);
}

void Correlation::calculateReducedCorrelB()
{	
	switch (reductionType_)
	{
	case (CorrelationReductionType::PCA):
		{
			assert(reducedRank_ < fullRank_);
			bool useNormalizeDiagonal = true;
			reducedCorrelMatrixB_ = PCA::doPCA(originalCorrelMatrix_, reducedRank_, useNormalizeDiagonal);  // YY TODO: there is some inefficient copy-coller, to change it latter.			
			break;
		}
	case(CorrelationReductionType::RA):
		{ 
			//! Do PCA
			assert(reducedRank_ < fullRank_);
			RebonatoAngle rb(originalCorrelMatrix_, reducedRank_);
			reducedCorrelMatrixB_ =  rb.get_BMatrix();
			break;
		}
	case(CorrelationReductionType::NON):
		{
			assert(reducedRank_ == fullRank_);
			reducedCorrelMatrixB_      = originalCorrelMatrix_ ;
			reducedCorrelMatrixApprox_ = originalCorrelMatrix_ ;
			break;
		}
	default:
		throw ("CorrelationReductionType not known");
	}
}

void Correlation::calculateReducedCorrelApprox()
{
	if(reductionType_ != CorrelationReductionType::NON)
		reducedCorrelMatrixApprox_ = reducedCorrelMatrixB_*transpose(reducedCorrelMatrixB_);
	else
		reducedCorrelMatrixApprox_ = originalCorrelMatrix_;
}


//! getter.
size_t Correlation::get_fullRank(){return fullRank_;}      
size_t Correlation::get_reducedRank(){return reducedRank_;}
const  Matrix& Correlation::get_originalCorrelMatrix() const {return originalCorrelMatrix_;}
const  Matrix& Correlation::get_reducedCorrelMatrixB() const {return reducedCorrelMatrixB_;}
const  Matrix& Correlation::get_reducedCorrelMatrixApprox() const {return reducedCorrelMatrixApprox_;}
const  CorrelationReductionType::CorrelationReductionType& Correlation::get_reductionType() const {return reductionType_;}
const  std::string Correlation::get_reductionTypeString() const
{
	if(reductionType_ == CorrelationReductionType::PCA)	return "PCA";

	if(reductionType_ == CorrelationReductionType::RA) return "RA";

	if(reductionType_ == CorrelationReductionType::NON) return "NON";

	throw("CorrelationReductionType is not valid");
}
bool   Correlation::get_flagApproxConstruced()const {return flagApproxConstruced_;}
//double Correlation::correlApprox(size_t i, size_t j) const 
//{
//	assert(flagApproxConstruced_==true);
//	return reducedCorrelMatrixApprox_[i][j];
//}

//! YY TODO: make the correlation homogeneous + piecewise constant: 
//! for time period [timeIndex_k-1, timeIndex_k]
double Correlation::correlApprox(size_t liborIndex_i , size_t liborIndex_j, size_t  timeIndex_k) const
{
	assert(flagApproxConstruced_==true);
	return reducedCorrelMatrixApprox_[liborIndex_i][liborIndex_j];  // this is because the vol is constant.
}

void Correlation::print(const std::string& filename) const
{
	assert(flagApproxConstruced_ == true);

	//std::string fileName = "Correlation.csv";
	printPlusPlus_matrices(filename);
}

void Correlation::printPlusPlus_matrices(const std::string& filename) const
{
	std::string pathPath =  LMMPATH::get_output_path() + filename;

	ofstream myfile;
	myfile.open(pathPath.c_str(), std::ios::app);
	
	myfile << "fullRank_,"<<fullRank_ <<","<<std::endl;
	myfile << "reducedRank_,"<<reducedRank_<<","<<std::endl;
	myfile << "reductionType_,"<<nameCorrelationReductionType()<<","<<std::endl;
	myfile <<std::endl;

	myfile << "originalCorrelMatrix_" << std::endl<< std::endl;
	for(size_t i=0; i<originalCorrelMatrix_.rows(); ++i)
	{
		for(size_t j=0; j<originalCorrelMatrix_.columns(); ++j)
		{
			myfile << originalCorrelMatrix_[i][j] << ",";
		}
		myfile << endl;
	}
	myfile << endl << endl; 


	myfile << "reducedRank MatrixB" << endl<< endl;
	if(reducedRank_ !=0)
	{
		for(size_t i=0; i<reducedCorrelMatrixB_.rows(); ++i)
		{
			for(size_t j=0; j<reducedCorrelMatrixB_.columns(); ++j)
			{
				myfile << reducedCorrelMatrixB_[i][j] << ",";
			}
			myfile << endl;
		}
	}
	else
	{
		myfile << "reducedRank = 0, so skip the printing. " << endl; 
	}
	myfile << endl << endl; 


	myfile << "reducedCorrelMatrixApprox_ :" << endl<< endl;
	if(reducedRank_ !=0)
	{
		for(size_t i=0; i<reducedCorrelMatrixApprox_.rows(); ++i)
		{
			for(size_t j=0; j<reducedCorrelMatrixApprox_.columns(); ++j)
			{
				myfile << reducedCorrelMatrixApprox_[i][j] << ",";
			}
			myfile << endl;
		}
	}
	else
	{
		myfile << "reducedRank = 0. so skip the printing. " << endl; 
	}
	myfile << endl << endl; 


	myfile << "difference between originalCorrelMatrix_ vs reducedCorrelMatrixApprox_" << endl;
	if(reducedRank_ !=0)
	{
		for(size_t i=0; i<fullRank_; ++i)
		{
			for(size_t j=0; j<fullRank_; ++j)
			{
				myfile << originalCorrelMatrix_[i][j] - reducedCorrelMatrixApprox_[i][j] << ",";
			}
			myfile << endl;
		}
	}
	else
	{
		myfile << "reducedRank = 0. so skip the printing. " << endl; 
	}

	myfile.close();
}

std::string Correlation::nameCorrelationReductionType()const
{
	switch (reductionType_)
	{
	case CorrelationReductionType::RA :
		return "RA";
	case CorrelationReductionType::PCA :
		return "PCA";
	case CorrelationReductionType::NON :
		return "NON";
	default:
		throw("Error: not valid CorrelationReductionType");
	}
}

// ------------------------------------------------------------------------------
//
//                          Shosemaker2_Correlation
//
// ------------------------------------------------------------------------------

Shosemaker2_Correlation::Shosemaker2_Correlation(size_t fullRank, size_t reducedRank, CorrelationReductionType::CorrelationReductionType& reductionType, double eta, double rhoInfty) // // YY: TODO don't like default parameter: use boost::defaultable<size_t> for reduceRank latter.
	: Correlation(fullRank,reducedRank, reductionType), 
	eta_(eta),
	rhoInfty_(rhoInfty)
{
	if(!checkParams())
		throw("Error in Shosemaker2_Correlation:: Parameters of class Correlation is not permitted.");
}

bool Shosemaker2_Correlation::checkParams() const
{
	std::cout << "condition not implemented yet... " << std::endl;
	return true;
}

double Shosemaker2_Correlation::correlOriginal(size_t index_i, size_t index_j) const
{
	size_t i = index_i + 1;  // convert i,j \in [0,rank-1] --> [1,rank]
	size_t j = index_j + 1;

	double coeff = -abs((int)i-(int)j)/(double)(fullRank_-1);

	double term1 = -log(rhoInfty_);
	double term2 = eta_
		* (i*i + j*j + i*j - 3*i*fullRank_ - 3*j*fullRank_ +3*i + 3*j +2*fullRank_*fullRank_ - fullRank_ - 4)
		/ ( (fullRank_-2)*(fullRank_-3) );

	double termSum = term1 + term2;
	return exp (coeff*termSum);
}



// ------------------------------------------------------------------------------
//
//                          Shosemaker3_Correlation
//
// ------------------------------------------------------------------------------
Shosemaker3_Correlation::Shosemaker3_Correlation(size_t fullRank, size_t reducedRank, CorrelationReductionType::CorrelationReductionType& reductionType, double eta1, double eta2, double rhoInfty)
	: Correlation(fullRank,reducedRank, reductionType),
	eta1_(eta1), 
	eta2_(eta2),
	rhoInfty_(rhoInfty)
{
	if(!checkParams())
		throw("Error in Shosemaker3_Correlation: Parameters of class Correlation is not permitted.");
}

bool Shosemaker3_Correlation::checkParams() const
{
	if(3*eta1_ < eta2_ || eta2_ <0 || 0 > eta1_+ eta2_ || eta1_+ eta2_ > -log(rhoInfty_))
		return false;
	else
		return true;
}

double Shosemaker3_Correlation::correlOriginal(size_t index_i, size_t index_j) const
{
	size_t i = index_i + 1;  // convert index: i,j \in [0,rank-1] --> [1,rank]
	size_t j = index_j + 1;

	double coeff = -abs((int)i-(int)j)/(double)(fullRank_-1);


	double term1 = -log(rhoInfty_);
	double term2 = eta1_
		* (i*i + j*j + i*j - 3*i*fullRank_ - 3*j*fullRank_ +3*i + 3*j +2*fullRank_*fullRank_ - fullRank_ - 4)
		/ ( (fullRank_-2)*(fullRank_-3) );
	double term3 = eta2_ 
		* (i*i + j*j + i*j - i*fullRank_ - j*fullRank_ -3*i -3*j + 3*fullRank_ + 2)
		/ ( (fullRank_-2)*(fullRank_-3) );

	double termSum = term1 + term2 + term3;
	return exp (coeff*termSum);
}



// ------------------------------------------------------------------------------
//
//                          Rebonato2_Correlation
//
// ------------------------------------------------------------------------------
Rebonato2_Correlation::Rebonato2_Correlation(size_t fullRank, size_t reducedRank, CorrelationReductionType::CorrelationReductionType& reductionType, double beta, double rhoInfty)
	: Correlation(fullRank,reducedRank, reductionType),
	beta_(beta),
	rhoInfty_(rhoInfty)
{
	if(!checkParams())
		throw("Error in Rebonato2_Correlation: Parameters of class Correlation is not permitted.");
}

bool Rebonato2_Correlation::checkParams() const
{
	std::cout << "condition not implemented yet... " << std::endl;
	//! YY: TODO:  not sure if this is the good condition
	if(beta_ > 0) 
		return true;
	else 
		return false;
}

double Rebonato2_Correlation::correlOriginal(size_t index_i, size_t index_j) const
{
	size_t i = index_i + 1;  // convert i,j \in [0,rank-1] --> [1,rank]
	size_t j = index_j + 1;

	double exp_index = -beta_*abs((int)i-(int)j);
	return rhoInfty_ + (1-rhoInfty_)* exp(exp_index);
}


// ------------------------------------------------------------------------------
//
//                          Rebonato3_Correlation
//
// ------------------------------------------------------------------------------

Rebonato3_Correlation::Rebonato3_Correlation(size_t fullRank, size_t reducedRank, CorrelationReductionType::CorrelationReductionType& reductionType, double alpha, double beta, double rhoInfty)
	: Correlation(fullRank, reducedRank, reductionType),
	alpha_(alpha), 
	beta_(beta),
	rhoInfty_(rhoInfty)
{
	if(!checkParams())
		throw("Error in Rebonato3_Correlation: Parameters of class Correlation is not permitted.");
}

bool Rebonato3_Correlation::checkParams() const
{
	std::cout << "condition not implemented yet... " << std::endl;
	//! YY: not sure if this is the good condition
	if(alpha_ < 0 && beta_ - alpha_ > 0) 
		return true;
	else 
		return false;
}

double Rebonato3_Correlation::correlOriginal(size_t index_i, size_t index_j) const// convert i,j \in [0,rank-1] --> [1,rank]
{
	size_t i = index_i + 1;
	size_t j = index_j + 1;

	double exp_index = -abs((int)i-(int)j)*(beta_-alpha_*(max(i,j)-1));
	return rhoInfty_ + (1-rhoInfty_)* exp(exp_index);
}



// ------------------------------------------------------------------------------
//
//                          XY_beta_Correlation
//
// ------------------------------------------------------------------------------

XY_beta_Correlation::XY_beta_Correlation(size_t fullRank, 
										 size_t reducedRank, 
										 CorrelationReductionType::CorrelationReductionType& reductionType,
										 double alpha, 
										 double beta)
										 : Correlation(fullRank, reducedRank, reductionType),
										 alpha_(alpha), 
										 beta_(beta)
{
	if(!checkParams())
		throw("Error in Rebonato3_Correlation: Parameters of class Correlation is not permitted.");
}

bool XY_beta_Correlation::checkParams() const
{
	std::cout << "condition not implemented yet... " << std::endl;
	if(beta_<0 || alpha_<0)
		throw("Error not permitted parameters");
	return true;
}

double XY_beta_Correlation::correlOriginal(size_t index_i, size_t index_j) const// convert i,j \in [0,rank-1] --> [1,rank]
{
	size_t i = index_i + 1;
	size_t j = index_j + 1;

	// e^  {- \beta |j-i| / (i+j)^{\alpha} }
	double numerator   = std::abs((double)j-i);
	double denominator = std::pow((double)i+j, alpha_);
	double fraction = numerator/denominator; 
	return exp(-beta_*fraction);
}

void XY_beta_Correlation::print(const std::string& filename) const 
{
	{
		std::string pathPath =  LMMPATH::get_output_path() + filename;
		ofstream myfile;

		myfile.open( pathPath.c_str() );
		myfile<<"XY_beta_Correlation,,alpha,"<<alpha_<<",beta,"<<beta_<<","<<std::endl<<std::endl;

		myfile.close();
	}

	printPlusPlus_matrices(filename);
}

// ------------------------------------------------------------------------------
//
//                          Const_Correlation
//
// ------------------------------------------------------------------------------

Const_Correlation::Const_Correlation  (size_t fullRank, 
									   size_t reducedRank, 
									   CorrelationReductionType::CorrelationReductionType& reductionType,
									   double correlConst)
									   :Correlation(fullRank,reducedRank,reductionType),
									   correlConst_(correlConst)
{}

bool Const_Correlation::checkParams() const
{
	if(correlConst_ <= 1.0 && correlConst_ >= -1.0)
		return true;
	else 
		return false;
}

double Const_Correlation::correlOriginal(size_t index_i, size_t index_j) const// convert i,j \in [0,rank-1] --> [1,rank]
{
	if(index_i != index_j)
		return correlConst_;
	else
		return 1.0;
}

void Const_Correlation::print(const std::string& filename) const 
{
	{
		std::string pathPath =  LMMPATH::get_output_path() + filename;
		ofstream myfile;

		myfile.open( pathPath.c_str() );
		myfile<<"Const_Correlation,, Constant Correlation,"<<correlConst_<<std::endl<<std::endl;

		myfile.close();
	}

	printPlusPlus_matrices(filename);
}