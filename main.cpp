#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <string.h>

// Struct to hold animation data
typedef struct AnimData 
{
	Rectangle rec;
	Vector2 pos;
	int frame;
	float updateTime;
	float runningTime;
} AnimData;

// Update animation logic
void UpdateAnimData(AnimData* data, float deltaTime, int maxFrame) 
{
	data->runningTime += deltaTime;
	if (data->runningTime >= data->updateTime) 
    {
		data->runningTime = 0.0f;
		data->frame = (data->frame + 1) % (maxFrame + 1);
		data->rec.x = data->frame * data->rec.width;
	}
}

// Draw character logic
void DrawCharacter(AnimData* data, Texture2D knightState, Vector2 knightPos, float rightLeft, float dT)
{
    // Draw character in state
    Rectangle source{data->rec.x, data->rec.y,rightLeft * (float)knightState.width/6.f, (float)knightState.height};
    Rectangle dest{knightPos.x, knightPos.y, 4.0f * (float)knightState.width/6.0f, 4.0f * (float)knightState.height};
    Vector2 origin{};
    DrawTexturePro(knightState, source, dest, origin, 0.f, WHITE);
    UpdateAnimData(data, dT, 6);
};


int main()
{
    // Window dimensions
    int windowDimensions[2]{384,384};

    // Window initialization
    InitWindow(windowDimensions[0], windowDimensions[1], "Top Down");
    SetTargetFPS(60);

    // Constants
    float speed {4.0};
    
    // Boolean flags

    // Textures
    Texture2D map = LoadTexture("nature_tileset/WorldMap.png");    
    Vector2 mapPos{-384/2, -384/2};

    Texture2D knight = LoadTexture("characters/knight_idle_spritesheet.png");
    Texture2D knightRun = LoadTexture("characters/knight_run_spritesheet.png");
    Vector2 knightPos
    {
        384/2.0f - 4.0f * (0.5f * (float)knight.width/6.0f),
        384/2.0f - 4.0f * (0.5f * (float)knight.height)
    };
    float rightLeft{1.f}; // 1: facing right, -1 facing left

    AnimData knightData = 
    {
		{0.0f, 0.0f, knight.width / 6.0f, (float)knight.height},
		{windowDimensions[0] / 2.0f - knight.width / 12.0f, (float)(windowDimensions[1] - knight.height)},
		0,
		1.0f / 12.0f,
		0.0f
	};

    AnimData knightRunData =
    {
		{0.0f, 0.0f, knightRun.width / 6.0f, (float)knightRun.height},
		{windowDimensions[0] / 2.0f - knightRun.width / 12.0f, (float)(windowDimensions[1] - knightRun.height)},
		0,
		1.0f / 12.0f,
		0.0f
	};

    while(!WindowShouldClose())
    {
        const float dT = GetFrameTime();
        BeginDrawing();
        ClearBackground(WHITE);

        Vector2 direction{};

        if (IsKeyDown(KEY_A)) direction.x -= 1.0;
        if (IsKeyDown(KEY_D)) direction.x += 1.0;
        if (IsKeyDown(KEY_W)) direction.y -= 1.0;
        if (IsKeyDown(KEY_S)) direction.y += 1.0;  
      

        DrawTextureEx(map, mapPos,0,4.0,WHITE);

        if (direction.x != 0.0) rightLeft = (direction.x < 0.f) ? -1.f : 1.f;

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