@echo off
chcp 65001
cls
setlocal EnableDelayedExpansion
set ESC = [

set /A m_countDir = 1
set m_dir[1]=

:countloop
for /F "tokens=%m_countDir% delims=\" %%i in ("%cd%") do (
	if "%%i" == "" goto endloop
	set m_dir[%m_countDir%]=%%i
	set /A  m_countDir+=1
	goto countloop
)
:endloop
set /A m_countDir-=1
set /A Part2=%m_countDir%-2
set /A Part3=%m_countDir%-1

set m_echo=

rem :printDir
if %m_countDir% geq 4 (
	setlocal EnableDelayedExpansion
	rem echo|set /p = [34;42m %m_dir[1]%[32;46m [37;46m!m_dir[%Part2%]![36;41m [37;41m!m_dir[%Part3%]![31;45m [37;45m!m_dir[%m_countDir%]![0;0m[35m [0;0m
	echo|set /p = [34;45m %m_dir[1]%[35;41m [30;41m↳\!m_dir[%Part2%]!\!m_dir[%Part3%]!\!m_dir[%m_countDir%]![0;0m[31m [0;0m
	endlocal
)
if %m_countDir% equ 3 (
	setlocal EnableDelayedExpansion
	rem echo|set /p = [34;42m %m_dir[1]%[32;46m [37;46m%m_dir[2]%[36;41m [37;41m%m_dir[3]%[0;0m[31m [0;0m
	echo|set /p = [34;45m %m_dir[1]%[35;41m [30;41m%m_dir[2]%\%m_dir[3]%[0;0m[31m [0;0m
	endlocal
)
if %m_countDir% equ 2 (
	setlocal EnableDelayedExpansion
	rem echo|set /p = [34;42m %m_dir[1]%[32;46m [37;46m%m_dir[2]%[0;0m[36m [0;0m
	echo|set /p = [34;45m %m_dir[1]%[35;41m [30;41m%m_dir[2]%[0;0m[31m [0;0m
	endlocal
)
if %m_countDir% equ 1 (
	setlocal EnableDelayedExpansion
	rem echo|set /p = [34;42m %m_dir[1]%[0;0m[32m [0;0m
	echo|set /p = [34;45m %m_dir[1]%[0;0m[35m [0;0m
	endlocal
)
endlocal

set /A m_countDir=1

set /p command=
call %command%
set command=

goto countloop