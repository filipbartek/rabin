SETLOCAL

CALL environment.bat

:start
IF "%1"=="" GOTO end
ECHO Building %1...
IF "%1"=="%main_executable_name%.exe" GOTO main.exe
IF "%1"=="lib%main_rgs_lib_name%.a" GOTO librgs.a
GOTO %1

:shift
ECHO Building %1 done.
SHIFT
GOTO start


:main.exe
CALL %build_bat% lib%main_rgs_lib_name%.a
CALL %build_bat% %main%.o
SETLOCAL
IF DEFINED matlab_include_dir SET matlab_option="%matlab_bin_dir%\libeng.dll" "%matlab_bin_dir%\libmx.dll"
%link_cpp% %main%.o -L%main_lib_dir% -l%main_rgs_lib_name% %matlab_option% -o %main_executable_name%
ENDLOCAL
GOTO shift

:librgs.a
SET objects=%Vertex%.o %RabinStreettPair%.o %RabinGame%.o %RabinGameSolver%.o %PitermanRabinGameSolver%.o %HornRabinGameSolver%.o
IF NOT DEFINED matlab_include_dir GOTO else
ECHO lib%main_rgs_lib_name%.a will be built with MATLAB solver.
SET objects=%objects% %MatlabRabinGameSolver%.o
GOTO endif
:else
ECHO lib%main_rgs_lib_name%.a will be built without MATLAB solver.
:endif
CALL %build_bat% %objects%
REM %link_cpp% -shared %objects% -o %main_rgs_lib_name%.dll
%ar% %arflags% lib%main_rgs_lib_name%.a %objects%
REM It should also compile a hpp file with all the used headers.

ECHO #include "Global.hpp"^

#include "Vertex.hpp"^

#include "RabinStreettPair.hpp"^

#include "RabinGame.hpp"^

#include "RabinGameSolver.hpp"^

#include "PitermanRabinGameSolver.hpp"^

#include "HornRabinGameSolver.hpp">%main_rgs_lib_name%.hpp

IF DEFINED matlab_include_dir ECHO #include "MatlabRabinGameSolver.hpp">>rgs.hpp

GOTO shift

:main.o
IF NOT DEFINED matlab_include_dir GOTO else
CALL %compile_bat% -I"%boost_include_dir%" -I"%cxxtl_include_dir%" -I"%tree_include_dir%" -I"%matlab_include_dir%" %main%.cpp
GOTO endif
:else
CALL %compile_bat% -I"%boost_include_dir%" -I"%cxxtl_include_dir%" -I"%tree_include_dir%" %main%.cpp
:endif
GOTO shift

:Vertex.o
CALL %compile_bat% -I"%boost_include_dir%" -I"%cxxtl_include_dir%" %Vertex%.cpp
GOTO shift

:RabinStreettPair.o
CALL %compile_bat% -I"%boost_include_dir%" -I"%cxxtl_include_dir%" %RabinStreettPair%.cpp
GOTO shift

:RabinGame.o
CALL %compile_bat% -I"%boost_include_dir%" -I"%cxxtl_include_dir%" %RabinGame%.cpp
GOTO shift

:RabinGameSolver.o
CALL %compile_bat% -I"%boost_include_dir%" -I"%cxxtl_include_dir%" %RabinGameSolver%.cpp
GOTO shift

:PitermanRabinGameSolver.o
CALL %compile_bat% -I"%boost_include_dir%" -I"%cxxtl_include_dir%" -I"%tree_include_dir%" %PitermanRabinGameSolver%.cpp
GOTO shift

:HornRabinGameSolver.o
CALL %compile_bat% -I"%boost_include_dir%" -I"%cxxtl_include_dir%" %HornRabinGameSolver%.cpp
GOTO shift

:MatlabRabinGameSolver.o
CALL %compile_bat% -I"%boost_include_dir%" -I"%cxxtl_include_dir%" -I"%matlab_include_dir%" %MatlabRabinGameSolver%.cpp
GOTO shift


:end

ENDLOCAL
