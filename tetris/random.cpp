#include "raylib.h"
#include "shapes.h"
#include "cell.h"
#include<string.h>
#include <vector>
const int screenWidth = 800;
const int screenHeight = 600;
const int cellSize = 40;
int shapeX = 10;
int shapeY = 0;
float planeX=9;
float planeY=8;
float timer = 0;

long long score=0;

bool planeFlag=false;

const int gridCols = 20;
const int gridRows = 15;
Cell gameGrid[gridRows][gridCols] = { 0 };

Vector2 speakerIconPositions={19*cellSize,cellSize};
bool speakerOnFlag=true;
bool gameOver=false;
// Function to get a random shape pointer
void summonPlane(){
    planeFlag=true;
}
Shape* GetRandomShape() {
    int type = GetRandomValue(5, 6); // Included all 5 shapes
    
    switch (type) {
        case 0: return new LShape();
        case 1: return new IShape();
        case 2: return new TShape();
        case 3:return new SShape();
        case 4: return new ZShape();
        case 5:return new JShape();
        case 6:summonPlane();
        default: return new OShape();
    }
}

void handlePlaneMovements(float &planeX,float &planeY){
    if (IsKeyDown(KEY_DOWN)) {
        if ((int)planeY * cellSize < screenHeight) {
            planeY += 0.02f;
        }
    } 
    if (IsKeyDown(KEY_UP)) { // Changed to 'if' so diagonal movement works
        if (planeY > 0.0f) {
            planeY -= 0.02f;
        }
    } 
    if (IsKeyDown(KEY_LEFT)) {
        if (planeX > 0.0f) {
            planeX -= 0.02f;
        }
    } 
    if (IsKeyDown(KEY_RIGHT)) {
        if ((int)planeX * cellSize < screenWidth) {
            planeX += 0.02f;
        }
    }
}


// Helper to check if a position is "blocked"
bool IsPositionBlocked(int x, int y, const Tetromino& blocks, int gridRows, int gridCols, Cell gameGrid[15][20]) {
    for (int i = 0; i < 4; i++) {
        int newX = x + blocks[i].x;
        int newY = y + blocks[i].y;
        // Wall/Floor checks
        if (newX < 0 || newX >= gridCols || newY >= gridRows) return true;
        
        // Landed blocks check (only if inside grid bounds)
        if (newY >= 0 && gameGrid[newY][newX].exists == 1) return true;
    }
    return false;
}
void ClearFullRows(Cell gameGrid[][20],long long &score) {
    for (int r = 14; r >= 0; r--) {
        bool rowIsFull = true;
        for (int c = 0; c < 20; c++) {
            if (gameGrid[r][c].exists == 0) {
                rowIsFull = false;
                break;
            }
        }
        if (rowIsFull) {
            for (int rowToMove = r; rowToMove > 0; rowToMove--) {
                for (int col = 0; col < 20; col++) {
                    gameGrid[rowToMove][col] = gameGrid[rowToMove - 1][col];
                }
            }
            
            // 2. Clear the very top row (since it was shifted down)
            for (int col = 0; col < 20; col++) {
                gameGrid[0][col] = {0,BLANK};
            }  
            score+=20; 
            r++; 
        }
    }
}
void handleMutingMusic(Vector2 mousePoint,Rectangle hitbox,Music &music){
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    Vector2 mousePoint = GetMousePosition();

    if (CheckCollisionPointRec(mousePoint, hitbox)) {
        speakerOnFlag = !speakerOnFlag; // Toggle the state
        SetMusicVolume(music,speakerOnFlag?0.2f:0.0f);
    }
}
}

int main() {
    
    InitWindow(screenWidth, screenHeight, "Tetris - Stack & Collision");
    
    Shape* currShape = GetRandomShape();
    
    SetTargetFPS(60);
    InitAudioDevice();              // Initialize audio device
    
    Image speakerOn = LoadImage("tetris\\resources\\speakerOn.png");     // Loaded in CPU memory (RAM)
    ImageResize(&speakerOn,40,40);
    Texture2D speakerOntexture = LoadTextureFromImage(speakerOn);          // Image converted to texture, GPU memory (VRAM)
    UnloadImage(speakerOn);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM
    
    Image plane = LoadImage("tetris\\resources\\plane.png");     // Loaded in CPU memory (RAM)
    ImageResize(&plane,80,80);
    Texture2D planetexture = LoadTextureFromImage(plane);          // Image converted to texture, GPU memory (VRAM)
    UnloadImage(plane);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM


    Rectangle hitbox = { speakerIconPositions.x, speakerIconPositions.y, (float)speakerOntexture.width, (float)speakerOntexture.height };
    
    Image speakerOff=LoadImage("tetris\\resources\\speakerOff.png");
    ImageResize(&speakerOff,40,40);
    Texture2D speakerOfftexture = LoadTextureFromImage(speakerOff);          // Image converted to texture, GPU memory (VRAM)
    UnloadImage(speakerOff);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

    Image catLaughing=LoadImage("tetris\\resources\\cat laughing.jpg");
    ImageResize(&catLaughing,500,500);
    Texture2D catLaughingTexture = LoadTextureFromImage(catLaughing);          // Image converted to texture, GPU memory (VRAM)
    UnloadImage(catLaughing);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM


    Music music = LoadMusicStream("tetris\\music\\Original Tetris theme (Tetris Soundtrack).mp3");
    Music endScreenMusic=LoadMusicStream("tetris\\music\\endScreenCatMusicTrimmed.mp3");

    Rectangle source = { 0.0f, 0.0f, (float)catLaughingTexture.width, (float)catLaughingTexture.height };

    Rectangle dest = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f, (float)catLaughingTexture.width, (float)catLaughingTexture.height };
    Vector2 origin = { (float)catLaughingTexture.width/2.0f, (float)catLaughingTexture.height/2.0f };
    SetMusicVolume(!gameOver?music:endScreenMusic,0.2f);
    music.looping=true;
    PlayMusicStream(music);
    while (!WindowShouldClose()) {
        UpdateMusicStream(!gameOver?music:endScreenMusic);
        if(gameOver){
            StopMusicStream(music); 
            if (!IsMusicStreamPlaying(endScreenMusic)) {
                PlayMusicStream(endScreenMusic);
            }
            // --- DRAW GAME OVER SCREEN ---
            BeginDrawing();
            ClearBackground(WHITE);
            DrawTexturePro(catLaughingTexture, source, dest, origin, 0.0f, WHITE);
            // DrawTexture(catLaughingTexture,3*cellSize,2*cellSize,WHITE);    
            EndDrawing();
            continue;
        }

        timer += GetFrameTime();
        // --- INPUT ---
        if (IsKeyPressed(KEY_UP)) {
            // Try rotating
            Tetromino oldState = currShape->blocks;
            currShape->Rotate();
            if (IsPositionBlocked(shapeX, shapeY, currShape->blocks, gridRows, gridCols, gameGrid)) {
                currShape->blocks = oldState;
            }
        }

        if (IsKeyPressed(KEY_LEFT)) {
            if (!IsPositionBlocked(shapeX - 1, shapeY, currShape->blocks, gridRows, gridCols, gameGrid)) shapeX--;
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            if (!IsPositionBlocked(shapeX + 1, shapeY, currShape->blocks, gridRows, gridCols, gameGrid)) shapeX++;
        }

        // --- FALLING LOGIC ---
        float fallSpeed = IsKeyDown(KEY_DOWN) ? 0.05f : 0.5f;

        if (timer >= fallSpeed) {
            // Check if moving down 1 results in a block
            if (IsPositionBlocked(shapeX, shapeY + 1, currShape->blocks, gridRows, gridCols, gameGrid)) {
                // LOCK PIECE
                for (int i = 0; i < 4; i++){
                    int gX = shapeX + currShape->blocks[i].x;
                    int gY = shapeY + currShape->blocks[i].y;
                    if (gX >= 0 && gX < gridCols && gY >= 0 && gY < gridRows)
                        gameGrid[gY][gX].exists = 1;
                        gameGrid[gY][gX].color=currShape->color;
                }
                ClearFullRows(gameGrid,score);
                // SPAWN NEW
                delete currShape;
                currShape = GetRandomShape();
                shapeX = 10;
                shapeY = 0;

                // Game Over Check: If the new shape starts on a blocked spot
                if (IsPositionBlocked(shapeX, shapeY, currShape->blocks, gridRows, gridCols, gameGrid)) {
                    // Reset grid for now as a simple Game Over
                    for(int r=0; r<gridRows; r++) for(int c=0; c<gridCols; c++) gameGrid[r][c].exists = 0;
                    gameOver=true;
                    score=0;
                }
            } else {
                shapeY++;
            }
            timer = 0;
        }

        // --- DRAWING ---
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            // Draw Grid Background
            for (int i = 0; i <= screenWidth; i += cellSize) DrawLine(i, 0, i, screenHeight, LIGHTGRAY);
            for (int i = 0; i <= screenHeight; i += cellSize) DrawLine(0, i, screenWidth, i, LIGHTGRAY);
            
            // Draw Landed Blocks
            for (int r = 0; r < gridRows; r++) {
                for (int c = 0; c < gridCols; c++) {
                    if (gameGrid[r][c].exists == 1) {
                        DrawRectangle(c * cellSize, r * cellSize, cellSize, cellSize, gameGrid[r][c].color);
                        DrawRectangleLines(c * cellSize, r * cellSize, cellSize, cellSize, BLACK);
                    }
                }
            }
            
            // Draw Falling Shape
            if(!planeFlag){
                currShape->Draw(shapeX, shapeY, cellSize);
            }
            DrawText(TextFormat("Score: %08i", score), 5*cellSize, cellSize, 50, RED);
            DrawTexture(speakerOnFlag?speakerOntexture:speakerOfftexture,speakerIconPositions.x,speakerIconPositions.y,WHITE);
            if(planeFlag){
                DrawTexture(planetexture,planeX*cellSize,planeY*cellSize,WHITE);
                handlePlaneMovements(planeX,planeY);
            }
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePoint = GetMousePosition();
                handleMutingMusic(mousePoint,hitbox,music);
            }

        EndDrawing();
    }

    UnloadMusicStream(music);   // Unload music stream buffers from RAM
    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)

    delete currShape;
    CloseWindow();
    return 0;
}