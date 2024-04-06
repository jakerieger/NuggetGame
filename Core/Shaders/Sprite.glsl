R""(
#define VERTEX
#version 460 core
layout(location = 0) in vec4 aVertex;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

out vec2 TexCoord;

void main() {
    gl_Position = u_Projection * u_View * u_Model * vec4(aVertex.xy, 0.0, 1.0);
    TexCoord = aVertex.zw;
}

#undef VERTEX

#define FRAGMENT
#version 460 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D u_Sprite;

void main() {
    FragColor = texture(u_Sprite, TexCoord);
}
#undef FRAGMENT
)"";