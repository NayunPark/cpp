#include "game.h"
#include <thread>
#include <ncurses.h>
#include <iomanip>

Game::Game()
: stages{
    /* ---------- Stage 1 ---------- */
    {{
      "2111111111111111111111111111111112",
      "1000000000000000000000000000000001",
      "1000000000000000000000000000000001",
      "1000001000000000000000000000000001",
      "1000001000000000000000000000000001",
      "1000001000000000000000000000000001",
      "1000001000000000000000000000000001",
      "1000001111000000000000000000000001",
      "1000000000000000000000000000000001",
      "1000000000000000000000000000000001",
      "2111100000000000000000111111111112",
      "1000000000000000000000000000000001",
      "1000000000000000000000000000000001",
      "1000000000000000000000000011000001",
      "1000000000000000000000000010000001",
      "1000000000000000000000000010000001",
      "1000000000000000000000100010000001",
      "1000000000000000000000111110000001",
      "1000000000000000000000000000000001",
      "1000000000000000000000000000000001",
      "2111111111111111111111111111111112"
    }, {8,5,1,2}},

    /* ---------- Stage 2 ---------- */
    {{
      "2111111111111111111111111111111112",
      "1000000000000000000000000000000001",
      "1000000000000000000000000000000001",
      "1000001000000000000000000000000001",
      "1000001000000000000000000000000001",
      "1000001000000000000000000000000001",
      "1000001000000000000000000000000001",
      "1000001111000000000000000000000001",
      "1000000000000000000000000000000001",
      "1000000000000000000000000000000001",
      "2111100000000000000000111111111112",
      "1000000000000000000000000000000001",
      "1000000000000000000000000000000001",
      "1000000000000000000000000011000001",
      "1000000000000000000000000010000001",
      "1000000000000000000000000010000001",
      "1000000000000000000000100010000001",
      "1000000000000000000000111110000001",
      "1000000000000000000000000000000001",
      "1000000000000000000000000000000001",
      "2111111111111111111111111111111112"
    }, {8,5,1,2}},

    /* ---------- Stage 3 ---------- */
    {{
      "2111111111111111111111111111111112",
      "1000000000000000000000000000000001",
      "1000000000000000000000000000000001",
      "1000000000000000000000000111000001",
      "1000000000000000000000000001110001",
      "1000000000000000000000000000000001",
      "1000000000000000000000000000000001",
      "1000000000000000000000000000000001",
      "1000000011110000000000000000000001",
      "1000000000010000000000000000000001",
      "1000000000011100000000000000000001",
      "1000000000000000000000000000000001",
      "1000000000000000000000000000000001",
      "1000000000000000000000000000000001",
      "1000000000000000000000000111110001",
      "1000111100000000000000001100010001",
      "1000000000000000000000000000010001",
      "1000000000000000000000000000000001",
      "1000000000000000000000000000000001",
      "1000000000000000000000000000000001",
      "2111111111111111111111111111111112"
    }, {8,5,1,2}},
    /* ---------- Stage 4 ---------- */
    {{
      "2111111111111111111111111111111112",
      "1000000000000000000000000000010001",
      "1000000000000000000000000000010001",
      "1000001000000000000000000000000001",
      "1000001111100000000000000000000001",
      "1000001000000000000000000011111112",
      "1000001000000000000000000000000001",
      "1000000000000000000000000000000001",
      "1000000000000000000000000000000001",
      "1000010000000000000000000000000001",
      "2111110000000000000000000000001112",
      "1000010000000000000000000000000001",
      "1000011100000000000000000000000001",
      "1000000000000000000000000000000001",
      "1000000000000000000000000111110001",
      "1000000000000000000000000000000001",
      "1000000000001111000000000000000001",
      "1000000001111000000000000000000001",
      "1000000000100000000001110000000001",
      "1000000000100000000000000000000001",
      "2111111111111111111111111111111112"
    }, {8,5,1,2}}
},
  map(stages[0].layout),
  mission(stages[0].mission)
{
    /* ── ncurses ────────────────── */
    initscr(); noecho(); curs_set(0);
    keypad(stdscr, TRUE);
    if(!has_colors()){ endwin(); throw std::runtime_error("no color"); }
    start_color();

    /* 색상: Cell값+1 = pair 번호 */
    init_pair(1, COLOR_BLACK, COLOR_BLACK);     // Empty
    init_pair(2, COLOR_BLACK, COLOR_WHITE);     // Wall / Immune
    init_pair(3, COLOR_BLACK, COLOR_BLUE);      // Gate
    init_pair(4, COLOR_BLACK, COLOR_GREEN);     // SnakeHead
    init_pair(5, COLOR_BLACK, COLOR_GREEN);     // SnakeBody
    init_pair(6, COLOR_BLACK, COLOR_CYAN);      // Growth
    init_pair(7, COLOR_BLACK, COLOR_RED);       // Poison
    init_pair(8, COLOR_BLACK, COLOR_YELLOW);    // (예비)

    /* ── 위치 보정 ───────────────── */
    int termH, termW; getmaxyx(stdscr, termH, termW);
    offsetY = std::max(0, (termH - map.H) / 2);
    offsetX = std::max(0, (termW - map.W) / 2);

    scoreWin = newwin(map.H, 26, offsetY, offsetX + map.W + 2);
    box(scoreWin, 0, 0);

    startTime = std::chrono::steady_clock::now();
}

Game::~Game(){
    delwin(scoreWin);
    endwin();
}

bool Game::handleInput(){
    int ch = getch();
    Dir inputDir = Dir::None;
    switch(ch){
    case KEY_UP:    inputDir = (Dir::Up);    break;
    case KEY_DOWN:  inputDir = (Dir::Down);  break;
    case KEY_LEFT:  inputDir = (Dir::Left);  break;
    case KEY_RIGHT: inputDir = (Dir::Right); break;
    case 'q': running=false; return false;
    }
    
    Dir curDir = map.snake.dir();

    if (curDir != Dir::None
        && static_cast<int>(inputDir) + static_cast<int>(curDir) == 0) {
        running = false;
        return false;
    }

    map.snake.setDir(inputDir);
    return running;
}

bool Game::checkFailure(){
    if(map.snake.length()<3)      return true;
    if(!teleportedThisTick && map.snake.collideSelf())   return true;
    return false;
}

Dir Game::gateExitDir(Coord exit, Dir inDir) const{
    if(exit.y==0)          return Dir::Down;
    if(exit.y==map.H-1)    return Dir::Up;
    if(exit.x==0)          return Dir::Right;
    if(exit.x==map.W-1)    return Dir::Left;

    const Dir cand[4] = {inDir,
                         (inDir==Dir::Up?Dir::Right: inDir==Dir::Right?Dir::Down:
                          inDir==Dir::Down?Dir::Left:Dir::Up),
                         (inDir==Dir::Up?Dir::Left: inDir==Dir::Left?Dir::Down:
                          inDir==Dir::Down?Dir::Right:Dir::Up),
                         static_cast<Dir>(-static_cast<int>(inDir))};
    for(Dir d:cand){
        Coord n = exit + dirToOffset(d);
        if(map.inBounds(n) && map.cellAt(n)!=Cell::Wall && map.cellAt(n)!=Cell::Immune)
            return d;
    }
    return inDir;
}

void Game::updateLogic(){
    teleportedThisTick = false;
    map.tickCnt++;
    map.spawnItems();
    map.decayItems();                     //  ← 추가
    map.spawnGateIfNeeded(static_cast<int>(maxLen));

    if (map.snake.dir() == Dir::None) return;

    if(!map.snake.step(map.snake.dir(), map.grid)){ running=false; return; }

    Coord h = map.snake.head();
    Cell cell = map.cellAt(h);

    if(cell==Cell::Growth || cell==Cell::Poison){
        for(auto it=map.items.begin(); it!=map.items.end(); ++it)
            if(it->pos==h){ map.items.erase(it); break; }
        map.setCell(h,Cell::Empty);
        if(cell==Cell::Growth){ map.snake.grow();   mission.curGrow++;   }
        else                  { map.snake.shrink(); mission.curPoison++; }
    }
    else if(cell==Cell::Gate){
        Coord out = map.gate.otherSide(h);
        map.snake.teleportHead(out);
        Dir newDir = gateExitDir(out, map.snake.dir());
        map.snake.forceDir(newDir);
        mission.curGate++;

        teleportedThisTick = true;
    }

    if(map.snake.length()>maxLen) maxLen = map.snake.length();
    if(map.snake.length()>=mission.needLen) mission.snakeLenOK=true;

    if(mission.passed()) nextStage();
    if(checkFailure())   running=false;
}

void Game::render() const{
    /* ── 맵 ─────────────────────── */
    for(int y=0;y<map.H;++y){
        for(int x=0;x<map.W;++x){
            Cell c = map.grid[y][x];
            attron(COLOR_PAIR(static_cast<int>(c)+1));
            mvaddch(offsetY+y, offsetX+x, ' ');
            attroff(COLOR_PAIR(static_cast<int>(c)+1));
        }
    }
    /* ── 스네이크 ───────────────── */
    bool first=true;
    for(const Coord& p: map.snake.segments()){
        Cell c = first?Cell::SnakeHead:Cell::SnakeBody; first=false;
        attron(COLOR_PAIR(static_cast<int>(c)+1));
        mvaddch(offsetY+p.y, offsetX+p.x, ' ');
        attroff(COLOR_PAIR(static_cast<int>(c)+1));
    }

    /* ── 스코어보드 ─────────────── */
    werase(scoreWin); box(scoreWin,0,0);
    int row=1;
    mvwprintw(scoreWin,row++,2,"[Score]"); row++;
    mvwprintw(scoreWin,row++,2,"B : %3zu / %3zu", map.snake.length(), maxLen);
    mvwprintw(scoreWin,row++,2,"+ : %3d", mission.curGrow);
    mvwprintw(scoreWin,row++,2,"- : %3d", mission.curPoison);
    mvwprintw(scoreWin,row++,2,"G : %3d", mission.curGate); row+=1;

    mvwprintw(scoreWin,row++,2,"[Mission]"); row++;
    auto check = [](bool ok){ return ok?'v':' '; };
    mvwprintw(scoreWin,row++,2,"B : %-3d (%c)", mission.needLen,  check(mission.snakeLenOK));
    mvwprintw(scoreWin,row++,2,"+ : %-3d (%c)", mission.needGrow, check(mission.curGrow>=mission.needGrow));
    mvwprintw(scoreWin,row++,2,"- : %-3d (%c)", mission.needPoison,check(mission.curPoison>=mission.needPoison));
    mvwprintw(scoreWin,row++,2,"G : %-3d (%c)", mission.needGate,  check(mission.curGate>=mission.needGate)); row+=1;

    auto sec = std::chrono::duration_cast<std::chrono::seconds>(
                 std::chrono::steady_clock::now()-startTime).count();
    mvwprintw(scoreWin,row++,2,"Time: %ld s", sec);

    wnoutrefresh(scoreWin);
    doupdate();
}

void Game::nextStage(){
    if(++curStageIdx >= stages.size()){ running=false; return; }

    Stage& s = stages[curStageIdx];
    map     = Map(s.layout);
    mission = s.mission;
    maxLen  = map.snake.length();

    int termH, termW; getmaxyx(stdscr, termH, termW);
    offsetY = std::max(0, (termH - map.H) / 2);
    offsetX = std::max(0, (termW - map.W) / 2);

    mvwin(scoreWin, offsetY, offsetX + map.W + 2);
    wresize(scoreWin, map.H, 26);

    startTime = std::chrono::steady_clock::now();
}

void Game::run(){
    nodelay(stdscr,TRUE);
    while(running){
        auto start = std::chrono::steady_clock::now();
        handleInput();
        updateLogic();
        render();
        std::this_thread::sleep_until(start + tick);
    }

    nodelay(stdscr,FALSE);
    mvprintw(offsetY + map.H + 2, offsetX, "Game Over! Press any key to exit.");
    refresh();
    getch();
}