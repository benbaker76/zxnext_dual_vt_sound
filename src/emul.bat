@ECHO OFF
IF NOT DEFINED CSPECT_HOME SET CSPECT_HOME=..\..\..\..\NextTools\CSpect
CD %~dp0\..\bin
REM %CSPECT_HOME%\CSpect.exe -brk -w4 -s28 -r -tv -rewind -zxnext -mmc=.\ zxnext_dual_vt_sound.nex
%CSPECT_HOME%\CSpect.exe -brk -w4 -s28 -r -tv -zxnext -mmc=.\ zxnext_dual_vt_sound.nex
