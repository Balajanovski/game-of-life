#ifndef LIFE_DRAWER_H
#define LIFE_DRAWER_H

#include <queue>
#include <deque>
#include "Cleanup.h"
#include "Coords.h"

#include <SDL2/SDL.h>

// Size in pixels of a cell
constexpr static int CELL_SIZE = 15;
constexpr static int LINE_WIDTH = 1;

class Drawer : private SdlHandle, public std::queue<Coords> {
public:
    Drawer(int x, int y);

    void flush();
    void clear();
private:
    int width,
        height;

    SdlWinPtr window;
    SdlRenPtr renderer;
};


#endif //LIFE_DRAWER_H
