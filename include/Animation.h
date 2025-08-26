#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "AnimationConfig.h"

class Animation {
private:
    std::vector<sf::IntRect> frames;
    sf::Texture* texture;
    int currentFrame;
    float frameTime;
    float currentTime;
    bool isLooping;
    bool isFinished;

public:
    Animation();
    Animation(sf::Texture* tex, int frameWidth, int frameHeight, int frameCount, float frameDuration = 0.1f, bool loop = true);
    ~Animation();
    
    void setTexture(sf::Texture* tex);
    void createFrames(int frameWidth, int frameHeight, int frameCount, int startX = 0, int startY = 0);
    void setFrameTime(float time);
    void setLooping(bool loop);
    
    void update(float deltaTime);
    void reset();
    
    sf::IntRect getCurrentFrame() const;
    bool getIsFinished() const;
    int getCurrentFrameIndex() const;
    int getFrameCount() const;
    
    void setCurrentFrame(int frame);
    
    // Scale methods
    void setScale(float scaleX, float scaleY);
    void setScale(float scale);
    sf::Vector2f getScale() const;
    
    // Quality settings
    void setSmooth(bool smooth);
    void setRepeated(bool repeated);
};

#endif
