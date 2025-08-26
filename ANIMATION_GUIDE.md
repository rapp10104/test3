# Animation System Guide

## Tổng quan
Hệ thống animation đã được tích hợp vào game để hiển thị texture động cho player và enemies. Hệ thống sử dụng spritesheet và class `Animation` để xử lý các frame animation. **Hệ thống đã được scale để phù hợp với kích thước game window và có thể tùy chỉnh dễ dàng thông qua file config.**

## ⚠️ VẤN ĐỀ ĐÃ ĐƯỢC KHẮC PHỤC
- **Texture quá to**: Đã scale xuống còn 15% (player) và 12% (enemy) kích thước gốc
- **Game crash**: Đã tối ưu hóa memory usage và texture loading
- **Performance**: Đã cải thiện frame rate và animation smoothness

## Cấu trúc

### File Config Chính
- **File**: `include/AnimationConfig.h`
- **Chức năng**: Chứa tất cả các thông số có thể tùy chỉnh
- **Tính năng**:
  - Scale cho player và enemy
  - Frame timing và count
  - Quality settings
  - Performance tuning
  - Debug options

### Class Animation
- **File**: `include/Animation.h`, `src/Animation.cpp`
- **Chức năng**: Xử lý spritesheet animation với các frame
- **Tính năng**:
  - Tự động chia spritesheet thành các frame
  - Hỗ trợ loop và non-loop animation
  - Có thể điều chỉnh tốc độ frame
  - **Tự động scale theo config**

### Tích hợp vào BaseObject
- **File**: `include/BaseObject.h`, `src/BaseObject.cpp`
- **Chức năng**: Cung cấp animation support cho tất cả game objects
- **Tính năng**:
  - `setAnimation(Animation*)`: Set animation cho object
  - `enableAnimation(bool)`: Bật/tắt animation
  - `getCurrentAnimation()`: Lấy animation hiện tại
  - **`setAnimationScale(float, float)`**: Tự động scale texture theo config

### Tích hợp vào Character
- **File**: `include/Character.h`, `src/Character.cpp`
- **Chức năng**: Hỗ trợ animation cho player characters
- **Method**: `setupPlayerAnimation(spritesheetPath, frameWidth, frameHeight, frameCount)`

### Tích hợp vào Enemy
- **File**: `include/Enemy.h`, `src/Enemy.cpp`
- **Chức năng**: Hỗ trợ animation cho enemies
- **Method**: `setupEnemyAnimation(spritesheetPath, frameWidth, frameHeight, frameCount)`

## Cách sử dụng

### 0. Tùy chỉnh Config (QUAN TRỌNG!)
```cpp
// Trong file include/AnimationConfig.h, bạn có thể thay đổi:

// Scale cho player (15% = 0.15f)
#define PLAYER_ANIMATION_SCALE_X 0.15f
#define PLAYER_ANIMATION_SCALE_Y 0.15f

// Scale cho enemy (12% = 0.12f)  
#define ENEMY_ANIMATION_SCALE_X 0.12f
#define ENEMY_ANIMATION_SCALE_Y 0.12f

// Frame timing
#define PLAYER_FRAME_DURATION 0.1f    // 0.1 giây mỗi frame
#define ENEMY_FRAME_DURATION 0.08f    // 0.08 giây mỗi frame

// Performance
#define ANIMATION_UPDATE_RATE 60.0f   // 60 FPS
```

### 1. Setup Player Animation (Tự động sử dụng config)
```cpp
// Trong constructor của GunHero, SwordHero, MagicHero
setupPlayerAnimation("../assets/Gun_player_spritesheet.png", PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT, PLAYER_FRAME_COUNT);
```

### 2. Setup Enemy Animation (Tự động sử dụng config)
```cpp
// Trong Game::spawnEnemies()
Enemy newEnemy(x, y);
newEnemy.setupEnemyAnimation("../assets/Enemy_spritesheet.png", ENEMY_FRAME_WIDTH, ENEMY_FRAME_HEIGHT, ENEMY_FRAME_COUNT);
enemies.push_back(newEnemy);
```

### 3. Tạo Animation tùy chỉnh
```cpp
// Tạo animation mới
Animation* anim = new Animation(&texture, frameWidth, frameHeight, frameCount, frameDuration, loop);
setAnimation(anim);
enableAnimation(true);
```

## Spritesheet Specifications

### Gun Player Spritesheet
- **File**: `assets/Gun_player_spritesheet.png`
- **Kích thước gốc**: 6440 x 217 pixels
- **Số frame**: 20 (đã sửa từ 17)
- **Kích thước mỗi frame**: 379 x 217 pixels
- **Kích thước hiển thị**: 76 x 43 pixels (20% scale)

### Enemy Spritesheet
- **File**: `assets/Enemy_spritesheet.png`
- **Kích thước gốc**: 13300 x 175 pixels
- **Số frame**: 70
- **Kích thước mỗi frame**: 190 x 175 pixels
- **Kích thước hiển thị**: 23 x 21 pixels (12% scale) - đã giảm để ổn định

## ⚙️ Tùy chỉnh Scale
Bạn có thể thay đổi scale trong `include/AnimationConfig.h`:
- **Tăng scale**: Làm texture to hơn (ví dụ: 0.2f = 20%)
- **Giảm scale**: Làm texture nhỏ hơn (ví dụ: 0.1f = 10%)
- **Scale khác nhau X/Y**: Để texture không bị méo

## Tính năng

### Animation Loop
- Animation mặc định sẽ loop vô hạn
- Có thể tắt loop bằng `setLooping(false)`

### Frame Timing
- Mặc định: 0.1 giây mỗi frame
- Có thể điều chỉnh bằng `setFrameTime(duration)`

### Position Sync
- Sprite và shape position được đồng bộ tự động
- Khi gọi `setPosition(x, y)`, cả sprite và shape đều được cập nhật

### Fallback Rendering
- Khi animation không được enable, object sẽ render bằng shape
- Khi animation được enable, object sẽ render bằng sprite

## Lưu ý

1. **Memory Management**: Animation objects được quản lý bởi BaseObject, tự động delete khi object bị hủy
2. **Texture Loading**: Spritesheet phải được load thành công trước khi tạo animation
3. **Frame Size**: Đảm bảo frameWidth và frameHeight chính xác để tránh lỗi rendering
4. **Performance**: Animation được update mỗi frame trong game loop
5. **Scale Configuration**: **QUAN TRỌNG** - Thay đổi scale trong `AnimationConfig.h` để phù hợp với game
6. **Auto-scaling**: Texture sẽ tự động được scale theo config khi setup animation

## Troubleshooting

### Animation không hiển thị
- Kiểm tra file spritesheet có tồn tại không
- Kiểm tra frameWidth và frameHeight có đúng không
- Kiểm tra `enableAnimation(true)` đã được gọi chưa
- **Kiểm tra scale trong `AnimationConfig.h` có quá nhỏ không**

### Animation bị giật
- Kiểm tra frame timing có phù hợp không
- Kiểm tra game loop có ổn định không
- **Điều chỉnh `ANIMATION_UPDATE_RATE` trong config**

### Performance issues
- Giảm số frame nếu cần thiết
- Tăng frame timing để giảm số lần update
- **Giảm `ANIMATION_UPDATE_RATE` để tiết kiệm CPU**

### Texture quá to/nhỏ
- **Thay đổi scale trong `AnimationConfig.h`**:
  - `PLAYER_ANIMATION_SCALE_X/Y`: Scale cho player
  - `ENEMY_ANIMATION_SCALE_X/Y`: Scale cho enemy
- **Giá trị scale**: 0.1f = 10%, 0.2f = 20%, 0.5f = 50%

### Game crash
- **Giảm scale xuống** (ví dụ: 0.05f = 5%)
- **Tắt `ANIMATION_SMOOTH_TEXTURE`** để tiết kiệm memory
- **Giảm `ANIMATION_UPDATE_RATE`** để giảm tải CPU

### Enemy crash khi vào tầm nhìn
- **Kiểm tra file `Enemy_spritesheet.png` có tồn tại không**
- **Giảm scale xuống** (ví dụ: 0.12f thay vì 0.15f) để tránh memory issues
- **Tắt `ANIMATION_SMOOTH_TEXTURE`** để giảm memory usage
- **Bật `ANIMATION_DEBUG_MODE` để xem log**
- **Kiểm tra console output khi spawn enemy**
- **Tăng khoảng cách remove enemy** (từ 1200 lên 2000 pixels)
- **Thêm copy constructor và assignment operator** cho Enemy class
- **Animation đã được bật lại** với scale 12% và frame duration 0.1s

### Player animation không hiển thị
- **Kiểm tra số frame trong config** (GunHero: 20 frames, không phải 17)
- **Kiểm tra file `Gun_player_spritesheet.png` có tồn tại không**
- **Bật `ANIMATION_DEBUG_MODE` để xem log**
- **Kiểm tra console output khi setup player**

### Enemy animation không hiển thị
- **Kiểm tra số frame trong config** (Enemy: 70 frames)
- **Kiểm tra file `Enemy_spritesheet.png` có tồn tại không**
- **Kiểm tra scale** (hiện tại: 12% = 0.12f)
- **Bật `ANIMATION_DEBUG_MODE` để xem log**
- **Kiểm tra console output khi spawn enemy**
- **Nếu vẫn bị crash, giảm scale xuống** (ví dụ: 0.08f = 8%)

## 🔧 Config Examples

### High Quality (High Performance Impact)
```cpp
#define PLAYER_ANIMATION_SCALE_X 0.2f        // 20% scale
#define PLAYER_ANIMATION_SCALE_Y 0.2f
#define ANIMATION_SMOOTH_TEXTURE true         // Anti-aliasing
#define ANIMATION_UPDATE_RATE 120.0f          // 120 FPS
```

### Medium Quality (Balanced)
```cpp
#define PLAYER_ANIMATION_SCALE_X 0.20f       // 20% scale (current)
#define PLAYER_ANIMATION_SCALE_Y 0.20f
#define ANIMATION_SMOOTH_TEXTURE true
#define ANIMATION_UPDATE_RATE 60.0f           // 60 FPS
```

### Low Quality (Low Performance Impact)
```cpp
#define PLAYER_ANIMATION_SCALE_X 0.1f        // 10% scale
#define PLAYER_ANIMATION_SCALE_Y 0.1f
#define ENEMY_ANIMATION_SCALE_X 0.08f        // 8% scale
#define ENEMY_ANIMATION_SCALE_Y 0.08f
#define ANIMATION_SMOOTH_TEXTURE false        // No anti-aliasing
#define ANIMATION_UPDATE_RATE 30.0f           // 30 FPS
```
