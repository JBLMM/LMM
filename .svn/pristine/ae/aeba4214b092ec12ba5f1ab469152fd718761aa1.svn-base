#################################################
#   chithanhnguyen.math@gmail.com               #
#                                               #
#   Lunalogic intership project 2014            #
#   LIBOR Market Model                          #
#   Technologies:                               #
#   - cmake                                     #
#   - boost                                     #
#   - QuantLib                                  #
#                                               #
#################################################

#CMAKE INITIALIZATION
cmake_minimum_required (VERSION 2.8)

project(LunaLMM)
 
###########################   GLOBAL SETTING   #################################
## header message printing for cmake debugging
set(LUNA_CMAKE_DEBUG_MSG " -LUNA ------  ")

## include the project configuration file
if(EXISTS "${CMAKE_SOURCE_DIR}/user-config.cmake")
    include(${CMAKE_SOURCE_DIR}/user-config.cmake)
else()
    include(${CMAKE_SOURCE_DIR}/config/default-config.cmake)
	configure_file("${CMAKE_SOURCE_DIR}/config/default-config.cmake" "${CMAKE_SOURCE_DIR}/user-config.cmake" @ONLY)
endif() 

## reset shorter luna packages variables for easier use 
set(LUNA_NUMERIC ${LUNA_NUMERIC_PKG_NAME})
set(LUNA_RNGENERATOR ${LUNA_RNGENERATOR_PKG_NAME})
set(LUNA_HELPER ${LUNA_HELPER_PKG_NAME})
set(LUNA_INSTRUMENT ${LUNA_INSTRUMENT_PKG_NAME})
set(LUNA_MODEL ${LUNA_MODEL_PKG_NAME})
set(LUNA_PRICER ${LUNA_PRICER_PKG_NAME})
set(LUNA_CALIBRATION ${LUNA_CALIBRATION_PKG_NAME})

if(NOT LUNA_BUILD_FOR_YUANLI)
	include(CTest)
#enable_testing()
endif()

## activate the Use Folder setting for Visual Studio
## http://www.cmake.org/pipermail/cmake/2010-December/041375.html
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


set(LUNA_ALLIN1_TARGET_DIR ${CMAKE_BINARY_DIR}/${LUNA_BUILD_FOLDER_NAME})

if(NOT EXISTS ${LUNA_ALLIN1_TARGET_DIR})
	file(MAKE_DIRECTORY ${LUNA_ALLIN1_TARGET_DIR})
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LUNA_ALLIN1_TARGET_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LUNA_ALLIN1_TARGET_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LUNA_ALLIN1_TARGET_DIR})




## disable unecessary warnings
if(MSVC)
	add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4996 /wd4535 /nologo" )
endif()

## force using of C++11X on Unix machine
if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") 
endif()
## Boost Finding Library
## You have to rightly specify your BOOST_ROOT environment variable
find_package(Boost COMPONENTS filesystem REQUIRED)
if(Boost_FOUND)
    set(Boost_USE_STATIC_LIBS ON)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif()

## QuantLib Finding Library
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/config/")
find_package(QuantLib)
if(QuantLib_FOUND)
    #set(QuantLib_USE_STATIC_LIBS ON)
    include_directories(${QuantLib_INCLUDE_DIR})
	#    link_directories(${QuantLib_LIBRARY_DIR})
endif()

# add the root source directory for include directory
include_directories(${CMAKE_SOURCE_DIR})

### Runtime path will be used for storing input files required when executing programs
### This path is dependent of OS, for windows, it's "\", for unix it's "/"
### In cmake, set always with forward space, it will by replaced by backward automatically if windows
###

# copy all data from source directory into binary directory in order to execute and retreive output
if(NOT EXISTS ${CMAKE_BINARY_DIR}/data)
	file(COPY "${CMAKE_SOURCE_DIR}/data/" DESTINATION "${CMAKE_BINARY_DIR}/data/")
endif()
	
set(LMM_OUTPUT_RUNTIMEPATH "${CMAKE_BINARY_DIR}/data/output/")
set(LMM_DATA_RUNTIMEPATH "${CMAKE_BINARY_DIR}/data/input/")

if(NOT EXISTS ${LMM_OUTPUT_RUNTIMEPATH})
	file(MAKE_DIRECTORY ${LMM_OUTPUT_RUNTIMEPATH})
endif()

### change "/" to be "\\" in the runtime path for RvsB_path.h
#if(WIN32)
#    string(REGEX REPLACE "/" "\\\\\\\\" LMM_OUTPUT_RUNTIMEPATH "${LMM_OUTPUT_RUNTIMEPATH}")
#    string(REGEX REPLACE "/" "\\\\\\\\" LMM_DATA_RUNTIMEPATH "${LMM_DATA_RUNTIMEPATH}") 
#endif()
#configure_file("${CMAKE_SOURCE_DIR}/config/generic_path.h.in" "${CMAKE_SOURCE_DIR}/LMM/helper/GenericPath.h" @ONLY)


## If build for Yuan Li, cmake enter into sub folders and parse all
## files of the LMM packages linke : numeric, instrument ... 
## and store these sources files name in appropriated variables
add_subdirectory(src)

## If build for Yuan Li, need a specific way to build the executable
if(NOT LUNA_BUILD_FOR_YUANLI)
	add_subdirectory(proc) 
	add_subdirectory(test)
endif()

##################################################
###        SPECIFIC BUILD FOR YUAN LI          ###
##################################################
if(LUNA_BUILD_FOR_YUANLI)
				
	add_library(CommonLMM
				${SRC_${LUNA_RNGENERATOR}}
				${SRC_${LUNA_NUMERIC}}
				${SRC_${LUNA_HELPER}}
				${SRC_${LUNA_INSTRUMENT}}
				${SRC_${LUNA_MODEL}}
				${SRC_${LUNA_PRICER}}
				${SRC_${LUNA_CALIBRATION}}
				)				
	
	source_group(${LUNA_RNGENERATOR} FILES ${SRC_${LUNA_RNGENERATOR}})
	source_group(${LUNA_NUMERIC} FILES ${SRC_${LUNA_NUMERIC}})
	source_group(${LUNA_HELPER} FILES ${SRC_LMM_helper})
	source_group(${LUNA_INSTRUMENT} FILES ${SRC_${LUNA_INSTRUMENT}})
	source_group(${LUNA_MODEL} FILES ${SRC_${LUNA_MODEL}})
	source_group(${LUNA_PRICER} FILES ${SRC_${LUNA_PRICER}})
	source_group(${LUNA_CALIBRATION} FILES ${SRC_${LUNA_CALIBRATION}})


	
	target_link_libraries(CommonLMM ${QuantLib_LIBRARY})
	## todo set different filter
	
	set(LMM_PROG_DIR "${CMAKE_SOURCE_DIR}/proc")

	add_executable(prog_MCvsApprox_SwaptionPricer "${LMM_PROG_DIR}/prog_MCvsApprox_SwaptionPricer.cpp")
	target_link_libraries(prog_MCvsApprox_SwaptionPricer CommonLMM)

	add_executable(prog_RebonatoVolatilityCalibrator "${LMM_PROG_DIR}/prog_RebonatoVolatilityCalibrator.cpp")
	target_link_libraries(prog_RebonatoVolatilityCalibrator CommonLMM)
endif()



##################################################
###            POST CMAKE PROCESS              ###
##################################################
if(LUNA_CMAKE_DEBUG1)
	message("    ")
	message("${LUNA_CMAKE_DEBUG_MSG} Compulation type ${CMAKE_BUILD_TYPE}")
	message("${LUNA_CMAKE_DEBUG_MSG} All *.exe and *.lib compiled files are in :")
	message("${LUNA_CMAKE_DEBUG_MSG}	${LUNA_ALLIN1_TARGET_DIR}")
	message("    ")
endif()

## Printing debug msg for cmake build of LMM projects
if(LUNA_CMAKE_DEBUG2)

    message("${LUNA_CMAKE_DEBUG_MSG} Sources files For SRC_${LUNA_RNGENERATOR}")
    foreach(SRC_LMM ${SRC_${LUNA_RNGENERATOR}})
        message("    ${SRC_LMM}")
    endforeach(SRC_LMM)
    message("    ")

	message("${LUNA_CMAKE_DEBUG_MSG} Sources files For SRC_${LUNA_NUMERIC}")
	foreach(SRC_LMM ${SRC_${LUNA_NUMERIC}})
		message("    ${SRC_LMM}")
	endforeach(SRC_LMM)
	message("    ")

    message("${LUNA_CMAKE_DEBUG_MSG} Sources files For SRC_${LUNA_HELPER}")
    foreach(SRC_LMM ${SRC_${LUNA_HELPER}})
        message("    ${SRC_LMM}")
    endforeach(SRC_LMM)
    message("    ")

    message("${LUNA_CMAKE_DEBUG_MSG} Sources files For SRC_${LUNA_INSTRUMENT}")
    foreach(SRC_LMM ${SRC_${LUNA_INSTRUMENT}})
        message("    ${SRC_LMM}")
    endforeach(SRC_LMM)
    message("    ")

    message("${LUNA_CMAKE_DEBUG_MSG} Sources files For SRC_${LUNA_MODEL}")
    foreach(SRC_LMM ${SRC_${LUNA_MODEL}})
        message("    ${SRC_LMM}")
    endforeach(SRC_LMM)
    message("    ")

    message("${LUNA_CMAKE_DEBUG_MSG} Sources files For SRC_${LUNA_PRICER}")
    foreach(SRC_LMM ${SRC_${LUNA_PRICER}})
        message("    ${SRC_LMM}")
    endforeach(SRC_LMM)
    message("    ")

    message("${LUNA_CMAKE_DEBUG_MSG} Sources files For SRC_${LUNA_CALIBRATION}")
    foreach(SRC_LMM ${SRC_${LUNA_CALIBRATION}})
        message("    ${SRC_LMM}")
    endforeach(SRC_LMM)
    message("    ")

endif()# end LUNA_CMAKE_DEBUG1

if(LUNA_CMAKE_DEBUG1)
	message("${LUNA_CMAKE_DEBUG_MSG} user-config.cmake file")		
    message("        LUNA_NUMERIC_PKG_NAME     '${LUNA_NUMERIC_PKG_NAME}' ")
    message("        LUNA_RNGENERATOR_PKG_NAME '${LUNA_RNGENERATOR_PKG_NAME}' ")
    message("        LUNA_HELPER_PKG_NAME      '${LUNA_HELPER_PKG_NAME}' ")
    message("        LUNA_INSTRUMENT_PKG_NAME  '${LUNA_INSTRUMENT_PKG_NAME}' ")
    message("        LUNA_MODEL_PKG_NAME       '${LUNA_MODEL_PKG_NAME}' ")
    message("        LUNA_PRICER_PKG_NAME      '${LUNA_PRICER_PKG_NAME}' ")
    message("        LUNA_CALIBRATION_PKG_NAME '${LUNA_CALIBRATION_PKG_NAME}' ")
	message("    ")
endif()



#This project is in C++ depend to external library
#- boost
#- QuantLib
#and use cmake to generate the project from source code.
#
#Ensure that you've fully built and installed boost, QuantLib library on your computer and cmake is installed.
#Cmake can automatically find and link libraries boost, QuantLib to this project, in condition that libraries 
#are installed in a 'standard' way. i.e you have the libraries contains as follows
#$YOUR_BOOST_DIR\          # root     directory of boost library
#.      |--------- boost   # include  directory of boost library
#.      |--------- lib     # lib      directory of boost library
#
#$YOUR_QUANLIB_DIR\    # root     directory of QuantLib library
#.      |--------- ql  # include  directory of QuantLib library
#.      |--------- lib # lib      directory of QuantLib library
#
#
#Define your ENVIRONMENT VARIABLES (google to see how to define environment variable on your OS)
#BOOST_ROOT    : path\to\your\boost_installed_version
#QUANTLIB_ROOT : path\to\your\QuantLib_installed_version
#(do not forget restarting your computer in order to taking in account the environment variable setting) 
#
#Open a command terminal and go into the folder of this sources code. 
#(For Windows user, open the 'Developer Command Prompt For VS2012') 
#May be you need to add 'path\to\your\cmake\bin\' on you $PATH system variable. )
#
#cd \path\to\checked_out_folder\src_LUNA\
#cd ..
#mkdir build_Luna
#cd build_Luna
#cmake ..\src_Luna -G "YOUR_APPROPRIATED_CMAKE_GENERATOR"
#
#For Visual Studio Express 2012, use the generator "Visual Studio 11" (for other version, google cmake generators)
#cmake ..\src_Luna -G "Visual Studio 11"
#
#then
#Msbuild LunaLMM.sln /maxcpucount:8 && ctest
#Or open build_Luna folder and the project is LunaLMM.sln
#
#After that, for windows user, need just to right-click on the procLMM and "set as startup Project"
#
#!! Not for Windows user, for debugging ublas matrix and vector, see
#http://stackoverflow.com/questions/13202157/natvis-visual-studio-2012-boost-vector-and-matrix/13361645#13361645
#copy the file 
#'$LUNA_SRC_ROOT\config\vchelper_debug_ublas.natvis' 
#to your folder
#%USERPROFILE%\My Documents\Visual Studio 2012\Visualizers\
#
#
#
#
#
#