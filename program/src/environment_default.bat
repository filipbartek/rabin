REM The following lines should be adjusted to suit your needs, preferably by overwriting the variables in local environment.bat.

REM Build type - options: debug, release
REM Should be specified from command line or in local environment.bat (or not at all).
REM SET build_type=

REM C++ preprocessor
SET cppflags=%cppflags%

REM C++ compiler
SET cxx=g++
SET cxxflags=%cxxflags% -Werror -Wall -pedantic
IF /i NOT "%build_type%"=="debug" GOTO else
SET cxxflags=%cxxflags% -O0 -g3
GOTO endif
:else
IF /i "%build_type%"=="release" SET cxxflags=%cxxflags% -O3
:endif

REM C++ linker
SET link_cpp=%cxx%


REM Binary filenames
SET main_lib_name=rgs
SET main_executable_name=main
SET bench_executable_name=bench
SET test_executable_name=test


REM Local environment.bat should define the following variables.
REM Variables marked as auxiliary aren't used in any other files.
REM Examples of the paths are given here.

SET include_dir=C:\include
REM include_dir is auxiliary.

REM Required.
REM Source: http://www.boost.org/
REM Available for free.
REM RabinGameSolver was tested with: boost_1_48_0
SET boost_root_dir=%include_dir%\boost_1_48_0
REM boost_root_dir is auxiliary.
SET boost_include_dir=%boost_root_dir%
SET boost_lib_dir=%boost_root_dir%\stage\lib
SET boost_program_options_lib_name=libboost_program_options-vc100-mt-1_48
REM Only %boost_program_options_lib_name% is used from %boost_lib_dir%.
REM It should first be compiled according to following instructions:
REM http://www.boost.org/doc/libs/1_48_0/more/getting_started/windows.html#prepare-to-use-a-boost-library-binary
REM boost_program_options_lib_name should be set to the (automatically built) file in %boost_lib_dir% that matches libboost_program_options-*.lib and doesn't match libboost_program_options-*-gd-*.lib

REM Required. (For PitermanRabinGameSolver.)
REM Source: http://tree.phi-sci.com/
REM Available for free.
REM RabinGameSolver was tested with: tree-2.81
SET tree_include_dir=%include_dir%\tree-2.81\src

REM Required. (For * in debug build.)
REM Source: http://cxxtl.sourceforge.net/
REM Available for free.
REM RabinGameSolver was tested with: libcxxtl_0.1.3
SET cxxtl_include_dir=%include_dir%\libcxxtl_0.1.3

REM Required. (For *Test.)
REM Source: http://code.google.com/p/googletest/
REM Available for free.
REM RabinGameSolver was tested with: gtest-1.6.0
SET gtest_root_dir=%include_dir%\gtest-1.6.0
REM gtest_root_dir is auxiliary.
SET gtest_include_dir=%gtest_root_dir%\include
SET gtest_lib_dir=%gtest_root_dir%\make
SET gtest_main_lib_name=gtest_main.a
REM Only %gtest_main_lib_name% is used from %gtest_lib_dir%.
REM It should first be compiled by calling %gtest_root_dir%\make\Makefile %gtest_main_lib_name%

REM Required for MatlabRabinGameSolver.
REM Source: http://www.mathworks.com/products/matlab/
REM Available comercially.
REM RabinGameSolver was tested with: Matlab7.10

REM Set matlab_root_dir to the value returned by the following MATLAB command:
REM  fullfile(matlabroot)
SET matlab_root_dir=C:\Program Files\Matlab7.10
REM matlab_root_dir is auxiliary.

REM Set matlab_include_dir to the value returned by the following MATLAB command:
REM  fullfile(matlabroot, 'extern', 'include')
REM matlab_include_dir indicates whether MATLAB solver is used.
REM If matlab_include_dir is nonempty, matlab_lib_dir and matlab_bin_dir must also be configured properly.
SET matlab_include_dir=%matlab_root_dir%\extern\include

SET matlab_lib_dir=%matlab_root_dir%\extern\lib\win64\microsoft

REM Set matlab_bin_dir to the value returned by the following MATLAB command:
REM  fullfile(matlabroot,'bin',computer('arch'))
SET matlab_bin_dir=%matlab_root_dir%\bin\win64
