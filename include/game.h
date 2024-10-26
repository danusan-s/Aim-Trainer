#ifndef GAME_H
#define GAME_H

#include "clickable_object.h"
#include "sprite_renderer_2D.h"
#include "game_object.h" // Also includes sprite_renderer_3D.h
#include "text_renderer.h"
#include "camera.h"
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Represents the current state of the game
enum class GameState { GAME_ACTIVE, GAME_MENU, GAME_OVER };

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game {
   private:
    // Game-related State data
    SpriteRenderer2D* renderer2D;
    SpriteRenderer3D* lightRenderer3D;
    SpriteRenderer3D* cubeRenderer3D;
    TextRenderer* text;
    ClickableObject* startButton;
    std::vector<std::vector<GameObject*>> gameObjects;

   public:
    // game state
    GameState State;
    int Score;
    bool Keys[1024];
    Camera* camera;
    float lastX, lastY;
    bool firstMouse;
    float ClickX, ClickY;
    float ReleaseX, ReleaseY;
    unsigned int Width, Height;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    // reset
    void ResetGame();
};

#endif
