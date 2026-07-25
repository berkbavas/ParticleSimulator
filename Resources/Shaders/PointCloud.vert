#version 430 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec4 aInitialPosition;
layout(location = 2) in vec4 aNormal;
layout(location = 3) in vec4 aParticleData;

uniform mat4 uVP;
uniform float uBasePointSize;
uniform float uTime;

out vec3 fsNormal;
out float fsDistance;
out float fsSpeed;
out float fsAge;
out float fsLifeRatio;
out float fsParticleType;

void main()
{
    fsDistance = distance(aPosition.xyz, aInitialPosition.xyz);
    fsNormal = aNormal.xyz;

    fsAge = aParticleData.x;
    float MaxAge = aParticleData.y;
    float SizeMult = aParticleData.z;
    fsParticleType = aParticleData.w;

    fsLifeRatio = clamp(fsAge / max(MaxAge, 0.001), 0.0, 1.0);

    fsSpeed = fsDistance * 0.1;

    gl_Position = uVP * vec4(aPosition.xyz, 1.0);

    float DistanceToCamera = gl_Position.w;
    float SizeFromDistance = uBasePointSize / (DistanceToCamera * 0.5 + 1.0);

    float AgeSizeMod = 1.0 - fsLifeRatio * 0.3;
    float PulseMod = 0.95 + 0.1 * sin(uTime * 5.0 + aPosition.x * 10.0);
    float TypeMod = (fsParticleType > 0.5) ? 1.5 : 1.0;

    gl_PointSize = max(1.0, SizeFromDistance * SizeMult * AgeSizeMod * PulseMod * TypeMod);
}
