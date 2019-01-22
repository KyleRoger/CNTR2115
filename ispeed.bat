@echo off
REM -----------------------------------------------------------------------
REM FILE			ispeed.bat
REM PROJECT			ispeed
REM PROGRAMMER		Arie Kraayenbrink
REM FIRST VERSION	Jan. 21, 2019
REM DESCRIPTION		Start the ispeed server or client based on the input parameters provided.
REM					The client and server are responsible to validate the input parameters.
REM -------------------------------------------------------------------------



if %1 == -p (
	cd server\bin
	ispeed.exe %1 %2
) else (
	cd client\bin
	ispeed.exe %1 %2 %3 %4 %5 %6 %7 %8 %9
)

cd ..\..

:END
