#include "../include/Input.h"
#include "../include/Camera.h"
#include <cmath>
#include <iostream>

// Static member definitions
sf::RenderWindow* Input::window = nullptr;
Camera* Input::camera = nullptr;
InputConfig Input::config = InputConfig(); // Default config
sf::Vector2i Input::lastMousePosition = sf::Vector2i(0, 0);
bool Input::leftMousePressed = false;
bool Input::rightMousePressed = false;
bool Input::previousLeftMousePressed = false;
bool Input::previousRightMousePressed = false;
bool Input::keyStates[sf::Keyboard::KeyCount] = {false};
bool Input::previousKeyStates[sf::Keyboard::KeyCount] = {false};

void Input::initialize(sf::RenderWindow* gameWindow, Camera* gameCamera) {
    window = gameWindow;
    camera = gameCamera;
    
    // Initialize all key states to false
    for (int i = 0; i < sf::Keyboard::KeyCount; i++) {
        keyStates[i] = false;
        previousKeyStates[i] = false;
    }
    
    // Initialize mouse state
    if (window) {
        lastMousePosition = sf::Mouse::getPosition(*window);
    }
    leftMousePressed = false;
    rightMousePressed = false;
}

void Input::setCamera(Camera* gameCamera) {
    camera = gameCamera;
}

void Input::setConfig(const InputConfig& newConfig) {
    config = newConfig;
}

InputConfig Input::getConfig() {
    return config;
}

void Input::update() {
    if (!window) return;
    
    // Update previous key states
    for (int i = 0; i < sf::Keyboard::KeyCount; i++) {
        previousKeyStates[i] = keyStates[i];
        keyStates[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
    }
    
    // Update mouse state
    lastMousePosition = sf::Mouse::getPosition(*window);
    previousLeftMousePressed = leftMousePressed;
    previousRightMousePressed = rightMousePressed;
    leftMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    rightMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);
}

bool Input::isKeyPressed(sf::Keyboard::Key key) {
    return keyStates[key];
}

bool Input::isKeyJustPressed(sf::Keyboard::Key key) {
    return keyStates[key] && !previousKeyStates[key];
}

bool Input::isKeyJustReleased(sf::Keyboard::Key key) {
    return !keyStates[key] && previousKeyStates[key];
}

sf::Vector2f Input::getMovementDirection() {
    return getWASDInput(); // Default to WASD
}

sf::Vector2f Input::getWASDInput() {
    sf::Vector2f direction(0.f, 0.f);

    // Use configurable keys
    if (isKeyPressed(config.moveUp)) direction.y -= 1.f;
    if (isKeyPressed(config.moveDown)) direction.y += 1.f;
    if (isKeyPressed(config.moveLeft)) direction.x -= 1.f;
    if (isKeyPressed(config.moveRight)) direction.x += 1.f;

    return normalizeVector(direction);
}

sf::Vector2f Input::getArrowKeyInput() {
    sf::Vector2f direction(0.f, 0.f);
    
    if (isKeyPressed(sf::Keyboard::Up)) direction.y -= 1.f;
    if (isKeyPressed(sf::Keyboard::Down)) direction.y += 1.f;
    if (isKeyPressed(sf::Keyboard::Left)) direction.x -= 1.f;
    if (isKeyPressed(sf::Keyboard::Right)) direction.x += 1.f;
    
    return normalizeVector(direction);
}

bool Input::isLeftMousePressed() {
    return leftMousePressed;
}

bool Input::isRightMousePressed() {
    return rightMousePressed;
}

bool Input::isLeftMouseJustPressed() {
    return leftMousePressed && !previousLeftMousePressed;
}

bool Input::isRightMouseJustPressed() {
    return rightMousePressed && !previousRightMousePressed;
}

sf::Vector2i Input::getMousePosition() {
    return lastMousePosition;
}

sf::Vector2f Input::getWorldMousePosition() {
    if (!window) return sf::Vector2f(0, 0);

    // Get current mouse position (not cached) for more accuracy
    sf::Vector2i currentMousePos = sf::Mouse::getPosition(*window);
    sf::Vector2f worldMousePos;

    // Use camera to convert screen coordinates to world coordinates
    // This matches the working implementation from Game.cpp
    if (camera) {
        sf::View originalView = window->getView();
        camera->apply(*window);
        worldMousePos = window->mapPixelToCoords(currentMousePos);
        window->setView(originalView); // Restore original view
    } else {
        worldMousePos = window->mapPixelToCoords(currentMousePos);
    }

    return worldMousePos;
}

sf::Vector2f Input::getMouseDirection(sf::Vector2f fromPosition) {
    sf::Vector2f worldMousePos = getWorldMousePosition();
    sf::Vector2f direction = worldMousePos - fromPosition;
    return normalizeVector(direction);
}

sf::Vector2f Input::normalizeVector(sf::Vector2f vector) {
    float length = vectorLength(vector);
    if (length > 0) {
        return sf::Vector2f(vector.x / length, vector.y / length);
    }
    return sf::Vector2f(0.f, 0.f);
}

float Input::vectorLength(sf::Vector2f vector) {
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

float Input::getAngle(sf::Vector2f vector) {
    return std::atan2(vector.y, vector.x) * 180.f / 3.14159f;
}

bool Input::isAnyKeyPressed() {
    for (int i = 0; i < sf::Keyboard::KeyCount; i++) {
        if (keyStates[i]) return true;
    }
    return false;
}

bool Input::isAnyMouseButtonPressed() {
    return leftMousePressed || rightMousePressed;
}

sf::Vector2f Input::getMouseDelta() {
    static sf::Vector2i previousMousePos = lastMousePosition;
    sf::Vector2f delta = sf::Vector2f(lastMousePosition - previousMousePos);
    previousMousePos = lastMousePosition;
    return delta;
}

bool Input::isMenuUpPressed() {
    return isKeyJustPressed(sf::Keyboard::Up) || isKeyJustPressed(sf::Keyboard::W);
}

bool Input::isMenuDownPressed() {
    return isKeyJustPressed(sf::Keyboard::Down) || isKeyJustPressed(sf::Keyboard::S);
}

bool Input::isMenuLeftPressed() {
    return isKeyJustPressed(sf::Keyboard::Left) || isKeyJustPressed(sf::Keyboard::A);
}

bool Input::isMenuRightPressed() {
    return isKeyJustPressed(sf::Keyboard::Right) || isKeyJustPressed(sf::Keyboard::D);
}

bool Input::isMenuSelectPressed() {
    return isKeyJustPressed(sf::Keyboard::Enter) || isKeyJustPressed(sf::Keyboard::Space);
}

bool Input::isMenuBackPressed() {
    return isKeyJustPressed(sf::Keyboard::Escape) || isKeyJustPressed(sf::Keyboard::Backspace);
}

bool Input::isAttackPressed() {
    // Use continuous press for mouse (not just pressed) to allow rapid fire
    return isLeftMousePressed() || isKeyPressed(config.attack);
}

bool Input::isSkillPressed() {
    return isKeyJustPressed(config.skill);
}

bool Input::isPausePressed() {
    return isKeyJustPressed(config.pause);
}

bool Input::isEscapePressed() {
    return isKeyJustPressed(sf::Keyboard::Escape);
}

bool Input::isMusicTogglePressed() {
    return isKeyJustPressed(sf::Keyboard::M);
}

bool Input::isVolumeUpPressed() {
    return isKeyJustPressed(sf::Keyboard::Up);
}

bool Input::isVolumeDownPressed() {
    return isKeyJustPressed(sf::Keyboard::Down);
}

bool Input::isCharacter1Selected() {
    return isKeyJustPressed(sf::Keyboard::Num1);
}

bool Input::isCharacter2Selected() {
    return isKeyJustPressed(sf::Keyboard::Num2);
}

bool Input::isCharacter3Selected() {
    return isKeyJustPressed(sf::Keyboard::Num3);
}

bool Input::isSettingsVolumeUpPressed() {
    return isKeyJustPressed(sf::Keyboard::Up);
}

bool Input::isSettingsVolumeDownPressed() {
    return isKeyJustPressed(sf::Keyboard::Down);
}

bool Input::isSettingsMusicTogglePressed() {
    return isKeyJustPressed(sf::Keyboard::M);
}

bool Input::handleTextInput(sf::Event& event, std::string& text, int maxLength) {
    if (event.type != sf::Event::TextEntered) return false;
    if (event.text.unicode >= 128) return false; // Only ASCII characters

    char inputChar = static_cast<char>(event.text.unicode);
    bool textChanged = false;

    if (inputChar == '\b') { // Backspace
        if (!text.empty()) {
            text.pop_back();
            textChanged = true;
        }
    }
    else if (inputChar == '\r' || inputChar == '\n') { // Enter
        // Return true but don't modify text - caller handles enter
        return true;
    }
    else if (inputChar >= 32 && inputChar < 127 && text.length() < maxLength) { // Printable characters
        text += inputChar;
        textChanged = true;
    }

    return textChanged;
}
