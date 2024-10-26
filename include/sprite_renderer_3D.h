#include "shader.h"
#include "texture.h"

class SpriteRenderer3D {
   public:
    SpriteRenderer3D(const Shader& shader);
    ~SpriteRenderer3D();

    void DrawSprite(const Texture2D& texture, glm::vec3 position, glm::vec3 size = glm::vec3(10.0f, 10.0f, 10.0f),
                    glm::vec2 rotate = glm::vec2(0.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f));

   private:
    Shader shader;
    unsigned int cubeVAO;

    void initRenderData();
};
