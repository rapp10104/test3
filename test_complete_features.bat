@echo off
echo ========================================
echo SOUL KNIGHT - COMPLETE FEATURE TEST
echo ========================================

echo.
echo 🎮 Testing all new features:
echo   ✅ BaseObject architecture
echo   ✅ Game time tracking
echo   ✅ Leaderboard system
echo   ✅ Background music support
echo.

echo 📁 Checking files...

echo.
echo 🏗️ Architecture Files:
if exist "include\BaseObject.h" (
    echo ✅ BaseObject.h
) else (
    echo ❌ Missing BaseObject.h
)

if exist "src\BaseObject.cpp" (
    echo ✅ BaseObject.cpp
) else (
    echo ❌ Missing BaseObject.cpp
)

echo.
echo 🎵 Music Files:
if exist "assets\background.ogg" (
    echo ✅ background.ogg [OGG Vorbis]
) else if exist "assets\background.wav" (
    echo ✅ background.wav [WAV Audio]
) else if exist "assets\background.mp3" (
    echo ✅ background.mp3 [MP3 Audio]
) else (
    echo ❌ No background music (game will work without it)
)

echo.
echo 🏆 Leaderboard:
if exist "leaderboard.txt" (
    echo ✅ leaderboard.txt exists
    echo.
    echo Current top scores:
    type leaderboard.txt
) else (
    echo ❌ No leaderboard file (will be created when you play)
)

echo.
echo ========================================
echo BUILDING GAME WITH NEW FEATURES
echo ========================================

call build.bat

if exist "game.exe" (
    echo.
    echo ✅ BUILD SUCCESSFUL!
    echo.
    echo 🎮 New Features Ready:
    echo   🕐 Game time tracking
    echo   🏆 Leaderboard system  
    echo   🎵 Background music support
    echo   🏗️ BaseObject architecture
    echo.
    echo 🎯 How to Test:
    echo   1. Start game → Select character
    echo   2. Watch time counter in top-left
    echo   3. Play until you die
    echo   4. Check leaderboard for your score + time
    echo   5. Try to beat your record!
    echo.
    echo 🚀 Starting game...
    game.exe
) else (
    echo ❌ Build failed! Check errors above.
)

echo.
echo ========================================
pause
