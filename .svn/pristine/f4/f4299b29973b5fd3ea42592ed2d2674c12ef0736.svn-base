
#include <LMM/numeric/Interpolation.h>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cassert>

namespace numeric
{

	void Interpolation::linearInterpolation
		(
		std::vector<double>& y         ,
		std::vector<double>& x         ,
		const double&        x_interp  ,
		const double epsilon
		)
	{
		// the inserted point has to be in xmax and xmin
		if (x_interp < x[0] || x_interp > x[x.size()-1])
		{
			throw("Interpolation.cpp ----> Error: invalid absciss input");
		}

		bool additional_node_is_coincide = false; // if true, no need to interpolate

		// iterate the whole x vector to find the appropriated interpolation position 
		// find the index after the add point
		size_t index_interp = 0;
		for (size_t i = 0; i < x.size(); ++i) 
		{
			if( std::abs(x_interp - x[i]) < epsilon ) // x_interp is too close to a node
			{
				additional_node_is_coincide=true;     // no need for interpolation
				break;
			}

			if(x_interp > x[i]) index_interp++;
		}


		if(!additional_node_is_coincide)
		{

			//-- Set the dates before and after maturity_date
			const double& date_prev = x[index_interp-1];
			const double& date_next = x[index_interp];

			//-- Search in the libor matrix for libors corresponding to the previous dates
			const double& libor_prev =  y[index_interp-1];
			const double& libor_next = y[index_interp];

			double coeff_1 = (date_next - x_interp)/(date_next - date_prev);
			double coeff_2 = (x_interp - date_prev)/(date_next - date_prev);

			double interpolated_libor = coeff_1*libor_prev + coeff_2*libor_next;

			//-- Update vectors of date and values
			x.insert(x.begin()+index_interp,x_interp);
			y.insert(y.begin()+index_interp,interpolated_libor);

		}
	}


	double Interpolation::linearInterpolation
		(
		const double& x1, 
		const double& x2, 
		const double& y1, 
		const double& y2, 
		const double& x_interp
		)
	{
		double coeff_1 = (x2 - x_interp)/(x2 - x1);
		double coeff_2 = (x_interp - x1)/(x2 - x1);
		double res = coeff_1*y1 + coeff_2*y2;
		return res;
	}


	QuantLib::Array Interpolation::interpolateVector( const QuantLib::Array & node_values
		, const std::vector<size_t>& node_indices
		, const size_t vector_size) const 
	{
		assert(node_indices.size()  > 0 );

		assert(node_indices.size()  < vector_size );  // the vector to be interpolate has to be lager than the known vector
		assert(node_indices.size() == node_values.size() );		

		QuantLib::Array interpolated_values(vector_size);
		interpolated_values[0]=10e100; // convention not use the first case

		// if there are only one node, every others element are extrapolated
		if(node_indices.size()  == 1 )
		{
			for(size_t iterValue=1;iterValue<interpolated_values.size();++iterValue)
			{
				interpolated_values[iterValue]=node_values[0];
			}			
			return interpolated_values;
		}


		// case there are at least two nodes		
		for(size_t iterNode=1;iterNode<node_indices.size();++iterNode)
		{
			size_t iNode  = node_indices[iterNode-1];
			size_t iiNode = node_indices[iterNode];
			assert(iiNode > iNode); // nodes indices saved has to be strictly increasing

			// fetching data to nodes
			interpolated_values[ iNode] = node_values[iterNode-1];
			interpolated_values[iiNode] = node_values[iterNode];

			// extrapolate constant the left queue of vector if this is the first node
			if( iterNode == 1 ) 
			{
				for(size_t iInterpolate=1;iInterpolate < iNode;++iInterpolate)
				{
					interpolated_values[iInterpolate] = node_values[iterNode-1];
				}
			}

			// extrapolate constant the right queue of vector if this is the first node
			if( iterNode == node_indices.size() - 1 ) 
			{
				for(size_t iInterpolate=iiNode ;iInterpolate < interpolated_values.size() ;++iInterpolate)
				{
					interpolated_values[iInterpolate] = node_values[iterNode];
				}
			}

			// interpolate between nodes
			for(size_t iInterpolate=iNode+1 ;iInterpolate < iiNode ; ++iInterpolate)
			{
				double node_interval    = (double) (iiNode - iNode) ;
				double right_coeff = (double) (iInterpolate-iNode) / node_interval ;
				double left_coeff  = (double) (iiNode-iInterpolate)/ node_interval ;

				interpolated_values[iInterpolate] = left_coeff*interpolated_values[iNode]  +  right_coeff*interpolated_values[iiNode];
			}
		}
		return interpolated_values;
	}


	double Interpolation::columnIntepolation(
		const double & lowvalue , const double& highvalue,
		size_t          lowIndex, size_t        highIndex, 
		size_t interpolIndex)
	{
		assert(lowIndex<highIndex);
		assert(lowIndex<interpolIndex && interpolIndex < highIndex);

		double interval    = (double) (highIndex - lowIndex) ;
		double low_coeff   = (double) (highIndex-interpolIndex)/interval  ;
		double hight_coeff = (double) (interpolIndex-lowIndex) /interval  ;

		return low_coeff*lowvalue  +  hight_coeff * highvalue ;
	}


	void Interpolation::fullMatrixInterpolate( FullMatrix& matrix
		, const std::vector<size_t> & row_indices_missed
		, const std::vector<size_t> & col_indices_missed)
	{

		if(!row_indices_missed.empty() ){
			assert( row_indices_missed[0] > 1 );           // missed indices has to be in the interior of matrix, no extrapolation
			assert( row_indices_missed.back() < matrix.size1() -1);
		}
		if(!col_indices_missed.empty() ){
			assert( col_indices_missed[0] > 1 );           // missed indices has to be in the interior of matrix, no extrapolation
			assert( col_indices_missed.back() <  matrix.size2() -1); 
		}

		//iterate cols, interpolate missed row
		for(size_t jCol=0;jCol<matrix.size2();++jCol)
		{
			//interpolate iff this is not a missed col
			if( std::find(col_indices_missed.begin(), col_indices_missed.end(), jCol) == col_indices_missed.end() )
			{
				size_t itRowMissed=0;
				while(itRowMissed<row_indices_missed.size() )
				{
					//find the top and bottom row indices that are not missed
					size_t topMatrixRow = row_indices_missed[itRowMissed]-1 ;
					size_t bottomMatrixRow = row_indices_missed[itRowMissed]+1;			

					while(  itRowMissed < row_indices_missed.size() 
						&& std::find(row_indices_missed.begin(), row_indices_missed.end(), bottomMatrixRow) != row_indices_missed.end() )
					{
						++itRowMissed;
						bottomMatrixRow = row_indices_missed[itRowMissed]+1;
						assert(bottomMatrixRow< matrix.size1() );
					}

					// interpolate all missed row from top to bottom
					for(size_t iRow=topMatrixRow+1;iRow<bottomMatrixRow;++iRow)
					{
						const double& top_value = matrix(topMatrixRow,jCol);
						const double& bottom_value = matrix(bottomMatrixRow,jCol);
						const double interpolated_value = columnIntepolation(top_value,bottom_value,topMatrixRow,bottomMatrixRow,iRow);
						matrix(iRow,jCol) = interpolated_value;					
					}	

					++itRowMissed;
				}
			}		
		}


		//iterate row, interpolate missed cols
		for(size_t iRow=0;iRow<matrix.size1();++iRow)
		{
			size_t itColMissed=0;
			while(itColMissed<col_indices_missed.size() )
			{
				//find the left and right col indices that are not missed
				size_t leftMatrixCol = col_indices_missed[itColMissed]-1 ;
				size_t rightMatrixCol = col_indices_missed[itColMissed]+1;			

				while(  itColMissed < col_indices_missed.size() 
					&& std::find(col_indices_missed.begin(), col_indices_missed.end(), rightMatrixCol) != col_indices_missed.end() )
				{
					++itColMissed;
					rightMatrixCol = col_indices_missed[itColMissed]+1;
					assert(rightMatrixCol< matrix.size2() );
				}

				// interpolate all missed col from left to right
				for(size_t jCol=leftMatrixCol+1;jCol<rightMatrixCol;++jCol)
				{
					const double& left_value = matrix(iRow,leftMatrixCol);
					const double& right_value = matrix(iRow,rightMatrixCol);
					const double interpolated_value = columnIntepolation(left_value,right_value,leftMatrixCol,rightMatrixCol,jCol);
					matrix(iRow,jCol) = interpolated_value;					
				}	

				++itColMissed;
			}

		}
	}

} //end namespace numerica
