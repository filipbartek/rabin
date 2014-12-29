REM C++ compiler
SET cxx=g++

REM C++ linker
SET link_cpp=%cxx%

REM Archiver
SET ar=ar

SET include_dir=C:\include

SET boost_root_dir=%include_dir%\boost_1_48_0
SET boost_include_dir=%boost_root_dir%
SET boost_lib_dir=%boost_root_dir%\stage\lib
SET boost_bin_dir=%boost_lib_dir%
SET boost_program_options_lib_name=boost_program_options-mgw46-mt-1_48

SET tree_include_dir=%include_dir%\tree-2.81\src

SET cxxtl_include_dir=%include_dir%\libcxxtl_0.1.3

SET matlab_root_dir=
SET matlab_include_dir=
SET matlab_lib_dir=
SET matlab_bin_dir=

SET path=%path%;%boost_bin_dir%
