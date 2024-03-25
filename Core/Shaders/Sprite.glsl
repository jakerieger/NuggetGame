R""(
#define VERTEX
#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoords = aTexCoords;
}

#undef VERTEX

#define FRAGMENT
#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_Texture;

void main() {
    vec4 color = texture(u_Texture, TexCoords);
    FragColor = color;
}
#undef FRAGMENT
)"";