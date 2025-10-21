#version 330 core

layout (location = 0) in vec3 positionLayout;

layout (location = 1) in vec2 textureLayout;


out vec2 textureCoord;

void main()
{
   gl_Position = vec4(positionLayout.x, positionLayout.y, positionLayout.z, 1.0);

   textureCoord = textureLayout;
}