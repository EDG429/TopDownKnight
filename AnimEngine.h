#ifndef ANIM_ENGINE_H
#define ANIM_ENGINE_H

#include "raylib.h"
#include "raymath.h"

class AnimEngine {
private:
    enum class State { Idle, Running }; // Enumeration for different animation states
    State currentState;
    Rectangle rec;
    Vector2 pos;
    float updateTime;
    float runningTime;
    int frame;
    Texture2D* idleTexture;
    Texture2D* runningTexture;

public:
    AnimEngine(Rectangle r, Vector2 p, float ut, Texture2D* idle, Texture2D* run) 
        : rec(r), pos(p), currentState(State::Idle), updateTime(ut), runningTime(0.0f), frame(0), 
          idleTexture(idle), runningTexture(run) {}

    void Update(float deltaTime, bool isMoving) {
        runningTime += deltaTime;
        if (runningTime >= updateTime) {
            runningTime = 0.0f;
            frame = (frame + 1) % 6; // Assuming 6 frames for each animation
            rec.x = frame * rec.width;
        }

        // Change state based on movement
        currentState = isMoving ? State::Running : State::Idle;
        // Update the rectangle's height based on the current texture
        rec.height = (currentState == State::Idle) ? idleTexture->height : runningTexture->height;
    }

    void Draw(float rightLeft, Vector2 characterPos) {
        Texture2D* currentTexture = (currentState == State::Idle) ? idleTexture : runningTexture;
        Rectangle source{rec.x, rec.y, rightLeft * (float)currentTexture->width / 6.f, rec.height};
        Rectangle dest{characterPos.x, characterPos.y, 4.0f * (float)currentTexture->width / 6.0f, 4.0f * rec.height};
        Vector2 origin{};
        DrawTexturePro(*currentTexture, source, dest, origin, 0.f, WHITE);
    }

    // Getter methods for the data we need to access outside the class
    Vector2 GetPos() const { return pos; }
};

#endif // ANIM_ENGINE_H