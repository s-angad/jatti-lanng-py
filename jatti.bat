@echo off
REM Jatti Language - Windows Command Line Wrapper
REM Usage: jatti run file.jatti

cd /d "%~dp0"
call "%~dp0jatti.cmd" %*
