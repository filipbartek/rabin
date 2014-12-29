REM environment.bat
REM This file sets global variables for building and running Rabin game solver and benchmark.
REM This file shouldn't be run directly, rather use one of the following access scripts:

REM main\build_all.bat
REM main\clean.bat
REM bench\build_all.bat
REM bench\clean.bat


REM You shouldn't edit this file.
REM If you want to change some build or run settings, change:
REM environment_user.bat for settings unspecific for this machine
REM local environment.bat (..\..\..\environment.bat) for settings specific for this machine

REM To make sure this file is run only once:
IF DEFINED environment_bat_ GOTO :EOF
SET environment_bat_=1

ECHO Running environment.bat...

REM Script filenames
SET build_bat=build.bat
SET compile_bat=compile.bat

REM Main dirs
SET main_root_dir=%~p0main
SET main_include_dir=%main_root_dir%
SET main_lib_dir=%main_root_dir%

REM environment_default.bat sets the user defined variables to their default values. See environment_default.bat for examples of variables and their values. You should create a file environment.bat at ..\..\.. and set the proper values to the variables there.
REM CALL %~p0environment_default.bat

REM environment_user.bat contains build and run settings unspecific for this machine
CALL %~p0env_user.bat

REM Local environment.bat contains build and run settings specific for this machine
CALL %~p0env_local.bat

REM If building a debug build, define DEBUG in preprocessor
IF /i "%build_type%"=="debug" SET cppflags=%cppflags% -DDEBUG
