#pragma once

#include <iostream>
#include <cassert>
#include <string>
#include <cassert>
//#include <boost/numeric/ublas/matrix_proxy.hpp>

#include <Cheyette/CheyetteModel/CheyetteDD_Model.h>
#include <LMM/LmmModel/McLmm.h>  //pour le namespace MCSchemeType
#include <LMM/RNGenerator/RNGenerator.h>
#include <LMM/RNGenerator/McGenerator.h>

//#include <LMM/helper/TypeConverter.h>


class MC_Cheyette
{
private:
	CheyetteDD_Model_PTR		cheyetteDD_Model_ ;
	RNGenerator_PTR				rnGenerator_;

	double fwdProbaT_;							// define the numeraire
	//dates ou on veut recuperer le vecteur de simulation :
	std::vector<double> datesOfSimulation_;				//fractions d'année ex 0.5 Y , 1Y, 1.5Y, 2Y  de taille N
	std::vector<size_t> discretizationBetweenDates_ ;	//nb de simus entre 2 dates : 100 simus, ..., 100 simus de taille N

	mutable std::vector<double> x_t_Cheyette_;
	mutable std::vector<double> y_t_Cheyette_ ;

public:

	MC_Cheyette(	CheyetteDD_Model_PTR		cheyetteDD_Model,
					RNGenerator_PTR				rnGenerator,
					double						fwdProbaT,
					std::vector<double>&			datesOfSimulation,		
					std::vector<size_t>&			discretizationBetweenDates   )
				:cheyetteDD_Model_(cheyetteDD_Model), rnGenerator_(rnGenerator), fwdProbaT_(fwdProbaT), 
				datesOfSimulation_(datesOfSimulation), discretizationBetweenDates_(discretizationBetweenDates), 
				x_t_Cheyette_(datesOfSimulation.size()), y_t_Cheyette_(datesOfSimulation.size()) 
	{
			//remplit x_t_Cheyette_ et y_t_Cheyette_ aux dates voulues
			simulate_Euler() ;
			if (datesOfSimulation_.size() != discretizationBetweenDates_.size())
				throw "In MC_Cheyette, vectors datesOfSimulation and discretizationBetweenDates must have the same size" ;
	} 

	virtual ~MC_Cheyette(){} 
	
	//-- Getters 
	CheyetteDD_Model_PTR		getCheyetteDD_Model_() const{return cheyetteDD_Model_ ;}
	RNGenerator_PTR				getRNGenerator_() const{return rnGenerator_ ;}
	double						getFwdProbaT_() const{return fwdProbaT_ ;}
	std::vector<double>			getDatesOfSimulation_() const{return datesOfSimulation_ ;}
	std::vector<size_t>			getDiscretizationBetweenDates_() const{return discretizationBetweenDates_ ;}
	std::vector<double>			get_x_t_Cheyette_() const{return x_t_Cheyette_ ;}
	std::vector<double>			get_y_t_Cheyette_() const{return y_t_Cheyette_ ;}

	//remplit les vecteurs xt et yt aux dates de la tenor structure
	void simulate_Euler() const;


	//double get_numeraire(size_t index)				  const {return numeraires_[index];}  // P(t, fwdProbaT_), index of dateOfSimulation; 

};

typedef boost::shared_ptr<MC_Cheyette> MC_Cheyette_PTR;
typedef boost::shared_ptr<const MC_Cheyette> MC_Cheyette_CONSTPTR;
