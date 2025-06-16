#include "block.h"

// 글리프·색상 테이블 (PAIR idx = value+1)
static const chtype GLYPH[]   = { ' ', '#', 'X', 'O', '@', 'o', '+', '-' };
static const short  COLOR_ID[] = { 1,2,2,3,4,4,5,6 };

chtype Block::glyph()   const { return GLYPH[static_cast<int>(type)]; }
short  Block::colorId() const { return COLOR_ID[static_cast<int>(type)]; }

Wall::Wall(Coord p,bool im)
: Block(p, im?Cell::Immune:Cell::Wall), immuneWall(im) {}

Gate::Gate() : Block({0,0},Cell::Gate) {}

void Gate::activate(Coord a, Coord b){
    endPos[0]=a; endPos[1]=b; pos=a; active=true;
}
void Gate::deactivate(){ active=false; }

Coord Gate::otherSide(const Coord& c) const {
    return (c == endPos[0] ? endPos[1] : endPos[0]);
}

Item::Item(Coord p,Cell t,int ttlTicks)
: Block(p,t), ttl(ttlTicks) {}
