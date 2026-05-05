@echo off
setlocal

REM Jatti Language - Windows Command Line Wrapper
REM Goal: short commands like: jatti run file.jatti

cd /d "%~dp0"

REM Prefer the C-based compiler/VM for `run`.
if /I "%~1"=="run" (
	if exist "c\bin\jatti.exe" (
		"c\bin\jatti.exe" %*
		exit /b %errorlevel%
	) else (
		echo jatti: missing c\bin\jatti.exe
		echo Build it with:
		echo   powershell -ExecutionPolicy Bypass -File .\c\build_msvc.ps1 -Config Release
		exit /b 1
	)
)

echo jatti: unsupported command.
echo This wrapper supports:
echo   jatti run path\to\file.jatti
exit /b 1
