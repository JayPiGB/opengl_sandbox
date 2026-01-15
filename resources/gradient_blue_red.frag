#version 330 core

layout(location = 0) out vec4 out_color;

in float v_x;
in float v_y;
void main() {
	out_color = vec4(v_y, 0.5, v_x, 1);
}
