#version 430 core

in float fsLife;

out vec4 outColor;

void main()
{
    if (fsLife < 0.1f)
    {
        // Fast tote Partikel blenden nach Schwarz über.
        outColor = mix(vec4(vec3(0.0), 1.0), vec4(0.0, 0.5, 1.0, 1.0), fsLife * 10.0);
    }
    else if (fsLife > 0.9)
    {
        // Neu geborene Partikel blenden von Schwarz ein.
        outColor = mix(vec4(0.6, 0.05, 0.0, 1.0), vec4(vec3(0.0), 1.0), (fsLife - 0.9) * 10.0);
    }
    else
    {
        // Lebensdauer Rot --> Blau.
        outColor = mix(vec4(0.0, 0.5, 1.0, 1.0), vec4(0.6, 0.05, 0.0, 1.0), fsLife);
    }
}
