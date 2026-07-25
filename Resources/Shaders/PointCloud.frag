#version 430 core

uniform float uColorMultiplier;
uniform float uAmbient;
uniform float uDiffuse;
uniform vec3 uLightDirection;
uniform float uTime;
uniform int uColorMode;

in vec3 fsNormal;
in float fsDistance;
in float fsSpeed;
in float fsAge;
in float fsLifeRatio;
in float fsParticleType;

out vec4 oColor;

vec3 HsvToRgb(vec3 Hsv)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 P = abs(fract(Hsv.xxx + K.xyz) * 6.0 - K.www);
    return Hsv.z * mix(K.xxx, clamp(P - K.xxx, 0.0, 1.0), Hsv.y);
}

vec3 TemperatureColor(float Parameter)
{
    Parameter = clamp(Parameter, 0.0, 1.0);

    vec3 Color;
    if (Parameter < 0.2)
    {
        Color = mix(vec3(0.0, 0.0, 0.5), vec3(0.0, 0.5, 1.0), Parameter * 5.0);
    }
    else if (Parameter < 0.4)
    {
        Color = mix(vec3(0.0, 0.5, 1.0), vec3(0.0, 1.0, 0.5), (Parameter - 0.2) * 5.0);
    }
    else if (Parameter < 0.6)
    {
        Color = mix(vec3(0.0, 1.0, 0.5), vec3(1.0, 1.0, 0.0), (Parameter - 0.4) * 5.0);
    }
    else if (Parameter < 0.8)
    {
        Color = mix(vec3(1.0, 1.0, 0.0), vec3(1.0, 0.3, 0.0), (Parameter - 0.6) * 5.0);
    }
    else
    {
        Color = mix(vec3(1.0, 0.3, 0.0), vec3(1.0, 1.0, 1.0), (Parameter - 0.8) * 5.0);
    }

    return Color;
}

vec3 NebulaColor(float Parameter, float Seed)
{
    Parameter = clamp(Parameter, 0.0, 1.0);

    vec3 C1 = vec3(0.4, 0.1, 0.8);
    vec3 C2 = vec3(0.1, 0.4, 0.9);
    vec3 C3 = vec3(0.1, 0.8, 0.7);
    vec3 C4 = vec3(0.9, 0.3, 0.6);
    vec3 C5 = vec3(1.0, 0.8, 0.3);

    float Phase = fract(Seed * 0.1 + uTime * 0.05);

    if (Phase < 0.25)
        return mix(C1, C2, Parameter);
    else if (Phase < 0.5)
        return mix(C2, C3, Parameter);
    else if (Phase < 0.75)
        return mix(C3, C4, Parameter);
    else
        return mix(C4, C5, Parameter);
}

vec3 FireColor(float Parameter)
{
    Parameter = clamp(Parameter, 0.0, 1.0);

    vec3 C1 = vec3(0.1, 0.0, 0.0);
    vec3 C2 = vec3(0.8, 0.2, 0.0);
    vec3 C3 = vec3(1.0, 0.6, 0.0);
    vec3 C4 = vec3(1.0, 0.9, 0.4);
    vec3 C5 = vec3(1.0, 1.0, 0.9);

    if (Parameter < 0.25)
        return mix(C1, C2, Parameter * 4.0);
    else if (Parameter < 0.5)
        return mix(C2, C3, (Parameter - 0.25) * 4.0);
    else if (Parameter < 0.75)
        return mix(C3, C4, (Parameter - 0.5) * 4.0);
    else
        return mix(C4, C5, (Parameter - 0.75) * 4.0);
}

void main()
{
    vec3 BaseColor;

    float SpeedFactor = clamp(fsSpeed * 0.5, 0.0, 1.0);
    float DistanceFactor = clamp(fsDistance * uColorMultiplier, 0.0, 1.0);

    if (fsParticleType > 0.5)
    {
        BaseColor = mix(vec3(1.0, 0.9, 0.5), vec3(1.0, 1.0, 1.0), SpeedFactor);
    }
    else
    {
        vec3 SpeedColor = TemperatureColor(SpeedFactor);
        vec3 DistanceColor = mix(vec3(0.2, 0.5, 1.0), vec3(1.0, 0.3, 0.5), DistanceFactor);
        vec3 Nebula = NebulaColor(DistanceFactor, fsDistance * 100.0);
        BaseColor = mix(SpeedColor, Nebula, 0.5);
        float AgeFade = 1.0 - fsLifeRatio * 0.3;
        BaseColor *= AgeFade;
    }

    float Pulse = 0.9f + 0.1f * sin(uTime * 3.0f + fsDistance * 10.0f);
    BaseColor *= Pulse;

    vec3 Ambient = uAmbient * BaseColor;
    vec3 Diffuse = uDiffuse * max(dot(fsNormal, -uLightDirection), 0.0f) * BaseColor;

    vec3 Result = Ambient + Diffuse;

    float Glow = 1.0f + SpeedFactor * 0.5f;
    Result *= Glow;

    oColor = vec4(Result, 1.0);
}
