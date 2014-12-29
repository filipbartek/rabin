SETLOCAL

CALL environment.bat

:start
IF "%1"=="" GOTO end
ECHO Building %1...
IF "%1"=="%bench_executable_name%.exe" GOTO bench.exe 
GOTO %1

:shift
ECHO Building %1 done.
SHIFT
GOTO start


:bench.exe
SETLOCAL
SET objects=Toss.o RandomRabinGameGenerate.o main.o redirecter.o GeneralTwoDTracer.o DiagonalTwoDTracer.o SquareTwoDTracer.o Scaler.o GeneralScaler.o ConstantScaler.o LinearScaler.o pow.o PolynomialScaler.o ExponentialScaler.o FactorialScaler.o ListGnuplot.o responsefile.o DynamicTwoDTracer.o
CALL %build_bat% %objects%
SETLOCAL
IF DEFINED matlab_include_dir SET matlab_option="%matlab_bin_dir%\libeng.dll" "%matlab_bin_dir%\libmx.dll"
%link_cpp% %objects% -L"%main_lib_dir%" -l%main_rgs_lib_name% -L"%boost_lib_dir%" -l%boost_program_options_lib_name% %matlab_option% -o %bench_executable_name%
ENDLOCAL
ENDLOCAL
GOTO shift

:main.o
SETLOCAL
IF DEFINED matlab_include_dir SET matlab_option=-I"%matlab_include_dir%"
CALL %compile_bat% -I"%main_include_dir%" -I"%boost_include_dir%" -I"%cxxtl_include_dir%" -I"%tree_include_dir%" %matlab_option% main.cpp
ENDLOCAL
GOTO shift

:RandomRabinGameGenerate.o
CALL %compile_bat% -I"%main_include_dir%" -I"%boost_include_dir%" -I"%cxxtl_include_dir%" RandomRabinGameGenerate.cpp
GOTO shift

:Toss.o
CALL %compile_bat% Toss.cpp
GOTO shift

:redirecter.o
CALL %compile_bat% -I"%cxxtl_include_dir%" redirecter.cpp
GOTO shift

:GeneralTwoDTracer.o
CALL %compile_bat% GeneralTwoDTracer.cpp
GOTO shift

:DiagonalTwoDTracer.o
CALL %compile_bat% DiagonalTwoDTracer.cpp
GOTO shift

:SquareTwoDTracer.o
CALL %compile_bat% SquareTwoDTracer.cpp
GOTO shift

:Scaler.o
CALL %compile_bat% Scaler.cpp
GOTO shift

:GeneralScaler.o
CALL %compile_bat% GeneralScaler.cpp
GOTO shift

:ConstantScaler.o
CALL %compile_bat% ConstantScaler.cpp
GOTO shift

:LinearScaler.o
CALL %compile_bat% LinearScaler.cpp
GOTO shift

:pow.o
CALL %compile_bat% pow.cpp
GOTO shift

:PolynomialScaler.o
CALL %compile_bat% PolynomialScaler.cpp
GOTO shift

:ExponentialScaler.o
CALL %compile_bat% ExponentialScaler.cpp
GOTO shift

:FactorialScaler.o
CALL %compile_bat% FactorialScaler.cpp
GOTO shift

:responsefile.o
CALL %compile_bat% -I"%boost_include_dir%" responsefile.cpp
GOTO shift

:DynamicTwoDTracer.o
CALL %compile_bat% DynamicTwoDTracer.cpp
GOTO shift

:ListGnuplot.o
CALL %compile_bat% -I"%cxxtl_include_dir%" -I"%boost_include_dir%" ListGnuplot.cpp
GOTO shift


:end

ENDLOCAL
