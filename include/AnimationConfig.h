#ifndef ANIMATION_CONFIG_H
#define ANIMATION_CONFIG_H

// ============================================================================
// ANIMATION SYSTEM CONFIGURATION
// ============================================================================
// Các thông số này có thể được tùy chỉnh để phù hợp với game

// Player Animation Settings
#define PLAYER_ANIMATION_SCALE_X 0.20f        // Scale X cho player texture (0.20 = 20% kích thước gốc) - tăng để dễ nhìn hơn
#define PLAYER_ANIMATION_SCALE_Y 0.20f        // Scale Y cho player texture (0.20 = 20% kích thước gốc) - tăng để dễ nhìn hơn
#define PLAYER_FRAME_WIDTH 322                // Kích thước frame gốc từ spritesheet
#define PLAYER_FRAME_HEIGHT 217               // Kích thước frame gốc từ spritesheet
#define PLAYER_FRAME_COUNT 20                 // Số frame trong spritesheet (đã sửa từ 17 thành 20)
#define PLAYER_FRAME_DURATION 0.1f            // Thời gian mỗi frame (giây)

// Enemy Animation Settings
#define ENEMY_ANIMATION_SCALE_X 0.12f         // Scale X cho enemy texture (0.12 = 12% kích thước gốc) - giảm để tránh crash
#define ENEMY_ANIMATION_SCALE_Y 0.12f         // Scale Y cho enemy texture (0.12 = 12% kích thước gốc) - giảm để tránh crash
#define ENEMY_FRAME_WIDTH 190                 // Kích thước frame gốc từ spritesheet
#define ENEMY_FRAME_HEIGHT 175                // Kích thước frame gốc từ spritesheet
#define ENEMY_FRAME_COUNT 70                  // Số frame trong spritesheet
#define ENEMY_FRAME_DURATION 0.1f             // Thời gian mỗi frame (giây) - tăng để ổn định hơn

// Animation Performance Settings
#define ANIMATION_UPDATE_RATE 60.0f           // Số lần update animation mỗi giây (FPS)
#define ANIMATION_ENABLE_LOOP true            // Bật/tắt loop animation mặc định

// Texture Quality Settings
#define ANIMATION_SMOOTH_TEXTURE false        // Bật/tắt smooth texture (anti-aliasing) - TẮT để tránh crash
#define ANIMATION_REPEATED_TEXTURE false      // Bật/tắt repeated texture

// Debug Settings
#define ANIMATION_DEBUG_MODE true             // Bật/tắt debug mode - BẬT để debug
#define ANIMATION_SHOW_FRAME_INFO true        // Hiển thị thông tin frame - BẬT để debug

#endif // ANIMATION_CONFIG_H
