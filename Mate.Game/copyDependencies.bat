@echo off
REM Uso: copyDependencies.bat <SolutionDir> <Platform> <Configuration>

set SOLUTION_DIR=%~1
set PLATFORM=%~2
set CONFIGURATION=%~3

REM Game assets path
set ORIGIN="%SOLUTION_DIR%Mate.Game\Assets"
set DESTINATION="%SOLUTION_DIR%Mate.Game\bin\%PLATFORM%\%CONFIGURATION%\Assets"
echo Copying game assets from %ORIGIN% to %DESTINATION%...
xcopy %ORIGIN% %DESTINATION% /E /I /Y

REM Basic assets path
set ORIGIN="%SOLUTION_DIR%Mate\Assets"
echo Copying basic assets from %ORIGIN% to %DESTINATION%...
xcopy %ORIGIN% %DESTINATION% /E /I /Y

echo Copia finalizada.
