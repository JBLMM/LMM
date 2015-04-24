#pragma once
#include <vector>


/*! get_Seed return seed with high precision, in nano second order,
 *  That help the noises generator to have more 'randomness'
 */
namespace NoiseHelper
{
	unsigned int get_Seed();
}


/*! Define some helper for noises, used when one desire to perturbe some thing with a small random noise
 *
 *		- use Multiplicative noise for relative noised values
 *      - use Additive       noise for absolute noised values
 */

/*!  Multiplicative noises help to sample real value around 1
 *   This sample multiply with original value give a relative noised values
 */
namespace Multiplicative
{
	//! return a vector of sample uniformly distributed in intervall [1-eps, 1+eps]
	//! If user desire generate noises with different seed in order to have different noise , use NoiseHelper::get_Seed()
	std::vector<double> UniformNoise(size_t nbSample, const double& epsilon, unsigned int seed_value);
	
	//! return a vector of sample flowing Bernouilly(0.5) distribution, with only two possible values {1-eps, 1+eps}
	//! If user desire generate noises with different seed in order to have different noise , use NoiseHelper::get_Seed()
	std::vector<double> BernouilliNoise(size_t nbSample, const double& epsilon, unsigned int seed_value);
}

/*!  Additive noises help to sample real value around 0
 *   This sample add with original value give a absolute noised values
 */
namespace Additive
{
	//! return a vector of sample uniformly distributed in intervall [-eps, +eps]
	//! If user desire generate noises with different seed in order to have different noise , use NoiseHelper::get_Seed()
	std::vector<double> UniformNoise(size_t nbSample, const double& epsilon, unsigned int seed_value);
}