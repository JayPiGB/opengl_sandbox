#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

out float v_height; //variables passed between shaders are called "varying"

void main() {
	gl_Position = vec4(in_position, 1);

	v_height = (in_position.y + 1.0) * 0.5;
}
