::
:: Script read all file .sln, .vcxproj, .filters on the %LUNA_ROOT% folder
:: Transform all absolute path created by cmake into relative path for VC++
:: In the way that the project have the portability
:: 
:: At the end of the script, use grep to test if the absolute path still exist
:: 
:: Path to be transform are
:: %BOOST_ROOT% absolute path to be change $(BOOST_ROOT) in VC++ Additional Include Dir
:: %QUANTLIB_ROOT% absolute path to be change $(QUANTLIB_ROOT) in VC++ Additional Include Dir
:: CMAKE_SOURCE_DIR absolute path to be change $(SolutionDir) in VC++ Additional Include Dir
::
:: see
:: http://scripts.dragon-it.co.uk/scripts.nsf/docs/batch-search-replace-substitute!OpenDocument&ExpandSection=3&BaseTarget=East&AutoFramed
::
::
::
::
:: chithanhnguyen.math@gmail.com

@echo off

set testfile=_testCommonLMM.vcxproj
set tempofile=_tempoCommonLMM.vcxproj


::setting relative paths
::::::::::::::::::::: <OutDir>$(SolutionDir)VCBuild\$(Configuration)\</OutDir>
set LUNA_REL_OutDir="	<OutDir>$(SolutionDir)VCBuild\$(Configuration)\</OutDir>"
:::::::::::::::::::::<IntDir>Tempo\$(Configuration)\</IntDir>
::set LUNA_REL_IntDir=::$LIntDir$GTempo\$(Configuration)\$L/IntDir$G
:::::::::::::::::::::::::<AdditionalIncludeDirectories>$(SolutionDir);$(QUANTLIB_ROOT);$(BOOST_ROOT);%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
::set LUNA_REL_IncludeDir=::$LAdditionalIncludeDirectories$G$(SolutionDir);$(QUANTLIB_ROOT);$(BOOST_ROOT);%(AdditionalIncludeDirectories)$L/AdditionalIncludeDirectories$G

set LUNA_REL_OutDir
set LUNA_REL_OutDir=%LUNA_REL_OutDir:"=%
set LUNA_REL_OutDir
echo LUNA_REL_OutDir     %LUNA_REL_OutDir%

::echo LUNA_REL_IntDir     %LUNA_REL_IntDir%
::echo LUNA_REL_IncludeDir %LUNA_REL_IncludeDir%

:: Use Grep to test if absolute paths still existe

set BOOST_ROOT_PATH_PATTERN=%BOOST_ROOT:\=\\%
set QUANTLIB_ROOT_PATH_PATTERN=%QUANTLIB_ROOT:\=\\%
set SOLUTION_ROOT_PATH_PATTERN=%CD:\=\\%

echo.
echo BOOST_ROOT pattern    = %BOOST_ROOT_PATH_PATTERN%
echo QUANTLIB_ROOT pattern = %QUANTLIB_ROOT_PATH_PATTERN%
echo LUNA_ROOT pattern     = %SOLUTION_ROOT_PATH_PATTERN%
echo.

::grep -n %BOOST_ROOT_PATH_PATTERN% %_tempoCommonLMM%
::grep -n %QUANTLIB_ROOT_PATH_PATTERN% %_tempoCommonLMM%
::grep -n %SOLUTION_ROOT_PATH_PATTERN% %_tempoCommonLMM%


::grep -n %BOOST_ROOT_PATH_PATTERN% *.vcxproj *.filter *.sln
::grep -n %QUANTLIB_ROOT_PATH_PATTERN% *.vcxproj *.filter *.sln
::grep -n %SOLUTION_ROOT_PATH_PATTERN% *.vcxproj *.filter *.sln