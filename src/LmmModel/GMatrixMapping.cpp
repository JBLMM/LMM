#include <LMM/LmmModel/GMatrixMapping.h>

size_t GMatrixMapping::get_TotalNbDelegateElement() const 
{
	int fullNB_gDelegateElement = (gDelegate_.size1()-2)*(gDelegate_.size1()-1)/2 ;

	fullNB_gDelegateElement-=truncated_cell_.size();

	return  fullNB_gDelegateElement ;
}

size_t GMatrixMapping::get_NbDelegateElementOnRow(const size_t iRowDelegate) const 
{
	assert( std::find(truncated_gDelegate_Rows_.begin(),truncated_gDelegate_Rows_.end(), iRowDelegate) == truncated_gDelegate_Rows_.end()  );// can not work with iRow if it is truncated

	size_t fullNB_gDelegateElementOnRow = gDelegate_.size1() - (iRowDelegate+1);

	size_t right_col_bound = gDelegate_.size2() - iRowDelegate;
	for(size_t j=0;j<truncated_gDelegate_Cols_.size();++j)
	{
		const size_t truncated_col = truncated_gDelegate_Cols_[j];

		if(truncated_col < right_col_bound) fullNB_gDelegateElementOnRow-=1;
	}

	return fullNB_gDelegateElementOnRow; 
}

size_t GMatrixMapping::get_gDelegateSizeFromHorizon(size_t horizon, size_t ratioFixedFloat)
{
	assert(horizon%ratioFixedFloat==0);
	return 1+horizon/ratioFixedFloat;// size = nbYear + 1
}
size_t GMatrixMapping::get_gSizeFromNbYear(size_t nbYear, size_t ratioFixedFloat)
{
	return 1 + nbYear*ratioFixedFloat;
}

QuantLib::Array GMatrixMapping::get_DelegateArray() const
{
	size_t nbElement = get_TotalNbDelegateElement();
	QuantLib::Array qlArray(nbElement);

	size_t array_index_counter=0;
	// first row and firsst column are ignored 
	for(size_t iDelegate=1; iDelegate<gDelegate_.size1()-1; ++iDelegate)
	{
		//std::cout<<"iDelegate["<<iDelegate<<"]";
		for(size_t jDelegate=1; jDelegate<gDelegate_.size2()-iDelegate; ++jDelegate)
		{

			std::pair<size_t,size_t> pair_indices(iDelegate,jDelegate);

			if(std::find(truncated_cell_.begin(),truncated_cell_.end(), pair_indices) == truncated_cell_.end() )  // YY TODO: too slow ... 
			{
				//std::cout<<"   jDelegate["<<jDelegate<<"]";
				qlArray[array_index_counter] = gDelegate_(iDelegate,jDelegate) ;
				++array_index_counter;
			}
		}
		//std::cout<<"               GMatrixMapping::get_DelegateArray"<<std::endl;
	}
	assert(array_index_counter == qlArray.size() );//check if qlArray is fully used
	return qlArray;
}

QuantLib::Array GMatrixMapping::get_DelegateArray(const size_t iRowDelegate) const
{
	assert( std::find(truncated_gDelegate_Rows_.begin(),truncated_gDelegate_Rows_.end(), iRowDelegate) == truncated_gDelegate_Rows_.end()  );// can not work with iRow if it is truncated

	size_t nbElementOnRow = get_NbDelegateElementOnRow(iRowDelegate);
	size_t right_col_bound = gDelegate_.size2() - iRowDelegate;
	QuantLib::Array qlArray(nbElementOnRow);
	size_t array_index_counter=0;
	for(size_t jDelegate=1;jDelegate<right_col_bound;++jDelegate)
	{
		if(std::find(truncated_gDelegate_Cols_.begin(),truncated_gDelegate_Cols_.end(), jDelegate) == truncated_gDelegate_Cols_.end() )
		{
			//std::cout<<"iDelegate["<<iRowDelegate<<"]    jDelegate["<<jDelegate<<"]";
			qlArray[array_index_counter] = gDelegate_(iRowDelegate,jDelegate);
			++array_index_counter;
		}
		//std::cout<<"               GMatrixMapping::get_DelegateArray"<<std::endl;

	}
	assert(nbElementOnRow == array_index_counter);
	return qlArray;
}

void GMatrixMapping::reset_gDelegate( const QuantLib::Array & qlArray)
{
	size_t array_index_counter=0;
	// first row and firsst column are ignored 
	for(size_t iDelegate=1; iDelegate<gDelegate_.size1()-1; ++iDelegate)
	{		
		for(size_t jDelegate=1; jDelegate<gDelegate_.size2()-iDelegate; ++jDelegate)
		{
			std::pair<size_t,size_t> pair_indices(iDelegate,jDelegate);

			if(std::find(truncated_cell_.begin(),truncated_cell_.end(), pair_indices) == truncated_cell_.end() )
			{
				gDelegate_(iDelegate,jDelegate) = qlArray[array_index_counter];
				++array_index_counter;
			}
		}
	}
	assert(array_index_counter == qlArray.size() );//check if qlArray is fully used

	global_proccess_transform();
}

void GMatrixMapping::reset_gDelegate( const QuantLib::Array & qlArray, const size_t iiRowDelegate )
{
	if(std::find(truncated_gDelegate_Rows_.begin(), truncated_gDelegate_Rows_.end(), iiRowDelegate) == truncated_gDelegate_Rows_.end() )
	{

		size_t nbElementOnRow = get_NbDelegateElementOnRow(iiRowDelegate);
		size_t right_col_bound = gDelegate_.size2() - iiRowDelegate;
		assert(qlArray.size() == nbElementOnRow );
		size_t array_index_counter=0;

		for(size_t jDelegate=1;jDelegate<right_col_bound;++jDelegate)
		{
			if(std::find(truncated_gDelegate_Cols_.begin(),truncated_gDelegate_Cols_.end(), jDelegate) == truncated_gDelegate_Cols_.end() )
			{
				gDelegate_(iiRowDelegate,jDelegate) = qlArray[array_index_counter];
				++array_index_counter;
			}
		}

		assert(array_index_counter == qlArray.size() );//check if qlArray is fully used

		rowDelegate_proccess_transform(iiRowDelegate);
	}
}

void GMatrixMapping::reset_gDelegate( const double& cell_value, const std::pair<size_t,size_t>& gDelegateCELL)
{
	assert( std::find(truncated_cell_.begin(),truncated_cell_.end(), gDelegateCELL) == truncated_cell_.end() );

	const size_t iDelegate = gDelegateCELL.first; 
	const size_t jDelegate = gDelegateCELL.second; 
	assert(iDelegate>0 && jDelegate>0); // not use first row, col
	assert(iDelegate + jDelegate < gDelegate_.size1() ); // upper property matrix

	//fetch data
	gDelegate_(iDelegate , jDelegate) = cell_value ;
	const size_t iRow_gTransformed = gDelegate_IndexMappping_gTransformed_(iDelegate,jDelegate).first;
	const size_t jCol_gTransformed = gDelegate_IndexMappping_gTransformed_(iDelegate,jDelegate).second;
	gTransformed_(iRow_gTransformed,jCol_gTransformed)=cell_value;

	const size_t gTransformed_size = gTransformed_.size1();
	const size_t max_gDelegate_column = gDelegate_.size2()-2;

	std::pair<size_t, size_t> node(iDelegate,jDelegate);

	std::pair<size_t, size_t> left_node(iDelegate,jDelegate);
	if(jDelegate>1) left_node.second= jDelegate-1;

	// If left node is a truncated node, move to the left 
	while(  left_node.second>1 && std::find(truncated_cell_.begin(),truncated_cell_.end(), left_node) != truncated_cell_.end() )
	{
		--left_node.second;
	}

	// if the found left node is at column 1, and it is also a truncated node, the whole line is extrapolated
	if(left_node.second == 1 && std::find(truncated_cell_.begin(),truncated_cell_.end(), left_node) != truncated_cell_.end() )
	{
		for(size_t jDelegate=left_node.second;jDelegate<node.second;++jDelegate)
		{
			//fetch data for extrapolate
			size_t iNode_gTransformed = gDelegate_IndexMappping_gTransformed_(iDelegate,jDelegate).first;
			size_t jNode_gTransformed = gDelegate_IndexMappping_gTransformed_(iDelegate,jDelegate).second;
			gTransformed_(iNode_gTransformed,jNode_gTransformed)=cell_value;
		}
	}

	std::pair<size_t, size_t> top_right_node(iDelegate,jDelegate);
	if(iDelegate>1) {top_right_node.first-=1;top_right_node.second+=1;}

	// If top-right-left node is a truncated node, top-right direction
	while(  (top_right_node.second < max_gDelegate_column && top_right_node.first > 1 )
		&& std::find(truncated_cell_.begin(),truncated_cell_.end(), top_right_node) != truncated_cell_.end() )
	{
		++top_right_node.second;
		--top_right_node.first;
	}

	assert(left_node.first == node.first);
	assert(node.first - top_right_node.first == top_right_node.second - node.second);

	// the case of top-left corner, every node are coincided
	if(left_node.second == node.second && node.first == top_right_node.first)
	{
		assert(iDelegate==1 && jDelegate == 1);
		interpolator_.extrapolate_TopLeftCorner(iRow_gTransformed,jCol_gTransformed);
	}

	// the case left_node coincide with node, this is a triangle
	if(left_node.second == node.second && node.first != top_right_node.first)// this is a triangle
	{
		assert(jDelegate==1);//triangle only when jDelegate=1
		const size_t topRowNODE    = gDelegate_IndexMappping_gTransformed_(top_right_node.first,top_right_node.second).first ;
		const size_t bottomRowNODE = gDelegate_IndexMappping_gTransformed_(iDelegate,jDelegate).first ;
		const size_t leftColNODE   = gDelegate_IndexMappping_gTransformed_(iDelegate,jDelegate).second ;
		const size_t rightColNODE = gDelegate_IndexMappping_gTransformed_(top_right_node.first,top_right_node.second).second ;

		interpolator_.interpolate_SubUpperTriangle(topRowNODE, bottomRowNODE, leftColNODE, rightColNODE);
		interpolator_.extrapolate_Left(leftColNODE, topRowNODE, bottomRowNODE);
	}

	// the case top-right_node coincide with node, this is a row segment
	if(top_right_node.first == node.first && left_node.second != node.second)// this is a row segment
	{
		assert(iDelegate==1);//row segment only when iDelegate=1
		const size_t rowNODE      = gDelegate_IndexMappping_gTransformed_(node.first,node.second).first;
		const size_t leftColNODE  = gDelegate_IndexMappping_gTransformed_(left_node.first,left_node.second).second;
		const size_t rightColNODE = gDelegate_IndexMappping_gTransformed_(node.first,node.second).second;

		interpolator_.interpolate_ROW(rowNODE, leftColNODE, rightColNODE );
		interpolator_.extrapolate_Top(rowNODE, leftColNODE, rightColNODE);
	}

	if(left_node.second != node.second && top_right_node.first != node.first) // this is a parrallelogram
	{
		assert(iDelegate>1 && jDelegate>1);
		const size_t leftleftColNODE = gDelegate_IndexMappping_gTransformed_(left_node.first,left_node.second).second    ;
		const size_t leftColNODE = jCol_gTransformed;
		const size_t rightrightColNODE = gDelegate_IndexMappping_gTransformed_(top_right_node.first,top_right_node.second).second    ;
		const size_t rightColNODE = rightrightColNODE - (leftColNODE-leftleftColNODE);

		const size_t topRowNODE = gDelegate_IndexMappping_gTransformed_(top_right_node.first,top_right_node.second).first;
		const size_t bottomRowNODE = iRow_gTransformed;

		interpolator_.interpolate_Parallelogram(topRowNODE, bottomRowNODE, leftleftColNODE, leftColNODE, rightColNODE,rightrightColNODE );

		if(top_right_node.second==1)
		{
			interpolator_.extrapolate_Top(topRowNODE, rightColNODE, rightrightColNODE);
		}

		//reset_gDelegateParrallelogram(cell_value, node, left_node, top_right_node);
	}

	update_g_from_gTransformed(1,gTransformed_size -2 ); // bourrin, transform all gTransformed to g
}

//void GMatrixMapping::reset_gDelegateParrallelogram( const double& cell_value
//												   , const std::pair<size_t,size_t>& node
//												   , const std::pair<size_t,size_t>& left_node
//												   , const std::pair<size_t,size_t>& top_right_node )
//{
//	// parallellograme condition
//	assert(node.first == left_node.first);
//	assert(node.first > top_right_node.first);
//	assert(node.second < top_right_node.second);
//
//	assert( std::find(truncated_gDelegate_Rows_.begin(),truncated_gDelegate_Rows_.end(), node.first) == truncated_gDelegate_Rows_.end() );
//	assert( std::find(truncated_gDelegate_Cols_.begin(),truncated_gDelegate_Cols_.end(), node.second) == truncated_gDelegate_Cols_.end() );
//
//
//	size_t gDelegate_matrix_size = gDelegate_.size1();
//	const size_t iDelegate = node.first;
//	const size_t jDelegate = node.second;
//
//	assert(gDelegate_.size1() == gDelegate_.size2());
//	assert(jDelegate<gDelegate_matrix_size-1);
//	assert(iDelegate<gDelegate_matrix_size-1);
//	assert(iDelegate+jDelegate < gDelegate_matrix_size);
//
//	gDelegate_(iDelegate , jDelegate) = cell_value ;
//
//	// fetch data only on the cell
//	size_t gTransformed_size = gTransformed_.size1();
//	size_t iRow_gTransformed = gDelegate_IndexMappping_gTransformed_(iDelegate,jDelegate).first;
//	size_t jCol_gTransformed = gDelegate_IndexMappping_gTransformed_(iDelegate,jDelegate).second;
//
//	gTransformed_(iRow_gTransformed,jCol_gTransformed)=cell_value;
//
//	// 1x1, extrapolate the corner
//	if(iDelegate==1 && jDelegate == 1)
//	{
//		interpolator_.extrapolate_TopLeftCorner(iRow_gTransformed,jCol_gTransformed);
//	}
//
//	// (1,.) interpolate line and extrapolate top
//	if(iDelegate==1 && jDelegate > 1)
//	{
//		assert(left_node.second < jDelegate); // non empty line segment
//		const size_t rowNODE      = iRow_gTransformed;
//		const size_t leftColNODE  = gDelegate_IndexMappping_gTransformed_(left_node.first,left_node.second).second ;
//		const size_t rightColNODE = gDelegate_IndexMappping_gTransformed_(iDelegate,jDelegate).second;
//		interpolator_.interpolate_ROW(rowNODE, leftColNODE, rightColNODE);
//		interpolator_.extrapolate_Top(rowNODE, leftColNODE, rightColNODE);
//	}
//
//	if( iDelegate>1 )
//	{
//		// interpolate upper and lower block of gTransformed, corresponding to the rectangle of g matrix
//
//		if(jDelegate == 1 ) //if(jDelegate == 1 ) ////// extrapolate left margin
//		{
//			assert(left_node.first == iDelegate) ;//left node is coincide with node
//			const size_t topRowNODE    = gDelegate_IndexMappping_gTransformed_(top_right_node.first,top_right_node.second).first  ;
//			const size_t bottomRowNODE = gDelegate_IndexMappping_gTransformed_(iDelegate  ,jDelegate).first;
//			const size_t leftcolNODE  = jCol_gTransformed ;
//			const size_t rightcolNODE  = gDelegate_IndexMappping_gTransformed_(top_right_node.first,top_right_node.second).second ;
//
//			interpolator_.interpolate_SubUpperTriangle(topRowNODE,bottomRowNODE, leftcolNODE, rightcolNODE );
//			interpolator_.extrapolate_Left(leftcolNODE,topRowNODE,bottomRowNODE);
//
//			if(top_right_node.second==1)
//			{
//				interpolator_.extrapolate_Top(topRowNODE, leftcolNODE, rightcolNODE);
//			}
//		}		
//		else
//		{		
//			const size_t leftleftColNODE = gDelegate_IndexMappping_gTransformed_(left_node.first,left_node.second).second    ;
//			const size_t leftColNODE = jCol_gTransformed;
//			const size_t rightrightColNODE = gDelegate_IndexMappping_gTransformed_(top_right_node.first,top_right_node.second).second    ;
//			const size_t rightColNODE = rightrightColNODE - (leftColNODE-leftleftColNODE);
//
//			const size_t topRowNODE = gDelegate_IndexMappping_gTransformed_(top_right_node.first,top_right_node.second).first;
//			const size_t bottomRowNODE = iRow_gTransformed;
//
//			interpolator_.interpolate_Parallelogram(topRowNODE, bottomRowNODE, leftleftColNODE, leftColNODE, rightColNODE,rightrightColNODE );
//
//			if(top_right_node.second==1)
//			{
//				interpolator_.extrapolate_Top(topRowNODE, rightColNODE, rightrightColNODE);
//			}
//		}
//
//
//
//	}
//
//	update_g_from_gTransformed(1,gTransformed_size -2 ); // bourrin, transform all gTransformed to g
//
//}


void GMatrixMapping::add_Truncated_gDelegate_Cells(const std::pair<size_t,size_t>& gDelegateCELL)
{
	assert(gDelegateCELL.first>0 && gDelegateCELL.second >0);
	assert(gDelegateCELL.first + gDelegateCELL.second < gDelegate_.size1() );

	if( std::find(truncated_cell_.begin(), truncated_cell_.end(), gDelegateCELL) == truncated_cell_.end() )
	{
		gDelegate_(gDelegateCELL.first,gDelegateCELL.second)=-10000; // set value to absurbe
		truncated_cell_.push_back(gDelegateCELL);
		std::sort( truncated_cell_.begin(), truncated_cell_.end() );
	}
	else
	{
		if(LMM::WARNLMM ())
			std::cout<<LMM::WARN_MSG<<"GMatrixMapping::add_Truncated_gDelegate_Cells "<<gDelegateCELL<<" already added"<<std::endl;
	}

	global_proccess_transform();
}

void GMatrixMapping::reset_Truncated_gDelegate_Rows(const std::vector<size_t> & gDelegate_rows_indices)
{
	if(gDelegate_rows_indices.size() > 0)
	{
		assert(gDelegate_rows_indices.front() > 0 ); 
		assert(gDelegate_rows_indices.back() < gDelegate_.size1()-1 ); 
		truncated_gDelegate_Rows_=gDelegate_rows_indices;

		//! set gDelegate truncated rows to absurb values
		for(size_t iTruncated=0;iTruncated<truncated_gDelegate_Rows_.size();++iTruncated)
		{
			const size_t iDelegate = truncated_gDelegate_Rows_[iTruncated];
			const size_t col_bound = gDelegate_.size2() - iDelegate;
			for(size_t jDelegate=1;jDelegate<col_bound;++jDelegate)
			{
				gDelegate_(iDelegate,jDelegate)=-100000;
			}
		}

		initiate_interpolation_indices();
	}
}
void GMatrixMapping::reset_Truncated_gDelegate_Cols(const std::vector<size_t> & gDelegate_cols_indices)
{
	if(gDelegate_cols_indices.size() > 0)
	{
		assert(gDelegate_cols_indices.front() > 0 ); 
		assert(gDelegate_cols_indices.back() < gDelegate_.size2()-1 ); 
		truncated_gDelegate_Cols_=gDelegate_cols_indices;

		//! set gDelegate truncated rows to absurb values
		for(size_t iTruncated=0;iTruncated<truncated_gDelegate_Cols_.size();++iTruncated)
		{
			const size_t jDelegate = truncated_gDelegate_Cols_[iTruncated];
			const size_t row_bound = gDelegate_.size1() - jDelegate;
			for(size_t iDelegate=1;iDelegate<row_bound;++iDelegate)
			{
				gDelegate_(iDelegate,jDelegate)=-100000;
			}
		}

		initiate_interpolation_indices();
	}
}


GMatrixMapping::GMatrixMapping(size_t full_g_size,
							   const UpperTriangularDoubleMatrix&  gDelegate, // used as calibration input
							   const UpperTriangularIndexPairMatrix& gDelegate_IndexMappping_gTransformed)
							   : g_(full_g_size, full_g_size)
							   , gTransformed_(full_g_size,full_g_size)
							   , gDelegateFlags_( gDelegate.size1(),gDelegate.size2() )
							   , gDelegate_(gDelegate)
							   , gDelegate_IndexMappping_gTransformed_(gDelegate_IndexMappping_gTransformed)
							   , interpolator_(gTransformed_)
{
	assert(gDelegate_IndexMappping_gTransformed.size1() == gDelegate_.size1() );
	assert(gDelegate_IndexMappping_gTransformed.size2() == gDelegate_.size2() );
	assert(gDelegate_.size1() == gDelegate_.size2());

	re_initiate_FlagMatrix();

	//! init g to absurd value
	for(size_t i=1; i<g_.size1(); ++i)
	{
		for(size_t j=1; j<=i; ++j)
		{
			g_(i,j) = 1.0e100;
		}
	}

	//! init gTranformed to absurd value
	for(size_t i=1; i<gTransformed_.size1(); ++i)
	{
		for(size_t j=1; j<gTransformed_.size2()-i; ++j)
		{
			gTransformed_(i,j) = 1.0e100;
		}
	}


	///// initiate first row and first column to indices
	for(size_t kDelegaete=0; kDelegaete<gDelegate_.size1(); ++kDelegaete)
	{
		gDelegate_(0,kDelegaete)=kDelegaete;
		gDelegate_(kDelegaete,0)=kDelegaete;
	}

	// initiate row and cols interpolations indices
	initiate_interpolation_indices();

	global_proccess_transform();
}	


void GMatrixMapping::re_initiate_FlagMatrix()
{
	//! init gTranformed to absurd value
	for(size_t i=1; i<gDelegateFlags_.size1(); ++i)
	{
		for(size_t j=1; j<gDelegateFlags_.size2()-i; ++j)
		{
			gDelegateFlags_(i,j) = true;
		}
	}
}

void GMatrixMapping::initiate_interpolation_indices()
{
	std::vector<size_t> row_nodes;
	std::vector<size_t> col_nodes;

	size_t delegate_size = gDelegate_IndexMappping_gTransformed_.size1();
	for(size_t kDelegate=1;kDelegate<delegate_size-1;++kDelegate)
	{
		//if node is not a truncated row, then add to node list
		if(std::find(truncated_gDelegate_Rows_.begin(),truncated_gDelegate_Rows_.end(), kDelegate) ==  truncated_gDelegate_Rows_.end() )
		{
			row_nodes.push_back( gDelegate_IndexMappping_gTransformed_(kDelegate,1).first );
		}

		//if node is not a truncated col, then add to node list
		if(std::find(truncated_gDelegate_Cols_.begin(),truncated_gDelegate_Cols_.end(), kDelegate) ==  truncated_gDelegate_Cols_.end() )
		{
			col_nodes.push_back( gDelegate_IndexMappping_gTransformed_(1,kDelegate).second );
		}
	}

	size_t gTransform_size = gTransformed_.size1();
	full_cols_interpolations_indices_.clear();
	full_rows_interpolations_indices_.clear();
	for(size_t kgTransformed=1;kgTransformed<gTransform_size-1;++kgTransformed)
	{
		if(std::find(row_nodes.begin(),row_nodes.end(), kgTransformed) == row_nodes.end() )
			full_rows_interpolations_indices_.push_back(kgTransformed);

		if(std::find(col_nodes.begin(),col_nodes.end(), kgTransformed) == col_nodes.end() )
			full_cols_interpolations_indices_.push_back(kgTransformed);;
	}
}

//! gDelegate_ (interpolates) --> gTransformed (update_g_from_gTransforme) -> g
void GMatrixMapping::global_proccess_transform()
{
	// interpolate gDelegate before processing globally when truncated cells is not empty
	if(truncated_cell_.size()>0 )
	{
		interpolate_gDelegate();
	}

	size_t max_DelegateRowIndex = get_MaxDelegateRowIndex() ;

	for (size_t iiRowDelegate=1; iiRowDelegate<= max_DelegateRowIndex ; ++iiRowDelegate) // row
	{
		rowDelegate_proccess_transform(iiRowDelegate);		
	}
}

void GMatrixMapping::fetch_gDelegate_to_gTransformd(const size_t iiRowDelegate)
{
	size_t nbDelegateCol = gDelegate_IndexMappping_gTransformed_.size2() - iiRowDelegate;
	for(size_t jColDelegate=1;jColDelegate<nbDelegateCol;++jColDelegate)
	{
		// fetch if the column is not truncated
		if(std::find(truncated_gDelegate_Cols_.begin(), truncated_gDelegate_Cols_.end(), jColDelegate) == truncated_gDelegate_Cols_.end() )
		{
			size_t iRow_gTransformed = gDelegate_IndexMappping_gTransformed_(iiRowDelegate,jColDelegate).first;
			size_t jCol_gTransformed = gDelegate_IndexMappping_gTransformed_(iiRowDelegate,jColDelegate).second;

			gTransformed_(iRow_gTransformed,jCol_gTransformed) = gDelegate_(iiRowDelegate,jColDelegate);
		}
	}
}

void GMatrixMapping::rowDelegate_proccess_transform(const size_t iiRowDelegate)
{
	assert(iiRowDelegate>0);

	// process iff this is not a truncated row
	if(std::find(truncated_gDelegate_Rows_.begin(), truncated_gDelegate_Rows_.end(), iiRowDelegate) == truncated_gDelegate_Rows_.end() )
	{
		fetch_gDelegate_to_gTransformd(iiRowDelegate);

		size_t iRowDelegate = iiRowDelegate-1;
		while(iRowDelegate>0 && std::find(truncated_gDelegate_Rows_.begin(), truncated_gDelegate_Rows_.end(), iRowDelegate) != truncated_gDelegate_Rows_.end() )
		{
			--iRowDelegate;
		}

		////interpolate_gTransform(iRowDelegate);
		//std::vector<size_t> rows_interpolation_indices = get_interpolationRowsIndices_gTransformed(iRowDelegate,iiRowDelegate);
		//UpperTriangularMatrixColRowInterpolator interpolator(gTransformed_);
		//interpolator.interpolate(rows_interpolation_indices, full_cols_interpolations_indices_);

		UpperTriangularMatrixBlockInterpolator interpolator(gTransformed_);
		interpolator.interpolate(gDelegate_IndexMappping_gTransformed_);

		size_t iRow_gtransformed  ;
		size_t iiRow_gtransformed ; 

		if(iRowDelegate == 0)
			iRow_gtransformed = 1;		
		else
			iRow_gtransformed  = gDelegate_IndexMappping_gTransformed_(iRowDelegate, 1 ).first;		

		if(iiRowDelegate < get_MaxDelegateRowIndex() )
			iiRow_gtransformed = gDelegate_IndexMappping_gTransformed_(iiRowDelegate   , 1 ).first;
		else
			iiRow_gtransformed = gTransformed_.size1()- 2;

		update_g_from_gTransformed(iRow_gtransformed,iiRow_gtransformed);
	}
}

void GMatrixMapping::update_g_from_gTransformed(const size_t iROW_gTransformed, const size_t iiROW_gTransformed)
{

	for(size_t iRow_gTransformed = iROW_gTransformed;iRow_gTransformed<=iiROW_gTransformed;++iRow_gTransformed)
	{
		size_t firstCol_gTransform = 1;
		size_t lastCol_gTransform = gTransformed_.size2() - (iRow_gTransformed+1); 

		for(size_t jCol_gTransformed=firstCol_gTransform;jCol_gTransformed <= lastCol_gTransform;++jCol_gTransformed)
		{
			std::pair<size_t,size_t> gCell = get_lowerMatrixCell_From_upperMatrixCell(std::pair<size_t,size_t>(iRow_gTransformed,jCol_gTransformed) );

			//size_t iRow_g = (iRow_gTransformed-1) + jCol_gTransformed;
			//size_t jCol_g = iRow_gTransformed; 

			g_(gCell.first,gCell.second) = gTransformed_(iRow_gTransformed,jCol_gTransformed);
		}

		// extrapolate the last element of g's column
		g_(g_.size1()-1,iRow_gTransformed) = g_(g_.size1()-2,iRow_gTransformed);
	}

	// the last row of g Matrix is special
	if(iiROW_gTransformed == gTransformed_.size1()-2)
	{
		g_(g_.size1()-1,g_.size2()-1) = g_(g_.size1()-2,g_.size2()-2);	
	}
}

void GMatrixMapping::print(const std::string& filename) const
{
	std::string path_OutPut = LMMPATH::get_output_path() + filename;

	{
		std::vector<PrintElement_PTR> elements_print;

		LowerTriangularDoubleMatrix reTransformedgDelegate = get_gDelegateInNaturalFormat();
		PrintElement_PTR reverse_gDelegate_print = PrintElement_PTR(new MatrixPrintElement<LowerTriangularDoubleMatrix>("gDelegate In Libor Format  ", reTransformedgDelegate));			
		PrintElement_PTR gDelegate_print = PrintElement_PTR(new MatrixPrintElement<UpperTriangularDoubleMatrix>("gDelegate",  gDelegate_));

		elements_print.push_back(reverse_gDelegate_print);
		elements_print.push_back(gDelegate_print);

		Printer printer(path_OutPut, elements_print);printer.print(); 
	}

	{/////    print index Mapping gDelegate to gTransformed
		std::ofstream gMatrixFile ;
		gMatrixFile.open(path_OutPut.c_str(), std::ios::app);
		gMatrixFile<<std::endl<<std::endl;
		gMatrixFile.close();

		std::vector<PrintElement_PTR> elements_print;
		PrintElement_PTR index_map_print = PrintElement_PTR(new MatrixPrintElement<UpperTriangularIndexPairMatrix>("index Map To gTransformed ",  gDelegate_IndexMappping_gTransformed_));
		elements_print.push_back(index_map_print);
		Printer printer(path_OutPut, elements_print);printer.print(false); 
	}

	{/////    print gDelegate truncated indices
		if(truncated_gDelegate_Rows_.size()>0 || truncated_gDelegate_Cols_.size()>0 )
		{
			std::ofstream gMatrixFile ;	gMatrixFile.open(path_OutPut.c_str(), std::ios::app);	gMatrixFile<<std::endl<<std::endl;	gMatrixFile.close();

			std::vector<PrintElement_PTR> elements_print;
			PrintElement_PTR index_rows_print = PrintElement_PTR(new VectorPrintElement<std::vector<size_t> >("Unused gDelegate Rows ",  truncated_gDelegate_Rows_));
			PrintElement_PTR index_cols_print = PrintElement_PTR(new VectorPrintElement<std::vector<size_t> >("Unused gDelegate Cols ",  truncated_gDelegate_Cols_));
			elements_print.push_back(index_rows_print);
			elements_print.push_back(index_cols_print);
			Printer printer(path_OutPut, elements_print);printer.print(false); 
		}
	}


	{/////    print gTransformed

		std::ofstream gMatrixFile ;
		gMatrixFile.open(path_OutPut.c_str(), std::ios::app);
		gMatrixFile<<std::endl<<std::endl;
		gMatrixFile.close();

		std::vector<PrintElement_PTR> elements_print;
		PrintElement_PTR gTransformed_print = PrintElement_PTR(new MatrixPrintElement<UpperTriangularDoubleMatrix>("gTransformed",  gTransformed_));
		elements_print.push_back(gTransformed_print);
		Printer printer(path_OutPut, elements_print); printer.print(false);
	}

	{/////    print g
		std::ofstream gMatrixFile ;
		gMatrixFile.open(path_OutPut.c_str(), std::ios::app);
		gMatrixFile<<std::endl<<std::endl;
		gMatrixFile.close();

		std::vector<PrintElement_PTR> elements_print;
		PrintElement_PTR g_print = PrintElement_PTR(new MatrixPrintElement<LowerTriangularDoubleMatrix>("full g ",  g_));			
		elements_print.push_back(g_print);
		Printer printer(path_OutPut, elements_print); printer.print(false); 
	}

	{/////  manually print gDelegateFlags_

		std::ofstream gMatrixFile ;
		gMatrixFile.open(path_OutPut.c_str(), std::ios::app);

		gMatrixFile<<std::endl<<std::endl;
		gMatrixFile<<"gDelegateFlags_,"<<std::endl;
		for(size_t i=0; i<gDelegateFlags_.size1(); ++i)
		{
			for(size_t j=0; j<gDelegateFlags_.size2()-i; ++j)
			{
				if(i==0 || j==0)
				{
					gMatrixFile<<"# NON #,";
				}
				else
				{
					if( gDelegateFlags_(i,j) )
					{
						gMatrixFile<<"1 ,";
					}
					else
					{
						gMatrixFile<<"  ,";
					}
				}				
			}
			gMatrixFile<<" ,"<<std::endl;
		}
		gMatrixFile.close();
	}/////  manually print gDelegateFlags_




}

std::vector<size_t> GMatrixMapping::get_interpolationRowsIndices_gTransformed(const size_t iRowDelegate, const size_t iiRowDelegate) const
{
	std::vector<size_t> row_interpolation_indices;

	size_t max_gTransform_row_index = gDelegate_IndexMappping_gTransformed_(iiRowDelegate,1).first;
	size_t min_gTransform_row_index=1;
	if(iRowDelegate > 0)
		min_gTransform_row_index= gDelegate_IndexMappping_gTransformed_(iRowDelegate,1).first;

	for(size_t iInterpole=0;iInterpole<full_rows_interpolations_indices_.size();++iInterpole)
	{
		if(full_rows_interpolations_indices_[iInterpole]>=max_gTransform_row_index )
		{
			break;
		}
		else if(full_rows_interpolations_indices_[iInterpole]<min_gTransform_row_index)
		{
			continue;
		}
		else
		{
			row_interpolation_indices.push_back( full_rows_interpolations_indices_[iInterpole] )  ;			
		}
	}

	////// the last iDelegate has to extrapolate the row queue
	size_t check_last_iiRowDelegate = iiRowDelegate + 1 ;
	bool extrapolate_last_rows=false;
	// while find iiRow in the truncated row indices, increase iiRow
	while(check_last_iiRowDelegate < gDelegate_IndexMappping_gTransformed_.size1()-2 
		&& std::find(truncated_gDelegate_Rows_.begin(), truncated_gDelegate_Rows_.end(),check_last_iiRowDelegate ) != truncated_gDelegate_Rows_.end()
		)
	{
		++check_last_iiRowDelegate;
	}
	if(check_last_iiRowDelegate == gDelegate_IndexMappping_gTransformed_.size1()-2 )
	{
		extrapolate_last_rows=true;
	}

	if(extrapolate_last_rows)
	{
		//// extrapolate for every row after map(iiRowDelegate)
		for(size_t iExtrapolate_gTransformed=max_gTransform_row_index+1;iExtrapolate_gTransformed<gTransformed_.size1()-1;++iExtrapolate_gTransformed)
		{
			row_interpolation_indices.push_back(iExtrapolate_gTransformed);
		}
	}

	return row_interpolation_indices;
}


//! method retransform the gDelegate matrix in natural format, coherent to the gMatrix
LowerTriangularDoubleMatrix GMatrixMapping::get_gDelegateInNaturalFormat() const
{
	LowerTriangularDoubleMatrix reTransformedgDelegate(gDelegate_.size1(),gDelegate_.size2());
	//Todo Print reTransformedgDelegate with first column and first row as libor indices

	for(size_t iDelegate=1;iDelegate<gDelegate_.size1();++iDelegate)
	{
		for(size_t jDelegate=1;jDelegate<gDelegate_.size2()-iDelegate;++jDelegate)
		{
			std::pair<size_t,size_t> natural_libor_indices = get_lowerMatrixCell_From_upperMatrixCell(std::pair<size_t,size_t>(iDelegate,jDelegate) );
			reTransformedgDelegate(natural_libor_indices.first,natural_libor_indices.second) = gDelegate_(iDelegate,jDelegate);
		}	
	}

	//last row and first col to indices
	for(size_t kDelegate=0;kDelegate<gDelegate_.size1();++kDelegate)
	{
		reTransformedgDelegate(kDelegate,0) = kDelegate                    ; //first col
		reTransformedgDelegate(gDelegate_.size1()-1,kDelegate) = kDelegate ; //last row
	}

	return reTransformedgDelegate;
}

std::pair<size_t,size_t> GMatrixMapping::get_lowerMatrixCell_From_upperMatrixCell(const std::pair<size_t,size_t>& gTransformedCellIndice) const
{
	const size_t & gT_row = gTransformedCellIndice.first;
	const size_t & gT_col = gTransformedCellIndice.second;

	// a col in gTransformed is a sub diagonal in g
	// a diagonal is characterized by its diff col-row 
	const size_t g_row_col_diff = gT_col-1;
	const size_t g_col = gT_row;
	const size_t g_row = g_col + g_row_col_diff;

	assert(g_row >= g_col) ; // lower triangular property
	//std::cout<<"get_lowerMatrixCell_From_upperMatrixCell			gTransformed[" <<gT_row<<","<<gT_col<<"]	-->  g["<<g_row<<","<<g_col<<"]"<<std::endl;
	return std::pair<size_t,size_t>(g_row,g_col);
}

//std::pair<size_t,size_t> GMatrixMapping::get_upperMatrixCell_From_lowerMatrixCell(const std::pair<size_t,size_t>& gCellIndice) const
//{
//	const size_t & g_row = gCellIndice.first;
//	const size_t & g_col = gCellIndice.second;
//
//	// a col in gTransformed is a sub diagonal in g
//	// a diagonal is characterized by its diff col-row 
//	const size_t g_row_col_diff = g_row - g_col;
//
//	const size_t gT_row = g_col ;
//	const size_t gT_col = g_row_col_diff ;
//		
//	//std::cout<<"get_upperMatrixCell_From_lowerMatrixCell			g["<<g_row<<","<<g_col<<"]		--> gTransformed[" <<gT_row<<","<<gT_col<<"]"<<std::endl;
//	return std::pair<size_t,size_t>(gT_row,gT_col);
//}

void GMatrixMapping::interpolate_gDelegate()
{
	assert(truncated_cell_.size()>0);
	size_t matrix_size = gDelegate_.size1();
	size_t nbTruncatedCell = truncated_cell_.size();
	for(size_t iCell=0;iCell<nbTruncatedCell;++iCell)
	{
		size_t iTruncated = truncated_cell_[iCell].first;
		size_t jTruncated = truncated_cell_[iCell].second;

		assert(iTruncated>0 && jTruncated>0);
		assert(iTruncated+jTruncated<matrix_size);

		//first cell --> extrapolate by row
		if( iTruncated==1 && jTruncated==1 )
		{
			gDelegate_(1,1) = gDelegate_(2,1);
		}

		//last cell on first row --> extrapolate by row
		if( iTruncated==1 && jTruncated== matrix_size - 2 )
		{
			gDelegate_(1 , matrix_size-2) = gDelegate_( 1 , matrix_size-3 );
		}

		//last cell on first col  --> extrapolate by col gDelegate
		if( iTruncated == matrix_size - 2 && jTruncated ==1 )
		{			
			std::pair<size_t, size_t> offset_cell(iTruncated - 1, 1 );
			while( std::find(truncated_cell_.begin(), truncated_cell_.end(), offset_cell ) != truncated_cell_.end()  &&  offset_cell.first > 1 )
			{
				offset_cell.first-=1;
			}

			for(size_t row_extrapolate=offset_cell.first+1; row_extrapolate <= iTruncated ; ++row_extrapolate)
			{
				gDelegate_(row_extrapolate , 1) = gDelegate_(offset_cell.first , 1);
			}
		}

		// on diagonal  --> interpolate by diagonal gDelegate
		if(iTruncated+jTruncated == matrix_size-1 && iTruncated>1 && jTruncated>1)
		{
			gDelegate_(iTruncated,jTruncated) = 0.5*( gDelegate_(iTruncated-1,jTruncated+1) + gDelegate_(iTruncated+1,jTruncated-1) );
		}

		//in gDelegate interior --> interpolate by col gDelegate
		if(iTruncated+jTruncated < matrix_size-1 && !(iTruncated==1 && jTruncated==1 ) )
		{
			// if truncted in first row, interpolate by row
			if(iTruncated == 1)
			{
				gDelegate_(iTruncated,jTruncated) = 0.5*( gDelegate_(iTruncated,jTruncated-1) + gDelegate_(iTruncated,jTruncated+1) );
			}
			else
			{
				gDelegate_(iTruncated,jTruncated) = 0.5*( gDelegate_(iTruncated-1,jTruncated) + gDelegate_(iTruncated+1,jTruncated) );
			}
		}
	}
}