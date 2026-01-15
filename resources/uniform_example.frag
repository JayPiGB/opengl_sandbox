#version 330 core

layout(location = 0) out vec4 out_color;

in vec3 v_position;
in vec3 v_color;

uniform float u_time;	//variable sent from the CPU to the shader

void main() {
	out_color = vec4(sin((u_time + v_color + 1) / 2), 1);
}