SETLOCAL

CALL environment.bat

%cxx% %cppflags% %cxxflags% -c %*

ENDLOCAL
