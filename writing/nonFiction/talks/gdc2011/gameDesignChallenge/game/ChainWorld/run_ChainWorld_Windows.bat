@echo off

echo Launching Minecraft...

echo --

echo Leave this window open until Minecraft exits.

echo --


IF NOT EXIST "%APPDATA%\.minecraft"  mkdir "%APPDATA%\.minecraft"

IF NOT EXIST "%APPDATA%\.minecraft\saves"  mkdir "%APPDATA%\.minecraft\saves"

xcopy /E /I /Q "data\minecraft\saves\Chain World" "%APPDATA%\.minecraft\saves\Chain World"


bin\Minecraft_old.exe

echo --

echo --

echo WAIT to press any key until Minecraft exits.

pause

echo --

echo Copying Chain World back to USB...

IF NOT EXIST "%APPDATA%\.minecraft\saves\Chain World" GOTO NOTFOUND

rmdir /S /Q "data\minecraft\saves\Chain World"

xcopy /E /I /Q "%APPDATA%\.minecraft\saves\Chain World" "data\minecraft\saves\Chain World"

IF NOT EXIST "data\minecraft\saves\Chain World" GOTO NOTFOUND_USB

rmdir /S /Q "%APPDATA%\.minecraft\saves\Chain World"


echo --

echo Chain World has been successfully saved to USB.

goto DONE


:NOTFOUND
echo ERROR:  Chain World save directory not found on local drive!

GOTO DONE


:NOTFOUND_USB
echo ERROR:  Failed to copy Chain World save directory back to USB!


:DONE
pause

