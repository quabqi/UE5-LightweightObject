@echo off
setlocal enabledelayedexpansion

:: ============================================================================
:: LightweightObject Test Runner
:: Usage: RunTests.bat [action] [engine_path]
::   action: run (default), list, simple
::   engine_path: Optional UE engine path
:: ============================================================================

set PROJECT_PATH=%~dp0LObjectTest.uproject
set TEST_FILTER=LightweightObject
set ACTION=%1
if "%ACTION%"=="" set ACTION=run

:: Find UE engine
if not "%~2"=="" (
    set UE_PATH=%~2
) else if exist "C:\EpicGames\UE_5.7\Engine\Build\BatchFiles\RunUAT.bat" (
    set UE_PATH=C:\EpicGames\UE_5.7
) else if exist "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\RunUAT.bat" (
    set UE_PATH=C:\Program Files\Epic Games\UE_5.7
) else if exist "C:\Program Files\Epic Games\UE_5.6\Engine\Build\BatchFiles\RunUAT.bat" (
    set UE_PATH=C:\Program Files\Epic Games\UE_5.6
) else if exist "C:\Program Files\Epic Games\UE_5.5\Engine\Build\BatchFiles\RunUAT.bat" (
    set UE_PATH=C:\Program Files\Epic Games\UE_5.5
) else (
    echo [ERROR] UE engine not found
    echo Usage: RunTests.bat [action] "C:\Path\To\UE_5.7"
    exit /b 1
)

set RUNUAT="%UE_PATH%\Engine\Build\BatchFiles\RunUAT.bat"
set UE_EDITOR=%UE_PATH%\Engine\Binaries\Win64\UnrealEditor.exe

:: Execute action
if /i "%ACTION%"=="list" goto :do_list
if /i "%ACTION%"=="simple" goto :do_simple
goto :do_run

:: ============================================================================
:do_list
:: List all matching tests
:: ============================================================================
echo Listing tests matching: %TEST_FILTER%
"%UE_EDITOR%" "%PROJECT_PATH%" -ExecCmds="Automation List;Quit" -unattended -nopause -NullRHI -log 2>&1 | findstr /i "%TEST_FILTER%"
exit /b %ERRORLEVEL%

:: ============================================================================
:do_simple
:: Run tests via Editor command line
:: ============================================================================
echo Running tests (simple mode)...
echo Engine: %UE_EDITOR%
echo Project: %PROJECT_PATH%

"%UE_EDITOR%" "%PROJECT_PATH%" ^
    -ExecCmds="Automation RunTests %TEST_FILTER%;Automation List;Quit" ^
    -TestExit="Automation Test Queue Empty" ^
    -unattended -nopause -NullRHI -log

echo.
echo Done. Log: %~dp0Saved\Logs\LObjectTest.log
exit /b %ERRORLEVEL%

:: ============================================================================
:do_run
:: Build and run tests via UAT
:: ============================================================================
echo ============================================================================
echo LightweightObject Unit Tests
echo ============================================================================
echo Engine: %UE_PATH%
echo Project: %PROJECT_PATH%
echo Filter: %TEST_FILTER%
echo ============================================================================

if not exist %RUNUAT% (
    echo [ERROR] RunUAT.bat not found
    exit /b 1
)

echo.
echo [1/2] Building...
call %RUNUAT% BuildCookRun ^
    -project="%PROJECT_PATH%" ^
    -skipcook -build -stage -pak -archive ^
    -archivedirectory="%~dp0Build" ^
    -clientconfig=Development ^
    -targetplatform=Win64

if %ERRORLEVEL% neq 0 (
    echo [ERROR] Build failed
    exit /b 1
)

echo.
echo [2/2] Running tests...
call %RUNUAT% RunAutomationTests ^
    -project="%PROJECT_PATH%" ^
    -testfilter="%TEST_FILTER%" ^
    -logoutputformat="%~dp0TestResults"

if %ERRORLEVEL% neq 0 (
    echo [ERROR] Tests failed
    exit /b 1
)

echo.
echo ============================================================================
echo Tests completed
echo ============================================================================
exit /b 0
