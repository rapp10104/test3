# 🏗️ Soul Knight Clone - Architecture

## 📋 Class Hierarchy

```
BaseObject (Abstract Base Class)
├── Character (Game Character Base)
│   ├── SwordHero (Melee Fighter)
│   └── GunHero (Ranged Fighter)
├── Enemy (AI Controlled)
└── Bullet (Projectile)
```

## 🎯 BaseObject Class

### **Purpose**
Common base class providing shared functionality for all game objects.

### **Core Properties**
```cpp
// Position and movement
sf::Vector2f position;
sf::Vector2f velocity; 
sf::Vector2f size;

// Visual properties
sf::RectangleShape shape;
sf::Color color;
float rotation;

// State management
bool isActive;
bool isVisible;
bool hasCollision;

// Collision detection
sf::FloatRect boundingBox;
```

### **Key Methods**
```cpp
// Position management
setPosition(x, y)
getPosition()
move(offset)

// Collision detection
getBounds()
intersects(other)
distanceTo(other)
directionTo(other)

// Visual properties
setColor(color)
setRotation(angle)
setSize(width, height)

// Rendering
virtual draw(window)
virtual update(deltaTime)
```

## 🎮 Character Class

### **Inheritance**
```cpp
class Character : public BaseObject
```

### **Additional Properties**
```cpp
// Character stats
int hp, maxHp;
int mana, maxMana;
int level, exp, expToNextLevel;
int attackDamage, armor;
float speed;
bool isAlive;

// Visual representation
WebPAnimationLoader webpLoader;
bool useWebPSprite;
```

### **Key Methods**
```cpp
// Combat
virtual attack() = 0;
virtual skill() = 0;
takeDamage(damage);
heal(amount);

// Progression
gainExp(amount);
levelUp();

// Animation
loadWebPAnimation(path);
```

## 👹 Enemy Class

### **Inheritance**
```cpp
class Enemy : public BaseObject
```

### **Additional Properties**
```cpp
// Enemy stats
int hp, maxHp;
int damage;
float speed;
bool isAlive;

// AI behavior
float detectionRange;
float attackRange;
sf::Vector2f targetPosition;
sf::Clock attackCooldown;
```

### **Key Methods**
```cpp
// AI behavior
update(playerPos, map);
canAttack();
isInAttackRange(playerPos);
attackPlayer();

// Combat
takeDamage(damage);
```

## 🔫 Bullet Class

### **Inheritance**
```cpp
class Bullet : public BaseObject
```

### **Additional Properties**
```cpp
// Bullet properties
sf::CircleShape bulletShape;
float speed;
int damage;
```

### **Key Methods**
```cpp
// Movement
update(); // Moves bullet based on velocity

// Combat
getDamage();
```

## 🎯 Benefits of BaseObject Architecture

### **1. Code Reusability**
- Common position/movement logic
- Shared collision detection
- Unified rendering interface

### **2. Polymorphism**
```cpp
std::vector<std::unique_ptr<BaseObject>> gameObjects;
// Can store Characters, Enemies, Bullets together
```

### **3. Consistent Interface**
- All objects have `update()` and `draw()`
- Standardized position/bounds methods
- Common collision detection

### **4. Easy Extension**
```cpp
class PowerUp : public BaseObject {
    // New game object type
};
```

## 🔧 Implementation Details

### **Position Synchronization**
```cpp
// Character overrides to sync with shape
void Character::setPosition(float x, float y) {
    BaseObject::setPosition(x, y); // Update BaseObject
    shape.setPosition(x, y);       // Sync visual shape
    updateSpritePosition();        // Sync WebP sprite
}
```

### **Collision Detection**
```cpp
// BaseObject provides unified collision
bool BaseObject::intersects(const BaseObject& other) {
    return getBounds().intersects(other.getBounds());
}

// Used in game logic
if (enemy.intersects(player)) {
    // Handle collision
}
```

### **Rendering Pipeline**
```cpp
// BaseObject default rendering
void BaseObject::draw(sf::RenderWindow& window) {
    if (isVisible) {
        window.draw(shape);
    }
}

// Character overrides for sprite support
void Character::draw(sf::RenderWindow& window) {
    if (useWebPSprite) {
        webpLoader.draw(window);
    } else {
        BaseObject::draw(window); // Use base shape
    }
}
```

## 📊 Memory Layout

### **BaseObject (Base)**
- Position: 8 bytes (2 floats)
- Velocity: 8 bytes (2 floats)
- Size: 8 bytes (2 floats)
- Shape: ~100 bytes (SFML object)
- Flags: 3 bytes (3 bools)
- **Total**: ~127 bytes

### **Character (Derived)**
- BaseObject: 127 bytes
- Stats: 44 bytes (11 ints)
- WebP Loader: ~50 bytes
- **Total**: ~221 bytes

### **Enemy (Derived)**
- BaseObject: 127 bytes
- Stats: 20 bytes (5 ints)
- AI properties: 20 bytes
- **Total**: ~167 bytes

### **Bullet (Derived)**
- BaseObject: 127 bytes
- Bullet shape: ~50 bytes
- Properties: 8 bytes
- **Total**: ~185 bytes

## 🎮 Usage Examples

### **Creating Objects**
```cpp
// Characters
auto swordHero = std::make_unique<SwordHero>();
auto gunHero = std::make_unique<GunHero>();

// Enemies
Enemy enemy(100, 100);

// Bullets
Bullet bullet(x, y, direction);
```

### **Collision Detection**
```cpp
// Check if enemy hits player
if (enemy.intersects(*player)) {
    player->takeDamage(enemy.getDamage());
}

// Check bullet vs enemy
if (bullet.intersects(enemy)) {
    enemy.takeDamage(bullet.getDamage());
}
```

### **Position Management**
```cpp
// Move objects
enemy.setPosition(100, 200);
bullet.move(velocity * deltaTime);

// Get distances
float dist = enemy.distanceTo(*player);
sf::Vector2f dir = enemy.directionTo(*player);
```

## 🚀 Future Extensions

### **Possible New Classes**
```cpp
class PowerUp : public BaseObject {
    // Health potions, mana potions, etc.
};

class Obstacle : public BaseObject {
    // Destructible walls, barriers
};

class Projectile : public BaseObject {
    // Enemy projectiles, magic spells
};
```

### **Enhanced Features**
- **Component system** for modular behavior
- **Physics integration** for realistic movement
- **Animation state machine** for complex animations
- **Event system** for object communication

---

**BaseObject provides a solid foundation for scalable game object architecture! 🎉**
