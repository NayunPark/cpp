#include "map.h"
#include <random>
#include <string>
#include <algorithm>

namespace {
    std::mt19937 rng{ std::random_device{}() };
    int randInt(int a,int b){
        std::uniform_int_distribution<int> dist(a,b);
        return dist(rng);
    }
}

Map::Map(const std::vector<std::string>& ascii)
: H(static_cast<int>(ascii.size())),
  W(static_cast<int>(ascii[0].size())),
  grid(H, std::vector<Cell>(W, Cell::Empty)),
  snake({H/2, W/2})
{
    for(int y=0;y<H;++y){
        for(int x=0;x<W;++x){
            char ch = ascii[y][x];
            switch(ch){
            case '1': grid[y][x]=Cell::Wall; break;
            case '2': grid[y][x]=Cell::Immune; break;
            default : grid[y][x]=Cell::Empty; break;
            }
        }
    }
}

Coord Map::randomEmpty() const{
    Coord c;
    do {
        c = { randInt(1, H-2), randInt(1, W-2) };   // 맵 경계 안쪽만
        bool onSnake = std::any_of(
            snake.segments().begin(), snake.segments().end(),
            [&](const Coord& s){ return s == c; });
        if (grid[c.y][c.x] == Cell::Empty && !onSnake) break;
    } while (true);
    return c;
}

void Map::spawnItems(){
    while(items.size()<3){
        Coord c = randomEmpty();
        Cell  t = (randInt(0,1)?Cell::Growth:Cell::Poison);
        int   ttl = 5000 / 150; // 5s / tick
        items.emplace_back(c,t,ttl);
        grid[c.y][c.x]=t;
    }
}

void Map::spawnGateIfNeeded(int maxLen){
    if (gate.isActive() || maxLen < 5) return;       // 길이 조건·중복 방지

    std::vector<Coord> walls;

    for (int y = 0; y < H; ++y){
        for (int x = 0; x < W; ++x){
            if (grid[y][x] != Cell::Wall) continue;   // Immune(2) 제외

            int emptyCnt = 0;
            const Coord around[4] = {{-1,0},{1,0},{0,-1},{0,1}};
            for (auto d : around){
                Coord n = { y + d.y, x + d.x };
                if (inBounds(n) && grid[n.y][n.x] == Cell::Empty)
                    ++emptyCnt;
            }
            if (emptyCnt >= 1 && emptyCnt <= 4)                        // 직선 한 면만 뚫린 벽
                walls.push_back({y,x});
        }
    }

    if (walls.size() < 2) return;                     // 후보 부족 → 포기

    Coord a = walls[randInt(0, walls.size() - 1)];
    Coord b;
    do { b = walls[randInt(0, walls.size() - 1)]; } while (b == a);

    gate.activate(a, b);
    grid[a.y][a.x] = grid[b.y][b.x] = Cell::Gate;
}

void Map::decayItems(){
    for(auto it=items.begin(); it!=items.end();){
        if(--(it->ttl)==0){
            grid[it->pos.y][it->pos.x]=Cell::Empty;
            it = items.erase(it);
        }else ++it;
    }
}
