#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in float life;

uniform mat4 VP; // View-Projection Matrix

out float fsLife;

void main()
{
    fsLife = life;
    gl_Position = VP * vec4(position, 1.0f);
}
