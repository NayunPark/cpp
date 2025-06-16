#pragma once
#include "common.h"
#include <deque>
#include <vector>

class Snake {
public:
    explicit Snake(Coord start, int initLen = 3);

    bool  step(Dir nextDir, const std::vector<std::vector<Cell>>& grid);
    void  grow();
    void  shrink();

    bool          collideSelf() const;
    Coord         head()   const { return body.front(); }
    size_t        length() const { return body.size(); }
    Dir           dir()    const { return curDir; }
    void          setDir(Dir d);                 // 역방향 입력 무시
    void          teleportHead(Coord newPos) { body.front() = newPos; }
    void          forceDir(Dir d) { curDir = d; }
    const std::deque<Coord>& segments() const { return body; }

private:
    std::deque<Coord> body;
    Dir               curDir = Dir::None;
};
