#version 430 core

uniform float colorMultiplier;
uniform float ambientFactor;
uniform float diffuseFactor;
uniform vec3 lightDirection;

in vec3 fsNormal;
in float fsDistance;

out vec4 outColor;

const vec4 lightColor = vec4(1.0f);

void main()
{
    vec4 modelColor = mix(vec4(1), vec4(1, 0, 0, 1), fsDistance * colorMultiplier);

    float diffuseStrength = max(dot(fsNormal, -lightDirection), 0.0f);

    vec4 result = (ambientFactor + diffuseFactor * diffuseStrength) * lightColor * modelColor;
    outColor = vec4(result.rgb, 1.0f);
}
