#ifndef ANIM_ENGINE_H
#define ANIM_ENGINE_H

#include "raylib.h"
#include "raymath.h"
#include <vector>

struct Animation {
    int frameCount; // Number of frames for this animation
    float updateTime; // Update time for this animation
    Texture2D* texture; // The texture for this animation
};

class AnimEngine {
private:
    enum class State { Idle, Running }; // Enumeration for different animation states
    State currentState;
    Rectangle rec;
    Vector2 pos;
    float runningTime;
    int frame;
    std::vector<Animation> animations; // Vector to hold animation data
    int currentAnimationIndex;

    Texture2D* getCurrentTexture() const {
        return animations[currentAnimationIndex].texture;
    }

public:
    // Constructor now takes a vector of Animation structures
    AnimEngine(Rectangle r, Vector2 p, const std::vector<Animation>& anims) 
        : rec(r), pos(p), currentState(State::Idle), runningTime(0.0f), frame(0), 
          animations(anims), currentAnimationIndex(0) {}

    void Update(float deltaTime, bool isMoving) {
        runningTime += deltaTime;
        if (runningTime >= animations[currentAnimationIndex].updateTime) {
            runningTime = 0.0f;
            frame = (frame + 1) % animations[currentAnimationIndex].frameCount;
            rec.x = frame * rec.width;
        }

        // Change state based on movement
        currentAnimationIndex = isMoving ? static_cast<int>(State::Running) : static_cast<int>(State::Idle);
        currentState = static_cast<State>(currentAnimationIndex);

        // Update the rectangle's height based on the current texture
        if (animations[currentAnimationIndex].texture) {
            rec.height = animations[currentAnimationIndex].texture->height;
        }
    }

    // Draw method remains largely the same but uses the new structure
    void Draw(float rightLeft, Vector2 characterPos) {
        Texture2D* currentTexture = getCurrentTexture();
        if (currentTexture) {
            Rectangle source{rec.x, rec.y, rightLeft * (float)currentTexture->width / animations[currentAnimationIndex].frameCount, rec.height};
            Rectangle dest{characterPos.x, characterPos.y, 4.0f * (float)currentTexture->width / animations[currentAnimationIndex].frameCount, 4.0f * rec.height};
            Vector2 origin{};
            DrawTexturePro(*currentTexture, source, dest, origin, 0.f, WHITE);
        }
    }

    // Getter methods for the data we need to access outside the class
    Vector2 GetPos() const { return pos; }
};

#endif // ANIM_ENGINE_H