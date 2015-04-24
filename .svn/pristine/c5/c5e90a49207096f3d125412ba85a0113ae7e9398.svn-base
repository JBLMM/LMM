
#include <LMM/numeric/UpperTriangularMatrixInterpolator.h>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cassert>


void UpperTriangularMatrixBlockInterpolator::extrapolate_TopLeftCorner(const size_t rowNODE, const size_t colNODE)
{
	assert(rowNODE>1 && colNODE>1);// top-left cell has to be greater than first cell 11
	assert( rowNODE + colNODE < matrix_.size2() );//upper matrix setting

	const size_t diagNODE = get_SubDiag_ID(rowNODE,colNODE);

	const double extrapolate_constant_value = matrix_(rowNODE,colNODE);
	for(size_t iDiag=1;iDiag<=diagNODE;++iDiag)
	{
		// nb Element On sub diag = diag ID
		for(size_t iterElementOnDiag=0;iterElementOnDiag<iDiag;++iterElementOnDiag)
		{
			// count from bottom to top of the diag
			const size_t iRow = iDiag - iterElementOnDiag  ;
			const size_t jCol =     1 + iterElementOnDiag  ;
			matrix_(iRow,jCol)= extrapolate_constant_value ;
		}
	}
}

void UpperTriangularMatrixBlockInterpolator::extrapolate_Top(const size_t rowNODE, const size_t leftColNODE, const size_t rightColNODE)
{
	assert(rowNODE>1 && leftColNODE<rightColNODE); //segment non empty and row >1

	//extrapolate from bottom to top
	for(size_t iRow=rowNODE-1;iRow>0;--iRow)
	{
		size_t seg_lenght = rowNODE - iRow ;
		for(size_t iterCol=leftColNODE;iterCol<=rightColNODE;++iterCol)
		{
			const size_t jCol = iterCol + seg_lenght;
			matrix_(iRow,jCol) = matrix_(rowNODE,iterCol) ;
		}
	}
}

void UpperTriangularMatrixBlockInterpolator::extrapolate_Left(const size_t colNODE, const size_t topRowNODE , const size_t bottomRowNODE)
{
	assert(colNODE>1 && topRowNODE<bottomRowNODE); //segment non empty and row >1

	// extrapolate from right to left
	for(size_t jCol=colNODE-1;jCol>0;--jCol)
	{
		size_t seg_lenght = colNODE - jCol ;

		for(size_t iterRow = topRowNODE;iterRow<=bottomRowNODE;++iterRow)
		{
			const size_t iRow = iterRow + seg_lenght;
			matrix_(iRow,jCol) = matrix_(iterRow,colNODE) ;
		}
	}
}

void UpperTriangularMatrixBlockInterpolator::interpolate_ROW(const size_t iRow, const size_t leftColNODE, const size_t rightColNODE)
{
	//std::cout<<"UpperTriangularMatrixBlockInterpolator::interpolate_ROW "<<iRow <<"	leftColNODE."<<leftColNODE <<"	rightColNODE."<<rightColNODE <<std::endl;

	assert(leftColNODE<rightColNODE);
	assert( iRow + rightColNODE < matrix_.size2() );//upper matrix setting

	const double& low_val = matrix_(iRow,leftColNODE);
	const double& high_val = matrix_(iRow,rightColNODE);

	for(size_t jCol=leftColNODE+1;jCol<rightColNODE;++jCol)
	{		
		matrix_(iRow,jCol) = segment_interplation( low_val , high_val, leftColNODE , rightColNODE , jCol );
	}
}

void UpperTriangularMatrixBlockInterpolator::interpolate_COL(const size_t jCol, const size_t topRowNODE , const size_t bottomRowNODE)
{
	//std::cout<<"UpperTriangularMatrixBlockInterpolator::interpolate_COL "<<jCol<<"   topRowNODE."<<topRowNODE <<"    bottomRowNODE."<< bottomRowNODE<<std::endl;

	assert(topRowNODE<bottomRowNODE);
	assert(jCol + bottomRowNODE <matrix_.size1() );//upper matrix setting

	const double& low_val = matrix_(topRowNODE,jCol);
	const double& high_val = matrix_(bottomRowNODE,jCol);

	for(size_t iRow=topRowNODE+1;iRow<bottomRowNODE;++iRow)
	{		
		matrix_(iRow,jCol) = segment_interplation( low_val , high_val, topRowNODE,bottomRowNODE, iRow );
	}
}

void UpperTriangularMatrixBlockInterpolator::interpolate_SubDiag(const size_t topRowNODE, const size_t bottomRowNODE, const size_t leftColNODE, const size_t rightColNODE)
{
	assert( bottomRowNODE + leftColNODE < matrix_.size1() );  // upper matrix feature
	assert( topRowNODE + rightColNODE < matrix_.size1() );    // upper matrix feature
	assert( topRowNODE >0 &&  leftColNODE > 0 );
	assert( bottomRowNODE > 2 &&  rightColNODE > 2 ); // smallest rectangle has at least indices 33 of bottom_right corner

	assert(topRowNODE<bottomRowNODE);
	assert(leftColNODE<rightColNODE);

	const size_t col_seg_lenght = bottomRowNODE - topRowNODE;
	const size_t row_seg_lenght = rightColNODE-leftColNODE;
	assert( col_seg_lenght == row_seg_lenght &&  col_seg_lenght>1 ); //! indices has to define a square with interior non empty

	const double& low_val = matrix_(bottomRowNODE,leftColNODE);
	const double& high_val = matrix_(topRowNODE,rightColNODE);

	size_t nb_element_on_diag_seg = bottomRowNODE - topRowNODE - 1 ;
	for(size_t elem=0;elem<nb_element_on_diag_seg;++elem)
	{
		const size_t iRow = bottomRowNODE - (elem + 1);	
		const size_t jCol = leftColNODE   + (elem + 1);
		matrix_(iRow,jCol) = segment_interplation( low_val , high_val, leftColNODE,rightColNODE, jCol );
	}
}

void UpperTriangularMatrixBlockInterpolator::interpolate_SubUpperTriangle(const size_t topRowNODE, const size_t bottomRowNODE, const size_t leftColNODE, const size_t rightColNODE)
{
	assert( bottomRowNODE + leftColNODE < matrix_.size1() );  // upper matrix feature
	assert( topRowNODE + rightColNODE < matrix_.size1() );    // upper matrix feature
	assert( topRowNODE >0 &&  leftColNODE > 0 );
	assert( bottomRowNODE > 2 &&  rightColNODE > 2 ); // smallest rectangle has at least indices 33 of bottom_right corner

	assert(topRowNODE<bottomRowNODE);
	assert(leftColNODE<rightColNODE);

	const size_t col_seg_lenght = bottomRowNODE - topRowNODE;
	const size_t row_seg_lenght = rightColNODE-leftColNODE;	
	assert( col_seg_lenght == row_seg_lenght &&  col_seg_lenght>1 ); //! indices has to define a square with interior non empty

	//interpolate the sides of upper block
	//interpolate_ROW(topRowNODE,leftColNODE,rightColNODE); // assuming that the up edge of the triangle is already interpolated
	interpolate_COL(leftColNODE,topRowNODE,bottomRowNODE);

	//interpolate the interior of the upper bloc
	for(size_t sub_diag_id=1;sub_diag_id<col_seg_lenght;++sub_diag_id)
	{
		const size_t float_bottomRowNODE = topRowNODE  + sub_diag_id + 1;
		const size_t float_rightColNODE  = leftColNODE + sub_diag_id + 1;
		interpolate_SubDiag(topRowNODE,float_bottomRowNODE, leftColNODE , float_rightColNODE );
	}
}

void UpperTriangularMatrixBlockInterpolator::interpolate_SubLowerTriangle(const size_t topRowNODE, const size_t bottomRowNODE, const size_t leftColNODE, const size_t rightColNODE)
{
	assert( bottomRowNODE + rightColNODE < matrix_.size1() );  // upper matrix feature, bttom-right cell has to be in the upper matrix
	assert( topRowNODE >0 &&  leftColNODE > 0 );

	const size_t col_seg_lenght = bottomRowNODE - topRowNODE;
	const size_t row_seg_lenght = rightColNODE-leftColNODE;	
	assert( col_seg_lenght == row_seg_lenght &&  col_seg_lenght>1 ); //! indices has to define a square with interior non empty

	//interpolate the sides of lower block
	interpolate_ROW(bottomRowNODE,leftColNODE,rightColNODE);
	interpolate_COL(rightColNODE,topRowNODE,bottomRowNODE);

	//interpolate the interior of lower block
	for(size_t sub_diag_id=0;sub_diag_id<col_seg_lenght-1;++sub_diag_id)
	{
		const size_t float_topRowNODE = topRowNODE  + sub_diag_id ;
		const size_t float_leftColNODE  = leftColNODE + sub_diag_id;
		interpolate_SubDiag(float_topRowNODE,bottomRowNODE, float_leftColNODE , rightColNODE );
	}
}

void UpperTriangularMatrixBlockInterpolator::interpolate_SubRectangle(const size_t topRowNODE, const size_t bottomRowNODE, const size_t leftColNODE, const size_t rightColNODE)
{
	assert( bottomRowNODE + rightColNODE < matrix_.size1() );  // upper matrix feature, bttom-right cell has to be in the upper matrix
	assert( topRowNODE >0 &&  leftColNODE > 0 );
	assert( bottomRowNODE > 2 &&  rightColNODE > 2 ); // smallest rectangle has at least indices 33 of bottom_right corner

	assert(bottomRowNODE > topRowNODE && rightColNODE > leftColNODE ); // rectangle properties
	const size_t col_seg_lenght = bottomRowNODE - topRowNODE;
	const size_t row_seg_lenght = rightColNODE-leftColNODE;	
	assert( col_seg_lenght>1 || row_seg_lenght > 1 ); //! indices has to define a regtangle with interior non empty, otherwise use interpolation segment

	// firstly interpolate the 4 side ' s regtangle
	interpolate_ROW(topRowNODE,leftColNODE,rightColNODE);
	interpolate_ROW(bottomRowNODE,leftColNODE,rightColNODE);
	interpolate_COL(leftColNODE,topRowNODE,bottomRowNODE);
	interpolate_COL(rightColNODE,topRowNODE,bottomRowNODE);

	for(size_t iRow=topRowNODE+1;iRow<bottomRowNODE;++iRow)
	{
		interpolate_ROW(iRow,leftColNODE,rightColNODE);
	}

	//for(size_t jCol=leftColNODE+1;jCol<rightColNODE;++jCol)
	//{
	//	interpolate_COL(jCol,topRowNODE,bottomRowNODE);
	//}


}

//! and parallelogram is determined by 6 values
void UpperTriangularMatrixBlockInterpolator::interpolate_Parallelogram( const size_t topRowNODE, const size_t bottomRowNODE
																	   , const size_t leftleftColNODE , const size_t leftColNODE
																	   , const size_t rightColNODE, const size_t rightrightColNODE)
{
	//non empty parallelogram
	assert(topRowNODE<bottomRowNODE);
	assert(leftColNODE>leftleftColNODE);
	assert(rightrightColNODE>rightColNODE);
	assert(rightrightColNODE - rightColNODE == leftColNODE - leftleftColNODE );

	//! interpolate top row segment
	//interpolate_ROW(topRowNODE,rightColNODE, rightrightColNODE);//assuming that the up edge of the parallelogram is already interpolated
	//! interpolate bottom row segment
	interpolate_ROW(bottomRowNODE,leftleftColNODE, leftColNODE);

	size_t nbSubDiag = rightrightColNODE - rightColNODE + 1;
	for(size_t iDiag=0;iDiag<nbSubDiag;++iDiag)
	{
		size_t leftcolNODE  = leftleftColNODE+iDiag;
		size_t rightcolNODE = rightColNODE+iDiag;
		interpolate_SubDiag(topRowNODE,bottomRowNODE,leftcolNODE,rightcolNODE);
	}
}



size_t UpperTriangularMatrixBlockInterpolator::get_SubDiag_ID(const size_t rowNODE, const size_t colNODE) const
{
	size_t sum_indices = rowNODE + colNODE; 
	assert(sum_indices < matrix_.size1()) ; // upper matrix feature 
	return sum_indices - 1;
}

double UpperTriangularMatrixBlockInterpolator::segment_interplation(const double & lowvalue, const double& highvalue, const size_t lowIndex, const size_t highIndex, const size_t interpolIndex) const
{
	assert(lowIndex<highIndex);
	assert(lowIndex<interpolIndex && interpolIndex < highIndex);

	double interval    = (double) (highIndex - lowIndex) ;
	double low_coeff   = (double) (highIndex-interpolIndex)/interval  ;
	double hight_coeff = (double) (interpolIndex-lowIndex) /interval  ;

	return low_coeff*lowvalue  +  hight_coeff * highvalue ;
}

void UpperTriangularMatrixBlockInterpolator::interpolate(const UpperTriangularIndexPairMatrix& indices_mapper)
{
	assert( indices_mapper.size1() == indices_mapper.size2() );
	size_t gDelegate_size = indices_mapper.size1();
	//interpolate first row and extrapolate top margin

	extrapolate_TopLeftCorner( indices_mapper(1,1).first , indices_mapper(1,1).second ) ;

	for(size_t jDelegate=1;jDelegate<gDelegate_size-2;++jDelegate)
	{
		size_t iNode  = indices_mapper(1,jDelegate).first;
		size_t jNode  = indices_mapper(1,jDelegate).second;
		size_t jjNode = indices_mapper(1,jDelegate+1).second;

		interpolate_ROW(iNode, jNode, jjNode);
	}

	// if top row has at least two nodes, extrapolate the top margin
	if(indices_mapper(1,1).second < indices_mapper(1,gDelegate_size-2).second ) 
	{
		extrapolate_Top(indices_mapper(1,1).first , indices_mapper(1,1).second,indices_mapper(1,gDelegate_size-2).second );
	}


	for(size_t iDelegate=1;iDelegate<gDelegate_size-2;++iDelegate)
	{
		size_t  iNode = indices_mapper( iDelegate   , 1 ).first;
		size_t iiNode = indices_mapper( iDelegate+1 , 1 ).first;
		interpolate_SubUpperTriangle(iNode,iiNode, indices_mapper( iDelegate   , 1 ).second , indices_mapper( iDelegate   , 2 ).second);
		extrapolate_Left(indices_mapper( iDelegate   , 1 ).second , iNode , iiNode);

		for(size_t jDelegate=2;jDelegate < gDelegate_size-1 - iDelegate ; ++jDelegate  )
		{
			size_t leftleftCol = indices_mapper( iDelegate+1 , jDelegate-1 ).second;
			size_t     leftCol = indices_mapper( iDelegate+1 , jDelegate   ).second;
			size_t      rightCol = indices_mapper( iDelegate , jDelegate   ).second;
			size_t rightrightCol = indices_mapper( iDelegate , jDelegate+1 ).second;

			interpolate_Parallelogram(iNode,iiNode,leftleftCol,leftCol,rightCol,rightrightCol);
		}
	}

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UpperTriangularMatrixColRowInterpolator::interpolate( const std::vector<size_t> & row_indices_missed
													     , const std::vector<size_t> & col_indices_missed)
{
	size_t m_size=matrix_.size1() ; assert( m_size==matrix_.size2() );

	if(!row_indices_missed.empty() ){
		assert( row_indices_missed[0] > 0 );           // ignore first row , col
		assert( row_indices_missed.back() < m_size-1);
	}
	if(!col_indices_missed.empty() ){
		assert( col_indices_missed[0] > 0 );           // ignore first row , col
		assert( col_indices_missed.back() < m_size-1); // upper matrix
	}

	////====================================================================================================================================
	// Interpolation by Cols
	size_t itColMissed=0;
	while(itColMissed<col_indices_missed.size() )
	{
		size_t leftMatrixCol = col_indices_missed[itColMissed]-1 ;
		//if(col_indices_missed[itColMissed] == 1) leftMatrixCol = 1;

		size_t rightMatrixCol = col_indices_missed[itColMissed]+1;			

		while(  itColMissed < col_indices_missed.size() 
			&& std::find(col_indices_missed.begin(), col_indices_missed.end(), rightMatrixCol) != col_indices_missed.end() )
		{
			++itColMissed;
			rightMatrixCol = col_indices_missed[itColMissed]+1;
			assert(rightMatrixCol<m_size);
		}

		if( leftMatrixCol == 0 )// extrapolate from right to left cols
		{
			for(size_t jCol=rightMatrixCol-1;jCol>leftMatrixCol;--jCol)
			{
				size_t rowBottomBoundIndex = m_size - (jCol+1);
				for(size_t iRow=1;iRow<rowBottomBoundIndex;++iRow)
				{						
					matrix_(iRow,jCol)= matrix_(iRow,jCol+1);											
				}
				matrix_(rowBottomBoundIndex,jCol)= matrix_(rowBottomBoundIndex-1,jCol+1);
			}			
		}

		if( rightMatrixCol== m_size-1)// extrapolate from left to right col
		{
			for(size_t jCol=leftMatrixCol+1;jCol<rightMatrixCol ;++jCol)
			{
				size_t rowBottomBoundIndex = m_size - (jCol+1);
				for(size_t iRow=1;iRow<=rowBottomBoundIndex;++iRow)
				{
					matrix_(iRow,jCol)= matrix_(iRow,jCol-1);	
				}					
			}
		}

		if(leftMatrixCol > 0 && rightMatrixCol < m_size-1 ) // interpolate between left and right cols
		{
			assert(leftMatrixCol< rightMatrixCol);
			size_t leftBottomRowBound    = m_size - (leftMatrixCol+1);
			size_t rightBottomRowBound = m_size - (rightMatrixCol+1);
			assert(leftBottomRowBound > rightBottomRowBound); // upper matrix

			for(size_t jCol=leftMatrixCol+1;jCol<rightMatrixCol;++jCol)
			{
				for(size_t iRow=1;iRow<=rightBottomRowBound;++iRow)
				{
					const double& left_value = matrix_(iRow ,leftMatrixCol);
					const double& right_value = matrix_(iRow, rightMatrixCol);
					const double interpolated_value = segment_interplation(left_value,right_value,leftMatrixCol,rightMatrixCol,jCol);
					matrix_(iRow,jCol) = interpolated_value;
				}				
			}		

			// extrapolate the triangle queue in the horizontal direction from left to right
			for(size_t iRow=rightBottomRowBound+1;iRow<leftBottomRowBound;++iRow)
			{
				const size_t maxColBound = m_size - iRow;
				for(size_t jCol = leftMatrixCol+1;jCol<maxColBound;++jCol)
				{
					matrix_(iRow,jCol)=matrix_(iRow,leftMatrixCol);
				}
			}


			//// interpolate the triangle queu in the diagonal direction
			//const size_t nbDiagSeg = rightMatrixCol - (leftMatrixCol+1);
			//for(size_t iDiag=0;iDiag<nbDiagSeg;++iDiag)
			//{
			//	const size_t bottomLeftRow = leftBottomRowBound - iDiag;
			//	const size_t topRightCol   = rightMatrixCol - iDiag;
			//	//const size_t topRightRow = leftMatrixCol + iDiag;
			//	//const size_t bottomLeftCol = leftTopColBound + iDiag;					

			//	const size_t nbElementOnDiagSeg = nbDiagSeg - iDiag;
			//	for(size_t iElemDiag=0;iElemDiag<nbElementOnDiagSeg;++iElemDiag)
			//	{
			//		size_t iRow = bottomLeftRow - (iElemDiag+1);
			//		size_t jCol = leftMatrixCol + (iElemDiag+1);
			//		const double & bottomleft_value = matrix_( bottomLeftRow, leftMatrixCol );
			//		const double & topright_value = matrix_( rightBottomRowBound, topRightCol );
			//		const double interpolated_value = columnIntepolation(bottomleft_value,topright_value,leftMatrixCol,topRightCol,jCol);
			//		matrix_(iRow,jCol) = interpolated_value;
			//	}
			//}
		}		
		++itColMissed;
	}

	//=====================================================================================================================
	// firstly interpolate by rows 
	size_t itRowMissed=0;
	while(itRowMissed<row_indices_missed.size() )
	{
		size_t topMatrixRow = row_indices_missed[itRowMissed]-1 ;
		//if(row_indices_missed[itRowMissed] == 1) topMatrixRow = 1;

		size_t bottomMatrixRow = row_indices_missed[itRowMissed]+1;			

		while(  itRowMissed < row_indices_missed.size() 
			&& std::find(row_indices_missed.begin(), row_indices_missed.end(), bottomMatrixRow) != row_indices_missed.end() )
		{
			++itRowMissed;
			bottomMatrixRow = row_indices_missed[itRowMissed]+1;
			assert(bottomMatrixRow<m_size);
		}

		// extrapolate from bottom to top row in the diagonal direction
		//! NOTE : this interpolation of first row in gTransformed is very very inportant
		//! because this is the horizontal direction of the gMatrix
		//! which is a natural direction in the cascade calibration algorithm
		if( topMatrixRow ==  0 )
		{
			for(size_t iiRow=bottomMatrixRow;iiRow>topMatrixRow+1;--iiRow)
			{
				matrix_(iiRow-1,1)= matrix_(iiRow,1);

				size_t maxColIndex = m_size - (iiRow+1);
				for(size_t jCol=1;jCol<=maxColIndex;++jCol)
				{						
					matrix_(iiRow-1,jCol+1)= matrix_(iiRow,jCol);											
				}					
			}			
		}

		if( bottomMatrixRow== m_size-1)// extrapolate from top bottom
		{
			for(size_t iRow=topMatrixRow+1;iRow<bottomMatrixRow ;++iRow)
			{
				size_t colLeftBoundIndex = m_size - (iRow+1);
				for(size_t jCol=1;jCol<=colLeftBoundIndex;++jCol)
				{
					matrix_(iRow,jCol)= matrix_(iRow-1,jCol);	
				}					
			}
		}

		if(topMatrixRow > 0 && bottomMatrixRow < m_size-1 ) // interpolate between left and right cols
		{
			assert(topMatrixRow< bottomMatrixRow);
			size_t rightTopColBound    = m_size - (topMatrixRow+1);
			size_t rightBottomColBound = m_size - (bottomMatrixRow+1);
			assert(rightBottomColBound < rightTopColBound);

			// interpolate the rectangle left part
			for(size_t iRow=topMatrixRow+1;iRow<bottomMatrixRow;++iRow)
			{
				for(size_t jCol=1;jCol<=rightBottomColBound;++jCol)
				{
					const double& top_value = matrix_(topMatrixRow,jCol);
					const double& bottom_value = matrix_(bottomMatrixRow,jCol);
					const double interpolated_value = segment_interplation(top_value,bottom_value,topMatrixRow,bottomMatrixRow,iRow);
					matrix_(iRow,jCol) = interpolated_value;
				}
			}		

			//// extrapolate the triangle queue in the vertical direction
			for(size_t jCol=rightBottomColBound+1;jCol<rightTopColBound;++jCol)
			{
				const size_t maxBottomRow = m_size - jCol;
				for(size_t iRow=topMatrixRow+1;iRow<maxBottomRow;++iRow)
				{
					matrix_(iRow,jCol) = matrix_(topMatrixRow,jCol) ;
				}				
			}

			//// interpolate the triangle queu in the diagonal direction
			//const size_t nbDiagSeg = bottomMatrixRow - (topMatrixRow+1);
			//for(size_t iDiag=0;iDiag<nbDiagSeg;++iDiag)
			//{
			//	const size_t bottomLeftRow = bottomMatrixRow - iDiag;
			//	const size_t topRightCol   = rightTopColBound - iDiag;
			//	//const size_t topRightRow = topMatrixRow + iDiag;
			//	//const size_t bottomLeftCol = leftTopColBound + iDiag;					

			//	const size_t nbElementOnDiagSeg = nbDiagSeg - iDiag;
			//	for(size_t iElemDiag=0;iElemDiag<nbElementOnDiagSeg;++iElemDiag)
			//	{
			//		size_t iRow = bottomLeftRow - (iElemDiag+1);
			//		size_t jCol = rightBottomColBound + (iElemDiag+1);

			//		// a bug come when missing cols cross with missing row and form a bigger triangle queue
			//		// this small algo find cols rank that do not have missing cols
			//		size_t left_col_no_missing = rightBottomColBound;
			//		size_t right_col_no_missing = topRightCol;
			//		size_t bottom_row_no_missing = bottomLeftRow;
			//		size_t top_row_no_missing  = topMatrixRow;
			//		while(std::find(col_indices_missed.begin(), col_indices_missed.end(), left_col_no_missing) != col_indices_missed.end() )
			//		{
			//			--left_col_no_missing;++bottom_row_no_missing;
			//		}
			//		while(std::find(col_indices_missed.begin(), col_indices_missed.end(), right_col_no_missing) != col_indices_missed.end() )
			//		{
			//			++right_col_no_missing;--top_row_no_missing;
			//		}

			//		const double & bottomleft_value = matrix_( bottom_row_no_missing, left_col_no_missing );
			//		const double & topright_value   = matrix_( top_row_no_missing, right_col_no_missing );
			//		const double interpolated_value = columnIntepolation(bottomleft_value,topright_value,left_col_no_missing,right_col_no_missing,jCol);
			//		matrix_(iRow,jCol) = interpolated_value;
			//	}
			//}
		}		
		++itRowMissed;
	}

}