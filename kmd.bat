@echo off
chcp 65001
cls
setlocal EnableDelayedExpansion
set ESC = [

set /A m_countDir = 1
set m_dir[1]=
set gitBranch=

rem loop for count segment of directory
:countloop
for /F "tokens=%m_countDir% delims=\" %%i in ("%cd%") do (
	if "%%i" == "" goto endloop
	set m_dir[%m_countDir%]=%%i
	set /A  m_countDir+=1
	goto countloop
)
:endloop

rem get segments of directory
set /A m_countDir-=1
set /A Part2=%m_countDir%-2
set /A Part3=%m_countDir%-1
set m_dir[1]=KeithPham

rem init variable for find .git folder
set gitdirs=.git
set isFoundGit=""
set /A countLoopback = 0
set gitBranch=[0;0m[34m [0;0m

:findGitloop
	if exist %gitdirs%  (
		set isFoundGit="found"
		goto endFindGitloop
	)
	if %countLoopback% equ %m_countDir% (
		goto endFindGitloop
	)
	set gitdirs=..\%gitdirs%
	set /A countLoopback+=1
	goto findGitloop
:endFindGitloop

if %isFoundGit% equ "found" (
	for /F "delims=" %%i in ('git rev-parse --abbrev-ref HEAD') do (
		set gitBranch=[34;46m [35;46m[96;46m %%i [0;0m[36m [0;0m 
	)
)

rem :printDir
if %m_countDir% geq 4 (
	setlocal EnableDelayedExpansion
	rem echo|set /p = [34;42m %m_dir[1]%[32;46m [37;46m!m_dir[%Part2%]![36;41m [37;41m!m_dir[%Part3%]![31;45m [37;45m!m_dir[%m_countDir%]![0;0m[35m [0;0m
	echo [37;40m %m_dir[1]%[30;44m [36;44m↳[37;44m\!m_dir[%Part2%]!\!m_dir[%Part3%]!\!m_dir[%m_countDir%]! %gitBranch%
	endlocal
)
if %m_countDir% equ 3 (
	setlocal EnableDelayedExpansion
	rem echo|set /p = [34;42m %m_dir[1]%[32;46m [37;46m%m_dir[2]%[36;41m [37;41m%m_dir[3]%[0;0m[31m [0;0m
	echo [37;40m %m_dir[1]%[30;44m [37;44m%m_dir[2]%\%m_dir[3]% %gitBranch%
	endlocal
)
if %m_countDir% equ 2 (
	setlocal EnableDelayedExpansion
	rem echo|set /p = [34;42m %m_dir[1]%[32;46m [37;46m%m_dir[2]%[0;0m[36m [0;0m
	echo [37;40m %m_dir[1]%[30;44m [37;44m%m_dir[2]% %gitBranch%
	endlocal
)
if %m_countDir% equ 1 (
	setlocal EnableDelayedExpansion
	rem echo|set /p = [34;42m %m_dir[1]%[0;0m[32m [0;0m
	echo [37;40m %m_dir[1]%[0;0m[30m [0;0m
	endlocal
)
endlocal

set /A m_countDir=1

set /p command=$ 

call %command%
set command=

goto countloop