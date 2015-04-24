#pragma once

#include <vector>
#include <stdlib.h>

#include <string>
#include <iostream>

/*! regroupe all input file name here 
*
*
*
*/

namespace InputFileManager
{

	inline std::vector<std::string> get_VCUB_FileList()
	{
		std::vector<std::string> file_list;

		file_list.push_back("VCUB_2008_01_02.csv");// 0
		file_list.push_back("VCUB_2008_04_02.csv");// 1
		file_list.push_back("VCUB_2008_07_02.csv");// 2
		file_list.push_back("VCUB_2008_10_01.csv");// 3
		file_list.push_back("VCUB_2009_01_07.csv");// 4
		file_list.push_back("VCUB_2009_04_01.csv");// 5
		file_list.push_back("VCUB_2009_07_01.csv");// 6
		file_list.push_back("VCUB_2009_10_07.csv");// 7
		file_list.push_back("VCUB_2010_01_06.csv");// 8
		file_list.push_back("VCUB_2010_04_07.csv");// 9
		file_list.push_back("VCUB_2010_07_07.csv");// 10
		file_list.push_back("VCUB_2010_10_06.csv");// 11
		file_list.push_back("VCUB_2011_01_05.csv");// 12
		file_list.push_back("VCUB_2011_04_06.csv");// 13
		file_list.push_back("VCUB_2011_07_06.csv");// 14
		file_list.push_back("VCUB_2011_10_05.csv");// 15
		file_list.push_back("VCUB_2012_01_04.csv");// 16
		file_list.push_back("VCUB_2012_04_04.csv");// 17
		file_list.push_back("VCUB_2012_07_04.csv");// 18
		file_list.push_back("VCUB_2012_10_03.csv");// 19
		file_list.push_back("VCUB_2013_01_02.csv");// 20
		file_list.push_back("VCUB_2013_04_04.csv");// 21
		file_list.push_back("VCUB_2013_07_03.csv");// 22
		file_list.push_back("VCUB_2013_10_03.csv");// 23
		file_list.push_back("VCUB_2014_01_01.csv");// 24
		//file_list.push_back("VCUB_2014_02_05.csv");// 25
		//file_list.push_back("VCUB_2014_03_05.csv");// 26
		file_list.push_back("VCUB_2014_04_02.csv");// 27
		//file_list.push_back("VCUB_2014_05_01.csv");// 28
		//file_list.push_back("VCUB_2014_06_04.csv");// 29
		file_list.push_back("VCUB_2014_07_02.csv");// 30
		//file_list.push_back("VCUB_2014_08_01.csv");// 31
		//file_list.push_back("VCUB_2014_08_04.csv");// 31
		//file_list.push_back("VCUB_2014_08_05.csv");// 33
		//file_list.push_back("VCUB_2014_08_06.csv");// 34
		//file_list.push_back("VCUB_2014_08_07.csv");// 35
		//file_list.push_back("VCUB_2014_08_08.csv");// 36
		//file_list.push_back("VCUB_2014_08_14.csv");// 37
		
		//file_list.push_back("VCUB_2004_04_07.csv");
		//file_list.push_back("VCUB_Dirty_2014_08_01.csv");// 	
		
		return file_list;
	};
}