#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include "Character.h"
#include "SwordHero.h"
#include "GunHero.h"
#include "MagicHero.h"
#include "Enemy.h"
#include "GameState.h"
#include "Map.h"
#include "Camera.h"
#include "Input.h"
#include "View.h"

class Game {
private:
    sf::RenderWindow window;
    GameState state;

    std::unique_ptr<Character> player;
    std::vector<Enemy> enemies;
    std::unique_ptr<Map> gameMap;

    // Camera system
    std::unique_ptr<Camera> camera;

    // fonts & text
    sf::Font font;
    std::vector<std::string> menuOptions; // String vectors for View system
    std::vector<std::string> pausedMenuOptions; // String vectors for View system
    int selectedOption;
    bool gameInProgress;

    // Name input
    std::string playerName; // Still needed for storing player name

    // Game stats
    int score;
    int enemiesKilled;
    sf::Clock gameTimer;
    sf::Clock enemySpawnTimer;

    // UI elements now handled by View system

    // Audio system
    sf::Music backgroundMusic;
    bool musicEnabled;
    float musicVolume;
    std::vector<std::string> settingsOptions;
    int settingsSelectedOption;

    void processEvents();
    void handleGameInput(); // Handle game-specific input using Input system
    void handleMenuInput(); // Handle menu navigation using Input system
    void handleCharacterSelection(); // Handle character selection using Input system
    void handleSettings(); // Handle settings input using Input system
    void handleLeaderboard(); // Handle leaderboard input using Input system
    void update();
    void render();

    // menu handlers
    void renderMainMenu();
    void renderPausedMenu();
    void renderNameInput();
    void renderCharacterSelect();
    void renderLeaderboard();
    void renderSettings();
    void startGame();
    void continueGame();

    // audio handlers
    void initializeAudio();
    void toggleMusic();
    void adjustVolume(float delta);

    // game logic
    void updateGameplay();
    void checkCollisions();
    void spawnEnemies();
    void renderGameplay();
    void renderUI();

    // camera functions
    void initializeCamera();
    void updateCamera();

    void loadFont();

    // leaderboard functions
    void saveToLeaderboard();
    std::string formatTime(float seconds);

public:
    Game();
    void run();
};

#endif
