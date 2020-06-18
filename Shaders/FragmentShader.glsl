#version 330 core

in vec3 g_Pos;
in vec2 g_TexCords;

uniform sampler2D u_Texture;
uniform bool u_DebugActive;

out vec4 fragColor;

void main() {
	fragColor = texture(u_Texture,  g_TexCords);
}