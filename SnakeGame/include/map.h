#pragma once
#include "block.h"
#include "snake.h"
#include <list>
#include <vector>
#include <string>
#include <algorithm>

class Map {
public:
    explicit Map(const std::vector<std::string>& layout); // Stage별 ASCII Map

    bool  inBounds(Coord c) const { return c.y>=0&&c.y<H && c.x>=0&&c.x<W; }

    void  spawnItems();
    void  spawnGateIfNeeded(int maxLen);
    void  decayItems();

    Cell  cellAt(Coord c) const { return grid[c.y][c.x]; }
    void  setCell(Coord c, Cell v) { grid[c.y][c.x]=v; }

    int   H{}, W{};
    std::vector<std::vector<Cell>> grid;

    Snake snake;
    Gate  gate;
    std::list<Item> items;

    int   tickCnt = 0;

private:
    Coord randomEmpty() const;
};
