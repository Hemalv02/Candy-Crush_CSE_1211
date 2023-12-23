#ifndef ENGINE_H
#define ENGINE_H

#include "Timer.h"
#include "Texture.h"
#include "Text.h"
#include "Sound.h"
#include <vector>
using std::vector;

enum Candys { Destroyed, Red, Green, Blue, Orange, Yellow, Purple, White, Total };
enum GameModes { Time, Zen, Endless, Total_Mode };
enum TimeModes { OneMinute, TwoMinutes, FiveMinutes, Total_Time };
enum SelectionChange { ContinueSelection, NewGameSelection, TimeSelection, Total_Selection };

class Engine {
private:
    const int WINDOW_WIDTH, WINDOW_HEIGHT;
    const std::string TITLE;
    SDL_Cursor *cursor;
    bool success;

    bool initSDL2();
    bool textureInit();
    bool fontInit();
    bool soundInit();

    void initSave();
    bool save();
    void exit();

public:
    Engine();
    ~Engine();

    Sint32 savedHighscore[Total_Mode][Total_Time];
    Sint32 savedScore;
    Uint32 savedTime;
    vector<vector<int>> savedBoard;

    Timer timer;
    Texture boardTexture;
    Texture candyTexture[Total];
    Texture selectorTexture;
    Texture hintTexture;
    Texture scoreTexture;
    Texture highlightTexture;
    Texture exitTexture;
    Texture startTexture;
    Texture endTexture;
    Texture buttonTexture;

    Text continueText;
    Text newGameText;
    Text gameModeText;
    Text timeModeText;
    Text scores;
    Text highscores;
    Text times;
    Text scoreText;
    Text highscoreText;
    Text timeText;
    Text startNotice;
    Text soundText;
    Text exitText;

    Sound music;
    Sound startSFX;
    Sound endSFX;
    Sound matchSFX[3];
    bool musicState = true;

    int getRandom();
    void render();
};

#endif
