#include "sprite_renderer_2D.h"

SpriteRenderer2D::SpriteRenderer2D(const Shader& shader) {
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer2D::~SpriteRenderer2D() {}

void SpriteRenderer2D::initRenderData() {
    // configure VAO/VBO
    unsigned int VBO;

    // The quad data is fixed, and will be stored in the gpu buffer
    // Shader will perform transform/scale/rotate etc calculations to determine displayed quad
    float vertices[] = {// pos      // tex
                        0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer2D::DrawSprite(const Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate,
                                  glm::vec3 color) {
    // prepare transformations
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    // First translate by half width and height to make center of quad the origin
    // This makes it so that the rotation transformation will now rotate around the center
    // After rotation revert the transform so quad is in expected position
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm ::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    this->shader.SetMatrix4("model", model);
    this->shader.SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
