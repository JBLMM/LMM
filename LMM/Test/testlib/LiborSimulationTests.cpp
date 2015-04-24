//#include "LiborSimulationTests.h"
//
//using namespace std;
//
//template<class T>
//void gen_normal(T & generator, std::vector<double> &res)
//{
//	for(size_t i = 0; i < res.size(); ++i)
//		res[i] = generator();
//}
//
//
//void showIntitialLiborMatrix()
//{
//
//	//--------------------------------------------------//
//	//             PUT IT IN VOL TEST CLASS             //
//	//--------------------------------------------------//
//
//	Real a = -0.06; 
//	Real b = 0.17; 
//	Real c = 0.54; 
//	Real d = 0.17;
//
//	std::vector<Real> abcdParams(4);
//	abcdParams[0] = a;
//	abcdParams[1] = b;
//	abcdParams[2] = c;
//	abcdParams[3] = d;
//
//	size_t N = 4; // Index of the last libor 
//	size_t nbFactors = 3; // number of factors for LMM
//	Real tenorLibor = 0.5; // = 6M per LIBOR
//
//	Rate r = 0.02;
//
//	std::vector<Time> tenorPeriods(N+1);
//	for (size_t i = 0; i < N+1; ++i) 
//		tenorPeriods[i] = tenorLibor;
//
//	
//	//-- Constant parameters for abcd vol structure
//	std::vector<double> g(N+1);
//	for (size_t i = 0; i < N+1; ++i)
//		g[i] = 1.;
//
//	DeterministicVol vol(nbFactors,N+1,abcdParams,g);
//
//	//--------------------------------------------------//
//
//
//	//-- RNG with fixed seed - for testing and comparisons with Excel
//	boost::variate_generator<boost::mt19937, boost::normal_distribution<> >
//		generator(boost::mt19937(0.5),
//		boost::normal_distribution<>());
//
//	std::vector<double> liborShifts(N+1);
//		for (size_t i = 0; i < N+1; ++i)
//			liborShifts[i] = 0.01;
//
//	//-- TODO : Take libors from linear interpolation 
//	std::vector<double> libors_init(N+1);
//	for (size_t i = 0; i < N+1; ++i)
//		libors_init[i] = 0.02013;
//
//	McTerminalLmm model(N,tenorPeriods,vol,liborShifts,libors_init);
//
//	model.printLiborMatrix();
//}
//
//
//void liborSimul_terminal_euler(std::vector<std::vector<double>> &lmm,
//							   const std::vector<std::vector<std::vector<double>>> &covTensor)
//{
//	Real a = -0.06; 
//	Real b = 0.17; 
//	Real c = 0.54; 
//	Real d = 0.17;
//
//	std::vector<Real> abcdParams(4);
//	abcdParams[0] = a;
//	abcdParams[1] = b;
//	abcdParams[2] = c;
//	abcdParams[3] = d;
//
//	size_t N = 4; // Index of the last libor 
//	size_t nbFactors = 3; // number of factors for LMM
//	Real tenorLibor = 0.5; // = 6M per LIBOR
//
//	Rate r = 0.02;
//
//	std::vector<Time> tenorPeriods(N+1);
//	for (size_t i = 0; i < N+1; ++i) 
//		tenorPeriods[i] = tenorLibor;
//
//	
//	//-- Constant parameters for abcd vol structure
//	std::vector<double> g(N+1);
//	for (size_t i = 0; i < N+1; ++i)
//		g[i] = 1.;
//
//	DeterministicVol vol(nbFactors,N+1,abcdParams,g);
//
//	//-- RNG with fixed seed - for testing and comparisons with Excel
//		boost::variate_generator<boost::mt19937, boost::normal_distribution<> >
//			generator(boost::mt19937(0.5),
//			boost::normal_distribution<>());
//
//	std::vector<double> liborShifts(N+1);
//		for (size_t i = 0; i < N+1; ++i)
//			liborShifts[i] = 0.01;
//
//	//-- TODO : Take libors from linear interpolation 
//	std::vector<double> libors_init(N+1);
//
//	McTerminalLmm model(N,tenorPeriods,vol,liborShifts,libors_init);
//
//	matrix_ gaussianMatrix(nbFactors,std::vector<double>(N+1));
//	std::vector<double> G(nbFactors,0.);
//	for (size_t k = 0; k < N+1; ++k) {
//		gen_normal(generator,G);
//		for (size_t i = 0; i < nbFactors; ++i)
//			gaussianMatrix[i][k] = G[i];
//	}
//
//	model.TerminalLmmModel_Euler();
//	matrix_ lmm = model.getLiborMatrix();
//	cout << "---- > TERMINAL EULER : " << endl << endl;
//	for(size_t i = 0; i < lmm.size(); ++i) 
//	{
//		for (size_t j = 0; j < lmm[0].size(); ++j)
//			cout << lmm[i][j] << " ";
//
//		cout << endl << endl;
//	}
//}
//
//void liborSimul_terminal_pc(McLmm& lmmModel,
//							std::vector<std::vector<double>> &lmm,
//							const std::vector<std::vector<std::vector<double>>> &covTensor)
//{
//	lmmModel.LmmSimulation(ipc);
//	lmm = lmmModel.getLiborMatrix();
//	cout << "---- > TERMINAL PC : " << endl << endl;
//	for(size_t i = 0; i < lmm.size(); ++i) 
//	{
//		for (size_t j = 0; j < lmm[0].size(); ++j)
//			cout << lmm[i][j] << " ";
//
//		cout << endl << endl;
//	}
//}
//
//
//void liborSimul_spot_euler(McLmm& lmmModel,
//						   std::vector<std::vector<double>> &lmm,
//						   const std::vector<std::vector<std::vector<double>>> &covTensor)
//{
//	lmmModel.LmmSimulation(euler);
//	lmm = lmmModel.getLiborMatrix();
//	cout << "---- > SPOT EULER : " << endl << endl;
//	for(size_t i = 0; i < lmm.size(); ++i) 
//	{
//		for (size_t j = 0; j < lmm[0].size(); ++j)
//			cout << lmm[i][j] << " ";
//
//		cout << endl << endl;
//	}
//}
//
//void liborSimul_spot_pc(McLmm& lmmModel,
//						std::vector<std::vector<double>> &lmm,
//						const std::vector<std::vector<std::vector<double>>> &covTensor)
//{
//	lmmModel.LmmSimulation(ipc);
//	lmm = lmmModel.getLiborMatrix();
//	cout << "---- > SPOT PC : " << endl << endl;
//	for(size_t i = 0; i < lmm.size(); ++i) 
//	{
//		for (size_t j = 0; j < lmm[0].size(); ++j)
//			cout << lmm[i][j] << " ";
//
//		cout << endl << endl;
//	}
//}