#include <Cheyette\Pricer\MC_Cheyette.h>
	

void MC_Cheyette::simulate_Euler() const
{
	
	//initialisation
	x_t_Cheyette_[0] = 0  ;	y_t_Cheyette_[0] = 0;

	double dt, t, t_plus_dt, x_t, x_t_plus_dt, y_t, y_t_plus_dt ;	
	x_t = 0 ; y_t = 0 ; t = 0 ;

	for (size_t date = 0; date < datesOfSimulation_.size() ; ++date)    
	{ 
		std::cout << "a voir pour changer la graine a chaque nouvelle simulation" << std::endl ;
		std::vector<double>   gaussian_tmp(discretizationBetweenDates_[date]);  
		rnGenerator_->generate(gaussian_tmp);		// generate Gaussian.
		if (date == 0)
		{
			dt = datesOfSimulation_[date] / discretizationBetweenDates_[date] ;
		}
		else
		{
			dt = (datesOfSimulation_[date] - datesOfSimulation_[date-1]) / discretizationBetweenDates_[date] ;
		}

		t_plus_dt = t ;
		////DEBUG
		//for (size_t i = 0 ; i < gaussian_tmp.size() ; ++i)    
		//{			
		//	std::cout << "gaussian_tmp " << i << " : " << gaussian_tmp[i] << std::endl ;
		//}

		for (size_t pasDiscretisation = 1 ; pasDiscretisation <= discretizationBetweenDates_[date] ; ++pasDiscretisation)    
		{

			t_plus_dt += dt ;

			x_t_plus_dt =  x_t	+ cheyetteDD_Model_->drift_x_QT(t, fwdProbaT_, x_t, y_t) * dt 
								+ cheyetteDD_Model_->diffusion_x_QT(t, x_t) * sqrt(dt) * gaussian_tmp[pasDiscretisation - 1] ;
			y_t_plus_dt =  y_t	+ cheyetteDD_Model_->drift_y_QT(t, x_t, y_t) * dt ;
			x_t = x_t_plus_dt ;
			y_t = y_t_plus_dt ;
			t = t_plus_dt ;
		}
		std::cout << "date t : " << t << ", valeur de x_t : " << x_t_plus_dt << ", valeur de y_t : " << y_t_plus_dt << std::endl ; 
		x_t_Cheyette_[date] = x_t_plus_dt ;
		y_t_Cheyette_[date] = y_t_plus_dt ;
	}
}


