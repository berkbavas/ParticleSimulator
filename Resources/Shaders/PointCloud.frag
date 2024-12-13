#version 430 core

in float fsLife;

out vec4 outColor;

void main()
{
    outColor = mix(vec4(0.0f, 0.2f, 1.0f, 1.0f), vec4(0.2f, 0.05f, 0.0f, 1.0f), fsLife);
}
