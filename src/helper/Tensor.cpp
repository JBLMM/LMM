#include <LMM/helper/Tensor.h>

#include <string>
#include <fstream>

#include <cassert>

Tensor::Tensor(size_t size1, size_t size2, size_t size3) 
: tensor_(size1, matrix(size2, size3,-1000))
{}

double & Tensor::operator()(size_t index_T, size_t indexLibor_i, size_t indexLibor_j) 
{
	//! YY Attention: This is a special condition for My LMM implementation, not general tensor condition.
	//! YY TODO: remove this check when the code is stable.
	assert(index_T >=1 && indexLibor_i>=index_T && indexLibor_j >= index_T);
	return tensor_[index_T](indexLibor_i, indexLibor_j);
}

const double & Tensor::operator()(size_t index_T, size_t indexLibor_i, size_t indexLibor_j) const
{
	//! YY Attention: This is a special condition for My LMM implementation, not general tensor condition.
	//! YY TODO: remove this check when the code is stable.
	assert(index_T >=1 && indexLibor_i>=index_T && indexLibor_j >= index_T);
	return tensor_[index_T](indexLibor_i, indexLibor_j);
}


const std::vector<Tensor::matrix>& Tensor::get_tensor() const { return tensor_; }

void Tensor::set_tensor(const std::vector<Tensor::matrix> & val) { tensor_ = val;}

const Tensor::matrix& Tensor::operator[](size_t indexT) const
{ 
	assert(indexT >=1);
	return tensor_[indexT];
}

void Tensor::write_to_stream(std::ostream& outputstream)const
{
	std::cout<<"Tensor;"<<std::endl<<std::endl;

	const unsigned int nbMatrix = tensor_.size();
	
	for(unsigned int iM=0;iM<nbMatrix;++iM)
	{
		outputstream <<" - Matrix("<<iM<<")th;"<<std::endl;

		const unsigned int nbLine = tensor_[iM].size1();
		const unsigned int nbCol  = tensor_[iM].size2();
		
		for(unsigned int i=0;i<nbLine;++i)
		{
			for(unsigned int j=0;j<nbCol;++j)
			{
				outputstream<<tensor_[iM](i,j)<<" ; ";
			}
			outputstream<<std::endl;		
		}	
		outputstream<<std::endl<<std::endl;
	}
}

void Tensor::print(const std::string& filename) const 
{
	std::string path_OutPut = LMMPATH::get_output_path() + filename;
	std::ofstream tensorOuput;
	
	tensorOuput.open(path_OutPut.c_str());
	tensorOuput<< *this <<std::endl;
	tensorOuput.close();

}

std::ostream& operator<<(std::ostream& os, const Tensor& m_tensor)
{
	m_tensor.write_to_stream(os); return os;
}