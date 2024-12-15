#ifndef CHARACTER_H
#define CHARACTER_H

#include "AnimEngine.h"
#include "raylib.h"
#include "raymath.h"

class Character {
private:
    Vector2 position;
    float speed;
    float direction; // -1 for left, 1 for right   
    int health; // for future health implementation 

public:
    AnimEngine animation;
    Character(const std::vector<Animation>& animations, const Animation& attackAnimation, Vector2 pos, float moveSpeed);

    void Update(float deltaTime, Vector2 inputDirection, bool attackInput);
    void Move(Vector2 direction, float deltaTime); 
    void Draw();
    Vector2 GetPosition() const { return position; }
    bool IsAttacking() const { return animation.IsAttacking(); }
    int TakeDamage(int damage);

private:
    void Move(Vector2 direction);
};

#endif // CHARACTER_H