# 🎵 Soul Knight Clone - Music & Audio Guide

## 🎯 Audio Features Added

### ✅ **Background Music System**
- **Auto-loop** during gameplay
- **Volume control** (30% default)
- **Multiple format support**: .ogg, .wav, .mp3
- **Graceful fallback** if no music found

### ✅ **Game Time Tracking**
- **Real-time display** during gameplay
- **MM:SS format** (minutes:seconds)
- **Saved to leaderboard** with score

### ✅ **Leaderboard System**
- **Top 10 scores** with play time
- **Automatic sorting** by score
- **Persistent storage** in leaderboard.txt
- **Ranked display** with colors

## 🎵 Adding Background Music

### **Quick Setup:**
1. **Download music** from:
   - https://freesound.org (free sounds)
   - https://opengameart.org (game music)
   - Use your own music files

2. **Rename file** to one of:
   - `assets/background.ogg` (recommended)
   - `assets/background.wav`
   - `assets/background.mp3`

3. **Play game** - music will auto-start!

### **Supported Formats:**
- **OGG Vorbis** (.ogg) - Best compression, recommended
- **WAV** (.wav) - Uncompressed, larger files
- **MP3** (.mp3) - Good compression, widely supported

### **Music Behavior:**
- **Starts** when entering game (after character select)
- **Loops** continuously during gameplay
- **Stops** when game ends or returning to menu
- **Volume** set to 30% for comfortable listening

## 🕐 Time Tracking

### **Display Location:**
- **In-game UI**: Top-left corner, cyan color
- **Format**: "Time: MM:SS" (e.g., "Time: 2:35")
- **Updates**: Real-time every frame

### **Leaderboard Integration:**
- **Saved** with final score when game ends
- **Displayed** in leaderboard with score
- **Sorted** by score (highest first)

## 🏆 Leaderboard System

### **Features:**
- **Top 10 scores** displayed
- **Score + Time** for each entry
- **Rank colors**:
  - 🥇 **1st place**: Gold (Yellow)
  - 🥈 **2nd place**: Silver (White)
  - 🥉 **3rd place**: Bronze (Orange)
  - **4th-10th**: Gray

### **File Format (leaderboard.txt):**
```
1500 125.5
1200 98.2
1000 156.7
```
Format: `score time_in_seconds`

### **Access:**
- **Main Menu** → "Leaderboard" option
- **ESC** to return to main menu

## 🎮 Enhanced UI

### **In-Game Display:**
```
Score: 1250        (Top-left, white)
HP: 85/100         (Below score, red)
Level: 3           (Below HP, yellow)
Time: 2:15         (Below level, cyan)
```

### **Game Over:**
- **Auto-save** score and time
- **Return** to main menu
- **View results** in leaderboard

## 🔧 Technical Implementation

### **Music System:**
```cpp
sf::Music backgroundMusic;
backgroundMusic.openFromFile("assets/background.ogg");
backgroundMusic.setLoop(true);
backgroundMusic.setVolume(30.f);
backgroundMusic.play();
```

### **Time Tracking:**
```cpp
sf::Clock gameTimer;
float gameTimeSeconds = gameTimer.getElapsedTime().asSeconds();

// Format for display
int minutes = static_cast<int>(gameTimeSeconds) / 60;
int seconds = static_cast<int>(gameTimeSeconds) % 60;
```

### **Leaderboard Storage:**
```cpp
std::vector<std::pair<int, float>> leaderboard; // score, time
std::sort(leaderboard.begin(), leaderboard.end(), 
         [](const auto& a, const auto& b) { return a.first > b.first; });
```

## 🎯 Usage Examples

### **Playing with Music:**
1. Add music file to `assets/background.ogg`
2. Run `./game.exe`
3. Select character → Music starts automatically
4. Enjoy gameplay with background music!

### **Checking Leaderboard:**
1. Main Menu → "Leaderboard"
2. View top 10 scores with times
3. ESC to return to menu

### **Setting Records:**
1. Play game and survive as long as possible
2. Kill enemies to increase score
3. When you die → score + time saved automatically
4. Check leaderboard to see your ranking!

## 🎵 Music Recommendations

### **Game Music Styles:**
- **Chiptune/8-bit** - Classic arcade feel
- **Electronic/Synthwave** - Modern action vibe
- **Orchestral** - Epic adventure atmosphere
- **Ambient** - Calm background music

### **Free Music Sources:**
- **Freesound.org** - Creative Commons music
- **OpenGameArt.org** - Game-specific music
- **YouTube Audio Library** - Royalty-free tracks
- **Incompetech.com** - Kevin MacLeod compositions

### **Volume Guidelines:**
- **30% volume** (current setting) - Good for background
- **Adjust in code** if needed: `backgroundMusic.setVolume(50.f)`
- **Consider** adding in-game volume controls

## 🚀 Future Enhancements

### **Sound Effects:**
- Attack sounds
- Enemy death sounds
- Level up sounds
- Menu navigation sounds

### **Music Variations:**
- Different music for menu vs gameplay
- Boss battle music
- Victory/defeat music

### **Audio Controls:**
- Volume sliders in settings menu
- Mute/unmute toggle
- Music/SFX separate controls

---

**Game now has complete audio and scoring system! 🎉🎵**
