#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <string.h>

class AnimData {
private:
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;

public:
    AnimData(Rectangle r, Vector2 p, float ut) 
        : rec(r), pos(p), frame(0), updateTime(ut), runningTime(0.0f) {}

    void Update(float deltaTime, int maxFrame) {
        runningTime += deltaTime;
        if (runningTime >= updateTime) {
            runningTime = 0.0f;
            frame = (frame + 1) % (maxFrame + 1);
            rec.x = frame * rec.width;
        }
    }

    // Getter methods
    Rectangle GetRec() const { return rec; }
    Vector2 GetPos() const { return pos; }
    int GetFrame() const { return frame; }
};

// Draw character logic
void DrawCharacter(AnimData *data, Texture2D knightState, Vector2 knightPos, float rightLeft, float dT)
{
    // Draw character in state
    Rectangle source{data->GetRec().x, data->GetRec().y, rightLeft * (float)knightState.width / 6.f, (float)knightState.height};
    Rectangle dest{knightPos.x, knightPos.y, 4.0f * (float)knightState.width / 6.0f, 4.0f * (float)knightState.height};
    Vector2 origin{};
    DrawTexturePro(knightState, source, dest, origin, 0.f, WHITE);
    data->Update(dT, 6);
}

int main()
{
    // Window dimensions
    int windowDimensions[2]{384, 384};

    // Window initialization
    InitWindow(windowDimensions[0], windowDimensions[1], "Top Down");
    SetTargetFPS(60);

    // Constants
    float speed{4.0};

    // Boolean flags

    // Textures
    Texture2D map = LoadTexture("nature_tileset/WorldMap.png");
    Vector2 mapPos{-384 / 2, -384 / 2};

    Texture2D knight = LoadTexture("characters/knight_idle_spritesheet.png");
    Texture2D knightRun = LoadTexture("characters/knight_run_spritesheet.png");
    Vector2 knightPos{
        384 / 2.0f - 4.0f * (0.5f * (float)knight.width / 6.0f),
        384 / 2.0f - 4.0f * (0.5f * (float)knight.height)};
    float rightLeft{1.f}; // 1: facing right, -1 facing left

    AnimData knightData(
        {0.0f, 0.0f, knight.width / 6.0f, (float)knight.height},
        {windowDimensions[0] / 2.0f - knight.width / 12.0f, (float)(windowDimensions[1] - knight.height)},
        1.0f / 12.0f
    );

    AnimData knightRunData(
        {0.0f, 0.0f, knightRun.width / 6.0f, (float)knightRun.height},
        {windowDimensions[0] / 2.0f - knightRun.width / 12.0f, (float)(windowDimensions[1] - knightRun.height)},
        1.0f / 12.0f
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

        if (Vector2Length(direction) == 0.0)
        {
            // Draw character in idle
            DrawCharacter(&knightData, knight, knightPos, rightLeft, dT);
        }
        else if (Vector2Length(direction) != 0.0)
        {
            mapPos = Vector2Subtract(mapPos, Vector2Scale(Vector2Normalize(direction), speed));

            // Draw character in motion
            DrawCharacter(&knightRunData, knightRun, knightPos, rightLeft, dT);
        }

        EndDrawing();
    }
    // Freeing memory
    UnloadTexture(map);
    UnloadTexture(knight);
    UnloadTexture(knightRun);

    CloseWindow();
}