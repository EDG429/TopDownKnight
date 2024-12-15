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
    Rectangle rec;
    Vector2 pos;
    State currentState;
    float runningTime;
    int frame;
    std::vector<Animation> animations; // Vector to hold animation data
    int currentAnimationIndex;

    // Attack animation
    Animation attackAnimation;
    bool isAttacking;
    float attackTime;
    float attackCooldown;
    int attackFrame;

    Texture2D* getCurrentTexture() const {
        return animations[currentAnimationIndex].texture;
    }

public:
    // Constructor now takes a vector of Animation structures and an attack animation
    AnimEngine(Rectangle r, Vector2 p, const std::vector<Animation>& anims, const Animation& attackAnim) 
        : rec(r), pos(p), currentState(State::Idle), runningTime(0.0f), frame(0), 
          animations(anims), currentAnimationIndex(0),
          attackAnimation(attackAnim), isAttacking(false), attackTime(0.0f), attackCooldown(0.0f), attackFrame(0) {}

    void Update(float deltaTime, bool isMoving, bool attackInput) {
        // Update base animation
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

        // Handle attack animation
        if (attackInput && attackCooldown <= 0.0f) {
            isAttacking = true;
            attackTime = 0.0f;
            attackFrame = 0;
            attackCooldown = 0.1f; // 0.1f second cooldown after attack
        }

        if (isAttacking) {
            attackTime += deltaTime;
            if (attackTime >= attackAnimation.updateTime) {
                attackTime = 0.0f;
                attackFrame = (attackFrame + 1) % attackAnimation.frameCount;
                if (attackFrame == 0) { // Reset attack animation
                    isAttacking = false;
                }
            }
        } else if (attackCooldown > 0.0f) {
            attackCooldown -= deltaTime;
        }
    }

    void Draw(float rightLeft, Vector2 characterPos) {
        Texture2D* currentTexture = getCurrentTexture();
        
        // Draw base animation
        if (currentTexture) {
            Rectangle source{rec.x, rec.y, rightLeft * (float)currentTexture->width / animations[currentAnimationIndex].frameCount, rec.height};
            Rectangle dest{characterPos.x, characterPos.y, 4.0f * (float)currentTexture->width / animations[currentAnimationIndex].frameCount, 4.0f * rec.height};
            Vector2 origin = {0, 0}; 
            DrawTexturePro(*currentTexture, source, dest, origin, 0.f, WHITE);
        }

        // Draw attack animation if attacking
        if (isAttacking) {
            Rectangle attackSource{attackFrame * attackAnimation.texture->width / attackAnimation.frameCount, 0, 
                                   rightLeft * (float)attackAnimation.texture->width / attackAnimation.frameCount, attackAnimation.texture->height};
            Rectangle attackDest{characterPos.x + rightLeft * 40, characterPos.y, 4.0f * (float)attackAnimation.texture->width / attackAnimation.frameCount, 4.0f * attackAnimation.texture->height};
            Vector2 origin = {0, 0};
            DrawTexturePro(*attackAnimation.texture, attackSource, attackDest, origin, 0.f, WHITE);
        }
    }

    // Getter methods for the data we need to access outside the class
    Vector2 GetPos() const { return pos; }
    bool IsAttacking() const { return isAttacking; }
};

#endif // ANIM_ENGINE_H