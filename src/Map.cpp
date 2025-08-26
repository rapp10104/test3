#include "../include/Map.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>

Map::Map() {
    // Initialize tiles vector
    tiles.resize(MAP_WIDTH, std::vector<Tile>(MAP_HEIGHT));

    loadTextures();
    initializeRoom();
    createWalls();
    createDoors();
    setSpawnPoints();
}

void Map::loadTextures() {
    wallTexture.loadFromFile("assets/wall.png");
    floorTexture.loadFromFile("assets/floor.png");
}

void Map::initializeRoom() {
    // Khởi tạo tất cả tiles là EMPTY
    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            tiles[x][y].type = TileType::EMPTY;
            tiles[x][y].isPassable = true;
            tiles[x][y].shape.setPosition(x * TILE_SIZE, y * TILE_SIZE);
            tiles[x][y].shape.setTexture(&floorTexture); // Set floor texture
        }
    }
}

void Map::createWalls() {
    // Tạo tường bao quanh map
    for (int x = 0; x < MAP_WIDTH; x++) {
        // Tường trên và dưới
        tiles[x][0].type = TileType::WALL;
        tiles[x][0].isPassable = false;
        tiles[x][0].shape.setTexture(&wallTexture); // Set wall texture

        tiles[x][MAP_HEIGHT-1].type = TileType::WALL;
        tiles[x][MAP_HEIGHT-1].isPassable = false;
        tiles[x][MAP_HEIGHT-1].shape.setTexture(&wallTexture); // Set wall texture
    }


    for (int y = 0; y < MAP_HEIGHT; y++) {
        // Tường trái và phải
        tiles[0][y].type = TileType::WALL;
        tiles[0][y].isPassable = false;
        tiles[0][y].shape.setTexture(&wallTexture); // Set wall texture

        tiles[MAP_WIDTH-1][y].type = TileType::WALL;
        tiles[MAP_WIDTH-1][y].isPassable = false;
        tiles[MAP_WIDTH-1][y].shape.setTexture(&wallTexture); // Set wall texture
    }

    // Tạo một số cụm tường nhỏ rải rác trong map
    for (int cluster = 0; cluster < 50; cluster++) {

        int centerX = 10 + (rand() % (MAP_WIDTH - 20));
        int centerY = 10 + (rand() % (MAP_HEIGHT - 20));

        // Avoid creating walls near center spawn area (larger safe zone)
        int mapCenterX = MAP_WIDTH / 2;
        int mapCenterY = MAP_HEIGHT / 2;
        if (abs(centerX - mapCenterX) < 5 && abs(centerY - mapCenterY) < 5) continue;

        // Tạo cụm tường 2x2 thay vì 3x3 để không quá đông
        for (int dx = 0; dx <= 1; dx++) {
            for (int dy = 0; dy <= 1; dy++) {
                int x = centerX + dx;
                int y = centerY + dy;

                if (x >= 2 && x < MAP_WIDTH - 2 && y >= 2 && y < MAP_HEIGHT - 2) {
                    // 60% chance để tạo tường trong cụm
                    if (rand() % 100 < 60) {
                        tiles[x][y].type = TileType::WALL;
                        tiles[x][y].isPassable = false;
                        tiles[x][y].shape.setTexture(&wallTexture); // Set wall texture
                    }
                }
            }
        }
    }
}

void Map::createDoors() {
    // Tạo cửa ở giữa các tường (hiện tại chỉ là placeholder)
    // Có thể mở rộng sau để chuyển room
}

void Map::setSpawnPoints() {
    // Find a safe spawn point for player near center
    sf::Vector2f centerPoint(MAP_WIDTH * TILE_SIZE / 2, MAP_HEIGHT * TILE_SIZE / 2);
    playerSpawnPoint = findSafeSpawnPoint(centerPoint, 100.0f); // Search within 100 pixels of center

    // Enemy spawn points - rải rác khắp thế giới
    enemySpawnPoints.clear();

    // Tạo nhiều spawn points hơn cho thế giới lớn
    for (int i = 0; i < 50; i++) {
        sf::Vector2f spawnPoint;
        bool validSpawn = false;
        int attempts = 0;

        while (!validSpawn && attempts < 100) {
            // Generate random position within map bounds (avoid edges)
            spawnPoint.x = (2 + rand() % (MAP_WIDTH - 4)) * TILE_SIZE + TILE_SIZE/2;
            spawnPoint.y = (2 + rand() % (MAP_HEIGHT - 4)) * TILE_SIZE + TILE_SIZE/2;

            // Kiểm tra xem spawn point có hợp lệ không (không trên tường)
            if (isPassable(spawnPoint)) {
                // Đảm bảo không spawn quá gần player
                float distanceToPlayer = std::sqrt(
                    (spawnPoint.x - playerSpawnPoint.x) * (spawnPoint.x - playerSpawnPoint.x) +
                    (spawnPoint.y - playerSpawnPoint.y) * (spawnPoint.y - playerSpawnPoint.y)
                );

                if (distanceToPlayer > 200.0f) { // Tối thiểu 200 pixels từ player
                    validSpawn = true;
                }
            }
            attempts++;
        }

        if (validSpawn) {
            enemySpawnPoints.push_back(spawnPoint);
        }
    }

    std::cout << "Generated " << enemySpawnPoints.size() << " valid enemy spawn points" << std::endl;
}

sf::Vector2f Map::findSafeSpawnPoint(sf::Vector2f preferredPoint, float searchRadius) {
    // First check if preferred point is safe
    if (isPassable(preferredPoint)) {
        return preferredPoint;
    }

    // Search in expanding circles around preferred point
    for (float radius = TILE_SIZE; radius <= searchRadius; radius += TILE_SIZE) {
        // Check points in a circle around preferred point
        for (int angle = 0; angle < 360; angle += 30) { // Check every 30 degrees
            float radians = angle * 3.14159f / 180.0f;
            sf::Vector2f testPoint(
                preferredPoint.x + radius * cos(radians),
                preferredPoint.y + radius * sin(radians)
            );

            // Make sure point is within map bounds
            if (testPoint.x >= TILE_SIZE && testPoint.x < (MAP_WIDTH - 1) * TILE_SIZE &&
                testPoint.y >= TILE_SIZE && testPoint.y < (MAP_HEIGHT - 1) * TILE_SIZE) {

                if (isPassable(testPoint)) {
                    std::cout << "Found safe spawn point at: " << testPoint.x << ", " << testPoint.y << std::endl;
                    return testPoint;
                }
            }
        }
    }

    // Fallback: find any passable tile in the map
    std::cout << "Warning: Could not find safe spawn near preferred point, searching entire map..." << std::endl;
    for (int x = 2; x < MAP_WIDTH - 2; x++) {
        for (int y = 2; y < MAP_HEIGHT - 2; y++) {
            sf::Vector2f testPoint = tileToWorld(sf::Vector2i(x, y));
            if (isPassable(testPoint)) {
                std::cout << "Fallback spawn point found at: " << testPoint.x << ", " << testPoint.y << std::endl;
                return testPoint;
            }
        }
    }

    // Ultimate fallback (should never happen)
    std::cout << "ERROR: No safe spawn point found! Using default position." << std::endl;
    return sf::Vector2f(100.0f, 100.0f);
}

void Map::draw(sf::RenderWindow& window) {
    // Legacy function - draws all tiles (not recommended for large maps)
    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            window.draw(tiles[x][y].shape);
        }
    }
}

void Map::drawInView(sf::RenderWindow& window, sf::Vector2f cameraCenter, sf::Vector2f viewSize) {
    // Calculate visible tile range
    int startX = std::max(0, static_cast<int>((cameraCenter.x - viewSize.x/2) / TILE_SIZE) - 1);
    int endX = std::min(MAP_WIDTH - 1, static_cast<int>((cameraCenter.x + viewSize.x/2) / TILE_SIZE) + 1);
    int startY = std::max(0, static_cast<int>((cameraCenter.y - viewSize.y/2) / TILE_SIZE) - 1);
    int endY = std::min(MAP_HEIGHT - 1, static_cast<int>((cameraCenter.y + viewSize.y/2) / TILE_SIZE) + 1);

    // Only draw tiles that are visible
    for (int x = startX; x <= endX; x++) {
        for (int y = startY; y <= endY; y++) {
            window.draw(tiles[x][y].shape);
        }
    }
}

bool Map::isPassable(float x, float y) const {
    return isPassable(sf::Vector2f(x, y));
}

bool Map::isPassable(sf::Vector2f position) const {
    sf::Vector2i tilePos = worldToTile(position);

    // Kiểm tra bounds - với tường bao, không thể đi ra ngoài map
    if (tilePos.x < 0 || tilePos.x >= MAP_WIDTH ||
        tilePos.y < 0 || tilePos.y >= MAP_HEIGHT) {
        return false; // Không thể đi ra ngoài map có tường bao
    }

    return tiles[tilePos.x][tilePos.y].isPassable;
}

sf::Vector2i Map::worldToTile(sf::Vector2f worldPos) const {
    return sf::Vector2i(
        static_cast<int>(worldPos.x / TILE_SIZE),
        static_cast<int>(worldPos.y / TILE_SIZE)
    );
}

sf::Vector2f Map::tileToWorld(sf::Vector2i tilePos) const {
    return sf::Vector2f(
        tilePos.x * TILE_SIZE + TILE_SIZE / 2.0f,
        tilePos.y * TILE_SIZE + TILE_SIZE / 2.0f
    );
}

