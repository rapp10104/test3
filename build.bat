@echo off
echo Building Soul Knight Clone...

REM Set SFML paths
set SFML_INCLUDE=C:\SFML\SFML-2.6.2\include
set SFML_LIB=C:\SFML\SFML-2.6.2\lib

REM Create obj directory if not exists
if not exist obj mkdir obj

echo Compiling source files...

REM Compile all source files with SFML paths
g++ -c src/Main.cpp -o obj/Main.o -I include -I "%SFML_INCLUDE%"
g++ -c src/Game.cpp -o obj/Game.o -I include -I "%SFML_INCLUDE%"
g++ -c src/BaseObject.cpp -o obj/BaseObject.o -I include -I "%SFML_INCLUDE%"
g++ -c src/Character.cpp -o obj/Character.o -I include -I "%SFML_INCLUDE%"
g++ -c src/Input.cpp -o obj/Input.o -I include -I "%SFML_INCLUDE%"
g++ -c src/View.cpp -o obj/View.o -I include -I "%SFML_INCLUDE%"
g++ -c src/SwordHero.cpp -o obj/SwordHero.o -I include -I "%SFML_INCLUDE%"
g++ -c src/GunHero.cpp -o obj/GunHero.o -I include -I "%SFML_INCLUDE%"
g++ -c src/MagicHero.cpp -o obj/MagicHero.o -I include -I "%SFML_INCLUDE%"
g++ -c src/Enemy.cpp -o obj/Enemy.o -I include -I "%SFML_INCLUDE%"
g++ -c src/Weapon.cpp -o obj/Weapon.o -I include -I "%SFML_INCLUDE%"
g++ -c src/Sword.cpp -o obj/Sword.o -I include -I "%SFML_INCLUDE%"
g++ -c src/Bullet.cpp -o obj/Bullet.o -I include -I "%SFML_INCLUDE%"
g++ -c src/MagicBullet.cpp -o obj/MagicBullet.o -I include -I "%SFML_INCLUDE%"

g++ -c src/Map.cpp -o obj/Map.o -I include -I "%SFML_INCLUDE%"
g++ -c src/Camera.cpp -o obj/Camera.o -I include -I "%SFML_INCLUDE%"

echo Linking...

REM Link all object files with SFML libraries
g++ obj/Main.o obj/Game.o obj/BaseObject.o obj/Character.o obj/Input.o obj/View.o obj/SwordHero.o obj/GunHero.o obj/MagicHero.o obj/Enemy.o obj/Weapon.o obj/Sword.o obj/Bullet.o obj/MagicBullet.o obj/Map.o obj/Camera.o -o game.exe -L "%SFML_LIB%" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

if exist game.exe (
    echo Build successful! Game executable created: game.exe
    echo.
    echo To run the game: ./game.exe
    echo.
    echo Controls:
    echo - Arrow keys: Navigate menu
    echo - Enter: Select option
    echo - Type name and Enter: Enter character name
    echo - 1/2/3: Select character (after name input)
    echo - WASD: Move in game
    echo - Left Mouse: Attack (aim with mouse)
    echo - Enter: Special skill (Level 10+ required)
    echo - P: Pause game
    echo - ESC: Back to menu
    echo - M: Toggle music (in-game)
    echo - Up/Down arrows: Adjust volume (in-game)
) else (
    echo Build failed! Check for errors above.
)

pause
