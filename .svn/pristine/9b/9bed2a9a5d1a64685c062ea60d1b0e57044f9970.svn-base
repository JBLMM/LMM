#ifndef LMM_HELPER_PRINTER_H
#define LMM_HELPER_PRINTER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/numeric/ublas/triangular.hpp>  // Triangular matrix

#include <LMM/helper/GenericPath.h>



//const std::string printPathOutput = LMMPATH::get_output_path();

class PrintElement
{
protected:
	std::string name_; 
	size_t nbRow_;
	size_t nbCol_;
public:
	PrintElement(const std::string& name, size_t nbRow, size_t nbCol)
		:name_(name), nbRow_(nbRow), nbCol_(nbCol){};

	const std::string& get_name() const {return name_;}
	const size_t get_nbRow() const {return nbRow_;}
	const size_t get_nbCol() const {return nbCol_;}

	void print_head(std::ofstream& myfile)
	{
		myfile << name_ <<",";
		for(size_t i=1; i<nbCol_; ++i)
		{
			myfile <<",";
		}
	}

	virtual void print_element(std::ofstream& myfile, size_t indexElementRow) const = 0;
};
typedef boost::shared_ptr<PrintElement> PrintElement_PTR;


template<class T>  // T is a scalar
class ScalarPrintElement : public PrintElement
{
	T element_;
public:
	ScalarPrintElement(const std::string& name, T element)
		:PrintElement(name, 1, 1),
		 element_(element) 
	{}

	void print_element(std::ofstream& myfile, size_t indexElementRow) const
	{
		if(indexElementRow < nbRow_) // row 0 is for the head
		{
			myfile << element_ <<",";
		}
		else  // has nothing to print
		{
			myfile <<",";
		}		
	}
};
//typedef boost::shared_ptr<ScalarPrintElement> ScalarPrintElement_PTR;


template<class T>  // T is a vector 
class VectorPrintElement : public PrintElement
{
	T element_;
public:
	VectorPrintElement(const std::string& name, T element)
		:PrintElement(name, element.size(),1),
		 element_(element) 
	{}

	void print_element(std::ofstream& myfile, size_t indexElementRow) const
	{
		if(indexElementRow < nbRow_) // row 0 is for the head
		{
			myfile << element_[indexElementRow] <<",";
		}
		else  // has nothing to print
		{
			myfile <<",";
		}		
	}
};
//typedef boost::shared_ptr<VectorPrintElement<T>> VectorPrintElement_PTR;


template<class T>  // T is a matrix
class MatrixPrintElement : public PrintElement
{
	T element_;
public:
	MatrixPrintElement(const std::string& name, T element)
		:PrintElement(name, element.size1(), element.size2()), 
		 element_(element) 
	{}

	void print_element(std::ofstream& myfile, size_t indexElementRow) const
	{
		if(indexElementRow < nbRow_) // row 0 is for the head
		{
			for(size_t i=0; i<nbCol_; ++i)
			{
				myfile << element_(indexElementRow,i) <<",";
			}			
		}
		else  // has nothing to print
		{
			for(size_t i=0; i<nbCol_; ++i)
			{
				myfile  <<",";
			}
		}		
	}
};
//typedef boost::shared_ptr<MatrixPrintElement> MatrixPrintElement_PTR;


// specialise for printing upper triangular matrix
typedef boost::numeric::ublas::matrix<double> UpperTriangularDoubleMatrix; // UTDM

template<>  // T is a upper triangular matrix
class MatrixPrintElement<UpperTriangularDoubleMatrix> : public PrintElement
{
	UpperTriangularDoubleMatrix element_;
public:
	MatrixPrintElement(const std::string& name, UpperTriangularDoubleMatrix element)
	: PrintElement(name, element.size1(), element.size2()) 
	, element_(element) 
	{
		assert( element.size1()== element.size2() );
	}

	void print_element(std::ofstream& myfile, size_t indexElementRow) const
	{
		if(indexElementRow < nbRow_) // row 0 is for the head
		{
			// print only the upper part
			for(size_t i=0; i< (nbCol_ - indexElementRow); ++i)
			{
				myfile << element_(indexElementRow,i) <<",";
			}
			for(size_t i= (nbCol_ - indexElementRow) ; i< nbCol_ ; ++i)
			{
				myfile  <<"0,";
			}
		}
		else  // has nothing to print
		{
			for(size_t i=0; i<nbCol_; ++i)
			{
				myfile  <<",";
			}
		}		
	}
};


class Printer
{
	size_t maxRow_;
	std::string path_;
	std::vector<PrintElement_PTR> elementVector_;
public:
	Printer(const std::string& path, const std::vector<PrintElement_PTR> elementVector) 
		:maxRow_(0), path_(path), elementVector_(elementVector)
	{
		for(size_t i=0; i<elementVector_.size(); ++i)
		{
			size_t elmementRow = elementVector_[i]->get_nbRow();
			if(maxRow_ < elmementRow)
				maxRow_ = elmementRow;
		}
	}


	void print(bool erase_file=true) const
	{
		if(maxRow_ <= 0)
			throw("Error: maxRow_<=0");

		std::ofstream myfile;
		if(erase_file)	
			myfile.open(path_.c_str());
		else
			myfile.open(path_.c_str(), std::ios::app);

		//! print head
		for(size_t itr_elment=0; itr_elment<elementVector_.size(); ++itr_elment)
		{
             elementVector_[itr_elment]->print_head(myfile);
			 myfile << ",";
		}
		myfile << std::endl;

		//! print element: line by line
		for(size_t indexElementRow=0; indexElementRow<maxRow_; ++indexElementRow)
		{
			if(indexElementRow > maxRow_)
				break;

			for(size_t itr_elment=0; itr_elment<elementVector_.size(); ++itr_elment)
			{
				elementVector_[itr_elment]->print_element(myfile, indexElementRow);
				myfile << ",";
			}
			myfile << std::endl;
		}

		myfile.close();
	}
};

#endif /* LMM_HELPER_PRINTER_H */