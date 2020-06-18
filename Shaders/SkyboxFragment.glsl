#version 330 core

in vec3 g_TexCords;

uniform samplerCube u_Skybox;
uniform float u_BlueColor;

out vec4 fragColor;

void main(){
	fragColor = texture(u_Skybox, g_TexCords) * vec4(1.0f, u_BlueColor, 1.0f, 1.0f);
}