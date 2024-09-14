#include "FlappyBird.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>

FlappyBird::FlappyBird()
    : m_BirdPos(glm::vec3(-0.5f, 0.0f, 0.0f)),
    m_BirdVelocity(glm::vec3(0.0f, 0.0f, 0.0f)),
    m_Gravity(-0.5f),
    m_JumpVelocity(0.4f),
    m_PipeSpawnTime(0.0f),
    m_GameOver(false)
{
    Init();
}

FlappyBird::~FlappyBird()
{
    delete m_VBO;
    delete m_IBO;
    delete m_Shader;
}

void FlappyBird::Init()
{
    // Define the bird as a simple square for now
    float birdVertices[] = {
        -0.05f, -0.05f, 0.0f, // Bottom-left
         0.05f, -0.05f, 0.0f, // Bottom-right
         0.05f,  0.05f, 0.0f, // Top-right
        -0.05f,  0.05f, 0.0f  // Top-left
    };

    unsigned int birdIndices[] = {
        0, 1, 2, 
        2, 3, 0 // Two triangles making a square
    };

    m_VBO = new VertexBuffer(birdVertices, sizeof(birdVertices));
    m_IBO = new IndexBuffer(birdIndices, 6);

    VertexBufferLayout layout;
    layout.push<float>(3);
    m_VAO.addBuffer(*m_VBO, layout);

    m_Shader = new Shader("res/Basic.shader");
    m_Shader->bind();

    m_Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    m_View = glm::mat4(1.0f);

    m_Shader->setUniformMat4f("u_Projection", m_Projection);
    m_Shader->setUniformMat4f("u_View", m_View);

    // Initialize the first pipe
    m_Pipes.emplace_back(1.0f, 0.0f);
}

void FlappyBird::Restart()
{
    m_BirdPos = glm::vec3(-0.5f, 0.0f, 0.0f);
    m_BirdVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    m_GameOver = false;
    m_Pipes.clear();
    m_Pipes.emplace_back(1.0f, 0.0f);
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

    // Update pipes
    for (auto& pipe : m_Pipes) {
        pipe.Update(deltaTime);
    }

    // Spawn new pipes
    m_PipeSpawnTime += deltaTime;
    if (m_PipeSpawnTime > 2.0f) {
        m_Pipes.emplace_back(1.0f, (rand() % 200 - 100) / 100.0f);
        m_PipeSpawnTime = 0.0f;
    }

    // Remove pipes that are off-screen
    m_Pipes.erase(std::remove_if(m_Pipes.begin(), m_Pipes.end(),
        [](Pipe& p) { return p.IsOffScreen(); }), m_Pipes.end());

    // Check for collisions
    CheckCollisions();
}

void FlappyBird::Render()
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    m_Shader->bind();

    if (m_GameOver) {
        std::cout << "Game Over! Press Enter to restart.\n";
        return;
    }

    // Render the bird
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_BirdPos);
    m_Shader->setUniformMat4f("u_Model", model);
    m_Shader->setUniform4f("uColor", 1.0f, 1.0f, 0.0f, 1.0f); // Yellow color for bird
    m_VAO.bind();
    m_IBO->Bind();
    m_Renderer->draw(m_VAO, *m_IBO, *m_Shader, GL_TRIANGLES);

    // Render the pipes
    for (auto& pipe : m_Pipes) {
        pipe.Render(*m_Renderer);
    }
}

void FlappyBird::CheckCollisions()
{
    // Check if the bird hits the ground or flies too high
    if (m_BirdPos.y - 0.05f <= -1.0f || m_BirdPos.y + 0.05f >= 1.0f) {
        m_GameOver = true;
    }

    // Check if the bird collides with a pipe
    for (auto& pipe : m_Pipes) {
        if (pipe.CollidesWithBird(m_BirdPos)) {
            m_GameOver = true;
            break;
        }
    }
}