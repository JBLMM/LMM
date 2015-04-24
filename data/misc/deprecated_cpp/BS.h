#ifndef BS_H
#define BS_H

class BS 
{
public:
	
	// Compute a derivative's price using Black's formula
	double BlackFormula(double forward,
						double strike,
						double sigma,
						double discount);


};


#endif