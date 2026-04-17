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

class OShape : public Shape { 
public:
    OShape() { color = YELLOW; blocks = {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}}; }
    void Rotate() override {
        return; 
    }

};
class SShape : public Shape {
public:
    SShape() { color = GREEN; blocks = {{{1, 0}, {2, 0}, {0, 1}, {1, 1}}}; }
    
    void Rotate() override {
        // Rotate around the block at index 3 (1, 1)
        int px = blocks[3].x;
        int py = blocks[3].y;
        for (int i = 0; i < 4; i++) {
            int oldX = blocks[i].x - px;
            int oldY = blocks[i].y - py;
            blocks[i].x = -oldY + px;
            blocks[i].y = oldX + py;
        }
    }
};

class ZShape : public Shape { 
public:
    ZShape() { color = RED; blocks = {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}}; }
    
    void Rotate() override {
        // Rotate around the block at index 2 (1, 1)
        int px = blocks[2].x;
        int py = blocks[2].y;
        for (int i = 0; i < 4; i++) {
            int oldX = blocks[i].x - px;
            int oldY = blocks[i].y - py;
            blocks[i].x = -oldY + px;
            blocks[i].y = oldX + py;
        }
    }
};

class LShape : public Shape {
public:
    LShape() { color = ORANGE; blocks = {{{0, -1}, {0, 0}, {0, 1}, {1, 1}}}; }
};

class JShape: public Shape{
public:
    // Adjusted J-Shape coordinates to rotate better around 0,0
    JShape(){ color = BLUE; blocks = {{{0, -1}, {0, 0}, {0, 1}, {-1, 1}}}; }
};

class IShape : public Shape {
public:
    IShape() { color =CYAN ; blocks = {{{0, -1}, {0, 0}, {0, 1}, {0, 2}}}; }
};

class TShape : public Shape {
public:
    TShape() { color = PURPLE; blocks = {{{0, 0}, {-1, 0}, {1, 0}, {0, 1}}}; }
};

#endif