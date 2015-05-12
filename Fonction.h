#pragma once

#include <vector>
#include <iostream>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

/***********************************************************
******* fonctions de R dans R 
***********************************************************/

class RR_Function  // fonction de R dans R
{
public:
	boost::function<double(double)> func_;

	//virtual double operator ()(double x) const = 0 ;
	virtual double evaluate(double x) = 0;
};



//fonction constante (pas très utile)
class constant_RR_Function : public  RR_Function // R->R
{
private :
	double y_ ;
public:
	constant_RR_Function(double y) : y_(y) {std::cout << "creation d'une fonction constante" << std::endl ;}
	boost::function<double(double)> func_;
	//RRFunction(const boost::function<double(double)>& func) : func_(func){};
	virtual ~constant_RR_Function(){};

	virtual double constant_RR_Function::evaluate(double x) {return y_;}

	double constant_RR_Function::operator ()(double x) const{ return func_(x) ;}  // this->evaluate(x)
};

//fonction constante par morceaux avec convention cadlag
class piecewiseconst_RR_Function : public RR_Function 
{
private :
	//si x est de longueur N, y doit être de longueur N-1
	std::vector<double> x_; //abscisses 
	std::vector<double> y_; //ordonnees

	//x0 = xmin ----------  x1 --------- x2 --------- x3 = xmax 
	//    [         y1       [     y2     [       y3    ]

public :
	//constructeur par défaut
	piecewiseconst_RR_Function(){
		std::vector<double> v_x, v_y ;
		v_x.push_back(0) ; v_x.push_back(1) ; v_y.push_back(1) ; 
		x_ = v_x ; y_ = v_y ;
	} 
	//constructeur
	piecewiseconst_RR_Function(std::vector<double> x, std::vector<double> y) : x_(x), y_(y) {} 

	//getters
	std::vector<double> piecewiseconst_RR_Function::getx_(){return x_ ;} 
	std::vector<double> piecewiseconst_RR_Function::gety_(){return y_ ;}

	// find interval and return the corresponding y : convention càdlàg
	// traiter cas extreme: extrapolation ! 
	double piecewiseconst_RR_Function::evaluate(double t){
		assert(x_[0] <= t || t <= x_[x_.size() - 1] ) ;
		assert(y_.size() +  1 == x_.size()) ; 
		double res ; 
		if (t == x_[x_.size() - 1]){
			res = y_[y_.size() - 1] ;
		}else{
			size_t i = 0 ;
			bool boolean ; 
			boolean = (x_[i] <= t && t < x_[i+1]) ;
			while (!boolean && i < (x_.size() - 2)){
				++i ;
				boolean = (x_[i] < t && t < x_[i+1]) ;
			}
			res = y_[i] ;
		}
		return (res) ;
	}

};

/***********************************************************
******* fonctions de R^2 dans R 
***********************************************************/

class R2R_Function 
{
public:
	boost::function<double(double, double)> func_;

	virtual double evaluate(double x, double y) = 0;
};

class piecewiseconst_R2R_Function : public R2R_Function 
{
private :  //fonction de x à t fixé
	//idem convention cadlag et longueur t_ = longueur f_ + 1
	std::vector<double> t_;
	std::vector<piecewiseconst_RR_Function> f_; //fonction de x

	//t0 ----------  t1 ----------- t2 ---------- t3 = T 
	// [     f1(x)    [     f2(x)    [      f3(x)    ]

public :
	piecewiseconst_R2R_Function(){}

	piecewiseconst_R2R_Function(std::vector<double> t, std::vector<piecewiseconst_RR_Function> f) : t_(t), f_(f) {} 

	// find interval and return the corresponding y : convention càdlàg
	// traiter cas extreme: extrapolation ! 
	double piecewiseconst_R2R_Function::evaluate(double t, double x){
		assert(t_[0] <= t || t <= t_[t_.size() - 1] ) ;
		assert(f_.size() +  1 == t_.size()) ; 
		double res ;
		piecewiseconst_RR_Function f_CM ;
		if (t == t_[t_.size() - 1]){
			f_CM = f_[f_.size() - 1] ;
		}else{
			size_t i = 0 ;
			bool boolean ; 
			boolean = (t_[i] <= t && t < t_[i+1]) ;
			while (!boolean && i < (t_.size() - 2)){
				++i ;
				boolean = (t_[i] < t && t < t_[i+1]) ;
			}
			f_CM = f_[i] ;
		}
		res = f_CM.evaluate(x) ;
		return (res) ;
	}
};


class piecewiseconst_DD_R2R_Function : public piecewiseconst_R2R_Function
{
private :
	piecewiseconst_RR_Function m_;		//m(t) (skew)
	piecewiseconst_RR_Function sigma_;	//sigma(t) (niveau de la vol)
	double L_ ;							//constante

public :  //à t fixé, c'est une fonction de x
	piecewiseconst_DD_R2R_Function(){}
	piecewiseconst_DD_R2R_Function(piecewiseconst_RR_Function m, piecewiseconst_RR_Function sigma, double L) 
		: m_(m), sigma_(sigma), L_(L)
	{
		// check pillar m & sigma is the same! :
		assert(m.getx_() == m.gety_()) ;
	} 

	double piecewiseconst_DD_R2R_Function::sigma_r(double t, double x)
	{
		return (m_.evaluate(t) * x + (1-m_.evaluate(t))*L_) * sigma_.evaluate(t);
		//à améliorer avec l'opérateur ()
	}

	double piecewiseconst_DD_R2R_Function::d_sigma_r_dx(double t) //, double x)
	{
		return (m_.evaluate(t) *sigma_.evaluate(t));
	}

	piecewiseconst_RR_Function get_m(){return m_ ;}
	piecewiseconst_RR_Function get_sigma(){return sigma_ ;}
	double getL(){return L_ ;}

};