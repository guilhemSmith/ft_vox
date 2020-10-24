#version 330 core

in vec4 norm;
in vec2 texcoords;
flat in int tex;
out vec4 FragColor;

void main()
{
    if (tex == 1)
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    else
        FragColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);
//    if (norm.z < 0)
//	    FragColor = vec4(0.7f, 0.7f, 0.7f, 1.0f);
//    else if (norm.z > 0)
//        FragColor = vec4(0.7f, 0.7f, 0.7f, 1.0f);
//     else if (norm.x < 0)
//        FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
//     else if (norm.x > 0)
//        FragColor = vec4(0.4f, 0.4f, 0.4f, 1.0f);
//     else if (norm.y < 0)
//        FragColor = vec4(0.9f, 0.9f, 0.9f, 1.0f);
//     else if (norm.y > 0)
//        FragColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);
}