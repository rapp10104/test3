#include "../include/Game.h"
#include "../include/AnimationConfig.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>



Game::Game() : window(sf::VideoMode(800, 600), "Soul Knight") {
    state = GameState::MAIN_MENU;
    selectedOption = 0;
    gameInProgress = false;
    playerName = "";
    score = 0;
    enemiesKilled = 0;

    // Initialize audio settings
    musicEnabled = true;
    musicVolume = 50.0f;
    settingsSelectedOption = 0;

    // Seed random number generator
    srand(static_cast<unsigned>(time(nullptr)));

    // Window settings
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false); // Prevent key repeat for better control

    loadFont();
    initializeCamera();
    initializeAudio();

    // Initialize Input system after camera
    Input::initialize(&window, camera.get());

    // Initialize View system
    View::initialize(&window, &font, camera.get());



    // setup main menu - title now handled by View system

    // Setup menu options as string vectors for View system
    menuOptions = {"NEW GAME", "LEADERBOARD", "SETTINGS", "EXIT"};
    std::cout << "Creating menu options..." << std::endl;
    for (size_t i = 0; i < menuOptions.size(); i++) {
        std::cout << "Menu option " << i << ": " << menuOptions[i] << std::endl;
    }



    // Setup paused menu (with Continue Game option)
    pausedMenuOptions = {"Continue Game", "New Game", "Leaderboard", "Settings", "Exit"};

    // Name input now handled by View system

    // Setup settings menu as string vector for View system
    settingsOptions = {"Music: ON", "Volume: 50%", "Back"};

    // UI texts are now handled by View system - no need for setup

    // Load menu background after everything is initialized
    View::loadMenuBackground("assets/background_menu.png");
}

void Game::loadFont() {
    // Try to load font from multiple sources
    if (!font.loadFromFile("assets/arial.TTF")) {
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cerr << "Warning: Could not load font. Text may not display properly.\n";
        }
    }
}

void Game::run() {
    while (window.isOpen()) {
        Input::update(); // Update input state first
        processEvents(); // Only text input and window events
        handleMenuInput(); // Handle menu navigation
        handleCharacterSelection(); // Handle character selection
        handleSettings(); // Handle settings input
        handleLeaderboard(); // Handle leaderboard input
        handleGameInput(); // Handle game input
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        // Only handle event-based input (text input, window events)
        if (state == GameState::NAME_INPUT) {
            if (Input::handleTextInput(event, playerName, 20)) {
                // Text was modified or Enter was pressed
                if (event.text.unicode == '\r' || event.text.unicode == '\n') {
                    // Enter pressed
                    if (!playerName.empty()) {
                        state = GameState::CHARACTER_SELECT;
                    }
                } else {
                    // Text was modified - no need to update sf::Text, View system handles it
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    state = GameState::MAIN_MENU;
                }
            }
        }

        // All other input handling moved to Input system methods
        // IN_GAME state doesn't need event handling - uses Input system in main loop
    }
}

void Game::handleMenuInput() {
    // Handle menu navigation using Input system (not event-based)
    if (state == GameState::MAIN_MENU) {
        if (Input::isMenuUpPressed()) {
            selectedOption = (selectedOption - 1 + menuOptions.size()) % menuOptions.size();
            std::cout << "Selected option: " << selectedOption << std::endl;
        } else if (Input::isMenuDownPressed()) {
            selectedOption = (selectedOption + 1) % menuOptions.size();
            std::cout << "Selected option: " << selectedOption << std::endl;
        } else if (Input::isMenuSelectPressed()) {
            std::cout << "Enter pressed, selected option: " << selectedOption << std::endl;

            if (selectedOption == 0) {
                // New Game - chuyển đến name input
                std::cout << "New Game selected, going to name input..." << std::endl;
                gameInProgress = false; // Reset game state
                playerName = ""; // Reset player name
                state = GameState::NAME_INPUT;
            }
            else if (selectedOption == 1) {
                std::cout << "Leaderboard selected" << std::endl;
                state = GameState::LEADERBOARD;
            }
            else if (selectedOption == 2) {
                std::cout << "Settings selected" << std::endl;
                state = GameState::SETTINGS;
            }
            else if (selectedOption == 3) {
                std::cout << "Exit selected" << std::endl;
                window.close();
            }
        }
    }
    else if (state == GameState::PAUSED_MENU) {
        if (Input::isMenuUpPressed()) {
            selectedOption = (selectedOption - 1 + pausedMenuOptions.size()) % pausedMenuOptions.size();
            std::cout << "Selected option: " << selectedOption << std::endl;
        } else if (Input::isMenuDownPressed()) {
            selectedOption = (selectedOption + 1) % pausedMenuOptions.size();
            std::cout << "Selected option: " << selectedOption << std::endl;
        } else if (Input::isMenuSelectPressed()) {
            std::cout << "Enter pressed, selected option: " << selectedOption << std::endl;

            if (selectedOption == 0) {
                // Continue Game
                std::cout << "Continue Game selected" << std::endl;
                continueGame();
            }
            else if (selectedOption == 1) {
                // New Game - reset và chuyển đến name input
                gameInProgress = false;
                playerName = ""; // Reset player name
                state = GameState::NAME_INPUT;
            }
            else if (selectedOption == 2) {
                std::cout << "Leaderboard selected" << std::endl;
                state = GameState::LEADERBOARD;
            }
            else if (selectedOption == 3) {
                std::cout << "Settings selected" << std::endl;
                state = GameState::SETTINGS;
            }
            else if (selectedOption == 4) {
                std::cout << "Exit selected" << std::endl;
                window.close();
            }
        } else if (Input::isPausePressed()) {
            // Quick resume with P key
            continueGame();
        }
    }
}

void Game::handleCharacterSelection() {
    if (state != GameState::CHARACTER_SELECT) return;

    if (Input::isCharacter1Selected()) {
        player = std::make_unique<SwordHero>();
        startGame();
    }
    else if (Input::isCharacter2Selected()) {
        player = std::make_unique<GunHero>();
        startGame();
    }
    else if (Input::isCharacter3Selected()) {
        player = std::make_unique<MagicHero>();
        startGame();
    }
    else if (Input::isEscapePressed()) {
        state = GameState::MAIN_MENU;
    }
}

void Game::handleSettings() {
    if (state != GameState::SETTINGS) return;

    if (Input::isSettingsVolumeUpPressed()) {
        adjustVolume(10.0f);
    }
    else if (Input::isSettingsVolumeDownPressed()) {
        adjustVolume(-10.0f);
    }
    else if (Input::isSettingsMusicTogglePressed()) {
        toggleMusic();
    }
    else if (Input::isEscapePressed()) {
        state = gameInProgress ? GameState::PAUSED_MENU : GameState::MAIN_MENU;
    }
}

void Game::handleLeaderboard() {
    if (state != GameState::LEADERBOARD) return;

    if (Input::isEscapePressed()) {
        state = gameInProgress ? GameState::PAUSED_MENU : GameState::MAIN_MENU;
    }
}

void Game::handleGameInput() {
    if (state != GameState::IN_GAME || !player) return;

    // Handle attack input
    if (Input::isAttackPressed()) {
        // Use Input system for mouse direction calculation
        sf::Vector2f attackDir = Input::getMouseDirection(player->getPosition());

        // Set aim direction for all heroes (unified)
        player->setAimDirection(attackDir);
        player->attack();
    }

    // Handle skill input
    if (Input::isSkillPressed()) {
        player->skill();
    }

    // Handle pause input
    if (Input::isPausePressed()) {
        state = GameState::PAUSED_MENU;
        selectedOption = 0; // Reset to first option (Continue Game)
    }

    // Handle escape input
    if (Input::isEscapePressed()) {
        state = GameState::MAIN_MENU;
        gameInProgress = false; // End game completely
    }

    // Handle music controls
    if (Input::isMusicTogglePressed()) {
        toggleMusic();
    }

    if (Input::isVolumeUpPressed()) {
        adjustVolume(10.0f);
    }

    if (Input::isVolumeDownPressed()) {
        adjustVolume(-10.0f);
    }
}

void Game::update() {
    if (state == GameState::IN_GAME) {
        updateGameplay();
    }
}

void Game::updateGameplay() {
    if (!player || !player->getIsAlive()) {
        // Game over logic - save to leaderboard
        saveToLeaderboard();
        gameInProgress = false;
        state = GameState::MAIN_MENU;
        return;
    }

    // Update player
    player->update(gameMap.get());
    
    // Update player animation
    if (player) {
        player->BaseObject::update(1.0f/ANIMATION_UPDATE_RATE); // Update animation using config rate
    }

    // Update camera to follow player
    updateCamera();
    
    #if ANIMATION_DEBUG_MODE
    // Debug: hiển thị camera và player position
    if (camera && player) {
        static sf::Vector2f lastCameraPos(-1, -1);
        sf::Vector2f cameraPos = camera->getCenter();
        if (cameraPos.x != lastCameraPos.x || cameraPos.y != lastCameraPos.y) {
            std::cout << "Camera position: (" << cameraPos.x << ", " << cameraPos.y << ")" << std::endl;
            std::cout << "Player position: (" << player->getPosition().x << ", " << player->getPosition().y << ")" << std::endl;
            std::cout << "Active enemies: " << enemies.size() << std::endl;
            lastCameraPos = cameraPos;
        }
    }
    #endif

    // Update enemies
    for (auto& enemy : enemies) {
        enemy.update(player->getPosition(), gameMap.get());
        // Update enemy animation
        enemy.BaseObject::update(1.0f/ANIMATION_UPDATE_RATE); // Update animation using config rate
        
        #if ANIMATION_DEBUG_MODE && ANIMATION_SHOW_FRAME_INFO
        // Debug: kiểm tra animation state
        if (enemy.isAnimationEnabled() && enemy.getCurrentAnimation()) {
            static int lastDebugFrame = -1;
            int currentFrame = enemy.getCurrentAnimation()->getCurrentFrameIndex();
            if (currentFrame != lastDebugFrame) {
                std::cout << "Enemy update - Frame: " << currentFrame << "/" << enemy.getCurrentAnimation()->getFrameCount() << std::endl;
                lastDebugFrame = currentFrame;
            }
        }
        #endif
    }

    // Remove dead enemies
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [this](const Enemy& enemy) {
                if (!enemy.getIsAlive()) {
                    score += 100;
                    enemiesKilled++;
                    player->gainExp(25);
                    return true;
                }
                return false;
            }),
        enemies.end()
    );

    // Check collisions
    checkCollisions();

    // Spawn new enemies
    spawnEnemies();
}

void Game::render() {
    View::clear();

    if (state == GameState::MAIN_MENU) {
        renderMainMenu();
    }
    else if (state == GameState::PAUSED_MENU) {
        renderPausedMenu();
    }
    else if (state == GameState::NAME_INPUT) {
        renderNameInput();
    }
    else if (state == GameState::CHARACTER_SELECT) {
        renderCharacterSelect();
    }
    else if (state == GameState::LEADERBOARD) {
        renderLeaderboard();
    }
    else if (state == GameState::SETTINGS) {
        renderSettings();
    }
    else if (state == GameState::IN_GAME) {
        renderGameplay();
        renderUI();
    }

    View::display();
}

void Game::renderGameplay() {
    // Apply camera view for world objects using View system
    View::applyCamera();

    // Draw map using View system
    if (gameMap && camera) {
        gameMap->drawInView(window, camera->getCenter(), sf::Vector2f(800.0f, 600.0f));
    }

    // Draw player using View system
    if (player) {
        View::drawCharacter(player.get());

        // Draw health bar above player using View system
        sf::Vector2f playerPos = player->getPosition();
        float healthBarWidth = 40.0f;
        float healthBarHeight = 6.0f;

        View::drawHealthBar(playerPos.x - healthBarWidth/2, playerPos.y - 25,
                           healthBarWidth, healthBarHeight,
                           player->getHp(), player->getMaxHp());
    }

    // Draw enemies - only those in view
    if (camera) {
        sf::Vector2f cameraCenter = camera->getCenter();
        for (auto& enemy : enemies) {
            // Simple frustum culling for enemies
            sf::Vector2f enemyPos = enemy.getPosition();
            float distance = std::sqrt(
                (enemyPos.x - cameraCenter.x) * (enemyPos.x - cameraCenter.x) +
                (enemyPos.y - cameraCenter.y) * (enemyPos.y - cameraCenter.y)
            );

            // Only draw enemies within reasonable distance from camera
            if (distance < 800.0f) { // Tăng tầm nhìn để hiển thị enemy
                View::drawEnemy(&enemy);
                
                #if ANIMATION_DEBUG_MODE
                // Debug: hiển thị enemy position và distance
                static int lastDebugEnemy = -1;
                if (enemy.getCurrentAnimation()) {
                    int currentFrame = enemy.getCurrentAnimation()->getCurrentFrameIndex();
                    if (currentFrame != lastDebugEnemy) {
                        std::cout << "Enemy visible at (" << enemyPos.x << ", " << enemyPos.y 
                                  << ") - Distance: " << distance << " - Frame: " << currentFrame << std::endl;
                        lastDebugEnemy = currentFrame;
                    }
                }
                #endif
            }
        }
    }
}

void Game::renderUI() {
    if (!player) return;

    // Calculate play time
    float currentPlayTime = gameTimer.getElapsedTime().asSeconds();

    // Use View system to draw HUD
    View::drawHUD(player.get(), score, enemiesKilled, currentPlayTime);
}

void Game::renderMainMenu() {
    // Draw background first
      View::drawMenuBackground();

    // Draw title using View system
    //View::drawText("SOUL KNIGHT", 400, 150, 48, View::getPrimaryColor(), true);

    // Draw menu using View system (menuOptions is already string vector)
    View::drawMenu(menuOptions, selectedOption, 350, 250, 50);
}

void Game::renderPausedMenu() {
    // Draw background first
    View::drawMenuBackground();

    // Draw title using View system
    View::drawText("GAME PAUSED", 400, 50, 40, View::getAccentColor(), true);

    // Draw menu using View system (pausedMenuOptions is already string vector)
    View::drawMenu(pausedMenuOptions, selectedOption, 250, 200, 50);

    // Draw instructions using View system
    View::drawText("Press P to quickly resume", 400, 450, 16, View::getSecondaryColor(), true);
    View::drawText("Use Arrow Keys and Enter to navigate", 400, 475, 16, View::getSecondaryColor(), true);
}

void Game::renderNameInput() {
    // Draw title using View system
    View::drawText("NEW GAME", 400, 100, 40, View::getAccentColor(), true);

    // Draw prompt using View system
    View::drawText("Enter your name:", 400, 200, 24, View::getTextColor(), true);

    // Draw input box using View system
    View::drawRectangle(200, 245, 400, 40, sf::Color(50, 50, 50), sf::Color::White, 2);

    // Draw input text using View system
    std::string displayText = playerName + "_";
    View::drawText(displayText, 210, 255, 20, View::getTextColor());

    // Draw instructions using View system
    View::drawText("Type your name and press ENTER to continue", 400, 320, 16, View::getSecondaryColor(), true);
    View::drawText("Press ESC to go back", 400, 345, 16, View::getSecondaryColor(), true);
    View::drawText("Max 20 characters", 400, 370, 16, View::getSecondaryColor(), true);
}

void Game::renderCharacterSelect(){
    // Title using View system
    View::drawText("CHARACTER SELECT", 400, 50, 48, View::getPrimaryColor(), true);

    // Character options using View system
    View::drawText("1 - SWORD HERO", 400, 150, 32, sf::Color::Blue, true);
    View::drawText("2 - GUN HERO", 400, 200, 32, sf::Color::Green, true);
    View::drawText("3 - MAGIC HERO", 400, 250, 32, sf::Color::Magenta, true);

    // Instructions using View system
    View::drawText("Press number key to select character", 400, 350, 20, View::getAccentColor(), true);
    View::drawText("Press ESC to go back", 400, 380, 20, View::getAccentColor(), true);

    // Visual character previews using View system
    View::drawRectangle(180, 150, 40, 40, sf::Color::Blue);
    View::drawRectangle(180, 200, 40, 40, sf::Color::Green);
    View::drawRectangle(180, 250, 40, 40, sf::Color::Magenta);
}

void Game::renderLeaderboard() {
    // Title using View system
    View::drawText("TOP 10 LEADERBOARD", 400, 50, 36, View::getAccentColor(), true);

    // Instructions using View system
    View::drawText("(Press ESC to go back)", 400, 90, 16, View::getSecondaryColor(), true);

    // Header using View system
    View::drawText("Rank", 50, 120, 18, View::getTextColor());
    View::drawText("Player Name", 150, 120, 18, View::getTextColor());
    View::drawText("Score", 350, 120, 18, View::getTextColor());
    View::drawText("Time", 450, 120, 18, View::getTextColor());

    // read leaderboard.txt
    std::ifstream file("leaderboard.txt");
    if (file.is_open()) {
        std::string line;
        int y = 150;
        int rank = 1;
        while (std::getline(file, line) && rank <= 10) {
            // Format: "Name - Score: X - Time: Y"
            size_t scorePos = line.find("Score: ");
            size_t timePos = line.find("Time: ");

            if (scorePos != std::string::npos && timePos != std::string::npos) {
                std::string name = line.substr(0, scorePos - 3);
                std::string scoreStr = line.substr(scorePos + 7);
                size_t scoreEnd = scoreStr.find(" - ");
                if (scoreEnd != std::string::npos) {
                    scoreStr = scoreStr.substr(0, scoreEnd);
                }
                std::string timeStr = line.substr(timePos + 6);
                // Truncate long names
                if (name.length() > 15) {
                    name = name.substr(0, 12) + "...";
                }

                // Color coding for ranks using View system
                sf::Color rankColor;
                if (rank == 1) rankColor = sf::Color(255, 215, 0); // Gold
                else if (rank == 2) rankColor = sf::Color(192, 192, 192); // Silver
                else if (rank == 3) rankColor = sf::Color(205, 127, 50); // Bronze
                else rankColor = sf::Color::White;

                // Draw each column separately for proper alignment
                View::drawText(std::to_string(rank) + ".", 50, y, 16, rankColor);
                View::drawText(name, 150, y, 16, rankColor);
                View::drawText(scoreStr, 350, y, 16, rankColor);
                View::drawText(timeStr, 450, y, 16, rankColor);

                y += 25;
                rank++;
            }
        }
        file.close();

        if (rank == 1) {
            View::drawText("No games played yet!", 400, 200, 20, sf::Color::Red, true);
        }
    } else {
        View::drawText("No leaderboard data found", 400, 200, 20, sf::Color::Red, true);
    }
}
void Game::startGame() {
    if (!player) {
        return; // Cannot start without player
    }

    state = GameState::IN_GAME;
    gameInProgress = true; // Mark game as in progress

    // Create map
    gameMap = std::make_unique<Map>();

    // Reset game stats
    score = 0;
    enemiesKilled = 0;
    gameTimer.restart();
    enemySpawnTimer.restart();

    // Position player at spawn point
    if (player && gameMap) {
        sf::Vector2f spawnPoint = gameMap->getPlayerSpawnPoint();

        // Double-check that spawn point is safe
        if (!gameMap->isPassable(spawnPoint)) {
            // Find alternative spawn point
            for (int x = 2; x < gameMap->getMapWidth() - 2; x++) {
                for (int y = 2; y < gameMap->getMapHeight() - 2; y++) {
                    sf::Vector2f testPoint(x * gameMap->getTileSize() + gameMap->getTileSize()/2,
                                         y * gameMap->getTileSize() + gameMap->getTileSize()/2);
                    if (gameMap->isPassable(testPoint)) {
                        spawnPoint = testPoint;
                        goto spawn_found;
                    }
                }
            }
            spawn_found:;
        }

        player->setPosition(spawnPoint.x, spawnPoint.y);
        player->setName(playerName); // Set player name

        // Initialize camera to follow player
        if (camera) {
            camera->follow(player->getPosition());
        }
    }

    // Clear and spawn initial enemies
    enemies.clear();
    const auto& spawnPoints = gameMap->getEnemySpawnPoints();
    
    #if ANIMATION_DEBUG_MODE
    std::cout << "=== INITIAL ENEMY SPAWN ===" << std::endl;
    std::cout << "Total spawn points available: " << spawnPoints.size() << std::endl;
    std::cout << "Will spawn " << std::min(spawnPoints.size(), size_t(3)) << " initial enemies" << std::endl;
    #endif
    
    for (size_t i = 0; i < std::min(spawnPoints.size(), size_t(3)); i++) {
        // Sử dụng emplace_back để tránh copy
        enemies.emplace_back(spawnPoints[i].x, spawnPoints[i].y);
        // Setup animation cho enemy vừa tạo
        enemies.back().setupEnemyAnimation("../assets/Enemy_spritesheet.png", ENEMY_FRAME_WIDTH, ENEMY_FRAME_HEIGHT, ENEMY_FRAME_COUNT);
        
        #if ANIMATION_DEBUG_MODE
        std::cout << "Initial enemy " << i << " spawned at (" << spawnPoints[i].x << ", " << spawnPoints[i].y << ")" << std::endl;
        std::cout << "Enemy animation: " << (enemies.back().isAnimationEnabled() ? "ENABLED" : "DISABLED") << std::endl;
        #endif
    }
    
    #if ANIMATION_DEBUG_MODE
    std::cout << "Total initial enemies: " << enemies.size() << std::endl;
    std::cout << "=== END INITIAL SPAWN ===" << std::endl;
    #endif
}

void Game::continueGame() {
    if (gameInProgress && player && gameMap) {
        state = GameState::IN_GAME;
    } else {
        playerName = ""; // Reset player name
        state = GameState::NAME_INPUT;
    }
}

void Game::checkCollisions() {
    if (!player) return;
    

    // Check bullet vs enemy collisions for GunHero
    if (auto* gunHero = dynamic_cast<GunHero*>(player.get())) {
        auto& bullets = gunHero->getBullets();

        for (size_t i = 0; i < bullets.size();) {
            bool bulletHit = false;

            for (auto& enemy : enemies) {
                if (enemy.getIsAlive() &&
                    bullets[i].getBounds().intersects(enemy.getBounds())) {

                    enemy.takeDamage(bullets[i].getDamage());
                    bullets.erase(bullets.begin() + i);
                    bulletHit = true;
                    break;
                }
            }

            if (!bulletHit) {
                ++i;
            }
        }
    }

    // Check magic bullet vs enemy collisions for MagicHero
    if (auto* magicHero = dynamic_cast<MagicHero*>(player.get())) {
        auto& magicBullets = magicHero->getBullets();

        for (size_t i = 0; i < magicBullets.size();) {
            bool bulletHit = false;

            for (auto& enemy : enemies) {
                if (enemy.getIsAlive() &&
                    magicBullets[i].getBounds().intersects(enemy.getBounds())) {

                    enemy.takeDamage(magicBullets[i].getDamage());
                    magicBullets.erase(magicBullets.begin() + i);
                    bulletHit = true;
                    break;
                }
            }

            if (!bulletHit) {
                ++i;
            }
        }
    }

    // Check sword hero vs enemy collisions
    if (auto* swordHero = dynamic_cast<SwordHero*>(player.get())) {
        if (swordHero->getIsAttacking()) { // Dùng getter thay vì truy cập trực tiếp
            sf::FloatRect attackArea = swordHero->getAttackArea();

            for (auto& enemy : enemies) {
                if (enemy.getIsAlive() &&
                    attackArea.intersects(enemy.getBounds())) {

                    // Check if there's a wall between player and enemy
                    sf::Vector2f playerPos = player->getPosition();
                    sf::Vector2f enemyPos = enemy.getPosition();

                    // Simple line-of-sight check
                    bool wallBlocking = false;
                    if (gameMap) {
                        // Check a few points along the line between player and enemy
                        for (float t = 0.1f; t < 1.0f; t += 0.2f) {
                            sf::Vector2f checkPoint = playerPos + t * (enemyPos - playerPos);
                            if (!gameMap->isPassable(checkPoint)) {
                                wallBlocking = true;
                                break;
                            }
                        }
                    }

                    if (!wallBlocking) {
                        enemy.takeDamage(swordHero->getAttackDamage());
                    }
                }
            }
        }
    }

    // Check enemy vs player collisions
    for (auto& enemy : enemies) {
        if (enemy.getIsAlive() &&
            enemy.getBounds().intersects(player->getBounds()) &&
            enemy.canAttack()) {

            player->takeDamage(enemy.getDamage());
            enemy.attackPlayer();
        }
    }
}

void Game::spawnEnemies() {
    // Spawn enemies every 3 seconds, but limit active enemies
    float spawnTime = enemySpawnTimer.getElapsedTime().asSeconds();
    
    #if ANIMATION_DEBUG_MODE
    static float lastDebugTime = -1.0f;
    if (spawnTime - lastDebugTime > 1.0f) { // Log mỗi giây
        std::cout << "Spawn timer: " << spawnTime << "s, Active enemies: " << enemies.size() << std::endl;
        lastDebugTime = spawnTime;
    }
    #endif
    
    if (spawnTime > 3.0f && enemies.size() < 15) {
        enemySpawnTimer.restart();

        const auto& spawnPoints = gameMap->getEnemySpawnPoints();
        if (!spawnPoints.empty() && player) {
            // Find spawn points within reasonable distance from player
            std::vector<sf::Vector2f> nearbySpawns;
            sf::Vector2f playerPos = player->getPosition();

            #if ANIMATION_DEBUG_MODE
            std::cout << "=== ENEMY SPAWN DEBUG ===" << std::endl;
            std::cout << "Player position: (" << playerPos.x << ", " << playerPos.y << ")" << std::endl;
            std::cout << "Total spawn points: " << spawnPoints.size() << std::endl;
            #endif

            for (const auto& spawn : spawnPoints) {
                float distance = std::sqrt(
                    (spawn.x - playerPos.x) * (spawn.x - playerPos.x) +
                    (spawn.y - playerPos.y) * (spawn.y - playerPos.y)
                );

                // Spawn enemies within 200-500 pixels from player để có thể vào tầm nhìn
                if (distance > 200.0f && distance < 500.0f) {
                    nearbySpawns.push_back(spawn);
                    
                    #if ANIMATION_DEBUG_MODE
                    std::cout << "Valid spawn point: (" << spawn.x << ", " << spawn.y << ") - Distance: " << distance << std::endl;
                    #endif
                }
            }

            #if ANIMATION_DEBUG_MODE
            std::cout << "Nearby spawns found: " << nearbySpawns.size() << std::endl;
            #endif

            if (!nearbySpawns.empty()) {
                int randomIndex = rand() % nearbySpawns.size();
                
                // Tạo enemy trực tiếp trong vector để tránh copy/move
                enemies.emplace_back(nearbySpawns[randomIndex].x, nearbySpawns[randomIndex].y);
                
                // Setup animation cho enemy vừa tạo
                enemies.back().setupEnemyAnimation("../assets/Enemy_spritesheet.png", ENEMY_FRAME_WIDTH, ENEMY_FRAME_HEIGHT, ENEMY_FRAME_COUNT);
                
                #if ANIMATION_DEBUG_MODE
                std::cout << "Enemy spawned at: (" << nearbySpawns[randomIndex].x 
                          << ", " << nearbySpawns[randomIndex].y << ") - Total enemies: " 
                          << enemies.size() << std::endl;
                std::cout << "Enemy animation state: " << (enemies.back().isAnimationEnabled() ? "ENABLED" : "DISABLED") << std::endl;
                std::cout << "Enemy animation object: " << (enemies.back().getCurrentAnimation() ? "OK" : "NULL") << std::endl;
                
                // Debug: kiểm tra distance từ player
                float spawnDistance = std::sqrt(
                    (nearbySpawns[randomIndex].x - playerPos.x) * (nearbySpawns[randomIndex].x - playerPos.x) +
                    (nearbySpawns[randomIndex].y - playerPos.y) * (nearbySpawns[randomIndex].y - playerPos.y)
                );
                std::cout << "Spawn distance from player: " << spawnDistance << " pixels" << std::endl;
                std::cout << "=== END SPAWN DEBUG ===" << std::endl;
                #endif
            } else {
                #if ANIMATION_DEBUG_MODE
                std::cout << "No valid spawn points found within range!" << std::endl;
                std::cout << "=== END SPAWN DEBUG ===" << std::endl;
                #endif
            }
        } else {
            #if ANIMATION_DEBUG_MODE
            std::cout << "Cannot spawn enemy: spawnPoints=" << (spawnPoints.empty() ? "EMPTY" : "OK") 
                      << ", player=" << (player ? "OK" : "NULL") << std::endl;
            #endif
        }
    }

    // Remove enemies that are too far from player to save memory
    if (player) {
        sf::Vector2f playerPos = player->getPosition();
        size_t enemiesBefore = enemies.size();
        
        #if ANIMATION_DEBUG_MODE
        std::cout << "=== ENEMY REMOVAL DEBUG ===" << std::endl;
        std::cout << "Checking " << enemies.size() << " enemies for removal..." << std::endl;
        #endif
        
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [playerPos](const Enemy& enemy) {
                    sf::Vector2f enemyPos = enemy.getPosition();
                    float distance = std::sqrt(
                        (enemyPos.x - playerPos.x) * (enemyPos.x - playerPos.x) +
                        (enemyPos.y - playerPos.y) * (enemyPos.y - playerPos.y)
                    );
                    
                    #if ANIMATION_DEBUG_MODE
                    if (distance > 1000.0f) {
                        std::cout << "Enemy at (" << enemyPos.x << ", " << enemyPos.y 
                                  << ") will be removed - Distance: " << distance << std::endl;
                    }
                    #endif
                    
                    return distance > 1000.0f; // Giảm khoảng cách để enemy có thể vào tầm nhìn
                }),
            enemies.end()
        );
        
        #if ANIMATION_DEBUG_MODE
        if (enemies.size() != enemiesBefore) {
            std::cout << "Enemies removed: " << (enemiesBefore - enemies.size()) 
                      << " (distance > 1000)" << std::endl;
            std::cout << "Remaining enemies: " << enemies.size() << std::endl;
        } else {
            std::cout << "No enemies removed" << std::endl;
        }
        std::cout << "=== END REMOVAL DEBUG ===" << std::endl;
        #endif
    }
}

void Game::initializeAudio() {
    // Load background music
    if (!backgroundMusic.openFromFile("assets/background_music.mp3")) {
        std::cerr << "Warning: Could not load background music from assets/background_music.mp3" << std::endl;
        musicEnabled = false;
        return;
    }

    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(musicVolume);

    if (musicEnabled) {
        backgroundMusic.play();
    }
}

void Game::toggleMusic() {
    musicEnabled = !musicEnabled;

    if (musicEnabled) {
        backgroundMusic.play();
        settingsOptions[0] = "Music: ON";
    } else {
        backgroundMusic.stop();
        settingsOptions[0] = "Music: OFF";
    }
}

void Game::adjustVolume(float delta) {
    musicVolume += delta;
    if (musicVolume < 0.0f) musicVolume = 0.0f;
    if (musicVolume > 100.0f) musicVolume = 100.0f;

    backgroundMusic.setVolume(musicVolume);
    settingsOptions[1] = "Volume: " + std::to_string(static_cast<int>(musicVolume)) + "%";
}

void Game::renderSettings() {
    // Title using View system
    View::drawText("SETTINGS", 400, 50, 40, View::getPrimaryColor(), true);

    // Draw settings options using View system
    std::string musicText = musicEnabled ? "Music: ON" : "Music: OFF";
    std::string volumeText = "Volume: " + std::to_string(static_cast<int>(musicVolume)) + "%";

    View::drawText(musicText, 250, 200, 30, View::getTextColor());
    View::drawText(volumeText, 250, 260, 30, View::getTextColor());
    View::drawText("Back", 250, 320, 30, View::getTextColor());

    // Instructions using View system
    View::drawText("Use Arrow Keys to navigate", 50, 400, 16, View::getSecondaryColor());
    View::drawText("Up/Down to adjust volume", 50, 425, 16, View::getSecondaryColor());
    View::drawText("M to toggle music", 50, 450, 16, View::getSecondaryColor());
    View::drawText("ESC to go back", 50, 475, 16, View::getSecondaryColor());
}

void Game::initializeCamera() {
    // Initialize camera
    camera = std::make_unique<Camera>(800.0f, 600.0f);
}

void Game::updateCamera() {
    if (player && camera) {
        // Make camera follow player
        camera->follow(player->getPosition());
    }
}

std::string Game::formatTime(float seconds) {
    int minutes = static_cast<int>(seconds) / 60;
    int secs = static_cast<int>(seconds) % 60;
    int millisecs = static_cast<int>((seconds - static_cast<int>(seconds)) * 100);

    std::string result = "";
    if (minutes > 0) {
        result += std::to_string(minutes) + "m ";
    }
    result += std::to_string(secs) + "." + (millisecs < 10 ? "0" : "") + std::to_string(millisecs) + "s";
    return result;
}

void Game::saveToLeaderboard() {
    if (!player) return;

    float playTime = gameTimer.getElapsedTime().asSeconds();
    std::string playerName = player->getName();

    // Read existing leaderboard
    struct LeaderboardEntry {
        std::string name;
        int score;
        float time;

        bool operator<(const LeaderboardEntry& other) const {
            if (score != other.score) {
                return score > other.score; // Higher score first
            }
            return time < other.time; // If same score, shorter time first
        }
    };

    std::vector<LeaderboardEntry> entries;

    // Read existing entries
    std::ifstream inFile("leaderboard.txt");
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            // Parse line format: "Name - Score: X - Time: Y"
            size_t scorePos = line.find("Score: ");
            size_t timePos = line.find("Time: ");

            if (scorePos != std::string::npos && timePos != std::string::npos) {
                std::string name = line.substr(0, scorePos - 3); // Remove " - "

                std::string scoreStr = line.substr(scorePos + 7);
                size_t scoreEnd = scoreStr.find(" - ");
                if (scoreEnd != std::string::npos) {
                    scoreStr = scoreStr.substr(0, scoreEnd);
                }

                std::string timeStr = line.substr(timePos + 6);

                try {
                    int entryScore = std::stoi(scoreStr);
                    float entryTime = std::stof(timeStr.substr(0, timeStr.find('s')));
                    entries.push_back({name, entryScore, entryTime});
                } catch (const std::exception& e) {
                    // Skip invalid entries
                    continue;
                }
            }
        }
        inFile.close();
    }

    // Add current game entry
    entries.push_back({playerName, score, playTime});

    // Sort entries
    std::sort(entries.begin(), entries.end());

    // Keep only top 10
    if (entries.size() > 10) {
        entries.resize(10);
    }

    // Write back to file
    std::ofstream outFile("leaderboard.txt");
    if (outFile.is_open()) {
        for (size_t i = 0; i < entries.size(); i++) {
            outFile << entries[i].name << " - Score: " << entries[i].score
                    << " - Time: " << formatTime(entries[i].time) << std::endl;
        }
        outFile.close();
        std::cout << "Leaderboard updated! Player: " << playerName
                 << ", Score: " << score << ", Time: " << formatTime(playTime) << std::endl;
    }
}
