#ifndef SHAPES_H
#define SHAPES_H
#include "raylib.h"
#include <array>

struct Block { int x; int y; };
typedef std::array<Block, 4> Tetromino;

class Shape {
public:
    Tetromino blocks;
    Color color;

    virtual void Rotate() {
        for (int i = 0; i < 4; i++) {
            int oldX = blocks[i].x;
            int oldY = blocks[i].y;
            blocks[i].x = -oldY;
            blocks[i].y = oldX;
        }
    }

    void Draw(int gridX, int gridY, int cellSize) {
        for (int i = 0; i < 4; i++) {
            DrawRectangle((gridX + blocks[i].x) * cellSize, 
                          (gridY + blocks[i].y) * cellSize, 
                          cellSize, cellSize, color);
            DrawRectangleLines((gridX + blocks[i].x) * cellSize, 
                               (gridY + blocks[i].y) * cellSize, 
                               cellSize, cellSize, BLACK);
        }
    }
};

class LShape : public Shape {
public:
    LShape() { color = ORANGE; blocks = {{{0, -1}, {0, 0}, {0, 1}, {1, 1}}}; }
};

class IShape : public Shape {
public:
    IShape() { color = SKYBLUE; blocks = {{{0, -1}, {0, 0}, {0, 1}, {0, 2}}}; }
};

class TShape : public Shape {
public:
    TShape() { color = PURPLE; blocks = {{{0, 0}, {-1, 0}, {1, 0}, {0, 1}}}; }
};

class OShape : public Shape { // Square doesn't rotate well with the math, so we'd override it
public:
    OShape() { color = YELLOW; blocks = {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}}; }
    void Rotate() override{
        return;
    }
};
#endif