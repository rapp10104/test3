# Soul Knight Clone

A 2D action game inspired by Soul Knight, built with C++ and SFML.

## Features

### Core Gameplay
- **Multiple Character Classes**: Sword Hero, Gun Hero, Magic Hero
- **Character Customization**: Enter custom player name
- **Real-time Combat**: Attack and special skills system
- **Enemy AI**: Enemies chase and attack player
- **Level System**: Gain experience and level up
- **Score System**: Track kills and performance
- **Time Tracking**: Play time displayed and recorded
- **Pause/Resume**: Pause game with P key and continue later
- **Health Display**: Health bar above character and in UI
- **Leaderboard**: Top 10 high scores with time comparison

### Technical Features
- **Input System**: Centralized input handling with configurable controls
- **Camera System**: Smooth camera following player
- **Audio System**: Background music with volume control
- **Weapon Inheritance**: Extensible weapon system
- **Map System**: Tile-based world with collision detection
- **Wall Collision**: Weapons cannot damage through walls
- **Progressive Difficulty**: Exponential leveling system
- **Mouse Aiming**: All attacks follow mouse cursor direction

## Controls
- **Arrow keys**: Navigate menu
- **Enter**: Select option
- **Type name + Enter**: Enter character name
- **1/2/3**: Select character (after name input)
- **WASD**: Move in game
- **Left Mouse**: Attack (aim with mouse cursor)
- **Enter**: Special skill (Level 10+ required)
- **P**: Pause game
- **ESC**: Back to menu
- **M**: Toggle music (in-game)
- **Up/Down arrows**: Adjust volume (in-game)

## Building

### Prerequisites
- C++ compiler (g++)
- SFML 2.6.2 installed at `C:\SFML\SFML-2.6.2\`

### Build Commands
```bash
# Windows (Command Prompt)
.\build.bat

# Windows (PowerShell)
.\build.ps1

# Linux/Mac (Make)
make
```

## Project Structure
```
├── src/           # Source files (.cpp)
├── include/       # Header files (.h)
├── assets/        # Game assets (images, sounds)
├── obj/           # Compiled object files
├── build.bat      # Windows build script
├── build.ps1      # PowerShell build script
├── Makefile       # Unix build script
└── game.exe       # Compiled executable
```

## Dependencies
- SFML Graphics
- SFML Window
- SFML System
- SFML Audio
- OpenAL (for audio)

## Recent Improvements
- **Input System**: Created centralized input handling with configurable controls
- **Template Method Pattern**: Unified common functionality across Hero classes
- **Removed Duplicate Code**: Eliminated ~125 lines of duplicate code from Hero classes
- **Removed Duplicate Properties**: Cleaned up inheritance hierarchy by removing redundant properties
- **Enhanced BaseObject**: Added sprite, texture, velocity, and state management
- **Improved Inheritance**: All game objects now inherit from enhanced BaseObject
- **Code Refactoring**: Cleaner, more maintainable object-oriented design
- **Fixed Mouse Aiming**: Corrected camera coordinate transformation for accurate aiming
- **Fixed Character Selection Bug**: Resolved isAlive initialization issue
- **Code Cleanup**: Removed WebP animation system and unused code
- **Debug Output Cleanup**: Removed unnecessary console output
- **Include Optimization**: Removed unused header includes
- Unified mouse aiming system for all character classes
- Added visual aim indicators for all heroes (different colors)
- Separated movement and attack directions completely
- Added SwordHero special skill: Long Range Slash (Level 10+ required)
- Changed attack controls from Space to Left Mouse Click with aiming
- Added wall collision detection for all weapons (no damage through walls)
- Implemented exponential leveling system (gets progressively harder)
- Enhanced UI layout with proper spacing and time display
- Added comprehensive leaderboard system with top 10 rankings
- Implemented time-based tiebreaker for equal scores
- Added character name input system with custom player names
- Added health bar display above player character
- Fixed player spawn point collision bug - no more spawning in walls
- Added pause/resume functionality with P key
- Moved Camera class to main project structure
- Created Sword weapon class inheriting from Weapon
- Updated SwordHero to use Sword weapon system
- Removed redundant "Character Select" from main menu
- Cleaned up duplicate files and folders
- Fixed font loading paths
- Updated all build scripts
- Removed unused project_game folder
- Consolidated README files
