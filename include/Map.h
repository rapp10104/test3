#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>

enum class TileType {
    EMPTY = 0,
    WALL = 1,
    DOOR = 2,
    SPAWN_POINT = 3
};

struct Tile {
    TileType type;
    sf::RectangleShape shape;
    bool isPassable;
    
    Tile() : type(TileType::EMPTY), isPassable(true) {
        shape.setSize(sf::Vector2f(32.f, 32.f));
    }
};

class Map {
private:
    static const int MAP_WIDTH = 100;  // 25 * 4
    static const int MAP_HEIGHT = 76;  // 19 * 4
    static const int TILE_SIZE = 32;

    std::vector<std::vector<Tile>> tiles;
    sf::Texture wallTexture;
    sf::Texture floorTexture;
    
    std::vector<sf::Vector2f> enemySpawnPoints;
    sf::Vector2f playerSpawnPoint;
    
    void initializeRoom();
    void createWalls();
    void createDoors();
    void setSpawnPoints();
    sf::Vector2f findSafeSpawnPoint(sf::Vector2f preferredPoint, float searchRadius);

public:
    Map();
    void loadTextures();
    void draw(sf::RenderWindow& window);
    void drawInView(sf::RenderWindow& window, sf::Vector2f cameraCenter, sf::Vector2f viewSize);
    bool isPassable(float x, float y) const;
    bool isPassable(sf::Vector2f position) const;
    
    // Getters
    sf::Vector2f getPlayerSpawnPoint() const { return playerSpawnPoint; }
    const std::vector<sf::Vector2f>& getEnemySpawnPoints() const { return enemySpawnPoints; }
    
    // Utility functions
    sf::Vector2i worldToTile(sf::Vector2f worldPos) const;
    sf::Vector2f tileToWorld(sf::Vector2i tilePos) const;
    
    static int getTileSize() { return TILE_SIZE; }
    static int getMapWidth() { return MAP_WIDTH; }
    static int getMapHeight() { return MAP_HEIGHT; }
};

#endif
