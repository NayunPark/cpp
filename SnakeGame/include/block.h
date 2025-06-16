#pragma once
#include "common.h"
#include <ncurses.h>

class Block {
public:
    Block(Coord p, Cell t): pos(p), type(t) {}
    virtual ~Block() = default;

    Coord pos;  Cell type;
    virtual chtype glyph()   const;
    virtual short  colorId() const;
};

class Wall : public Block {
public:
    Wall(Coord p, bool immune=false);
    bool immuneWall;
};

class Gate : public Block {
public:
    Gate();
    void activate(Coord a, Coord b);
    void deactivate();
    bool isActive() const { return active; }

    /// 게이트 통과 시 반대쪽 좌표 돌려줌
    Coord otherSide(const Coord& cur) const;

private:
    Coord endPos[2];
    bool  active=false;
};

class Item : public Block {
public:
    Item(Coord p, Cell t, int ttlTicks);
    int ttl;          // tick 단위
};
