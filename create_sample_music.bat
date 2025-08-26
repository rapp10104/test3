@echo off
echo ========================================
echo CREATING SAMPLE BACKGROUND MUSIC
echo ========================================

echo.
echo This script will help you add background music to the game.
echo.

echo 📁 Checking assets folder...
if not exist "assets" mkdir assets

echo.
echo 🎵 Music File Options:
echo.
echo The game supports these audio formats:
echo   - .ogg (recommended - best compression)
echo   - .wav (uncompressed - larger files)
echo   - .mp3 (compressed - good quality)
echo.

echo Current music search order:
echo   1. assets/background.ogg
echo   2. assets/background.wav  
echo   3. assets/background.mp3
echo.

if exist "assets\background.ogg" (
    echo ✅ Found: assets/background.ogg
    goto :music_found
)

if exist "assets\background.wav" (
    echo ✅ Found: assets/background.wav
    goto :music_found
)

if exist "assets\background.mp3" (
    echo ✅ Found: assets/background.mp3
    goto :music_found
)

echo ❌ No background music found.
echo.

echo ========================================
echo HOW TO ADD BACKGROUND MUSIC
echo ========================================
echo.

echo OPTION 1: Download free music
echo   1. Go to: https://freesound.org or https://opengameart.org
echo   2. Search for "background music" or "game music"
echo   3. Download as .ogg, .wav, or .mp3
echo   4. Rename to "background.ogg" (or .wav/.mp3)
echo   5. Place in assets/ folder
echo.

echo OPTION 2: Use your own music
echo   1. Choose any music file (.ogg/.wav/.mp3)
echo   2. Rename to "background.ogg" (or .wav/.mp3)
echo   3. Place in assets/ folder
echo.

echo OPTION 3: Create simple tone (Windows only)
echo   This will create a simple beep tone for testing:
echo.
choice /c yn /m "Create simple test tone? (y/n)"
if errorlevel 2 goto :no_music
if errorlevel 1 goto :create_tone

:create_tone
echo.
echo Creating simple test tone...

REM Create a simple WAV file using PowerShell (Windows 10+)
powershell -Command "Add-Type -AssemblyName System.Speech; $synth = New-Object System.Speech.Synthesis.SpeechSynthesizer; $synth.SetOutputToWaveFile('assets\background.wav'); $synth.Speak('Game music test'); $synth.Dispose()"

if exist "assets\background.wav" (
    echo ✅ Created: assets/background.wav
    goto :music_found
) else (
    echo ❌ Failed to create test tone
    goto :no_music
)

:music_found
echo.
echo ✅ Background music ready!
echo.
echo 🎮 Music will play when you start a game.
echo 🔊 Volume is set to 30% (comfortable level)
echo 🔄 Music will loop continuously during gameplay
echo 🛑 Music stops when game ends or you return to menu
echo.
goto :end

:no_music
echo.
echo ℹ️  Game will work without music.
echo   You can add music later by placing audio files in assets/
echo.

:end
echo ========================================
echo READY TO PLAY!
echo ========================================
echo.
echo Build and run the game:
echo   ./build.bat
echo   ./game.exe
echo.
pause
