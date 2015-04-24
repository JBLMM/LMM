//#include <fstream>
//
//#include "SwaptionCostFunction.h"
//#include "Approximation.h"
//#include "NumericalMethods.h"
//
//SwaptionCostFunction::SwaptionCostFunction(const matrix_& swaptionMatrix,
//										   const matrix_& swaptionStrikes,
//
//										   const indexMatrix& floatingIndexMatrix,
//										   matrix_ swapRates,
//										   matrix_ annuities,
//										   std::vector<double> bonds_0,
//										   std::vector<double> libors_0,
//										   std::vector<double> libor_shifts,
//										   const Approximation& approximation,
//										   
//										   matrix_ weights,
//										   matrix_ weights_maturity,
//										   matrix_ weights_tenor,
//										   matrix_ weights_maturity_2,
//										   matrix_ weights_tenor_2)
//
//	: swaptionMatrix_Mkt_(swaptionMatrix),swaptionStrikes_(swaptionStrikes),floatingIndexMatrix_(floatingIndexMatrix),
//	  swapRates_(swapRates),annuities_(annuities),bonds_0_(bonds_0),libors_0_(libors_0),libor_shifts_(libor_shifts),
//	  approximation_(approximation), 
//	  weights_(weights),weights_maturity_(weights_maturity),weights_tenor_(weights_tenor),
//	  weights_maturity_2_(weights_maturity_2),weights_tenor_2_(weights_tenor_2)
//
//{
//	//-- Shift swap rates and strikes
//	for (size_t i = 0; i < swapRates.size(); ++i)
//	{
//		for (size_t j = 0; j < swapRates[i].size(); ++j)
//			approximation_.accumulateShiftedSwapRateAndStrike(swapRates_[i][j],swaptionStrikes_[i][j],bonds_0_,libor_shifts_,floatingIndexMatrix_[i][j],annuities_[i][j]);
//		
//	}
//}
//
//SwaptionCostFunction::~SwaptionCostFunction(){}
//
//Real SwaptionCostFunction::value(const Array& x) const 
//{
//	size_t nbRows = swaptionMatrix_Mkt_.size();
//	size_t nbCols = swaptionMatrix_Mkt_[0].size();
//
//	Array tmpRes = values(x);
//	Array weightArray = map_MatrixtoArray(weights_);
//
//	Real res = 0;
//	for (size_t i = 0; i < tmpRes.size(); ++i)
//	{
//		tmpRes[i] *= weightArray[i];
//		res += tmpRes[i]*tmpRes[i];
//	}
//
//	res = sqrt(res);
//
//	Real sum_of_weights = 0.;
//	for each( std::vector<double> vec in weights_)
//	{
//		for each (double weight in vec)
//			sum_of_weights += weight;
//	}
//
//	res /= sum_of_weights;
//	res += regularisation(x,0.1,0.05,0.05,0.08);
//	
//	return res;
//}
//
//
////-- Array x is obtained by mapping the vol matrix
//Disposable<Array> SwaptionCostFunction::values(const Array& x) const
//{
//	
//	size_t nbRows = swaptionMatrix_Mkt_.size();
//	size_t nbCols = swaptionMatrix_Mkt_[0].size();
//
//	//-- Vol matrix obtained with model
//	matrix_ swaptionVol_Mdl(nbRows,std::vector<double>(nbCols));
//	
//
//	for (size_t i = 0; i < nbRows; ++i)
//	{
//		for (size_t j = 0; j < swapRates_[i].size(); ++j)
//		{
//			if ((floatingIndexMatrix_[i][j]).size() == 0)
//				 continue;
//
//			size_t indexStart = floatingIndexMatrix_[i][j][0]-1;
//			swaptionVol_Mdl[i][j] = approximation_.computeRebonatoVolatility(x,indexStart,swapRates_[i][j],annuities_[i][j],floatingIndexMatrix_[i][j],bonds_0_,libors_0_,libor_shifts_);
//			swaptionVol_Mdl[i][j] = sqrt(swaptionVol_Mdl[i][j]);
//		}
//	}
//
//	Array swaptionArray_Mkt = map_MatrixtoArray(swaptionMatrix_Mkt_);
//	Array swaptionArray_Mdl = map_MatrixtoArray(swaptionVol_Mdl);
//	Array values(swaptionArray_Mkt.size());
//
//	for (size_t i = 0; i < values.size(); ++i)
//		values[i] = swaptionArray_Mdl[i] - swaptionArray_Mkt[i];
//
//	return values;
//}
//
//Real SwaptionCostFunction::regularisation(const Array& x, Real c1, Real c2, Real c3, Real c4) const
//{
//	//-- Map array x to matrix;
//	matrix_ H = map_ArrayToMatrix(x);
//
//	//-- Compute derivatives
//	matrix_ derivatives_maturity;
//	
//	matrix_ derivatives_tenor;
//	matrix_ derivatives_maturity_2;
//	matrix_ derivatives_tenor_2;
//
//    double sum_derivatives_maturity = 0.;
//	for (size_t i = 0; i < H.size(); ++i)
//	{
//		for (size_t j = 0; j < H[i].size()-1; ++j)
//			sum_derivatives_maturity += weights_maturity_[i][j] * abs(H[i][j+1]-H[i][j]);
//	}
//	sum_derivatives_maturity *= (c1/sum_all_weights_regularisation(weights_maturity_));
//
//	double sum_derivatives_tenor = 0;
//	for (size_t i = 0; i < H.size()-1; ++i)
//	{
//		for (size_t j = 0; j < H[i].size(); ++j)
//			sum_derivatives_tenor += weights_tenor_[i][j] * abs(H[i+1][j]-H[i][j]);
//	}
//	sum_derivatives_tenor *= (c2/sum_all_weights_regularisation(weights_tenor_));
//
//	double delta = 1.;
//	double sum_derivatives2_maturity = 0;
//	for (size_t i = 0; i < H.size(); ++i)
//	{
//		for (size_t j = 0; j < H[i].size()-2; ++j)
//			sum_derivatives2_maturity += weights_maturity_2_[i][j] * abs(H[i][j+2]-2*H[i][j+1]+H[i][j])/delta;
//	}
//	sum_derivatives2_maturity *= (c3/sum_all_weights_regularisation(weights_maturity_2_));
//
//	double sum_derivatives2_tenor = 0;
//	for (size_t i = 0; i < H.size()-2; ++i)
//	{
//		for (size_t j = 0; j < H[i].size(); ++j)
//			sum_derivatives2_tenor += weights_tenor_2_[i][j] * abs(H[i+2][j]-2*H[i+1][j]+H[i][j])/delta;
//	}
//	sum_derivatives2_tenor *= (c4/sum_all_weights_regularisation(weights_tenor_2_));
//	
//	Real regulator = sum_derivatives_maturity + sum_derivatives_tenor + sum_derivatives2_maturity + sum_derivatives2_tenor;
//
//	return regulator;
//}
//
//
//Real SwaptionCostFunction::sum_all_weights_regularisation(const matrix_& weights) const 
//{
//	Real sum = 0.;
//	for (size_t i = 0; i < weights.size(); ++i)
//	{
//		for (size_t j = 0; j < weights[i].size(); ++j)
//			sum += weights[i][j];
//	}
//	return sum;
//}
//
////Real SwaptionCostFunction::sum_all_derivatives_regularisation(const matrix_& weights, const matrix_& derivatives)
////{
////	Real sum = 0.;
////	for (size_t i = 0; i < weights.size(); ++i)
////	{
////		for (size_t j = 0; j < weights[0].size(); ++j)
////			sum += weights[i][j] * derivatives[i][j];
////	}
////	return sum;
////}
//
//void SwaptionCostFunction::getMarketInfo(string fileName)
//{
//	matrix_ result;
//
//	ifstream inputFile;
//	inputFile.open(fileName);
//
//	inputFile.close();
//}
//
//matrix_ SwaptionCostFunction::map_ArrayToMatrix(const Array& x) const
//{
//	matrix_ H;
//	size_t x_size = x.size();
//	size_t nb_alloc = 1;
//	size_t current_index = 0;
//
//	while (current_index < x_size)
//	{
//		std::vector<double> H_row;
//		for (size_t i = 0; i < nb_alloc; ++i)
//		{
//			H_row.push_back(x[current_index]);
//			current_index++;
//		}
//		H.push_back(H_row);
//		nb_alloc++;
//	}
//	
//	for (size_t i = 0; i < H.size(); ++i)
//	{
//		size_t current_index = H[i].size();
//		for (size_t j = current_index; j < H.size(); ++j)
//			H[i].push_back(0.);
//	}
//
//	return H;
//}
//
//Array SwaptionCostFunction::map_MatrixtoArray(const matrix_& mat) const
//{
//	std::vector<double> tmpRes;
//
//	for each (std::vector<double> vect in mat)
//	{
//		for each (double val in vect)
//		{
//			if (val == 0.)
//				continue;
//
//			tmpRes.push_back(val);
//		}
//	}
//
//	size_t nbVol = tmpRes.size();
//	Array res(nbVol);
//	for (size_t i = 0; i < nbVol; ++i)
//		res[i] = tmpRes[i];
//
//	return res;
//}
//
//size_t SwaptionCostFunction::get_swaptionMatrixRows() {return swaptionMatrix_Mkt_.size();}
//
