#pragma once
#include "map.h"
#include <chrono>
#include <ncurses.h>
#include <vector>
#include <string>

struct Mission {
    int needLen, needGrow, needPoison, needGate;
    int curGrow=0, curPoison=0, curGate=0;
    bool snakeLenOK=false;

    bool passed() const {
        return snakeLenOK &&
               curGrow   >= needGrow   &&
               curPoison >= needPoison &&
               curGate   >= needGate;
    }
};

struct Stage {
    std::vector<std::string> layout; // ASCII 맵
    Mission                  mission;
};

class Game {
public:
    Game();
    ~Game();

    void run();  // Stage loop

private:

    int offsetY = 0, offsetX = 0; // ScoreBoard 위치 조정
    
    bool handleInput();
    void updateLogic();
    void render() const;
    void nextStage();

    /// Gate 출구 방향 계산 (규칙 #4)
    Dir  gateExitDir(Coord exit, Dir inDir) const;

    /// 실패 조건 체크
    bool checkFailure();

    // -------- data --------
    std::vector<Stage> stages;
    size_t curStageIdx = 0;

    Map  map;
    Mission mission;

    bool running = true;
    ms   tick    = ms(150);

    size_t maxLen = 0;
    std::chrono::steady_clock::time_point startTime;

    bool teleportedThisTick = false;
    WINDOW* scoreWin = nullptr;
};
