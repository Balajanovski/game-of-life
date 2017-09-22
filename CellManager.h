#ifndef LIFE_GAME_H
#define LIFE_GAME_H

#include <vector>
#include <set>
#include <memory>

#include "Coords.h"
#include "Drawer.h"


/*
 * Semantics used for cells:
 * NONE is an empty space
 * DECEASED is a cell that has recently died but still counts as living
 * ALIVE is a cell that is alive
 * Regenerated is a cell that was recently created, but still counts as dead
 */

class CellManager {
public:
    enum CellState {
        NONE = 0,
        DECEASED = 1,
        ALIVE = 2,
    };

    // Set cell as alive
    void make_cell(int x, int y);

    // Set cell as none
    void remove_cell(int x, int y);

    // Undertake an iteration of the game
    void apply_rules();

    // For things that need to be done every tick
    void iterate();

    CellManager(int width, int height, std::shared_ptr<Drawer> d);
private:
    // Returns a cell's state
    const CellState check_cell(int x, int y);

    // Returns how many neighbours there are around a cell
    const int get_neighbours(int x, int y);

    // Width and height of field
    int width;
    int height;

    // Vector representing the field
    std::vector< std::vector<CellState> > field;

    // List of alive cells
    std::set<Coords> alive_cells;

    // List of deceased cells
    std::set<Coords> deceased_cells;

    // List of regenerating cells
    std::set<Coords> regen_cells;

    // Util functions to get values, yet also account for wrapping around the board
    const int one_left(int x);
    const int one_right(int x);
    const int one_up(int y);
    const int one_down(int y);

    std::shared_ptr<Drawer> drawer;
};


#endif //LIFE_GAME_H
