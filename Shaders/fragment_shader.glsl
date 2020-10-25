#version 400 core

in vec4 norm;
in vec2 texcoords;
flat in int tex;
out vec4 FragColor;

uniform sampler2D grass;
uniform sampler2D grass_side;
uniform sampler2D dirt;
uniform sampler2D rock;
uniform sampler2D sand;

void main()
{
    if (tex == 1)
    {
        if (norm.z < 0)
            FragColor = vec4(0.7f, 0.7f, 0.7f, 1.0f) * texture(grass_side, texcoords);
        else if (norm.z > 0)
            FragColor = vec4(0.7f, 0.7f, 0.7f, 1.0f) * texture(grass_side, texcoords);
        else if (norm.x < 0)
            FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f) * texture(grass_side, texcoords);
        else if (norm.x > 0)
            FragColor = vec4(0.4f, 0.4f, 0.4f, 1.0f) * texture(grass_side, texcoords);
        else if (norm.y < 0)
            FragColor = vec4(0.9f, 0.9f, 0.9f, 1.0f) * texture(grass, texcoords);
        else if (norm.y > 0)
            FragColor = vec4(0.3f, 0.3f, 0.3f, 1.0f) * texture(dirt, texcoords);
    }
}