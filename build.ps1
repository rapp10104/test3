Write-Host "Building Soul Knight Clone..."

$SFML_INCLUDE = "C:\SFML\SFML-2.6.2\include"
$SFML_LIB = "C:\SFML\SFML-2.6.2\lib"

# Create obj directory if not exists
if (!(Test-Path "obj")) {
    New-Item -ItemType Directory -Path "obj"
}

Write-Host "Compiling source files..."

# Compile all source files with SFML paths
g++ -c src/Main.cpp -o obj/Main.o -I include -I "$SFML_INCLUDE"
g++ -c src/Game.cpp -o obj/Game.o -I include -I "$SFML_INCLUDE"
g++ -c src/BaseObject.cpp -o obj/BaseObject.o -I include -I "$SFML_INCLUDE"
g++ -c src/Character.cpp -o obj/Character.o -I include -I "$SFML_INCLUDE"
g++ -c src/SwordHero.cpp -o obj/SwordHero.o -I include -I "$SFML_INCLUDE"
g++ -c src/GunHero.cpp -o obj/GunHero.o -I include -I "$SFML_INCLUDE"
g++ -c src/MagicHero.cpp -o obj/MagicHero.o -I include -I "$SFML_INCLUDE"
g++ -c src/Enemy.cpp -o obj/Enemy.o -I include -I "$SFML_INCLUDE"
g++ -c src/Weapon.cpp -o obj/Weapon.o -I include -I "$SFML_INCLUDE"
g++ -c src/Sword.cpp -o obj/Sword.o -I include -I "$SFML_INCLUDE"
g++ -c src/Bullet.cpp -o obj/Bullet.o -I include -I "$SFML_INCLUDE"
g++ -c src/MagicBullet.cpp -o obj/MagicBullet.o -I include -I "$SFML_INCLUDE"
g++ -c src/Map.cpp -o obj/Map.o -I include -I "$SFML_INCLUDE"
g++ -c src/Camera.cpp -o obj/Camera.o -I include -I "$SFML_INCLUDE"

Write-Host "Linking..."

# Link all object files with SFML libraries
g++ obj/Main.o obj/Game.o obj/BaseObject.o obj/Character.o obj/SwordHero.o obj/GunHero.o obj/MagicHero.o obj/Enemy.o obj/Weapon.o obj/Sword.o obj/Bullet.o obj/MagicBullet.o obj/Map.o obj/Camera.o -o game.exe -L "$SFML_LIB" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

if (Test-Path "game.exe") {
    Write-Host "Build successful! Game executable created: game.exe"
    Write-Host ""
    Write-Host "To run the game: .\game.exe"
    Write-Host ""
    Write-Host "Controls:"
    Write-Host "- Arrow keys: Navigate menu"
    Write-Host "- Enter: Select option"
    Write-Host "- Type name and Enter: Enter character name"
    Write-Host "- 1/2/3: Select character (after name input)"
    Write-Host "- WASD: Move in game"
    Write-Host "- Left Mouse: Attack (aim with mouse)"
    Write-Host "- Enter: Special skill (Level 10+ required)"
    Write-Host "- P: Pause game"
    Write-Host "- ESC: Back to menu"
    Write-Host "- M: Toggle music (in-game)"
    Write-Host "- Up/Down arrows: Adjust volume (in-game)"
} else {
    Write-Host "Build failed! Check for errors above."
}

Read-Host "Press Enter to continue"
