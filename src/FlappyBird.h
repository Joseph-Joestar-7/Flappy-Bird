#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Renderer.h"
#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "Pipe.h"
#include "GLFW/glfw3.h"
#include <iostream>

class FlappyBird
{
private:
    //Vertex Arrays, Vertex Buffers,Index Buffers
    VertexArray m_VAO;
    VertexBuffer* m_VBO;
    IndexBuffer* m_IBO;
    Shader* m_Shader;
    Renderer* m_Renderer;


public:
    FlappyBird();
    ~FlappyBird();    
};
