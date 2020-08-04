@echo off
setlocal

if /i "%1" == "-game" goto CleanGameDir

rem Clean out tfc
if exist ..\..\..\game\tfc\shaders rd /s /q ..\..\..\game\tfc\shaders
goto CleanOtherStuff

:CleanGameDir
set __GameDir=%~2
if not exist "%__GameDir%\gameinfo.txt" goto MissingGameInfo
if exist "%__GameDir%\shaders" rd /s /q "%2\shaders"
goto CleanOtherStuff

:CleanOtherStuff
if exist Debug_dx9_* rd /s /q Debug_dx9_*
if exist Release_dx9_* rd /s /q Release_dx9_*
if exist include rd /s /q include
if exist shaders rd /s /q shaders
goto end

:MissingGameInfo
echo Invalid -game parameter specified (no "%__GameDir%\gameinfo.txt" exists).
goto end


:end