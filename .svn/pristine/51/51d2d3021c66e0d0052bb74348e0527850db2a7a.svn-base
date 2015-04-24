#include <LMM/Test/Tests.h>
#include <LMM/helper/Noise.h>

#include <iostream>
#include <fstream>
#include <LMM/helper/GenericPath.h>

#include <climits>//test

void test_Noise()
{
	std::string file_name =  LMMPATH::get_output_path()  + "test_Noise.csv";

	std::ofstream noise_file;
	noise_file.open(file_name.c_str() );
	
	noise_file << "uniform_mul_noise , bernouilli_mul_noise , uniform_add_noise , ------, constant seed"<<std::endl;
	for(size_t iTest=0;iTest<10;++iTest)
	{
		std::cout<<std::endl<<std::endl;

		size_t nbSample = 10 ; 
		std::vector<double> uniform_mul_noise = Multiplicative::UniformNoise(nbSample,0.04,0);

		std::vector<double> bernouilli_mul_noise = Multiplicative::BernouilliNoise(nbSample,0.04,0);

		std::vector<double> uniform_add_noise = Additive::UniformNoise(nbSample,0.05,0);

		for(size_t i=0;i<nbSample;++i)
		{
			noise_file<<uniform_mul_noise[i]<<","<<bernouilli_mul_noise[i]<<","<<uniform_add_noise[i]<<","<<std::endl;
		}		noise_file<<std::endl;
	}

	noise_file<<std::endl<< "uniform_mul_noise , bernouilli_mul_noise , uniform_add_noise , ------, 'random' seed"<<std::endl;
	for(size_t iTest=0;iTest<10;++iTest)
	{
		std::cout<<std::endl<<std::endl;

		size_t nbSample = 10 ; 
		std::vector<double> uniform_mul_noise = Multiplicative::UniformNoise(nbSample,0.04,NoiseHelper::get_Seed());

		std::vector<double> bernouilli_mul_noise = Multiplicative::BernouilliNoise(nbSample,0.04,NoiseHelper::get_Seed());

		std::vector<double> uniform_add_noise = Additive::UniformNoise(nbSample,0.05,NoiseHelper::get_Seed());

		for(size_t i=0;i<nbSample;++i)
		{
			noise_file<<uniform_mul_noise[i]<<","<<bernouilli_mul_noise[i]<<","<<uniform_add_noise[i]<<","<<std::endl;
		}		noise_file<<std::endl;
	}

	noise_file<< " seed helper"<<std::endl;
	for(size_t iTest=0;iTest<100;++iTest)
	{
		noise_file<<NoiseHelper::get_Seed()<<","<<std::endl;
	}

	noise_file.close();
}

