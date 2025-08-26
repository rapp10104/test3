@echo off
echo ========================================
echo DOWNLOAD SAMPLE BACKGROUND MUSIC
echo ========================================

echo.
echo This will download a free sample music file for testing.
echo.

if not exist "assets" mkdir assets

echo Checking for curl or PowerShell...

REM Try to download a free sample music file
echo.
echo Downloading sample music from OpenGameArt.org...

REM Use PowerShell to download (available on Windows 10+)
powershell -Command "try { Invoke-WebRequest -Uri 'https://opengameart.org/sites/default/files/audio_preview/Soliloquy_0.ogg' -OutFile 'assets\background.ogg' -ErrorAction Stop; Write-Host 'Downloaded: background.ogg' } catch { Write-Host 'Download failed - you can add music manually' }"

if exist "assets\background.ogg" (
    echo.
    echo ✅ SUCCESS: Sample music downloaded!
    echo File: assets/background.ogg
    echo.
    echo 🎵 This is a free sample track for testing.
    echo 🎮 Music will play when you start a game.
    echo.
) else (
    echo.
    echo ❌ Download failed. You can add music manually:
    echo.
    echo 1. Download any .ogg, .wav, or .mp3 file
    echo 2. Rename to "background.ogg" (or .wav/.mp3)
    echo 3. Place in assets/ folder
    echo.
    echo Free music sources:
    echo   - https://freesound.org
    echo   - https://opengameart.org
    echo   - https://incompetech.com
    echo.
)

echo ========================================
echo READY TO TEST!
echo ========================================
echo.
echo Build and run the game:
echo   ./build.bat
echo   ./game.exe
echo.
echo Features to test:
echo   ✅ Background music (if downloaded)
echo   ✅ Game time display
echo   ✅ Leaderboard with scores + times
echo.
pause
