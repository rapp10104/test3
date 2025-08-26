#include "../include/Animation.h"
#include "../include/AnimationConfig.h"
#include <iostream>

Animation::Animation() 
    : texture(nullptr), currentFrame(0), frameTime(0.1f), currentTime(0.0f), isLooping(ANIMATION_ENABLE_LOOP), isFinished(false) {
}

Animation::Animation(sf::Texture* tex, int frameWidth, int frameHeight, int frameCount, float frameDuration, bool loop)
    : texture(tex), currentFrame(0), frameTime(frameDuration), currentTime(0.0f), isLooping(loop), isFinished(false) {
    if (texture) {
        createFrames(frameWidth, frameHeight, frameCount);
        
        // Apply quality settings from config
        texture->setSmooth(ANIMATION_SMOOTH_TEXTURE);
        texture->setRepeated(ANIMATION_REPEATED_TEXTURE);
    }
}

Animation::~Animation() {
    // Clear frames trước khi destroy để tránh crash
    try {
        // Reset texture reference trước
        texture = nullptr;
        
        // Clear frames một cách an toàn
        if (!frames.empty()) {
            frames.clear();
        }
    } catch (...) {
        // Ignore any exceptions during cleanup
        texture = nullptr;
        frames.clear();
    }
}

void Animation::setTexture(sf::Texture* tex) {
    texture = tex;
    
    // Apply quality settings from config
    if (texture) {
        texture->setSmooth(ANIMATION_SMOOTH_TEXTURE);
        texture->setRepeated(ANIMATION_REPEATED_TEXTURE);
    }
}

void Animation::createFrames(int frameWidth, int frameHeight, int frameCount, int startX, int startY) {
    // Clear frames một cách an toàn
    try {
        frames.clear();
        
        if (!texture) return;
        
        int textureWidth = texture->getSize().x;
        int textureHeight = texture->getSize().y;
        
        // Reserve space để tránh reallocation
        frames.reserve(frameCount);
        
        for (int i = 0; i < frameCount; ++i) {
            int x = startX + (i * frameWidth) % textureWidth;
            int y = startY + (i * frameWidth / textureWidth) * frameHeight;
            
            // Ensure we don't go out of bounds
            if (x + frameWidth <= textureWidth && y + frameHeight <= textureHeight) {
                frames.push_back(sf::IntRect(x, y, frameWidth, frameHeight));
            }
        }
        
        #if ANIMATION_DEBUG_MODE
        std::cout << "Animation frames created: " << frames.size() << " frames" << std::endl;
        #endif
    } catch (...) {
        // Nếu có lỗi, clear frames và log
        frames.clear();
        #if ANIMATION_DEBUG_MODE
        std::cout << "ERROR: Failed to create animation frames" << std::endl;
        #endif
    }
}

void Animation::setFrameTime(float time) {
    frameTime = time;
}

void Animation::setLooping(bool loop) {
    isLooping = loop;
}

void Animation::update(float deltaTime) {
    if (frames.empty()) return;
    
    currentTime += deltaTime;
    
    if (currentTime >= frameTime) {
        currentTime = 0.0f;
        currentFrame++;
        
        if (currentFrame >= frames.size()) {
            if (isLooping) {
                currentFrame = 0;
            } else {
                currentFrame = frames.size() - 1;
                isFinished = true;
            }
        }
    }
}

void Animation::reset() {
    currentFrame = 0;
    currentTime = 0.0f;
    isFinished = false;
}

sf::IntRect Animation::getCurrentFrame() const {
    try {
        // Kiểm tra vector frames có hợp lệ không
        if (frames.empty()) {
            return sf::IntRect();
        }
        
        // Kiểm tra currentFrame có trong range không
        if (currentFrame < 0 || currentFrame >= static_cast<int>(frames.size())) {
            return sf::IntRect();
        }
        
        // Kiểm tra frame có hợp lệ không
        if (currentFrame < frames.size()) {
            return frames[currentFrame];
        }
        
        return sf::IntRect();
    } catch (...) {
        // Nếu có exception, return empty rect
        return sf::IntRect();
    }
}

bool Animation::getIsFinished() const {
    return isFinished;
}

int Animation::getCurrentFrameIndex() const {
    return currentFrame;
}

int Animation::getFrameCount() const {
    return frames.size();
}

void Animation::setCurrentFrame(int frame) {
    if (frame >= 0 && frame < frames.size()) {
        currentFrame = frame;
    }
}

// Scale methods implementation
void Animation::setScale(float scaleX, float scaleY) {
    // Scale will be applied when rendering
}

void Animation::setScale(float scale) {
    setScale(scale, scale);
}

sf::Vector2f Animation::getScale() const {
    return sf::Vector2f(1.0f, 1.0f); // Default scale
}

// Quality settings implementation
void Animation::setSmooth(bool smooth) {
    if (texture) {
        texture->setSmooth(smooth);
    }
}

void Animation::setRepeated(bool repeated) {
    if (texture) {
        texture->setRepeated(repeated);
    }
}
