/*###############################################
#   chithanhnguyen.math@gmail.com               #
#                                               #
#   Lunalogic intership project 2014            #
#   LIBOR Market Model                          #
#   Technologies:                               #
#   - cmake                                     #
#   - boost                                     #
#   - QuantLib                                  #
#                                               #
###############################################*/

#ifndef LMM_GENERIC_PATH_H
#define LMM_GENERIC_PATH_H

#include <string>
#include <direct.h>
#include <cassert>
#include <iostream>

class LMMPATH
{
	/*! Function returning the absolut output/input path
	 *  User when writing a program and need to printout to a file at execution time, 
	 *  it is recommaned to store the output file in this folder
	 *  for exemple in your programme, save the file to the path,   
	 *
	 *  LMMPATH::get_output_path() + "my_output_file.txt"
	 */
	//! ctntodo use of boost::filesystem in order to have a generic path
	
	//! adapt your runtime output path here
public:
	// useful to mark the time-date of runing a test

	static std::string get_TimeDateNow();

	// get base file name from a full file name (pop extension part)
	static std::string get_BaseFileName(const std::string& filename);	

	static std::string get_output_path();	

	//! adapt your runtime input path here
	static std::string get_runtime_datapath();

	static void reset_Output_SubFolder(const std::string& foldername);

	static std::string get_Root_OutputPath();	
private :
	// tested for both directory name end with double slash or not
	static void create_output_directory(const std::string& foldername);
	
	static std::string output_subFolder;
};

#endif /* LMM_GENERIC_PATH_H */
