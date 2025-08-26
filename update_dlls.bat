@echo off
echo Updating SFML DLL files...

REM Copy latest SFML DLL files from installation directory
echo Copying SFML DLL files...
copy "C:\SFML\SFML-2.6.2\bin\*.dll" . /Y

REM Remove debug versions to avoid confusion
echo Removing debug DLL files...
del *-d-2.dll 2>nul

echo.
echo SFML DLL files updated successfully!
echo.
echo Current DLL files:
dir *.dll

pause
