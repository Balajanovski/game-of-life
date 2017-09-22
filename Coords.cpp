
#include "Coords.h"

bool operator<(const Coords& a, const Coords& b) {
    return b.x < a.x || (b.x == a.x && b.y < a.y);
}