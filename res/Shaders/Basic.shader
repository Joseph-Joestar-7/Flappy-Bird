#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;     // Position attribute
layout(location = 1) in vec2 aTexCoord; // Texture coordinate attribute

out vec2 v_TexCoord;  // Pass the texture coordinate to the fragment shader

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    v_TexCoord = aTexCoord;  // Pass texture coordinates to fragment shader
    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

in vec2 v_TexCoord;  // Receive the texture coordinates from the vertex shader
out vec4 FragColor;

uniform sampler2D u_Texture;  // Texture uniform

void main()
{
    FragColor = texture(u_Texture, v_TexCoord);  // Sample and output texture color
}
