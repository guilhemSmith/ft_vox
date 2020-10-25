#version 400 core

in vec4 norm;
in vec2 texcoords;
flat in int tex;
out vec4 FragColor;

uniform sampler2D dirt;

void main()
{
    vec4 textured;
    if (tex == 1)
        textured = texture(dirt, texcoords);
    if (norm.z < 0)
        FragColor = vec4(0.7f, 0.7f, 0.7f, 1.0f) * textured;
    else if (norm.z > 0)
        FragColor = vec4(0.7f, 0.7f, 0.7f, 1.0f) * textured;
    else if (norm.x < 0)
        FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f) * textured;
    else if (norm.x > 0)
        FragColor = vec4(0.4f, 0.4f, 0.4f, 1.0f) * textured;
    else if (norm.y < 0)
        FragColor = vec4(0.9f, 0.9f, 0.9f, 1.0f) * textured;
    else if (norm.y > 0)
        FragColor = vec4(0.3f, 0.3f, 0.3f, 1.0f) * textured;

}