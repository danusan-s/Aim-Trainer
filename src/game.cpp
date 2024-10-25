#include "game.h"
#include "resource_manager.h"
#include <iostream>

float fallTimeAccumulator = 0.0f;
float moveTimeAccumulator = 0.0f;
float flashTimer = 0.0f;

Game::Game(unsigned int width, unsigned int height)
    : State(GameState::GAME_MENU), Keys(), Width(width), Height(height) {}

Game::~Game() {
    std::cout << "Attempting to delete Game Object" << std::endl;

    delete renderer;
    renderer = nullptr;
    std::cout << "Deleted renderer\n";

    delete text;
    text = nullptr;
    std::cout << "Deleted text\n";

    std::cout << "Game Object successfully deleted" << std::endl;
}

void Game::Init() {
    // Seed the random number generator
    // srand(static_cast<unsigned int>(time(0)));

    // load shaders
    std::cout << "Loading Shaders" << std::endl;
    ResourceManager::LoadShader("../shaders/color_vert.glsl", "../shaders/color_frag.glsl", nullptr, "color");
    ResourceManager::LoadShader("../shaders/light_cube_vert.glsl", "../shaders/light_cube_frag.glsl", nullptr, "light");

    // configure shaders
    glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    // set render-specific controls
    renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    text = new TextRenderer(this->Width, this->Height);
    text->Load("../fonts/JetBrainsMonoNerdFont-Bold.ttf", 72);

    // load textures
    std::cout << "Loading textures" << std::endl;
    ResourceManager::LoadTexture("../textures/tetris_block.jpg", false, "block");
    ResourceManager::LoadTexture("../textures/solid_block.png", false, "solid");

    startButton = new ClickableObject(glm::vec2(Width * 0.5f), glm::vec2(Width * 0.5f, Width * 0.1f), 0);
}

void Game::Update(float dt) {
    if (this->State == GameState::GAME_ACTIVE) {
    }
}

void Game::ProcessInput(float dt) {
    if (this->State == GameState::GAME_MENU) {
        if (startButton->CheckClickPos(ClickX, ClickY)) {
            startButton->Clicked = true;
            startButton->ChangeScale(0.8f);
            ClickX = -1;
            ClickY = -1;
        }
        if (ReleaseX > 0 && ReleaseY > 0) {
            if (startButton->CheckClickPos(ReleaseX, ReleaseY) && startButton->Clicked) {
                this->State = GameState::GAME_ACTIVE;
            }
            startButton->ChangeScale(1.0f);
            startButton->Clicked = false;
            ReleaseX = -1;
            ReleaseY = -1;
        }
    }
}

void Game::Render() {
    if (this->State == GameState::GAME_ACTIVE || this->State == GameState::GAME_OVER) {
        // Render the game scene
    } else if (this->State == GameState::GAME_MENU) {
        renderer->DrawSprite(ResourceManager::GetTexture("solid"), startButton->Position, startButton->Size, 0.0f,
                             glm::vec3(1.0f));
        text->RenderText("TETRIS", 0.5f * Width, 0.2 * Height, 0.002f * Width);
        text->RenderText("Start game", 0.5f * Width, 0.5f * Width, 0.0005f * Width, glm::vec3(0.0f));
    }
}

void Game::ResetGame() {
    this->State = GameState::GAME_MENU;
}
