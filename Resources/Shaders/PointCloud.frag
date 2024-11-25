// This code is taken from https://github.com/MauriceGit/Partikel_accelleration_on_GPU

// Copyright (c) 2015, 2016 Maurice Tollmien <maurice.tollmien@gmail.com>

// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.

// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

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
