#include "Character.h"
#include "AnimEngine.h"

Character::Character(const std::vector<Animation>& animations, const Animation& attackAnimation, Vector2 pos, float moveSpeed)
    : position(pos), speed(moveSpeed), direction(1.0f),
      animation({0.0f, 0.0f, static_cast<float>(animations[0].texture->width) / animations[0].frameCount, static_cast<float>(animations[0].texture->height)}, pos, animations, attackAnimation) {}

void Character::Move(Vector2 direction, float deltaTime) {
    position = Vector2Add(position, Vector2Scale(Vector2Normalize(direction), speed * deltaTime));
}

void Character::Update(float deltaTime, Vector2 inputDirection, bool attackInput) {
    if (Vector2Length(inputDirection) > 0.0f) {
        direction = (inputDirection.x < 0.0f) ? -1.0f : 1.0f;
        Move(inputDirection, deltaTime); // Pass deltaTime to Move
    }
    animation.Update(deltaTime, Vector2Length(inputDirection) > 0.0f, attackInput);
}

void Character::Draw() {
    
    animation.Draw(direction, position);
}

int Character::TakeDamage(int damage)
{
    health -= damage;
    return health;
}