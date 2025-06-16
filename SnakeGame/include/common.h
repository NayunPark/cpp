#pragma once
#include <cstdint>
#include <chrono>

// 셀 종류 (과제 규칙 일치)
enum class Cell : int8_t {
    Empty = 0, Wall = 1, Immune = 2, Gate = 3,
    SnakeHead = 4, SnakeBody = 5,
    Growth = 6, Poison = 7
};

struct Coord { int y, x; };
inline bool operator==(const Coord& a,const Coord& b){return a.y==b.y && a.x==b.x;}
inline Coord operator+(Coord a, Coord b){return {a.y+b.y,a.x+b.x};}

// 방향 : 반대값끼리 더하면 0
enum class Dir : int8_t { Up=-1, Down=1, Left=-2, Right=2, None=0 };

static constexpr Coord DIR_OFFSET[] = {{-1,0},{1,0},{0,-1},{0,1}};
inline Coord dirToOffset(Dir d){
    switch(d){
    case Dir::Up:    return DIR_OFFSET[0];
    case Dir::Down:  return DIR_OFFSET[1];
    case Dir::Left:  return DIR_OFFSET[2];
    case Dir::Right: return DIR_OFFSET[3];
    default: return {0,0};
    }
}
using ms = std::chrono::milliseconds;
