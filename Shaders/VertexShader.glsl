#version 330 core
layout(location = 0) in vec3 vs_Pos;
layout(location = 1) in vec2 vs_TexCords;

out vec3 g_Pos;
out vec2 g_TexCords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
	gl_Position = u_Projection * u_View * u_Model * vec4(vs_Pos, 1.0f);

	g_Pos = vs_Pos;
	g_TexCords = vec2(vs_TexCords.x, 1 - vs_TexCords.y);
}