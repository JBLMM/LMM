#include <LMM/helper/GenericPath.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <ctime>

std::string LMMPATH::output_subFolder;

std::string LMMPATH::get_TimeDateNow()
{
	std::stringstream ss;
	std::time_t t = time(0);   // get time now
    struct std::tm * now = std::localtime( &t );
	ss << "Date " <<now->tm_mday << "/" << (now->tm_mon + 1) << '/' << (now->tm_year + 1900)  <<"  ";
	ss << now->tm_hour << ":" << now->tm_min << ':' <<  now->tm_sec;
	return ss.str();
}

std::string LMMPATH::get_BaseFileName(const std::string& filename)
{
	std::stringstream copy_input_file_name(filename) ;
	std::string base_file_name ;
	getline(copy_input_file_name,base_file_name,'.');
	return base_file_name ;
}

std::string LMMPATH::get_Root_OutputPath()
{
	// set the absolute path to your output folder here (use in execute time)
	// absolute path common for Yuan Li's computer 
	return "C:\\Users\\junbin.liu\\Documents\\LunaLMM\\data\\output\\";
}

void LMMPATH::reset_Output_SubFolder(const std::string& foldername)
{
	LMMPATH::output_subFolder.clear();
	LMMPATH::output_subFolder=foldername;
	LMMPATH::create_output_directory(foldername);
}

//! adapt your runtime output path here
std::string LMMPATH::get_output_path()
{
	std::string output_path= LMMPATH::get_Root_OutputPath();

	output_path+=LMMPATH::output_subFolder;

	return output_path;
}


//! adapt your runtime input path here
std::string LMMPATH::get_runtime_datapath()
{
	// set the absolute path to your input folder here (use in execute time)

	// absolute path common for Yuan Li's computer 
	return "C:\\Users\\junbin.liu\\Documents\\LunaLMM\\data\\input\\";		        
}

// tested for both directory name end with double slash or not
void LMMPATH::create_output_directory(const std::string& dir_name_slash_end)
{
	//get the folder path
	std::vector<std::string> folders_path;
	std::stringstream copy_sub_folder_path(dir_name_slash_end) ;
	std::string foldername;
	while(getline(copy_sub_folder_path,foldername,'\\'))
	{
		if(!foldername.empty() )
		{
			folders_path.push_back(foldername);
		}
	}

	const std::string root_output_path = LMMPATH::get_Root_OutputPath();

	std::string full_path = root_output_path ;
	for(size_t iFolder=0;iFolder<folders_path.size();++iFolder)
	{
		full_path += folders_path[iFolder];
		full_path += "\\";
		
		if(_mkdir( full_path.c_str() ) == 0 )
		{
			std::cout<<" Succesfully created directory "<<full_path<<std::endl;
		}
		else
		{
			std::cout<<" LMM ERROR "<<full_path<< "	can not be created " <<std::endl;
		}
	}
}
