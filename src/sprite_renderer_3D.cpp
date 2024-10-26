#include "sprite_renderer_3D.h"

SpriteRenderer3D::SpriteRenderer3D(const Shader& shader) {
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer3D::~SpriteRenderer3D() {}

void SpriteRenderer3D::initRenderData() {
    // configure VAO/VBO
    unsigned int VBO, EBO;

    // The cube data is fixed, and will be stored in the gpu buffer
    // Shader will perform transform/scale/rotate etc calculations to determine displayed quad
    float vertices[] = {
        // Positions          // Normals         // Texture Coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Vertex 0 (Front-bottom-left)
        0.5f,  -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // Vertex 1 (Front-bottom-right)
        0.5f,  0.5f,  -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // Vertex 2 (Front-top-right)
        -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // Vertex 3 (Front-top-left)

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Vertex 4 (Back-bottom-left)
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // Vertex 5 (Back-bottom-right)
        0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Vertex 6 (Back-top-right)
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f  // Vertex 7 (Back-top-left)
    };

    unsigned int indices[] = {
        0, 1, 2, 0, 2, 3, // Front face
        4, 5, 6, 4, 6, 7, // Back face
        0, 3, 7, 0, 7, 4, // Left face
        1, 5, 6, 1, 6, 2, // Right face
        3, 2, 6, 3, 6, 7, // Top face
        0, 4, 5, 0, 5, 1  // Bottom face
    };

    // Create VBO and EBO
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &this->cubeVAO);

    // Bind the Vertex Array Object (VAO)
    glBindVertexArray(this->cubeVAO);

    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind and set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture Coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind the VAO (it's good practice)
    glBindVertexArray(0);
}

void SpriteRenderer3D::DrawSprite(const Texture2D& texture, glm::vec3 position, glm::vec3 size, glm::vec2 rotate,
                                  glm::vec3 color) {
    // // prepare transformations
    // this->shader.Use();
    // glm::mat4 model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(position, 0.0f));
    //
    // // First translate by half width and height to make center of quad the origin
    // // This makes it so that the rotation transformation will now rotate around the center
    // // After rotation revert the transform so quad is in expected position
    // model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    // model = glm::rotate(model, glm::radians(rotate), glm ::vec3(0.0f, 0.0f, 1.0f));
    // model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    //
    // model = glm::scale(model, glm::vec3(size, 1.0f));
    //
    // this->shader.SetMatrix4("model", model);
    // this->shader.SetVector3f("spriteColor", color);
    //
    // glActiveTexture(GL_TEXTURE0);
    // texture.Bind();
    //
    // Draw the cube
    glBindVertexArray(this->cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
