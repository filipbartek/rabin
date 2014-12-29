ECHO OFF

CD %~p0src\main

CALL build_all.bat

CD %~p0src\bench

CALL build_all.bat

CD %~p0
