ECHO OFF

CD src\main

CALL run.bat -m %~p0matlab %*

CD ..\..
