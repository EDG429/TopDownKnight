#include "raylib.h"
#include "raymath.h"
#include "AnimEngine.h"
#include <stdlib.h>
#include <string.h>

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

    Texture2D knightIdle = LoadTexture("characters/knight_idle_spritesheet.png");
    Texture2D knightRun = LoadTexture("characters/knight_run_spritesheet.png");
    Vector2 knightPos{
        384 / 2.0f - 4.0f * (0.5f * (float)knightIdle.width / 6.0f),
        384 / 2.0f - 4.0f * (0.5f * (float)knightIdle.height)};
    float rightLeft{1.f}; // 1: facing right, -1 facing left

    // Creating the animation engine for the knight
    AnimEngine knightAnimation(
        {0.0f, 0.0f, knightIdle.width / 6.0f, (float)knightIdle.height},
        {windowDimensions[0] / 2.0f - knightIdle.width / 12.0f, (float)(windowDimensions[1] - knightIdle.height)},
        1.0f / 12.0f,
        &knightIdle, &knightRun
    );

    while (!WindowShouldClose())
    {
        const float dT = GetFrameTime();
        BeginDrawing();
        ClearBackground(WHITE);

        Vector2 direction{};

        if (IsKeyDown(KEY_A))
            direction.x -= 1.0;
        if (IsKeyDown(KEY_D))
            direction.x += 1.0;
        if (IsKeyDown(KEY_W))
            direction.y -= 1.0;
        if (IsKeyDown(KEY_S))
            direction.y += 1.0;

        DrawTextureEx(map, mapPos, 0, 4.0, WHITE);

        if (direction.x != 0.0)
            rightLeft = (direction.x < 0.f) ? -1.f : 1.f;

        knightAnimation.Update(dT, Vector2Length(direction) != 0.0);
        knightAnimation.Draw(rightLeft, knightPos);

        if (Vector2Length(direction) != 0.0) {
            mapPos = Vector2Subtract(mapPos, Vector2Scale(Vector2Normalize(direction), speed));
        }

        EndDrawing();
    }
    // Freeing memory
    UnloadTexture(map);
    UnloadTexture(knightIdle);
    UnloadTexture(knightRun);

    CloseWindow();
    return 0;
}