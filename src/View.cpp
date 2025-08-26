#include "../include/View.h"
#include "../include/Character.h"
#include "../include/Enemy.h"
#include "../include/Map.h"
#include "../include/Camera.h"
#include <cmath>
#include <iostream>

// Static member definitions
sf::RenderWindow* View::window = nullptr;
sf::Font* View::font = nullptr;
Camera* View::camera = nullptr;

// Background textures
sf::Texture View::menuBackgroundTexture;
sf::Sprite View::menuBackgroundSprite;
bool View::menuBackgroundLoaded = false;

// Default color theme
sf::Color View::primaryColor = sf::Color::Blue;
sf::Color View::secondaryColor = sf::Color::Cyan;
sf::Color View::accentColor = sf::Color::Yellow;
sf::Color View::backgroundColor = sf::Color::Black;
sf::Color View::textColor = sf::Color::White;

void View::initialize(sf::RenderWindow* gameWindow, sf::Font* gameFont, Camera* gameCamera) {
    window = gameWindow;
    font = gameFont;
    camera = gameCamera;
}

void View::setCamera(Camera* gameCamera) {
    camera = gameCamera;
}

void View::clear(sf::Color color) {
    if (window) {
        window->clear(color);
    }
}

void View::display() {
    if (window) {
        window->display();
    }
}

bool View::loadMenuBackground(const std::string& filepath) {
    if (menuBackgroundTexture.loadFromFile(filepath)) {
        menuBackgroundSprite.setTexture(menuBackgroundTexture);

        // Scale the sprite to fit the window
        if (window) {
            sf::Vector2u windowSize = window->getSize();
            sf::Vector2u textureSize = menuBackgroundTexture.getSize();

            float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
            float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);

            menuBackgroundSprite.setScale(scaleX, scaleY);
        }

        menuBackgroundLoaded = true;
        std::cout << "Menu background loaded successfully from: " << filepath << std::endl;
        return true;
    } else {
        std::cerr << "Failed to load menu background from: " << filepath << std::endl;
        menuBackgroundLoaded = false;
        return false;
    }
}

void View::drawMenuBackground() {
    if (window && menuBackgroundLoaded) {
        // Reset to default view for background
        resetView();
        window->draw(menuBackgroundSprite);
    }
}

void View::drawText(const std::string& text, float x, float y, int fontSize, sf::Color color, bool centered) {
    drawText(text, sf::Vector2f(x, y), fontSize, color, centered);
}

void View::drawText(const std::string& text, sf::Vector2f position, int fontSize, sf::Color color, bool centered) {
    if (!window || !font) return;
    
    sf::Text textObj;
    textObj.setFont(*font);
    textObj.setString(text);
    textObj.setCharacterSize(fontSize);
    textObj.setFillColor(color);
    
    if (centered) {
        sf::FloatRect textBounds = textObj.getLocalBounds();
        textObj.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    }
    
    textObj.setPosition(position);
    window->draw(textObj);
}

void View::drawRectangle(float x, float y, float width, float height, 
                        sf::Color fillColor, sf::Color outlineColor, float outlineThickness) {
    if (!window) return;
    
    sf::RectangleShape rect(sf::Vector2f(width, height));
    rect.setPosition(x, y);
    rect.setFillColor(fillColor);
    
    if (outlineThickness > 0) {
        rect.setOutlineColor(outlineColor);
        rect.setOutlineThickness(outlineThickness);
    }
    
    window->draw(rect);
}

void View::drawCircle(float x, float y, float radius, 
                     sf::Color fillColor, sf::Color outlineColor, float outlineThickness) {
    if (!window) return;
    
    sf::CircleShape circle(radius);
    circle.setPosition(x - radius, y - radius); // Center the circle
    circle.setFillColor(fillColor);
    
    if (outlineThickness > 0) {
        circle.setOutlineColor(outlineColor);
        circle.setOutlineThickness(outlineThickness);
    }
    
    window->draw(circle);
}

void View::drawLine(sf::Vector2f start, sf::Vector2f end, sf::Color color, float thickness) {
    if (!window) return;
    
    sf::Vector2f direction = end - start;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    sf::RectangleShape line(sf::Vector2f(length, thickness));
    line.setPosition(start);
    line.setFillColor(color);
    
    // Calculate rotation angle
    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f;
    line.setRotation(angle);
    
    window->draw(line);
}

void View::drawCharacter(const Character* character) {
    if (!window || !character) return;
    
    // Use character's own draw method for now
    // This can be expanded to handle rendering logic here
    const_cast<Character*>(character)->draw(*window);
}

void View::drawEnemy(const Enemy* enemy) {
    if (!window || !enemy) return;
    
    // Use enemy's own draw method for now
    const_cast<Enemy*>(enemy)->draw(*window);
    
    #if ANIMATION_DEBUG_MODE && ANIMATION_SHOW_FRAME_INFO
    // Debug: kiểm tra animation state khi render
    if (enemy->isAnimationEnabled() && enemy->getCurrentAnimation()) {
        static int lastRenderFrame = -1;
        int currentFrame = enemy->getCurrentAnimation()->getCurrentFrameIndex();
        if (currentFrame != lastRenderFrame) {
            std::cout << "Enemy render - Frame: " << currentFrame << "/" << enemy->getCurrentAnimation()->getFrameCount() << std::endl;
            lastRenderFrame = currentFrame;
        }
    }
    #endif
}

void View::drawMap(const Map* map) {
    if (!window || !map) return;
    
    // Use map's own draw method for now
    const_cast<Map*>(map)->draw(*window);
}

void View::drawHealthBar(float x, float y, float width, float height, 
                        float currentHealth, float maxHealth, 
                        sf::Color healthColor, sf::Color bgColor) {
    if (!window || maxHealth <= 0) return;
    
    // Background
    drawRectangle(x, y, width, height, bgColor);
    
    // Health bar
    float healthPercentage = std::max(0.f, std::min(1.f, currentHealth / maxHealth));
    drawRectangle(x, y, width * healthPercentage, height, healthColor);
    
    // Border
    drawRectangle(x, y, width, height, sf::Color::Transparent, sf::Color::White, 2.f);
}

void View::drawProgressBar(float x, float y, float width, float height, 
                          float progress, sf::Color fillColor, sf::Color bgColor) {
    if (!window) return;
    
    // Background
    drawRectangle(x, y, width, height, bgColor);
    
    // Progress
    float clampedProgress = std::max(0.f, std::min(1.f, progress));
    drawRectangle(x, y, width * clampedProgress, height, fillColor);
    
    // Border
    drawRectangle(x, y, width, height, sf::Color::Transparent, sf::Color::White, 1.f);
}

void View::drawMenu(const std::vector<std::string>& options, int selectedOption, 
                   float x, float y, float spacing) {
    if (!window || !font) return;
    
    for (size_t i = 0; i < options.size(); i++) {
        sf::Color color = (i == selectedOption) ? accentColor : sf::Color::Black;
        drawText(options[i], x + 45, y + i * spacing + 65, 30, color, true);
    }
}

void View::drawButton(const std::string& text, float x, float y, float width, float height, 
                     bool selected, sf::Color buttonColor) {
    if (!window) return;
    
    sf::Color bgColor = selected ? accentColor : buttonColor;
    sf::Color textCol = selected ? backgroundColor : textColor;
    
    // Button background
    drawRectangle(x, y, width, height, bgColor, sf::Color::White, 2.f);
    
    // Button text (centered)
    drawText(text, x + width / 2.f, y + height / 2.f, 20, textCol, true);
}

sf::Vector2f View::getTextSize(const std::string& text, int fontSize) {
    if (!font) return sf::Vector2f(0, 0);
    
    sf::Text textObj;
    textObj.setFont(*font);
    textObj.setString(text);
    textObj.setCharacterSize(fontSize);
    
    sf::FloatRect bounds = textObj.getLocalBounds();
    return sf::Vector2f(bounds.width, bounds.height);
}

void View::applyCamera() {
    if (window && camera) {
        camera->apply(*window);
    }
}

void View::resetView() {
    if (window) {
        window->setView(window->getDefaultView());
    }
}

void View::drawHUD(const Character* player, int score, int enemiesKilled, float playTime) {
    if (!window || !player) return;

    // Reset to default view for UI
    resetView();

    // Health bar
    float healthPercentage = static_cast<float>(player->getHp()) / static_cast<float>(player->getMaxHp());
    drawHealthBar(20, 20, 200, 20, player->getHp(), player->getMaxHp());

    // Player info
    drawText("HP: " + std::to_string(player->getHp()) + "/" + std::to_string(player->getMaxHp()),
             20, 50, 18, textColor);
    drawText("Level: " + std::to_string(player->getLevel()), 20, 75, 18, textColor);
    drawText("Score: " + std::to_string(score), 20, 100, 18, textColor);
    drawText("Enemies: " + std::to_string(enemiesKilled), 20, 125, 18, textColor);

    // Format time
    int minutes = static_cast<int>(playTime) / 60;
    int seconds = static_cast<int>(playTime) % 60;
    std::string timeStr = std::to_string(minutes) + ":" +
                         (seconds < 10 ? "0" : "") + std::to_string(seconds);
    drawText("Time: " + timeStr, 20, 150, 18, textColor);

    // Player name
    drawText("Player: " + player->getName(), 20, 175, 18, accentColor);
}

void View::drawMinimap(const Character* player, const std::vector<Enemy>& enemies,
                      float x, float y, float size) {
    if (!window || !player) return;

    // Reset to default view for UI
    resetView();

    // Minimap background
    drawRectangle(x, y, size, size, sf::Color(0, 0, 0, 128), sf::Color::White, 2.f);

    // Player position (center of minimap)
    sf::Vector2f playerPos = player->getPosition();
    float centerX = x + size / 2.f;
    float centerY = y + size / 2.f;

    // Draw player
    drawCircle(centerX, centerY, 3.f, sf::Color::Green);

    // Draw enemies (relative to player)
    float scale = 0.1f; // Scale factor for minimap
    for (const auto& enemy : enemies) {
        sf::Vector2f enemyPos = enemy.getPosition();
        sf::Vector2f relativePos = enemyPos - playerPos;

        float mapX = centerX + relativePos.x * scale;
        float mapY = centerY + relativePos.y * scale;

        // Only draw if within minimap bounds
        if (mapX >= x && mapX <= x + size && mapY >= y && mapY <= y + size) {
            drawCircle(mapX, mapY, 2.f, sf::Color::Red);
        }
    }
}

void View::drawFadeOverlay(float alpha, sf::Color color) {
    if (!window) return;

    resetView();

    sf::Color overlayColor = color;
    overlayColor.a = static_cast<sf::Uint8>(alpha * 255);

    sf::Vector2u windowSize = window->getSize();
    drawRectangle(0, 0, windowSize.x, windowSize.y, overlayColor);
}

void View::drawBorder(float thickness, sf::Color color) {
    if (!window) return;

    resetView();

    sf::Vector2u windowSize = window->getSize();

    // Top border
    drawRectangle(0, 0, windowSize.x, thickness, color);
    // Bottom border
    drawRectangle(0, windowSize.y - thickness, windowSize.x, thickness, color);
    // Left border
    drawRectangle(0, 0, thickness, windowSize.y, color);
    // Right border
    drawRectangle(windowSize.x - thickness, 0, thickness, windowSize.y, color);
}

void View::setColorTheme(sf::Color primary, sf::Color secondary, sf::Color accent,
                        sf::Color background, sf::Color text) {
    primaryColor = primary;
    secondaryColor = secondary;
    accentColor = accent;
    backgroundColor = background;
    textColor = text;
}
