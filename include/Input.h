#ifndef INPUT_H
#define INPUT_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Camera; // Forward declaration

// Input configuration structure
struct InputConfig {
    // Movement keys
    sf::Keyboard::Key moveUp = sf::Keyboard::W;
    sf::Keyboard::Key moveDown = sf::Keyboard::S;
    sf::Keyboard::Key moveLeft = sf::Keyboard::A;
    sf::Keyboard::Key moveRight = sf::Keyboard::D;

    // Action keys
    sf::Keyboard::Key attack = sf::Keyboard::Space;
    sf::Keyboard::Key skill = sf::Keyboard::Enter;
    sf::Keyboard::Key pause = sf::Keyboard::P;
    sf::Keyboard::Key menu = sf::Keyboard::Escape;

    // Mouse buttons
    sf::Mouse::Button primaryAttack = sf::Mouse::Left;
    sf::Mouse::Button secondaryAttack = sf::Mouse::Right;
};

class Input {
private:
    static sf::RenderWindow* window;
    static Camera* camera;
    static InputConfig config;

    // Mouse state
    static sf::Vector2i lastMousePosition;
    static bool leftMousePressed;
    static bool rightMousePressed;
    static bool previousLeftMousePressed;
    static bool previousRightMousePressed;

    // Keyboard state
    static bool keyStates[sf::Keyboard::KeyCount];
    static bool previousKeyStates[sf::Keyboard::KeyCount];

public:
    // Initialization
    static void initialize(sf::RenderWindow* gameWindow, Camera* gameCamera = nullptr);
    static void setCamera(Camera* gameCamera);

    // Configuration
    static void setConfig(const InputConfig& newConfig);
    static InputConfig getConfig();
    
    // Update input state (call once per frame)
    static void update();
    
    // Keyboard input
    static bool isKeyPressed(sf::Keyboard::Key key);
    static bool isKeyJustPressed(sf::Keyboard::Key key);
    static bool isKeyJustReleased(sf::Keyboard::Key key);
    
    // Movement input (returns normalized direction vector)
    static sf::Vector2f getMovementDirection();
    static sf::Vector2f getWASDInput();
    static sf::Vector2f getArrowKeyInput();
    
    // Mouse input
    static bool isLeftMousePressed();
    static bool isRightMousePressed();
    static bool isLeftMouseJustPressed();
    static bool isRightMouseJustPressed();
    
    // Mouse position
    static sf::Vector2i getMousePosition();
    static sf::Vector2f getWorldMousePosition();
    static sf::Vector2f getMouseDirection(sf::Vector2f fromPosition);
    
    // Advanced input features
    static bool isAnyKeyPressed();
    static bool isAnyMouseButtonPressed();
    static sf::Vector2f getMouseDelta(); // Mouse movement since last frame

    // Menu navigation helpers
    static bool isMenuUpPressed();
    static bool isMenuDownPressed();
    static bool isMenuLeftPressed();
    static bool isMenuRightPressed();
    static bool isMenuSelectPressed();
    static bool isMenuBackPressed();

    // Game-specific input handlers
    static bool isAttackPressed();
    static bool isSkillPressed();
    static bool isPausePressed();
    static bool isEscapePressed();
    static bool isMusicTogglePressed();
    static bool isVolumeUpPressed();
    static bool isVolumeDownPressed();

    // Character selection
    static bool isCharacter1Selected();  // 1 key
    static bool isCharacter2Selected();  // 2 key
    static bool isCharacter3Selected();  // 3 key

    // Settings input
    static bool isSettingsVolumeUpPressed();
    static bool isSettingsVolumeDownPressed();
    static bool isSettingsMusicTogglePressed();

    // Text input helpers
    static bool handleTextInput(sf::Event& event, std::string& text, int maxLength = 20);

    // Utility functions
    static sf::Vector2f normalizeVector(sf::Vector2f vector);
    static float vectorLength(sf::Vector2f vector);
    static float getAngle(sf::Vector2f vector);
};

#endif
