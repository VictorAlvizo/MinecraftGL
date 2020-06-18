#version 330 core

in vec3 g_Pos;
in vec2 g_TexCords;

uniform sampler2D u_Texture;
uniform float u_Time;

out vec4 fragColor;

void main() {
	fragColor = texture(u_Texture, vec2(g_TexCords.x + sin(u_Time * 0.3f), g_TexCords.y + sin(u_Time) / 10.0f));
}