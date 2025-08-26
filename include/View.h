#ifndef VIEW_H
#define VIEW_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

// Forward declarations
class Character;
class Enemy;
class Map;
class Camera;

class View {
private:
    static sf::RenderWindow* window;
    static sf::Font* font;
    static Camera* camera;

    // Background textures
    static sf::Texture menuBackgroundTexture;
    static sf::Sprite menuBackgroundSprite;
    static bool menuBackgroundLoaded;

    // UI Colors
    static sf::Color primaryColor;
    static sf::Color secondaryColor;
    static sf::Color accentColor;
    static sf::Color backgroundColor;
    static sf::Color textColor;

public:
    // Initialization
    static void initialize(sf::RenderWindow* gameWindow, sf::Font* gameFont, Camera* gameCamera = nullptr);
    static void setCamera(Camera* gameCamera);
    
    // Basic rendering
    static void clear(sf::Color color = sf::Color::Black);
    static void display();

    // Background rendering
    static bool loadMenuBackground(const std::string& filepath);
    static void drawMenuBackground();
    
    // Text rendering
    static void drawText(const std::string& text, float x, float y, int fontSize = 24, 
                        sf::Color color = sf::Color::White, bool centered = false);
    static void drawText(const std::string& text, sf::Vector2f position, int fontSize = 24, 
                        sf::Color color = sf::Color::White, bool centered = false);
    
    // Shape rendering
    static void drawRectangle(float x, float y, float width, float height, 
                             sf::Color fillColor = sf::Color::White, 
                             sf::Color outlineColor = sf::Color::Transparent, float outlineThickness = 0.f);
    static void drawCircle(float x, float y, float radius, 
                          sf::Color fillColor = sf::Color::White, 
                          sf::Color outlineColor = sf::Color::Transparent, float outlineThickness = 0.f);
    static void drawLine(sf::Vector2f start, sf::Vector2f end, sf::Color color = sf::Color::White, float thickness = 1.f);
    
    // Game object rendering
    static void drawCharacter(const Character* character);
    static void drawEnemy(const Enemy* enemy);
    static void drawMap(const Map* map);
    
    // UI rendering
    static void drawHealthBar(float x, float y, float width, float height, 
                             float currentHealth, float maxHealth, 
                             sf::Color healthColor = sf::Color::Green, 
                             sf::Color backgroundColor = sf::Color::Red);
    static void drawProgressBar(float x, float y, float width, float height,
                               float progress, sf::Color fillColor = sf::Color::Blue,
                               sf::Color backgroundColor = sf::Color(128, 128, 128));
    
    // Menu rendering
    static void drawMenu(const std::vector<std::string>& options, int selectedOption, 
                        float x, float y, float spacing = 50.f);
    static void drawButton(const std::string& text, float x, float y, float width, float height, 
                          bool selected = false, sf::Color buttonColor = sf::Color::Blue);
    
    // HUD rendering
    static void drawHUD(const Character* player, int score, int enemiesKilled, float playTime);
    static void drawMinimap(const Character* player, const std::vector<Enemy>& enemies, 
                           float x, float y, float size = 150.f);
    
    // Screen effects
    static void drawFadeOverlay(float alpha = 0.5f, sf::Color color = sf::Color::Black);
    static void drawBorder(float thickness = 5.f, sf::Color color = sf::Color::White);
    
    // Utility functions
    static sf::Vector2f getTextSize(const std::string& text, int fontSize = 24);
    static void applyCamera();
    static void resetView();
    
    // Color utilities
    static void setColorTheme(sf::Color primary, sf::Color secondary, sf::Color accent, 
                             sf::Color background, sf::Color text);
    static sf::Color getPrimaryColor() { return primaryColor; }
    static sf::Color getSecondaryColor() { return secondaryColor; }
    static sf::Color getAccentColor() { return accentColor; }
    static sf::Color getBackgroundColor() { return backgroundColor; }
    static sf::Color getTextColor() { return textColor; }
};

#endif