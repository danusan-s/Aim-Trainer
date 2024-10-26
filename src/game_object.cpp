#include "game_object.h"

GameObject::GameObject()
    : Position(0.5f, 0.5f, 0.5f), Size(1.0f, 1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(),
      IsSolid(false), Destroyed(false) {}

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color, glm::vec3 velocity)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false),
      Destroyed(false) {}

void GameObject::Draw(SpriteRenderer3D& renderer) {
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}
