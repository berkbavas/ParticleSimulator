#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 initialPosition;
layout(location = 1) in vec4 normal;

uniform mat4 VP; // View-Projection Matrix

out vec3 fsNormal;
out float fsDistance;

void main()
{
    fsDistance = distance(position.xyz, initialPosition.xyz);
    fsNormal = normal.xyz;
    gl_Position = VP * vec4(position.xyz, 1.0f);
}
