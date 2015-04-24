#define BOOST_TEST_MODULE LMM_unit_test_model_Correlation
#include <boost/test/included/unit_test.hpp>

#include <ql/math/matrixutilities/symmetricschurdecomposition.hpp>

#include <LMM/helper/GenericPath.h>
#include <LMM/ModelMCLMM/Correlation.h>

//ctnrefactor write here a common CorrelConfig as test fixture
//-----------------------------------------------------------------------------------------------
//
//                                             Test 
//
//-----------------------------------------------------------------------------------------------
void test_Robonato2_Correlation(size_t fullRank, size_t reducedRank, CorrelationReductionType::CorrelationReductionType& reductionType)
{
	double beta  = 0.1;
	double rhoInfty = 0.0158;

	Correlation* rb2_Correlation = new Robonato2_Correlation(fullRank, reducedRank, reductionType, beta, rhoInfty);
	rb2_Correlation->calculate();


		//-- Put all results in Excel
	std::string fileName = LMM::get_output_path() + "unitTest_ModelMCLMM_Robonato2_Correlation.csv";
	rb2_Correlation->print_details(fileName);

	delete rb2_Correlation;
}

void test_Robonato3_Correlation(size_t fullRank, size_t reducedRank, CorrelationReductionType::CorrelationReductionType& reductionType)
{
	double alpha = -0.001;
	double beta  = 0.1;
	double rhoInfty = 0.0158;

	Correlation* rb3_Correlation = new Robonato3_Correlation(fullRank, reducedRank, reductionType, alpha, beta, rhoInfty);
	rb3_Correlation->calculate();

	std::string fileName = LMM::get_output_path() + "unitTest_ModelMCLMM_Robonato3_Correlation.csv";
	rb3_Correlation->print_details(fileName);

	delete rb3_Correlation;
}

void test_Shosemaker2_Correlation(size_t fullRank, size_t reducedRank, CorrelationReductionType::CorrelationReductionType& reductionType)
{
	double eta = 1.036;
	double rhoInfty = 0.0158;

	Correlation* sm2_Correlation = new Shosemaker2_Correlation(fullRank, reducedRank, reductionType, eta, rhoInfty);
	sm2_Correlation->calculate();

	std::string fileName = LMM::get_output_path() + "unitTest_ModelMCLMM_ShoseMaker2_Correlation.csv";
	sm2_Correlation->print_details(fileName);

	delete sm2_Correlation;
}

void test_Shosemaker3_Correlation(size_t fullRank, size_t reducedRank, CorrelationReductionType::CorrelationReductionType& reductionType)
{
	double eta1 = 1.036;
	double eta2 = 3.108;
	double rhoInfty = 0.0158;

	Correlation* sm3_Correlation = new Shosemaker3_Correlation(fullRank, reducedRank, reductionType, eta1, eta2, rhoInfty);
	sm3_Correlation->calculate();

	std::string fileName = LMM::get_output_path() + "unitTest_ModelMCLMM_ShoseMaker3_Correlation.csv";
	sm3_Correlation->print_details(fileName);

	delete sm3_Correlation;
}


void test_XY_beta_Correlation(size_t fullRank, size_t reducedRank, CorrelationReductionType::CorrelationReductionType& reductionType)
{
	double beta  = 0.3;
	double alpha = 0.5;

	Correlation* xyBeta_Correlation = new XY_beta_Correlation(fullRank, reducedRank, reductionType, alpha,beta);
	xyBeta_Correlation->calculate();

	std::string fileName = LMM::get_output_path() + "unitTest_ModelMCLMM_XY_beta_Correlation.csv";
	xyBeta_Correlation->print_details(fileName);

	delete xyBeta_Correlation;
}



void test_PCAEigenVectorRobonato2(size_t fullRank, size_t reducedRank) // to print out eigenVector. // don't care about the reduced rank.
{
	//Robonato2 
	double beta  = 0.1;
	double rhoInfty = 0.0158;

	CorrelationReductionType::CorrelationReductionType reductionType = CorrelationReductionType::PCA;
	Correlation* rb2_Correlation = new Robonato2_Correlation(fullRank, reducedRank, reductionType, beta, rhoInfty);
	rb2_Correlation->calculate();
	Matrix m = rb2_Correlation->get_originalCorrelMatrix();

	SymmetricSchurDecomposition ssd(m);
	Matrix eigenvectors = ssd.eigenvectors();

	std::string fileName = LMM::get_output_path() + "unitTest_ModelMCLMM_Robonato2_Correlation_PCA_egienVector_Robonato_2.csv";
	ofstream myfile;
	myfile.open(fileName.c_str());

	myfile << "Eigenvectors - in columns" << endl;
	for(size_t i=0; i<eigenvectors.rows(); ++i)
	{
		myfile << i << ",";
		for(size_t j=0; j<eigenvectors.columns(); ++j)
		{
			myfile << eigenvectors[i][j] << ",";
		}
		myfile << std::endl;
	}
	myfile.close();
	delete rb2_Correlation;
}



void test_PCAEigenVectorXY_beta(size_t fullRank, size_t reducedRank) // to print out eigenVector. // don't care about the reduced rank.
{
	//! XY beta
	double beta  = 0.3;
	double alpha = 0.5;



	CorrelationReductionType::CorrelationReductionType reductionType = CorrelationReductionType::PCA;
	Correlation* xyBeta_Correlation = new XY_beta_Correlation(fullRank, reducedRank, reductionType, alpha,beta);
	xyBeta_Correlation->calculate();
	Matrix m = xyBeta_Correlation->get_originalCorrelMatrix();

	SymmetricSchurDecomposition ssd(m);
	Matrix eigenvectors = ssd.eigenvectors();

	std::string fileName = LMM::get_output_path() + "unitTest_ModelMCLMM_Robonato2_Correlation_PCA_egienVector_xyBeta_Correlation.csv";

	ofstream myfile;
	myfile.open(fileName.c_str());

	myfile << "Eigenvectors - in columns" << endl;
	for(size_t i=0; i<eigenvectors.rows(); ++i)
	{
		myfile << i << ",";
		for(size_t j=0; j<eigenvectors.columns(); ++j)
		{
			myfile << eigenvectors[i][j] << ",";
		}
		myfile << std::endl;
	}
	myfile.close();
	delete xyBeta_Correlation;
}


BOOST_AUTO_TEST_SUITE(lmm_test_model_Correlation)

BOOST_AUTO_TEST_CASE(test_Correlation)
{
	try
	{
		size_t fullRank = 40;    // 40=15Y/6M
		size_t reducedRank = 3;
		CorrelationReductionType::CorrelationReductionType reductionType =  CorrelationReductionType::RA; // PCA, RA;

		//! TEST Correlation reduction approximation
		test_Shosemaker2_Correlation(fullRank,reducedRank,reductionType);
		test_Shosemaker3_Correlation(fullRank,reducedRank,reductionType);
		test_Robonato2_Correlation(fullRank,reducedRank,reductionType);
		test_Robonato3_Correlation(fullRank,reducedRank,reductionType);
		test_XY_beta_Correlation(fullRank,reducedRank,reductionType);

		//! Test PCA eigenVector
		test_PCAEigenVectorRobonato2(fullRank,reducedRank);
		test_PCAEigenVectorXY_beta(fullRank,reducedRank);

	}catch(string& message)
	{
		cout << "Error message: "  << message << endl;
	}

	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()

