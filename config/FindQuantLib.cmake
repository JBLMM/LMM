#=============================================================================
# Chi Thanh NGUYEN  Mai 2014                                                 #
# chithanhnguyen.math@gmail.com                                              # 
#                                                                            #
# When working for the first time with QuantLib, as experienced developer    #
# I try to create a file FindQuantLib.cmake, inspired from FindBoost.cmake   #
# int order to facilite the multiplateform linking aspect of QuantLib        #
#                                                                            #
# The module was written in the coresponding versions (creation Mai 2014)    #
# cmake version 2.8                                                          #
# QuantLib 1.4                                                               #
# Boost 1.55.0                                                               #
# Visual Studio Express 2012                                                 #
#                                                                            #
# This FindQuantLib.cmake find                                               #
#  QuantLib_INCLUDE_DIR         - Directory containing QuantLib headers      #
#  QuantLib_LIBRARY_DIR         - Directory containing QuantLib libraries    #
#  QuantLib_LIBRARY             - Directory containing QuantLib libraries    #
#                                                                            #
# Note that I am not an experienced cmake writer, this script was writing    #
# in a quick way, that ensure only for specific system and not all. If       #
# someone work on other system and modify this script to adapt, it's better  #
#=============================================================================
#
# The easiest away for that FindQuantLib.cmake work is that after installing  
# the QuantLib library, one set the environment variable QUANTLIB_ROOT as for boost.
# Depending of the Operating system, one can find on internet how to set the environment 
# variables for Windows, Mac OS X, Linux. This QUANTLIB_ROOT variable has to assigned
# to the path -prefixe=... when you install QuantLib
#  QUANTLIB_ROOT=/path/to/the/root/of/installed/QuantLib/   
# Depending the system, the content of installed QuantLib library has different structure
#
# On Windows
#   -$QUANTLIB_ROOT                     --> QUANTLIB_INCLUDEDIR
#         |-$QUANTLIB_ROOT/ql   
#         |-$QUANTLIB_ROOT/lib          --> QUANTLIB_LIBRARYDIR
#
# On UNIX (Mac OS X, Linux)
#   -$QUANTLIB_ROOT
#         |-$QUANTLIB_ROOT/include/ql   --> QUANTLIB_INCLUDEDIR
#         |-$QUANTLIB_ROOT/lib          --> QUANTLIB_LIBRARYDIR
#
# 
# Use this module by invoking find_package with the form:
#  find_package(QuantLib)
# This module finds headers and requested component libraries 
# The results are reported in variables:
#  QuantLib_FOUND            - True if headers and requested libraries were found
#  QuantLib_INCLUDE_DIRS     - QuantLib include directories
#  QuantLib_LIBRARY_DIRS     - Link directories for QuantLib libraries
#  QuantLib_LIBRARY          - QuantLib component libraries to be linked
#  QuantLib_VERSION          - QUANTLIB_VERSION value from ql/config.hpp
#  QuantLib_LIB_VERSION      - Version string appended to library filenames
#
# This module reads hints about search locations from variables:
#  QUANTLIB_ROOT             - Preferred installation prefix
#   (or QUANTLIBROOT)
#  QUANTLIB_INCLUDEDIR       - Preferred include directory e.g. <prefix>/include
#  QUANTLIB_LIBRARYDIR       - Preferred library directory e.g. <prefix>/lib
#  QuantLib_NO_SYSTEM_PATHS  - Set to ON to disable searching in locations not
#                           specified by these hint variables. Default is OFF.
#
# and saves search results persistently in CMake cache entries:
#  QuantLib_INCLUDE_DIR         - Directory containing QuantLib headers
#  QuantLib_LIBRARY_DIR         - Directory containing QuantLib libraries
#
#
# Users may set these hints or results as cache entries.  Projects should
# not read these entries directly but instead use the above result variables.
# Note that some hint names start in upper-case "QUANTLIB".  One may specify
# these as environment variables if they are not specified as CMake variables
# or cache entries.
#
# This module first searches for the QuantLib header files using the above hint
# variables (excluding QUANTLIB_LIBRARYDIR) and saves the result in
# QuantLib_INCLUDE_DIR.  Then it searches for requested component libraries using
# the above hints (excluding QUANTLIB_INCLUDEDIR),
# "lib" directories near QuantLib_INCLUDE_DIR, and the library name configuration
# settings below.  It saves the library directory in QuantLib_LIBRARY_DIR
#
# QuantLib libraries come in many variants encoded in their file name.  Users or
# projects may tell this module which variant to find by setting variables:
#
#  QuantLib_USE_MULTITHREADED  - Set to OFF to use the non-multithreaded
#                                libraries ('mt' tag).  Default is ON.
#  QuantLib_USE_STATIC_LIBS    - Set to ON to force the use of the static
#                                libraries.  Default is OFF.
#  QuantLib_USE_STATIC_RUNTIME - Set to ON or OFF to specify whether to use
#                                libraries linked statically to the C++ runtime
#                                ('s' tag).  Default is platform dependent.
#  QuantLib_USE_DEBUG_PYTHON   - Set to ON to use libraries compiled with a
#                                debug Python build ('y' tag). Default is OFF.
#  QuantLib_USE_STLPORT        - Set to ON to use libraries compiled with
#                                STLPort ('p' tag).  Default is OFF.
#  QuantLib_USE_STLPORT_DEPRECATED_NATIVE_IOSTREAMS
#                              - Set to ON to use libraries compiled with
#                                STLPort deprecated "native iostreams"
#                                ('n' tag).  Default is OFF.
#  QuantLib_COMPILER           - Set to the compiler-specific library suffix
#                                (e.g. "-gcc43").  Default is auto-computed
#                                for the C++ compiler in use.
#  QuantLib_THREADAPI          - Suffix for "thread" component library name,
#                                such as "pthread" or "win32".  Names with
#                                and without this suffix will both be tried.
# Other variables one may set to control this module are:
#  QuantLib_DEBUG              - Set to ON to enable debug output from FindQuantLib.
#                                Please enable this before filing any bug report.
#
#
# On Visual Studio and Borland compilers QuantLib headers request automatic
# linking to corresponding libraries.  This requires matching libraries to be
# linked explicitly or available in the link library search path.  In this
# case setting QuantLib_USE_STATIC_LIBS to OFF may not achieve dynamic linking.

# Example to find QuantLib headers only:
#  find_package(QuantLib)
#  if(QuantLib_FOUND)
#    include_directories(${QuantLib_INCLUDE_DIR})
#    add_executable(foo foo.cc)
#  endif()
# Example to find QuantLib headers and some libraries:
#  set(QuantLib_USE_STATIC_LIBS        ON)
#  set(QuantLib_USE_MULTITHREADED      ON)
#  set(QuantLib_USE_STATIC_RUNTIME    OFF)
#  find_package(QuantLib)
#  if(QuantLib_FOUND)
#    include_directories(${QuantLib_INCLUDE_DIR})
#    add_executable(foo foo.cc)
#    target_link_libraries(foo ${QuantLib_LIBRARY})
#  endif()
#	
# QuantLib CMake ----------------------------------------------------------
#


############# main package ##########

message(" - Cmake FindQuantLib --------")
set(QUANTLIB_CMAKE_DEBUG_MSG "    - QuantLib Cmake Msg ")


set(QuantLib_LIBRARY)
set(QuantLib_INCLUDE_DIR)
set(QuantLib_LIBRARY_DIR)
set(QuantLib_VERSION)

set(_QuantLib_KNOWN_VERSIONS "0.8" "0.9" "1.0" "1.1" "1.2" "1.3" "1.4")## Not use yet, todo, parse version in ql/config.hpp

#--------------------------
#    Search library name
#-------------------------
# All these variables has to be determined depending on the OS,
# The build options, that has to deal with the library name

set(_ql_MT "")               # Multithread Compiled Lib
set(_ql_COMPILER "")         #
set(_ql_PREFIX "")           #
set(_ql_POSTFIX "")          #
set(_ql_SUFFIX "")           #
set(_ql_RELEASE_ABI_TAG "-")
set(_ql_DEBUG_ABI_TAG   "-")

set(_QuantLib_RELEASE_NAMES)
set(_QuantLib_DEBUG_NAMES)

if(MSVC12)
    set(_ql_COMPILER "-vc120")
elseif(MSVC11)
    set(_ql_COMPILER "-vc110")
elseif(MSVC10)
    set(_ql_COMPILER "-vc100")
elseif(MSVC90)
    set(_ql_COMPILER "-vc90")
elseif(MSVC80)
    set(_ql_COMPILER "-vc80")
elseif(MSVC71)
    set(_ql_COMPILER "-vc71")
elseif(MSVC70) # Good luck!
    set(_ql_COMPILER "-vc7") # yes, this is correct
elseif(MSVC60) # Good luck!
    set(_ql_COMPILER "-vc6") # yes, this is correct
elseif(UNIX)
	set(_ql_COMPILER "")
endif()

if(WIN32)
	set(_ql_SUFFIX "lib")         
	set(_ql_MT "-mt")      # Specifity Windows
elseif(UNIX)
	set(_ql_PREFIX "lib")
	set(_ql_SUFFIX "a")
endif()	

### Setting ABI tag for Windows QuantLib library name
if(WIN32)
## Key       Use this library when:
##  s        linking statically to the C++ standard library and
##           compiler runtime support libraries.
    if(QuantLib_USE_STATIC_RUNTIME)
      set( _ql_RELEASE_ABI_TAG "${_ql_RELEASE_ABI_TAG}s")
      set( _ql_DEBUG_ABI_TAG   "${_ql_DEBUG_ABI_TAG}s")
    endif()
##  g        using debug versions of the standard and runtime
##           support libraries
    if(MSVC OR "${CMAKE_CXX_COMPILER}" MATCHES "icl" OR "${CMAKE_CXX_COMPILER}" MATCHES "icpc")
        set(_ql_DEBUG_ABI_TAG "${_ql_DEBUG_ABI_TAG}g")
    endif()
##  d        using a debug version of your code
    set(_ql_DEBUG_ABI_TAG "${_ql_DEBUG_ABI_TAG}d")
else()## TODO for instance on NON WIN32 OS, debug and release QuantLib is the same, there are no futher tag 
    set( _ql_RELEASE_ABI_TAG "")
    set(_ql_DEBUG_ABI_TAG "")
endif()

# We want to use the tag inline below without risking double dashes
if(_ql_RELEASE_ABI_TAG)
    if(${_ql_RELEASE_ABI_TAG} STREQUAL "-")
    	set(_ql_RELEASE_ABI_TAG "")
    endif()
endif()
if(_ql_DEBUG_ABI_TAG)
    if(${_ql_DEBUG_ABI_TAG} STREQUAL "-")
    	set(_ql_DEBUG_ABI_TAG "")
    endif()
endif()


set(_QuantLib_RELEASE_NAMES ${_ql_PREFIX}QuantLib${_ql_COMPILER}${_ql_MT}${_ql_RELEASE_ABI_TAG}.${_ql_SUFFIX})
set(_QuantLib_DEBUG_NAMES ${_ql_PREFIX}QuantLib${_ql_COMPILER}${_ql_MT}${_ql_DEBUG_ABI_TAG}.${_ql_SUFFIX})

message("${QUANTLIB_CMAKE_DEBUG_MSG} :_QuantLib_RELEASE_NAMES[${_QuantLib_RELEASE_NAMES}]")
message("${QUANTLIB_CMAKE_DEBUG_MSG} :_QuantLib_DEBUG_NAMES[${_QuantLib_RELEASE_NAMES}]")



#------------------------
#      Search paths
#------------------------
set(QuantLib_VERSION "1.4")## Todo : to find in the ql/config.hpp file

if(NOT "$ENV{QUANTLIB_ROOT}" STREQUAL "")
	if(WIN32)

		find_path(QuantLib_INCLUDE_DIR
		NAMES         ql/quantlib.hpp
		HINTS         $ENV{QUANTLIB_ROOT}
		#PATH_SUFFIXES ${_QuantLib_PATH_SUFFIXES}
		)
		
		# Avoid passing backslashes to _Boost_FIND_LIBRARY due to macro re-parsing.
		string(REPLACE "\\" "/" _ql_tmp_LIB_SEARCH_DIRS "$ENV{QUANTLIB_ROOT}/lib")

		find_path(QuantLib_LIBRARY_DIR
		NAMES         ${_QuantLib_RELEASE_NAMES}
		HINTS         ${_ql_tmp_LIB_SEARCH_DIRS}
		#PATH_SUFFIXES ${_QuantLib_PATH_SUFFIXES}
		)

	elseif(UNIX)
	
		find_path(QuantLib_INCLUDE_DIR
		NAMES         ql/quantlib.hpp
		HINTS         $ENV{QUANTLIB_ROOT}/include
		#PATH_SUFFIXES ${_QuantLib_PATH_SUFFIXES}
		)

		find_path(QuantLib_LIBRARY_DIR
		NAMES         ${_QuantLib_RELEASE_NAMES}
		HINTS         $ENV{QUANTLIB_ROOT}/lib
		#PATH_SUFFIXES ${_QuantLib_PATH_SUFFIXES}
		)

	endif()

message("${QUANTLIB_CMAKE_DEBUG_MSG} :QuantLib_INCLUDE_DIR[${QuantLib_INCLUDE_DIR}]")
message("${QUANTLIB_CMAKE_DEBUG_MSG} :QuantLib_LIBRARY_DIR[${QuantLib_LIBRARY_DIR}]")

    if(QuantLib_INCLUDE_DIR AND QuantLib_LIBRARY_DIR)
    	set(QuantLib_FOUND 1)
    endif()
endif()





#------------------------------
#          Post Process
#------------------------------
if(QuantLib_FOUND)
	if(WIN32)
		set(QuantLib_LIBRARY optimized ${QuantLib_LIBRARY_DIR}/${_QuantLib_RELEASE_NAMES} debug ${QuantLib_LIBRARY_DIR}/${_QuantLib_DEBUG_NAMES})
		# handle the QUIETLY and REQUIRED arguments and set QuantLib_FOUND to TRUE if
		# all listed variables are TRUE
		include(FindPackageHandleStandardArgs)
		find_package_handle_standard_args(QuantLib DEFAULT_MSG QuantLib_LIBRARY QuantLib_INCLUDE_DIR)
		mark_as_advanced(QuantLib_LIBRARY)
	elseif(UNIX)
		set(QuantLib_LIBRARY optimized ${QuantLib_LIBRARY_DIR}/${_QuantLib_RELEASE_NAMES} debug ${QuantLib_LIBRARY_DIR}/${_QuantLib_DEBUG_NAMES})
		# handle the QUIETLY and REQUIRED arguments and set QuantLib_FOUND to TRUE if
		# all listed variables are TRUE
		include(FindPackageHandleStandardArgs)
		find_package_handle_standard_args(QuantLib DEFAULT_MSG QuantLib_LIBRARY QuantLib_INCLUDE_DIR)
		mark_as_advanced(QuantLib_LIBRARY)
	endif()
    message("${QUANTLIB_CMAKE_DEBUG_MSG}")
	message(STATUS "Found QuantLib ${QuantLib_VERSION} library:")
	message("        QuantLib_INCLUDE_DIR")
	message("	        [${QuantLib_INCLUDE_DIR}]")
	message("        QuantLib_LIBRARY_DIR")
	message("	        [${QuantLib_LIBRARY_DIR}]")
	message("        QuantLib_LIBRARY")
	message("                [${QuantLib_LIBRARY}]")
	
else()
message("${QUANTLIB_CMAKE_DEBUG_MSG}")
	message(" ")
	message("!!!!!! QuantLib can not be found")
	message("!!!!!! Be sure that you have correctly define your QUANTLIB_ROOT environment variable")
	message("!!!!!! This variable QUANTLIB_ROOT indicate where is installed your QuantLib library")
	message(" ")
endif()

message(" - Cmake FindQuantLib  --------")
message("    ")