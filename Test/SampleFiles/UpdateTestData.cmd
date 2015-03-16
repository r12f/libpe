@echo off

set SAMPLE_PROJECT_ROOT=%~dp0.
set TEST_DATA_ROOT=%SAMPLE_PROJECT_ROOT%\..\Data
set OUTPUT_ROOT=%SAMPLE_PROJECT_ROOT%\..\..\Output\Release

copy "%OUTPUT_ROOT%\x86\Bin\SampleExe.exe" "%TEST_DATA_ROOT%\x86Exe\Test.exe"
copy "%OUTPUT_ROOT%\x86\Bin\SampleDll.dll" "%TEST_DATA_ROOT%\x86Dll\Test.dll"

copy "%OUTPUT_ROOT%\x64\Bin\SampleExe.exe" "%TEST_DATA_ROOT%\x64Exe\Test.exe"
copy "%OUTPUT_ROOT%\x64\Bin\SampleDll.dll" "%TEST_DATA_ROOT%\x64Dll\Test.dll"
