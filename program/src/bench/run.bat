ECHO OFF

SETLOCAL

CALL environment.bat

ECHO Running %bench_executable_name%...

%bench_executable_name% %*

ENDLOCAL
