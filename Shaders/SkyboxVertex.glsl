#version 330 core
layout(location = 0) in vec3 vs_Pos;

out vec3 g_TexCords;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main(){
	g_TexCords = vs_Pos;
	gl_Position = u_Projection * u_View * vec4(vs_Pos, 1.0f);
}