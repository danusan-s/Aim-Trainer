#include "game.h"
#include "resource_manager.h"
#include <iostream>

Game::Game(unsigned int width, unsigned int height)
    : State(GameState::GAME_MENU), Keys(), Width(width), Height(height), ClickX(-1), ClickY(-1), ReleaseX(-1),
      ReleaseY(-1), gameObjects(std::vector<std::vector<GameObject*>>(10, std::vector<GameObject*>(10))) {}

Game::~Game() {
    std::cout << "Attempting to delete Game Object" << std::endl;

    delete this->renderer2D;
    this->renderer2D = nullptr;
    std::cout << "Deleted renderer\n";

    delete this->text;
    text = nullptr;
    std::cout << "Deleted text\n";

    std::cout << "Game Object successfully deleted" << std::endl;
}

void Game::Init() {
    // Seed the random number generator
    // srand(static_cast<unsigned int>(time(0)));

    // load shaders
    std::cout << "Loading Shaders" << std::endl;
    ResourceManager::LoadShader("../shaders/color_vert.glsl", "../shaders/color_frag.glsl", nullptr, "cube");
    ResourceManager::LoadShader("../shaders/light_cube_vert.glsl", "../shaders/light_cube_frag.glsl", nullptr, "light");
    ResourceManager::LoadShader("../shaders/sprite_vert.glsl", "../shaders/sprite_frag.glsl", nullptr, "sprite2D");

    camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    lastX = this->Width / 2.0f;
    lastY = this->Height / 2.0f;
    firstMouse = true;

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            gameObjects[i][j] =
                new GameObject(glm::vec3(i, j, 0), glm::vec3(1.0f, 1.0f, 1.0f), ResourceManager::GetTexture("solid"));
        }
    }

    // configure shaders
    glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    // directional light
    ResourceManager::GetShader("cube").Use().SetVector3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
    ResourceManager::GetShader("cube").Use().SetVector3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    ResourceManager::GetShader("cube").Use().SetVector3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    ResourceManager::GetShader("cube").Use().SetVector3f("dirLight.specular", 0.5f, 0.5f, 0.5f);

    // set render-specific controls
    renderer2D = new SpriteRenderer2D(ResourceManager::GetShader("sprite2D"));
    lightRenderer3D = new SpriteRenderer3D(ResourceManager::GetShader("light"));
    cubeRenderer3D = new SpriteRenderer3D(ResourceManager::GetShader("cube"));
    text = new TextRenderer(this->Width, this->Height);
    text->Load("../fonts/JetBrainsMonoNerdFont-Bold.ttf", 36);

    // load textures
    std::cout << "Loading textures" << std::endl;
    ResourceManager::LoadTexture("../textures/solid.png", false, "solid");

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
    } else if (this->State == GameState::GAME_ACTIVE) {
        if (Keys['R']) {
            this->State = GameState::GAME_MENU;
        }
    }
}

void Game::Render() {
    if (this->State == GameState::GAME_ACTIVE || this->State == GameState::GAME_OVER) {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                gameObjects[i][j]->Draw(*cubeRenderer3D);
            }
        }

    } else if (this->State == GameState::GAME_MENU) {
        renderer2D->DrawSprite(ResourceManager::GetTexture("solid"), startButton->Position, startButton->Size, 0.0f,
                               glm::vec3(1.0f));
        text->RenderText("Aim Trainer", 0.5f * Width, 0.2 * Height, 0.002f * Width);
        text->RenderText("Start game", 0.5f * Width, 0.5f * Width, 0.0005f * Width, glm::vec3(0.0f));
    }
}

void Game::ResetGame() {
    this->State = GameState::GAME_MENU;
}
