#include "raylib.h"
#include "raymath.h"
#include "AnimEngine.h"
#include "Character.h"
#include <stdlib.h>
#include <string.h>
#include <vector> 

int main()
{
    // Window dimensions
    int windowDimensions[2]{384, 384};

    // Window initialization
    InitWindow(windowDimensions[0], windowDimensions[1], "Top Down");
    SetTargetFPS(60);

    // Constants
    float speed{4.0};

    // Textures
    Texture2D map = LoadTexture("nature_tileset/WorldMap.png");
    Vector2 mapPos{-384 / 2, -384 / 2};

    // Sprite sheets
    Texture2D knightIdle = LoadTexture("characters/knight_idle_spritesheet.png");
    Texture2D knightRun = LoadTexture("characters/knight_run_spritesheet.png");
    Texture2D knightAttack = LoadTexture("characters/weapon_sword-Sheet-sheet.png"); 

    std::vector<Animation> animations = {
        {6, 1.0f / 12.0f, &knightIdle},
        {6, 1.0f / 12.0f, &knightRun}
    };

    Animation attackAnimation = {2, 1.0f / 12.0f, &knightAttack}; 

    // Create character
    Character knight(animations, attackAnimation, 
                     {windowDimensions[0] / 2.0f - knightIdle.width / 12.0f, (float)(windowDimensions[1] - knightIdle.height)}, 
                     speed);
    
    // Camera
    Camera2D camera = { 0 };
    camera.target = knight.animation.GetPos(); // Set target to knight's position
    camera.offset = (Vector2){windowDimensions[0] / 2.0f - 45, windowDimensions[1] / 2.0f - 45}; // Center of the screen
    camera.rotation = 0.0f;
    camera.zoom = 1.0f; // Adjust zoom if needed

    camera.target = knight.animation.GetPos(); // This ensures the camera follows the knight



    while (!WindowShouldClose())
    {
        const float dT = GetFrameTime();
        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(WHITE);

        Vector2 direction{};
        bool attack = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        if (IsKeyDown(KEY_A)) direction.x -= 1.0;
        if (IsKeyDown(KEY_D)) direction.x += 1.0;
        if (IsKeyDown(KEY_W)) direction.y -= 1.0;
        if (IsKeyDown(KEY_S)) direction.y += 1.0;

        DrawTextureEx(map, mapPos, 0, 4.0, WHITE);

        knight.Update(dT, direction, attack);
        knight.Draw();

        if (Vector2Length(direction) != 0.0) {
            mapPos = Vector2Subtract(mapPos, Vector2Scale(Vector2Normalize(direction), speed));
        }

        EndDrawing();
    }
    // Freeing memory
    UnloadTexture(map);
    UnloadTexture(knightIdle);
    UnloadTexture(knightRun);
    UnloadTexture(knightAttack);

    CloseWindow();
    return 0;
}