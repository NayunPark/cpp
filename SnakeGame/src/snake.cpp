#include "snake.h"

Snake::Snake(Coord start,int initLen){
    for(int i=0;i<initLen;++i) body.push_back({start.y, start.x+i});
    curDir = Dir::None;                // 시작 시 정지
}

void Snake::setDir(Dir d){
    if(d==Dir::None) return;
    if(static_cast<int>(d)+static_cast<int>(curDir)==0) return; // 역방향 입력 무시
    curDir = d;
}

bool Snake::step(Dir next, const std::vector<std::vector<Cell>>& g){
    if(static_cast<int>(next)+static_cast<int>(curDir)==0) next = curDir;
    if(next != Dir::None) curDir = next;

    Coord nh = body.front() + dirToOffset(curDir);
    if(nh.y<0||nh.y>=static_cast<int>(g.size())||
       nh.x<0||nh.x>=static_cast<int>(g[0].size())) return false;

    Cell target = g[nh.y][nh.x];
    if(target==Cell::Wall || target==Cell::Immune || target==Cell::SnakeBody)
        return false;

    body.push_front(nh);
    body.pop_back();
    return true;
}

void Snake::grow()  { body.push_back(body.back()); }
void Snake::shrink(){ body.pop_back(); }

bool Snake::collideSelf() const{
    Coord h = body.front();
    for(size_t i=1;i<body.size();++i)
        if(body[i]==h) return true;
    return false;
}