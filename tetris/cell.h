#ifndef CELL_H
#define CELL_H
#include "raylib.h"

struct Cell {
    int exists;   // 0 for empty, 1 for occupied
    Color color;  // The color of the block that landed here
};

#endif