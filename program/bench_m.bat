ECHO OFF

CD src\bench

CALL run.bat --solver-matlab-path=%~p0matlab %*

CD ..\..
