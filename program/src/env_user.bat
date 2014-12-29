REM C++ compiler flags
SET cxxflags=%cxxflags% -Wall -pedantic
IF /i NOT "%build_type%"=="debug" GOTO else
SET cxxflags=%cxxflags% -O0 -g3
GOTO endif
:else
IF /i "%build_type%"=="release" SET cxxflags=%cxxflags% -O3
:endif

REM Archiver flags
SET arflags=-rcs

REM Binary filenames
SET main_executable_name=rgs
SET main_rgs_lib_name=rgs
SET bench_executable_name=bench
