#ifndef LIFE_COORDS_H
#define LIFE_COORDS_H

struct Coords {
    int x;
    int y;

    Coords(int nx, int ny) : x(nx), y(ny) { }
};

bool operator<(const Coords& a, const Coords& b);

#endif //LIFE_COORDS_H
