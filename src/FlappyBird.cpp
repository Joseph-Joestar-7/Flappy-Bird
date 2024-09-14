#include "FlappyBird.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>

FlappyBird::FlappyBird()
    : m_BirdPos(glm::vec3(-0.5f, 0.0f, 0.0f)),
    m_BirdVelocity(glm::vec3(0.0f, 0.0f, 0.0f)),
    m_Gravity(-0.5f),
    m_JumpVelocity(0.4f),
    m_GameOver(false)
{
    Init();
}

FlappyBird::~FlappyBird()
{
    delete m_VBO;
    delete m_IBO;
    delete m_Shader;
    delete m_Renderer;
    for (Texture* texture : m_Textures) {
        delete texture;
    }
    m_Textures.clear();
}

void FlappyBird::Init()
{
    // Initialize bird vertices and the texture coords
    float birdVertices[] = {
    -0.05f, -0.05f, 0.0f,  0.165f, 0.165f,  // Bottom-left
     0.05f, -0.05f, 0.0f,  0.95f, 0.165f,  // Bottom-right
     0.05f,  0.05f, 0.0f,  0.95f, 0.95f,  // Top-right
    -0.05f,  0.05f, 0.0f,  0.165f, 0.95f   // Top-left
    };

    unsigned int birdIndices[] = {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };

    m_VBO = new VertexBuffer(birdVertices, sizeof(birdVertices));
    m_IBO = new IndexBuffer(birdIndices, 6);

    VertexBufferLayout layout;
    layout.push<float>(3);  // 3 floats per vertex (x, y, z)
    layout.push<float>(2); // for the texture co-ords
    m_VAO.addBuffer(*m_VBO, layout);

    m_Shader = new Shader("res/Shaders/Basic.shader");
    m_Shader->bind();

    m_Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    m_View = glm::mat4(1.0f);

    m_Shader->setUniformMat4f("u_Projection", m_Projection);
    m_Shader->setUniformMat4f("u_View", m_View);

    m_Textures.push_back(new Texture("res/Textures/bluebird1.png"));
    m_Textures.push_back(new Texture("res/Textures/bluebird2.png"));
    m_Textures.push_back(new Texture("res/Textures/bluebird3.png"));
    m_Textures.push_back(new Texture("res/Textures/bluebird4.png"));

    m_Textures[0]->bind();

    m_Shader->setUniform1i("u_Texture", 0);

    m_Renderer = new Renderer();

}

void FlappyBird::Restart()
{
    m_BirdPos = glm::vec3(-0.5f, 0.0f, 0.0f);
    m_BirdVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    m_GameOver = false;
}

void FlappyBird::ProcessInput(float deltaTime)
{
    if (m_GameOver && glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_PRESS) {
        Restart();
    }

    if (!m_GameOver && glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS) {
        m_BirdVelocity.y = m_JumpVelocity;
    }
}

void FlappyBird::Update(float deltaTime)
{
    // Apply gravity to the bird
    m_BirdVelocity.y += m_Gravity * deltaTime;
    m_BirdPos += m_BirdVelocity * deltaTime;

    // Update animation time
    m_AnimationTime += deltaTime;

    // Alternate between textures based on time
    float animationCycle = 0.25f;  // How long each frame lasts

    int currentFrame = static_cast<int>(fmod(m_AnimationTime, animationCycle * 4.0f) / animationCycle);
    m_Textures[currentFrame]->bind(0);

}

void FlappyBird::Render()
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    m_Shader->bind();

    // Render the bird
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_BirdPos);  
    m_Shader->setUniformMat4f("u_Model", model);
    m_VAO.bind();
    m_IBO->Bind(); 
    m_Renderer->draw(m_VAO, *m_IBO, *m_Shader, GL_TRIANGLES); 
}

void FlappyBird::CheckCollisions()
{
    
}
