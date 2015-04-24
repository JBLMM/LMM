#include <LMM/helper/Noise.h>

#include <ctime>
#include <cassert>
#include <thread>         // std::this_thread::sleep_for
#include <chrono> 

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/bernoulli_distribution.hpp>

namespace NoiseHelper
{
	unsigned int get_Seed()
	{
		//not urgence
		//todo pauses the system about a nanoseconds or miliseconds in order to have different seed each time call
		auto now = std::chrono::high_resolution_clock::now();
		auto nano_timer = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
		return static_cast<unsigned long> (nano_timer) ;
	}
}



namespace Multiplicative
{

	std::vector<double> UniformNoise(size_t nbSample, const double& percent, unsigned int seed_value)
	{
		assert(percent<1.); // percentage perturbation has to be in interval [0,1[
		if(percent>0.)
		{
			boost::random::mt19937 gen;
			boost::random::uniform_real_distribution<> dist(1-percent,1+percent);
			boost::variate_generator<boost::mt19937&,boost::random::uniform_real_distribution<> > rng_uniform(gen, dist);

			rng_uniform.engine().seed( seed_value  ); 

			std::vector<double> uniform_noise_vector( nbSample );
			for(size_t i=0;i<nbSample;++i)
			{  
				uniform_noise_vector[i] = rng_uniform(); 
			}

			return uniform_noise_vector;
		}
		else 
			return std::vector<double>(nbSample,1.);
	}


	std::vector<double> BernouilliNoise(size_t nbSample, const double& percent, unsigned int seed_value)
	{
		assert(percent<1.); // percentage perturbation has to be in interval [0,1[
		if(percent>0.)
		{
			boost::random::mt19937 gen;
			boost::random::bernoulli_distribution<> bernouilli_dist(0.5);
			boost::variate_generator<boost::mt19937&,boost::random::bernoulli_distribution<> > rng_bernouilli(gen, bernouilli_dist);

			rng_bernouilli.engine().seed( seed_value  );

			std::vector<double> bernouilli_noise_vector( nbSample );
			for(size_t i=0;i<nbSample;++i)
			{   
				//! transform bernouilli value {0,1} to {1-eps , 1+eps}
				bernouilli_noise_vector[i] = 1.+ percent*(2.*rng_bernouilli() - 1.); 
			}

			return bernouilli_noise_vector;
		}
		else 
			return std::vector<double>(nbSample,1.);
	}
}

namespace Additive
{
	std::vector<double> UniformNoise(size_t nbSample, const double& epsilon, unsigned int seed_value)
	{
		assert(epsilon >= 0.); // noise has to be positive or null
		if(epsilon>0.)
		{
			boost::random::mt19937 gen;
			boost::random::uniform_real_distribution<> dist(-epsilon,epsilon);
			boost::variate_generator<boost::mt19937&,boost::random::uniform_real_distribution<> > rng_uniform(gen, dist);

			rng_uniform.engine().seed( seed_value ); 

			std::vector<double> uniform_noise_vector( nbSample );
			for(size_t i=0;i<nbSample;++i)
			{  
				uniform_noise_vector[i] = rng_uniform(); 
			}

			return uniform_noise_vector;
		}
		else 
			return std::vector<double>(nbSample,0.);	
	}
}