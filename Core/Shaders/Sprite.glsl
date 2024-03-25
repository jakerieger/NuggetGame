R""(
#define VERTEX
#version 460 core
layout (location = 0) in vec4 aVertex;

in mat4 u_ProjModel;

out vec2 TexCoords;

void main() {
    TexCoords = aVertex.zw;
    vec4 pos = vec4(aVertex.xy, 0.0, 1.0);
    gl_Position = pos;
}

#undef VERTEX

#define FRAGMENT
#version 460 core
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D u_Texture;

void main() {
    vec4 color = texture(u_Texture, TexCoords);
    FragColor = color;
}
#undef FRAGMENT
)"";