#version 330 core

layout(location = 0) out vec4 out_color;

in float v_height;

void main() {
	vec3 color = vec3(v_height);
	out_color = vec4(color, 1);
}
