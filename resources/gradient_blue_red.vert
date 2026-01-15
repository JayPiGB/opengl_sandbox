#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 in_color;

out float v_x;
out float v_y;
void main() {
	gl_Position = vec4(a_position, 1);
	v_x = a_position.x * -1;
	v_y = a_position.y;
}
