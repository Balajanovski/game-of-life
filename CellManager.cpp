#include "CellManager.h"
#include "Coords.h"

#include <cassert>
#include <memory>

const int CellManager::one_left(int x) {
    return (x - 1) < 0 ? width - 1 : x - 1;
}

const int CellManager::one_right(int x) {
    return (x + 1) % width;
}

const int CellManager::one_up(int y) {
    return (y - 1) < 0 ? height - 1 : y - 1;
}

const int CellManager::one_down(int y) {
    return (y + 1) % height;
}

CellManager::CellManager(int field_width, int field_height, std::shared_ptr<Drawer> d) : width(field_width), height(field_height), drawer(d) {
    assert(width >= 0);
    assert(height >= 0);

    field.reserve(width * height);
    for (int y = 0; y < height; ++y) {
        std::vector<CellState> row;
        row.reserve(width);

        for (int x = 0; x < width; ++x) {
            row.push_back(NONE);
        }

        field.push_back(row);
    }
}

void CellManager::iterate() {
    // Send coords of alive cells to drawer to draw
    for (auto& cell : alive_cells) {
        drawer->push(cell);
    }
    for (auto& cell : regen_cells) {
        drawer->push(cell);
    }
}

void CellManager::apply_rules() {

    // Apply the rules of Conway's Game of Life
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            int neighbours = get_neighbours(x, y);
            if (check_cell(x, y) == ALIVE) {

                // If there are less than two neighbours or more than three, kill the cell off
                if (neighbours < 2) {
                    field[y][x] = DECEASED;
                    deceased_cells.emplace(x, y);
                    alive_cells.erase({x, y});
                }
                else if (neighbours > 3) {
                    field[y][x] = DECEASED;
                    deceased_cells.emplace(x, y);
                    alive_cells.erase({x, y});
                }
            }
            else {

                // If there are 3 neighbours, regen the cell
                if (neighbours == 3) {
                    if (check_cell(x, y) == DECEASED) {
                        deceased_cells.erase({x, y});
                    }
                    regen_cells.emplace(x, y);
                }
            }

        }
    }

    // Set all deceased cells as NONE
    for (const auto& dc : deceased_cells) {
        field[dc.y][dc.x] = NONE;
    }
    deceased_cells.clear();

    // Set all regenerated cells as ALIVE
    for (const auto& rc : regen_cells) {
        make_cell(rc.x, rc.y);
    }
    regen_cells.clear();
}

void CellManager::make_cell(int x, int y) {
    assert(x >= 0);
    assert(y >= 0);

    field[y][x] = ALIVE;
    alive_cells.emplace(x, y);
}

void CellManager::remove_cell(int x, int y) {
    assert(x >= 0);
    assert(y >= 0);

    field[y][x] = NONE;
    alive_cells.erase({x, y});
}

const CellManager::CellState CellManager::check_cell(int x, int y) {
    assert(x >= 0);
    assert(y >= 0);

    return field[y][x];
}

const int CellManager::get_neighbours(int x, int y) {
    int living = 0;

    if (field[one_up(y)][one_left(x)]) {
        ++living;
    }
    if (field[one_up(y)][x]) {
        ++living;
    }
    if (field[one_up(y)][one_right(x)]) {
        ++living;
    }
    if (field[y][one_left(x)]) {
        ++living;
    }
    if (field[y][one_right(x)]) {
        ++living;
    }
    if (field[one_down(y)][one_left(x)]) {
        ++living;
    }
    if (field[one_down(y)][x]) {
        ++living;
    }
    if (field[one_down(y)][one_right(x)]) {
        ++living;
    }

    return living;
}